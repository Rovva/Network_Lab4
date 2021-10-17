#include "SocketSetup.h"

SocketSetup::SocketSetup() {

    RecvSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Fill memory locations for server variable.
    memset(&server, '0', sizeof(server));
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(49152);

    bind(RecvSocket, (struct sockaddr*)&server, sizeof(server));

    /*Bind
    if ( == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }

    puts("Bind done");*/

    //Listen to incoming connections
    listen(RecvSocket, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

}

int SocketSetup::getSocket() {
    return RecvSocket;
}

sockaddr_in SocketSetup::getServer() {
    return server;
}
