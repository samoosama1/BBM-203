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
            showQinfo(clients, command);
        } else if (comKeyword == "SEND") {
            send(clients);
        } else if (comKeyword == "RECEIVE") {
            receive(clients);
        } else if (comKeyword == "PRINT_LOG") {
            printLogs(clients, command);
        } else {
            std::cout << "Invalid command.\n";
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
    ifs.close();
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
    size_t chunkNum = messageChunks.size();
    Client* sender = retrieve_with_id(clients, senderID);
    Client* routeReceiver = retrieve_with_id(clients, sender->routing_table[receiverID]);
    Client* endReceiver = retrieve_with_id(clients, receiverID);
    create_frames(messageChunks, *sender, *routeReceiver, receiverID, endReceiver->client_ip, senderPort, receiverPort);
    print_message_output(chunkNum, message, sender->outgoing_queue);
    Log log{createTime(), message, static_cast<int>(messageChunks.size()), 0, senderID, receiverID, true, ActivityType::MESSAGE_SENT};
    sender->log_entries.push_back(log);
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
            string frameDash = dash(8);
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

Client* Network::retrieve_with_id(vector<Client>& clients, string& clientID) {
    for (Client& client : clients) {
        if(client.client_id == clientID)
            return &client;
    }
    return nullptr;
}

void Network::create_frames(vector<string>& messageChunks, Client& sender, Client& routeReceiver, const string& receiverID, const string& receiverIP,
                            const string& senderPort, const string& receiverPort) {
    for (const string& chunk : messageChunks) {
        stack<Packet*> frame;
        frame.push(new ApplicationLayerPacket(0, sender.client_id, receiverID, chunk));
        frame.push(new TransportLayerPacket(1, senderPort, receiverPort));
        frame.push(new NetworkLayerPacket(2, sender.client_ip, receiverIP));
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
    Client* sender = retrieve_with_id(clients, clientID);
    string _str = queueType == "out" ? "outgoing" : "incoming";
    auto& _queue = queueType == "out" ? sender->outgoing_queue : sender->incoming_queue;
    if(_queue.size() < frameNum) {
        std::cout << "No such frame.\n";
        return;
    }
    size_t initSize = _queue.size();
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
            auto* app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
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

void Network::showQinfo(vector<Client> &clients, const string &command) {
    string comKeyword;
    string clientID;
    string queueType;
    stringstream ss{command};
    ss >> comKeyword >> clientID >> queueType;
    string conditStr = queueType == "out" ? "Outgoing" : "Incoming";
    Client* client = retrieve_with_id(clients, clientID);
    auto& _queue = queueType == "out" ? client->outgoing_queue : client->incoming_queue;
    size_t queueSize = _queue.size();
    std::cout << "Client " << clientID << " " << conditStr << " Queue Status\n";
    std::cout << "Current total number of frames: " << queueSize << "\n";
}

void Network::send(vector<Client> &clients) {
    for (Client& client : clients) {
        int frameNum = 1;
        while(!client.outgoing_queue.empty()) {
            auto frame = client.outgoing_queue.front();
            auto physical = dynamic_cast<PhysicalLayerPacket*>(frame.top());
            auto nextHop = retrieve_with_mac(clients, physical->receiver_MAC_address);
            stack<Packet*> temp;
            std::cout << "Client " << client.client_id << " sending frame #" << frameNum << " to client " << nextHop->client_id << "\n";
            while(!frame.empty()) {
                frame.top()->num_of_hops++;
                temp.push(frame.top());
                frame.top()->print();
                frame.pop();
            }
            auto app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
            std::cout << "Message chunk carried: \"" << app->message_data << "\"\n";
            std::cout << "Number of hops so far: " << app->num_of_hops << "\n";
            std::cout << dash(8) << "\n";
            if(app->message_data.back() == '.' || app->message_data.back() == '!' || app->message_data.back() == '?') {
                frameNum = 0;
            }
            while(!temp.empty()) {
                frame.push(temp.top());
                temp.pop();
            }
            nextHop->incoming_queue.push(frame);
            client.outgoing_queue.pop();
            frameNum++;
        }
    }
}

Client* Network::retrieve_with_mac(vector<Client> &clients, string &clientMAC) {
    for (Client& client : clients) {
        if(client.client_mac == clientMAC)
            return &client;
    }
    return nullptr;
}


void Network::receive(vector<Client> &clients) {
    for (Client& client : clients) {
        queue<stack<Packet*>> tempQueue;
        while(!client.incoming_queue.empty()) {
            stack<Packet*> frame = client.incoming_queue.front();
            tempQueue.push(frame);
            stack<Packet*> temp;
            while(!frame.empty()) {
                temp.push(frame.top());
                frame.pop();
            }
            auto app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
            string senderID = app->sender_ID;
            string receiverID = app->receiver_ID;
            if(app->message_data.back() == '.' || app->message_data.back() == '!' || app->message_data.back() == '?') {
                if(client.client_id == app->receiver_ID) {
                    assemble(clients, tempQueue, senderID);
                } else {
                    forward(clients, tempQueue, receiverID);
                }
            }
            client.incoming_queue.pop();
        }
    }
}

void Network::assemble(vector<Client>& clients, queue<stack<Packet*>>& tempQueue, string& senderID) {
    int frameNum = 0;
    Client* endReceiver;
    string message;
    string endStr;
    string prevHop;
    int numOfHops;
    while(!tempQueue.empty()) {
        frameNum++;
        stack<Packet*> frame = tempQueue.front();
        stack<Packet*> temp;
        auto physical = dynamic_cast<PhysicalLayerPacket*>(frame.top());
        endReceiver = retrieve_with_mac(clients ,physical->receiver_MAC_address);
        endStr = endReceiver->client_id;
        prevHop = retrieve_with_mac(clients, physical->sender_MAC_address)->client_id;
        std::cout << "Client " << endStr << " receiving frame #" << frameNum << " from client " << prevHop << ", originating from client " << senderID << "\n";
        while(!frame.empty()) {
            temp.push(frame.top());
            frame.top()->print();
            frame.pop();
        }
        auto app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
        message.append(app->message_data);
        std::cout << "Message chunk carried: \"" << app->message_data << "\"\n";
        std::cout << "Number of hops so far: " << app->num_of_hops << "\n";
        std::cout << dash(8) << "\n";
        numOfHops = app->num_of_hops;
        while(!temp.empty()) {
            delete temp.top();
            temp.pop();
        }
        tempQueue.pop();
    }
    Log log{createTime(), message, frameNum, numOfHops, senderID, endStr, true, ActivityType::MESSAGE_RECEIVED};
    endReceiver->log_entries.push_back(log);
    std::cout << "Client " << endStr << " received the message \"" << message << "\" from client " << senderID << ".\n";
    std::cout << dash(8) << "\n";
}

void Network::forward(vector<Client>& clients, queue<stack<Packet*>>& tempQueue, string& endReceiver) {
    bool drop = false;
    int frameNum = 0;
    int numOfHops;
    Client* curHop;
    string _message;
    string senderID;
    auto physical = dynamic_cast<PhysicalLayerPacket*>(tempQueue.front().top());
    string receiverMAC = physical->receiver_MAC_address;
    string senderMAC = physical->sender_MAC_address;
    std::cout << "Client " << retrieve_with_mac(clients, receiverMAC)->client_id << " receiving frame #1 from client " <<
    retrieve_with_mac(clients, senderMAC)->client_id << ", but intended for client " << endReceiver << ". Forwarding..." << "\n";
    while(!tempQueue.empty()) {
        frameNum++;
        stack<Packet*> frame = tempQueue.front();
        tempQueue.pop();
        auto phy = dynamic_cast<PhysicalLayerPacket*>(frame.top());
        numOfHops = phy->num_of_hops;
        curHop = retrieve_with_mac(clients, receiverMAC);
        Client* nextHop = retrieve_with_id(clients, curHop->routing_table[endReceiver]);
        stack<Packet*> temp;
        while(!frame.empty()) {
            temp.push(frame.top());
            frame.pop();
        }
        auto app = dynamic_cast<ApplicationLayerPacket*>(temp.top());
        senderID = app->sender_ID;
        _message.append(app->message_data);
        while(!temp.empty()) {
            frame.push(temp.top());
            temp.pop();
        }
        // Drop
        if(nextHop == nullptr) {
            drop = true;
            std::cout << "Error: Unreachable destination. Packets are dropped after " << phy->num_of_hops << " hops!\n";
            if(!tempQueue.empty()) {
                std::cout << "Client " << retrieve_with_mac(clients, receiverMAC)->client_id << " receiving frame #" << frameNum + 1<< " from client " <<
                          retrieve_with_mac(clients, senderMAC)->client_id << ", but intended for client " << endReceiver << ". Forwarding... "  << "\n";
            }
            while(!frame.empty()) {
                delete frame.top();
                frame.pop();
            }
            continue;
        }
        // Forward
        phy->receiver_MAC_address = nextHop->client_mac;
        phy->sender_MAC_address = curHop->client_mac;
        std::cout << "Frame #" << frameNum << " MAC address change: New sender MAC " << phy->sender_MAC_address << ", new receiver MAC " << phy->receiver_MAC_address << "\n";
        curHop->outgoing_queue.push(frame);
    }
    if(drop) {
        Log log{createTime(), _message, frameNum, numOfHops, senderID, endReceiver, false, ActivityType::MESSAGE_DROPPED};
        curHop->log_entries.push_back(log);
    } else {
        Log log{createTime(), _message, frameNum, numOfHops, senderID, endReceiver, true, ActivityType::MESSAGE_FORWARDED};
        curHop->log_entries.push_back(log);
    }
    std::cout << dash(8) << "\n";
}

void Network::printLogs(vector<Client> &clients, const string& command) {
    string comKeyword;
    string clientID;
    stringstream ss{command};
    ss >> comKeyword >> clientID;
    Client* client = retrieve_with_id(clients, clientID);
    if(client->log_entries.empty())
        return;
    std::cout << "Client " << clientID << " Logs:\n";
    int logNum = 0;
    for (Log& log : client->log_entries) {
        logNum++;
        string activity;
        bool printMessage = false;
        switch (log.activity_type) {
            case ActivityType::MESSAGE_SENT:
                activity = "Message Sent";
                printMessage = true;
                break;
            case ActivityType::MESSAGE_FORWARDED:
                activity = "Message Forwarded";
                break;
            case ActivityType::MESSAGE_RECEIVED:
                activity = "Message Received";
                printMessage = true;
                break;
            case ActivityType::MESSAGE_DROPPED:
                activity = "Message Dropped";
                break;
        }
        std::cout << dash(14) << "\n";
        std::cout << "Log Entry #" << logNum << ":\n";
        std::cout << "Activity: " << activity << "\n";
        std::cout << "Timestamp: " << log.timestamp << "\n";
        std::cout << "Number of frames: " << log.number_of_frames << "\n";
        std::cout << "Number of hops: " << log.number_of_hops << "\n";
        std::cout << "Sender ID: " << log.sender_id << "\n";
        std::cout << "Receiver ID: " << log.receiver_id << "\n";
        std::cout << "Success: " << (log.success_status ? "Yes" : "No") << "\n";
        if(printMessage) {
            std::cout << "Message: \"" << log.message_content << "\"\n";
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
