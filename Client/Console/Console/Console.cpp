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
#ifndef MY_HEADER_FILE_IS_INCLUDED

#define MY_HEADER_FILE_IS_INCLUDED

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
    //ReaderThread* readerThread;
    //readerThread = new ReaderThread(ConnectSocket);
    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition);
    WriterThread *writer = new WriterThread(ConnectSocket);
    Sleep(1000);
    writer->sendJoin();
    // I found no other way to get starting coordinates then sending a wrong move.
    Sleep(1000);
    writer->sendMoveEvent(localClientID, localStartingPosition, &seq);
    Client *localClient;
    localClient = new Client(localClientID, localStartingPosition);

    //std::thread writer(WriterThread(ConnectSocket));
    //readerThread->runThread();
    int direction = 0;
    Coordinate cords;
    //Sleep(1000);
    //writer->sendJoin();
    Sleep(5000);
    std::cout << "Local client ID: " << localClientID << std::endl;
    std::cout << "Local client position: " << localStartingPosition.x << std::endl;
    while (1) {

    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}