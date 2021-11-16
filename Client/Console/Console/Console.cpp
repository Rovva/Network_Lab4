/**
* @file Console.cpp
* @brief Där allting börjar.
* @author Christoffer Rova
* @version 1.0
* @date 2021-10-26
*/

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

#define _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET ConnectSocket;

int main()
{
	// Skapa en vektor som sparar alla klienter.
    std::vector<Client*> clients;

	// Skapa ett objekt som hanterar anslutning till servern.
    ConnectToServer *serverConnection;
    serverConnection = new ConnectToServer();
    serverConnection->setupConnection();
	
	// Spara den socket som skapats i ConnectToServer för att skicka
	// vidare till ReaderThread.
    SOCKET ConnectSocket = serverConnection->getSocket();

	// Lokala variabler för den lokala klienten.
    int seq = 0;
    int localClientID = -1;
    Coordinate localStartingPosition;
	
	// Startvärden för klientens position.
    localStartingPosition.x = -200;
    localStartingPosition.y = -200;

    bool updateClients = false;

	// Skapa ett objekt som tar emot alla meddelanden från servern till klienten.
    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition, &clients, &updateClients);

    reader.detach();

    WriterThread *writer = new WriterThread(ConnectSocket);

    std::string option;

	// Fråga om GUI ska hanteras eller inte.
    std::cout << "Start game with GUI? (y/n)\n";
    std::cin >> option;
	// Om GUI ska hanteras av klienten så skapa objekt som körs i trådar.
    if (option == "y") {
        std::cout << "Starting with GUI...\n";
        std::thread toGui(ToGui(), &localClientID, &clients, &updateClients);
        toGui.detach();
		
        std::thread fromGui(FromGui(), &localClientID, &seq, &clients, *writer);
        fromGui.detach();
    }

	// Skicka ett "Join" meddelande till servern.
    writer->sendJoin();

	// Ett gäng lokala variabler som behövs när ny position beräknas.
    int moveDirection = 0;
    Coordinate newPos, oldPos;
    oldPos.x = 0; oldPos.y = 0;
    newPos.x = 0; newPos.y = 0;


    while (1) {
        oldPos.x = 0; oldPos.y = 0;
        newPos.x = 0; newPos.y = 0;
		
		// Ta emot en siffra som motsvarar åt vilket håll klienten vill förflyttas.
		// 4 = Vänster, 6 = Höger, 8 = Upp och 2 = Neråt. (Numpad) 5 = Nuvarande position.
        std::cout << "Local client id is: " << localClientID << " Which direction?\n";
        std::cin >> moveDirection;


		// Gå igenom alla klienter som finns lagrade i vektorn "clients" för att hitta
		// vilken position den lokala klienten har.
        for (int i = 0; i < clients.size(); i++) {
            std::cout << "Client: " << clients.at(i)->getClientID() << " is in the list. X: "
                << clients.at(i)->getPosition().x << " Y: " << clients.at(i)->getPosition().y << "\n";
            if (clients.at(i)->getClientID() == localClientID) {
                oldPos = clients.at(i)->getPosition();
            }
        }

		// Beräkning av den nya positionen baserat på den gamla.
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
		// Nummer 5 visar vilken position den lokala klienten har.
        if (moveDirection == 5) {
            std::cout << "Current position: X: " << oldPos.x << " Y: " << oldPos.y << "\n";
        }
    }
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}