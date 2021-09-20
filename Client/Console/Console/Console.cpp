// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

#include "ConnectToServer.h"
#include "ReaderThread.h"
#include "WriterThread.h"
#include "Client.h"
#ifndef MESSAGES_H

#define MESSAGES_H

#include "Messages.h"

#endif 

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"

SOCKET ConnectSocket;

int main()
{
    std::vector<Client*> clients;

    ConnectToServer *serverConnection;
    serverConnection = new ConnectToServer();

    serverConnection->setupConnection();

    SOCKET ConnectSocket = serverConnection->getSocket();

    int seq = 0;
    int localClientID = -1;
    Coordinate localStartingPosition;
    localStartingPosition.x = -200;
    localStartingPosition.y = -200;

    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition, &clients);
    WriterThread *writer = new WriterThread(ConnectSocket);
    writer->sendJoin();
    int direction = 0;
    int moveDirection = 0;
    Coordinate newPos, oldPos;
    oldPos.x = 0; oldPos.y = 0;
    newPos.x = 0; newPos.y = 0;
    while (1) {
        oldPos.x = 0; oldPos.y = 0;
        newPos.x = 0; newPos.y = 0;
        std::cout << "Local client id is: " << localClientID << " Which direction?\n";
        std::cin >> moveDirection;

        for (int i = 0; i < clients.size(); i++) {
            std::cout << "Client: " << clients.at(i)->getClientID() << " is in the list.\n";
            if (clients.at(i)->getClientID() == localClientID) {
                oldPos = clients.at(i)->getPosition();
            }
        }

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
        
        if (moveDirection == 5) {
            std::cout << "Current position: X: " << oldPos.x << " Y: " << oldPos.y << "\n";
        }
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}