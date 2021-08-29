// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "Messages.cpp"

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"

int main()
{
    WSADATA wsaData;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
// the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

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

    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    int lol = 0;

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