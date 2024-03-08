#include "Network.h"

string dash(size_t size);
string createTime();
Network::Network() {
}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    for (const string& command : commands) {
        stringstream ss{command};
        string comKeyword;
        string comStr = "Command: " + command;
        string dashes = dash(comStr.size());
        ss >> comKeyword;
        std::cout << dashes << "\n";
        std::cout << comStr << "\n";
        std::cout << dashes << "\n";
        if (comKeyword == "MESSAGE") {
            message(clients, command, sender_port, receiver_port, message_limit);
        } else if (comKeyword == "SHOW_FRAME_INFO") {
            showFrameInfo(clients, command);
        } else if (comKeyword == "SHOW_Q_INFO") {

        } else if (comKeyword == "SEND") {

        } else if (comKeyword == "RECEIVE") {

        } else if (comKeyword == "PRINT_LOG") {

        } else {

        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    ifstream ifs{filename};
    int numOfClients;
    string _str;
    ifs >> numOfClients;
    while(getline(ifs, _str)) {
        if(_str.empty())
            continue;
        stringstream ss{_str};
        string id;
        string ip;
        string mac;
        ss >> id >> ip >> mac;
        Client client{id, ip, mac};
        clients.push_back(client);
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    ifstream ifs{filename};
    auto it = clients.begin();
    string _str;
    while(getline(ifs, _str)) {
        if(_str.empty())
            continue;
        if(_str == "-") {
            it++;
            continue;
        }
        if(it == clients.end())
            break;
        string destID;
        string nearestID;
        stringstream ss{_str};
        ss >> destID >> nearestID;
        it->routing_table[destID] = nearestID;
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    ifstream ifs{filename};
    string _str;
    int numOfCom;
    ifs >> numOfCom;
    while(getline(ifs, _str)) {
        if(_str.empty())
            continue;
        commands.push_back(_str);
    }
    return commands;
}

void Network::message(vector<Client>& clients, const string& command, const string& senderPort, const string& receiverPort, int messageLimit) {
    stringstream ss{command};
    string keyword;
    string senderID;
    string receiverID;
    string _str;
    string message;
    ss >> keyword;
    ss >> senderID >> receiverID;
    while(getline(ss, _str, '#')) {
        message.append(_str);
    }
    message = message.substr(1, message.size() - 1);
    auto messageChunks = split_message(message, messageLimit);
    int chunkNum = messageChunks.size();
    Client& sender = retrieve_client(clients, senderID);
    Client& routeReceiver = retrieve_client(clients, sender.routing_table[receiverID]);
    create_frames(messageChunks, sender, routeReceiver, receiverID, senderPort, receiverPort);
    print_message_output(chunkNum, message, sender.outgoing_queue);
    Log log{createTime(), message, static_cast<int>(messageChunks.size()), 0, senderID, receiverID, true, ActivityType::MESSAGE_SENT};
    sender.log_entries.push_back(log);
}

void Network::print_message_output(int chunkNum, string& message, queue<stack<Packet *>>& outQueue) {
    std::cout << "Message to be sent: \"" << message << "\"\n\n";
    size_t initSize = outQueue.size();
    for (int i = 0; i < initSize; ++i) {
        stack<Packet*> frame = outQueue.front();
        outQueue.pop();
        outQueue.push(frame);
        if(initSize - i <= chunkNum) {
            string frameNum = "Frame #" + to_string(chunkNum - initSize + i + 1);
            string frameDash = dash(frameNum.size());
            std::cout << frameNum << "\n";
            stack<Packet*> temp;
            while(!frame.empty()) {
                Packet* ptr = frame.top();
                frame.pop();
                temp.push(ptr);
                ptr->print();
            }
            auto app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
            std::cout << "Message chunk carried: \"" << app->message_data << "\"\n";
            std::cout << "Number of hops so far: " << app->num_of_hops << "\n";
            std::cout << frameDash << "\n";
            while(!temp.empty()) {
                frame.push(temp.top());
                temp.pop();
            }
        }
    }
}

Client& Network::retrieve_client(vector<Client>& clients, string& clientID) {
    for (Client& client : clients) {
        if(client.client_id == clientID)
            return client;
    }
}

void Network::create_frames(vector<string>& messageChunks, Client& sender, Client& routeReceiver, const string& receiverID,
                            const string& senderPort, const string& receiverPort) {
    for (const string& chunk : messageChunks) {
        stack<Packet*> frame;
        frame.push(new ApplicationLayerPacket(0, sender.client_id, receiverID, chunk));
        frame.push(new TransportLayerPacket(1, senderPort, receiverPort));
        frame.push(new NetworkLayerPacket(2, sender.client_ip, routeReceiver.client_ip));
        frame.push(new PhysicalLayerPacket(3, sender.client_mac, routeReceiver.client_mac));
        sender.outgoing_queue.push(frame);
    }
}

vector<string> Network::split_message(string message, int messageLimit) {
    vector<string> messageParts;
    while(message.size() > messageLimit) {
        messageParts.push_back(message.substr(0, messageLimit));
        message = message.substr(messageLimit);
    }
    messageParts.push_back(message);
    return messageParts;
}

void Network::showFrameInfo(vector<Client> &clients, const string &command) {
    string comKeyword;
    string clientID;
    string queueType;
    int frameNum;
    stringstream ss{command};
    ss >> comKeyword >> clientID >> queueType >> frameNum;
    Client& sender = retrieve_client(clients, clientID);
    string _str = queueType == "out" ? "outgoing" : "incoming";
    auto& _queue = queueType == "out" ? sender.outgoing_queue : sender.incoming_queue;
    if(_queue.size() < frameNum) {
        std::cout << "No such frame.\n";
        return;
    }
    int initSize = _queue.size();
    for (int i = 0; i < initSize; ++i) {
        auto frame = _queue.front();
        _queue.pop();
        _queue.push(frame);
        if(i == frameNum - 1) {
            stack<Packet*> temp;
            while(!frame.empty()) {
                temp.push(frame.top());
                frame.pop();
            }
            ApplicationLayerPacket* app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
            std::cout << "Current Frame #" << frameNum << " on the " << _str << " queue of client " << clientID << "\n";
            std::cout << "Carried Message: \"" << app->message_data << "\"\n";
            size_t tempSize = temp.size();
            for (int j = 0; j < tempSize; ++j) {
                frame.push(temp.top());
                std::cout << "Layer " << j << " info: ";
                temp.top()->print();
                temp.pop();
            }
            std::cout << "Number of hops so far: " << frame.top()->num_of_hops << "\n";
        }
    }
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}

string dash(size_t size) {
    string dashes;
    for (int i = 0; i < size; ++i) {
        dashes.append("-");
    }
    return dashes;
}

string createTime() {
    time_t _time = time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&_time));
    string timeString{buf};
    return timeString;
}
