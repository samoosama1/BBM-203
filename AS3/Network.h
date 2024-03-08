#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
    void message(vector<Client>& clients, const string& command, const string& senderPort, const string& receiverPort, int messageLimit);
    void create_frames(vector<string>& messageChunks, Client& sender, Client& routeReceiver, const string& receiverID, const string& receiverIP,
                            const string& senderPort, const string& receiverPort);
    void print_message_output(int chunkNum, string& message, queue<stack<Packet *>>& outQueue);
    Client* retrieve_with_id(vector<Client>& clients, string& clientID);
    Client* retrieve_with_mac(vector<Client>& clients, string& clientMAC);
    vector<string> split_message(string message, int messageLimit);
    void showQinfo(vector<Client>& clients, const string& command);
    void showFrameInfo(vector<Client>& clients, const string& command);
    void send(vector<Client>& clients);
    void receive(vector<Client>& clients);
    void assemble(vector<Client>& clients, queue<stack<Packet*>>& tempQueue, string& senderID);
    void forward(vector<Client>& clients, queue<stack<Packet*>>& tempQueue, string& endReceiver);
    void printLogs(vector<Client>& clients, const string& command);
};

#endif  // NETWORK_H
