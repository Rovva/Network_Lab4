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
    int localClientID = 0;
    Coordinate localStartingPosition;
    localStartingPosition.x = 0;
    localStartingPosition.y = 0;
    //ReaderThread* readerThread;
    //readerThread = new ReaderThread(ConnectSocket);
    std::thread reader(ReaderThread(ConnectSocket), &seq, &localClientID, &localStartingPosition);
    WriterThread* writer = new WriterThread(ConnectSocket);
    writer->sendJoin();
    Client *localClient;
    localClient = new Client(localClientID, localStartingPosition);

    //std::thread writer(WriterThread(ConnectSocket));
    Sleep(5000);
    //readerThread->runThread();


    while (1) {



        Sleep(4000);
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}