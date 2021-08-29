// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Messages.cpp"
#include "ConnectToServer.h"

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"

SOCKET ConnectSocket;

int main()
{
    ConnectToServer *test;
    test = new ConnectToServer();
        //((PCSTR) "127.0.0.1",(PCSTR) 49152);

    test->setupConnection();

    #define DEFAULT_BUFLEN 512

    int recvbuflen = DEFAULT_BUFLEN;

    //const char* sendbuf = 0;
    char recvbuf[DEFAULT_BUFLEN];

    // Send an initial buffer
    /*iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    */

    int iResult;

    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    int lol = 0;

    SOCKET ConnectSocket = test->getSocket();

    lol = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Bytes Sent: %ld\n", lol);
    int clientId = 0;

    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            MsgHead* msghead;
            printf("Bytes received: %d\n", iResult);
            msghead = (MsgHead*)recvbuf;
            if (msghead->type == Change) {
                std::cout << "Change detected\n";
                ChangeMsg* chgmsg = (ChangeMsg*)msghead;

                if (chgmsg->type == NewPlayer) {
                    std::cout << "New player detected\n";
                }
                if (chgmsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected\n";
                    NewPlayerPositionMsg* newplayer = (NewPlayerPositionMsg*)chgmsg;
                    std::cout << "X: " << newplayer->pos.x << "Y: " << newplayer->pos.y << "\n";
                }
            }
            clientId = msghead->id;
            std::cout << "ID: " << msghead->id << "\n";
            std::cout << "Length: " << msghead->length << "\n";
            std::cout << "SEQ: " << msghead->seq_no << "\n";
            std::cout << "Type: " << msghead->type << "\n";
            /*
            EventMsg eventMsg;
            eventMsg.head.id = clientId;
            eventMsg.head.type = Event;
            eventMsg.head.length = sizeof(eventMsg);
            eventMsg.type = Move;
            MoveEvent moveEvent;
            moveEvent.event = eventMsg;
            moveEvent.pos.x = -100;
            moveEvent.pos.y = -99;
            moveEvent.dir.x = 0;
            moveEvent.dir.y = 0;

            lol = send(ConnectSocket, (char*)&moveEvent, sizeof(moveEvent), 0);
            printf("Bytes Sent: %ld\n", lol);*/
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}