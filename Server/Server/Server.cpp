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
    std::vector<Client*>* clients = new std::vector<Client*>;
    std::vector<std::thread> threads;

    SocketSetup* socket = new SocketSetup();

    SOCKET RecvSocket = socket->getSocket();
    sockaddr_in server = socket->getServer();

    Broadcaster* broad = new Broadcaster(clients);

    std::thread connection(ConnectionThread(&RecvSocket, server), broad);
    connection.detach();

    while (1) {
    /*    for (int i = 0; i < clients.size(); i++) {
            if (clients.at(i)->getUseFlag() == false) {
                std::thread(ReaderThread(clients.at(i)->getSocket())).detach();
            }
        }*/
    }
}