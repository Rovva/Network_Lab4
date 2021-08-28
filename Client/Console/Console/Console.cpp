// Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"

#define MAXNAMELEN  32

enum MsgType {
    Join,// Client joining game at server  
    Leave,// Client leaving game  
    Change,// Information to clients  
    Event,// Information from clients to server  
    TextMessage// Send text messages to one or all
};// Included first in all messagesstruct 
struct MsgHead {
    unsigned int length;// Total length for whole message  
    unsigned int seq_no;  // Sequence number  
    unsigned int id;// Client ID or 0;
    MsgType   type;// Type of message
};

enum ObjectDesc {
    Human,
    NonHuman,
    Vehicle,
    StaticObject
};
enum ObjectForm {
    Cube,
    Sphere,
    Pyramid,
    Cone
};

struct JoinMsg {
    MsgHead head;
    ObjectDesc  desc;
    ObjectForm  form;
    char name[MAXNAMELEN];
};

struct LeaveMsg {
    MsgHead  head;
};

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
    LeaveMsg leave;
    leave.head.id = 1;

    lol = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Bytes Sent: %ld\n", lol);

    MsgHead* msghead;

    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            msghead = (MsgHead*)recvbuf;
            std::cout << msghead->id << "\n";
            std::cout << msghead->length << "\n";
            std::cout << msghead->seq_no << "\n";
            std::cout << msghead->type << "\n";
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