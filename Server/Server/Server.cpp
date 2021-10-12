#include <iostream>
#include <vector>
#include <thread>

#include <io.h>
#include <stdio.h>
#include <winsock2.h>

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
    // Create a vector that cóntains all the clients.
    std::vector<Client*>* clients = new std::vector<Client*>;

    // Call SocketSetup to bind a socket.
    SocketSetup* socket = new SocketSetup();

    // Store the socket created above and send it to other classes.
    SOCKET RecvSocket = socket->getSocket();
    sockaddr_in server = socket->getServer();

    // Create Broadcaster object that handles sending messages to all the clients.
    Broadcaster* broad = new Broadcaster(clients);

    // Create a thread that runs ConnectionThread object that handles all the
    // accepted connections.
    std::thread connection(ConnectionThread(&RecvSocket, server), broad);
    connection.detach();

    while (1) {

    }
}