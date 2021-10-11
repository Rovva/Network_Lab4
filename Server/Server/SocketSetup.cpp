#include "SocketSetup.h"

SocketSetup::SocketSetup() {

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
    }

    printf("Initialised.\n");

    //Create a socket
    if ((RecvSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    // Fill memory locations for server variable.
    ZeroMemory(&server, sizeof server);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(49152);

    //Bind
    if (bind(RecvSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }

    puts("Bind done");

    //Listen to incoming connections
    listen(RecvSocket, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

}

SOCKET SocketSetup::getSocket() {
    return RecvSocket;
}

sockaddr_in SocketSetup::getServer() {
    return server;
}