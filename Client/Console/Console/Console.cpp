// Konsollklienten f�r Laboration 4
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
    // Skapa en vektor som lagrar alla klienter.
    std::vector<Client*> clients;

    // Skapa ett objekt som ansvarar f�r att ansluta till servern.
    ConnectToServer *serverConnection;
    serverConnection = new ConnectToServer();
    serverConnection->setupConnection();
    // Lagra den socket som skapas i ConnectSocket.
    SOCKET ConnectSocket = serverConnection->getSocket();

    // Lokala variabler f�r att h�lla koll p� den lokala klientens data.
    int seq = 0;
    int localClientID = -1;
    Coordinate localStartingPosition;
    // Ogiltliga koordinater som startposition.
    localStartingPosition.x = -200;
    localStartingPosition.y = -200;

    // Skapa objekt som ansvarar f�r att l�sa alla meddelanden som skickas fr�n servern till klienten.
    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition, &clients);
    WriterThread *writer = new WriterThread(ConnectSocket);
    // Skicka meddelande om att ansluta till servern.
    writer->sendJoin();

    // Lokala variabler f�r att skicka nya positiondata till servern.
    int moveDirection = 0;
    Coordinate newPos, oldPos;
    oldPos.x = 0; oldPos.y = 0;
    newPos.x = 0; newPos.y = 0;

    while (1) {
        oldPos.x = 0; oldPos.y = 0;
        newPos.x = 0; newPos.y = 0;

        // Mata in siffror f�r att f�rflytta klienten.
        // 4 = V�nster, 6 = H�ger, 8 = Upp�t, 2 = Ner�t
        std::cout << "Local client id is: " << localClientID << " Which direction?\n";
        std::cin >> moveDirection;

        // Loopa igenom alla klienter f�r att hitta positionen som den lokala klienten har just nu.
        for (int i = 0; i < clients.size(); i++) {
            std::cout << "Client: " << clients.at(i)->getClientID() << " is in the list.\n";
            if (clients.at(i)->getClientID() == localClientID) {
                oldPos = clients.at(i)->getPosition();
            }
        }

        // Ber�kning av den nya positionen som blir n�r man f�rflyttar sig �t valfritt h�ll.
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
        // Om man trycker 5 s� f�r man upp den nuvarande positionen.
        if (moveDirection == 5) {
            std::cout << "Current position: X: " << oldPos.x << " Y: " << oldPos.y << "\n";
        }
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}