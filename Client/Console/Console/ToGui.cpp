// Class that handles communication to the GUI via UDP and IPv6.
#include "ToGui.h"

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 4444	//The port on which to listen for incoming data

ToGui::ToGui() {
}

void ToGui::SendTest() {
    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    sockaddr_in6 RecvAddr;
    ZeroMemory(&RecvAddr, sizeof RecvAddr);

    unsigned short Port = 4444;

    char SendBuf[1024];
    int BufLen = 1024;

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
    }

    //---------------------------------------------
    // Create a socket for sending data
    SendSocket = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
    }
    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "192.168.1.1")
    // and the specified port number.
    RecvAddr.sin6_family = PF_INET6;
    RecvAddr.sin6_port = htons(Port);
    //RecvAddr.sin6_addr = in6addr_loopback;
    //RecvAddr.sin6_addr = inet_addr("::1");
    inet_pton(PF_INET6, "::1", &RecvAddr.sin6_addr);
    char test[] = { 0,1,1,2 };

    //---------------------------------------------
    // Send a datagram to the receiver
    wprintf(L"Sending a datagram to the receiver...\n");
    iResult = sendto(SendSocket,
        (char*)&test, sizeof(test), 0, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
    }
}