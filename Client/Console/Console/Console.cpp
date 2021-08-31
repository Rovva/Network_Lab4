// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#include "Messages.cpp"
#include "ConnectToServer.h"
#include "ReaderThread.h"

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"

SOCKET ConnectSocket;

int main()
{
    ConnectToServer *test;
    test = new ConnectToServer();
        //((PCSTR) "127.0.0.1",(PCSTR) 49152);

    test->setupConnection();

 

    // Send an initial buffer
    /*iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    */

    //int iResult;

    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    int lol = 0;

    SOCKET ConnectSocket = test->getSocket();

    lol = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Bytes Sent: %ld\n", lol);
    int seq = 0;
    //ReaderThread* readerThread;
    //readerThread = new ReaderThread(ConnectSocket);
    std::thread reader(ReaderThread(ConnectSocket), &seq);
    //readerThread->runThread();
    LeaveMsg leave;
    leave.head.id = 4;
    leave.head.length = sizeof(leave);
    leave.head.type = Leave;

    int clientId = 0;
            
    while (1) {

        MoveEvent move;
        move.event.type = Move;
        move.event.head.id = clientId;
        move.event.head.length = sizeof(move);
        move.event.head.seq_no = seq++;
        move.event.head.type = Event;

        move.pos.x = -100;
        move.pos.y = -99;

        lol = send(ConnectSocket, (char*)&move, sizeof(move), 0);
        printf("Bytes Sent: %ld\n", lol);

        Sleep(4000);
    }
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}