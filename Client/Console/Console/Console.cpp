// Console client for Laboration 4
//
#pragma once
#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>

#include <thread>
#include <vector>
#include <string>

#include "ConnectToServer.h"
#include "ReaderThread.h"
#include "WriterThread.h"
#include "Client.h"
#include "ToGui.h"
#include "FromGui.h"

#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif 

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"


#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 4444	//The port on which to listen for incoming data
#define _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET ConnectSocket;

int main()
{
    // Create a vector to store all the clients.
    std::vector<Client*> clients;

    // Create a object that handles the connection to the server.
    ConnectToServer *serverConnection;
    serverConnection = new ConnectToServer();
    serverConnection->setupConnection();
    // Store the socket created above from ConnectToServer object.
    SOCKET ConnectSocket = serverConnection->getSocket();

    // Local variables for the local client.
    int seq = 0;
    int localClientID = -1;
    Coordinate localStartingPosition;
    // Assign invalid position for the client.
    localStartingPosition.x = -200;
    localStartingPosition.y = -200;

    bool updateClients = false;

    // Create the object that is responsible for reading all the messages sent from the server to clients.
    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition, &clients, &updateClients);

    reader.detach();

    WriterThread *writer = new WriterThread(ConnectSocket);

    std::string option;

    std::cout << "Start game with GUI? (y/n)\n";
    std::cin >> option;

    if (option == "y") {
        std::cout << "Starting with GUI...\n";
        std::thread toGui(ToGui(), &localClientID, &clients, &updateClients);
        toGui.detach();
        std::thread fromGui(FromGui(), &localClientID, &seq, &clients, *writer);
        fromGui.detach();
    }

    // Send a Join message to server.
    writer->sendJoin();

    // Local variables needed for calculating new position for the local client.
    int moveDirection = 0;
    Coordinate newPos, oldPos;
    oldPos.x = 0; oldPos.y = 0;
    newPos.x = 0; newPos.y = 0;


    while (1) {
        oldPos.x = 0; oldPos.y = 0;
        newPos.x = 0; newPos.y = 0;
        //togui->SendTest();

        // Number input to move the client around.
        // 4 = Left, 6 = Right, 8 = Upwards, 2 = Downwards.
        std::cout << "Local client id is: " << localClientID << " Which direction?\n";
        std::cin >> moveDirection;


        // Go through all the clients in the vector to find the current position for the local client.
        for (int i = 0; i < clients.size(); i++) {
            std::cout << "Client: " << clients.at(i)->getClientID() << " is in the list. X: "
                << clients.at(i)->getPosition().x << " Y: " << clients.at(i)->getPosition().y << "\n";
            if (clients.at(i)->getClientID() == localClientID) {
                oldPos = clients.at(i)->getPosition();
            }
        }

        // Calculating the new position depending on input.
        if (moveDirection == 2) {
            newPos.x = oldPos.x;
            newPos.y = oldPos.y - 1;
            writer->sendMoveEvent(localClientID, newPos, &seq);
        } else if (moveDirection == 8) {
            newPos.x = oldPos.x;
            newPos.y = oldPos.y + 1;
            writer->sendMoveEvent(localClientID, newPos, &seq);
        } else if (moveDirection == 4) {
            newPos.x = oldPos.x - 1;
            newPos.y = oldPos.y;
            writer->sendMoveEvent(localClientID, newPos, &seq);
        } else if (moveDirection == 6) {
            newPos.x = oldPos.x + 1;
            newPos.y = oldPos.y;
            writer->sendMoveEvent(localClientID, newPos, &seq);
        }
        // Number 5 is used to display the current position.
        if (moveDirection == 5) {
            std::cout << "Current position: X: " << oldPos.x << " Y: " << oldPos.y << "\n";
        }
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}