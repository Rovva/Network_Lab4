#include <iostream>
#include <vector>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SocketSetup.h"
#include "ConnectionThread.h"
#include "Client.h"
#include "ReaderThread.h"
#include "Broadcaster.h"
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif

int main()
{
    int test = 0;
    // Create a vector that c�ntains all the clients.
    std::vector<Client*>* clients = new std::vector<Client*>;

    int seq = 0;

    // Call SocketSetup to bind a socket.
    SocketSetup* socket = new SocketSetup();

    // Store the socket created above and send it to other classes.
    int RecvSocket = socket->getSocket();
    sockaddr_in server = socket->getServer();

    // Create Broadcaster object that handles sending messages to all the clients.
    Broadcaster* broad = new Broadcaster(clients);

    // Create a thread that runs ConnectionThread object that handles all the
    // accepted connections.
    std::thread connection(ConnectionThread(&RecvSocket, server), broad, &seq);
    connection.detach();

    while (1) {

    }
}
