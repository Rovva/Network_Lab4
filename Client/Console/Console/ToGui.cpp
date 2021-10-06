// Class that handles communication to the GUI via UDP and IPv6.
#include "ToGui.h"

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 4444	//The port on which to listen for incoming data

ToGui::ToGui() {
    WSADATA wsaData;

    SendSocket = INVALID_SOCKET;
    ZeroMemory(&SendAddr, sizeof SendAddr);

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
    SendAddr.sin6_family = PF_INET6;
    SendAddr.sin6_port = htons(Port);
    //RecvAddr.sin6_addr = in6addr_loopback;
    //RecvAddr.sin6_addr = inet_addr("::1");
    inet_pton(PF_INET6, "::1", &SendAddr.sin6_addr);
}

void ToGui::SendMoveToGui(int x, int y, int z) {
    char move[] = { 0, x, y, z };

    std::cout << "Updating GUI with X: " << x << " Y: " << y << " Color: " << z << "\n";
    iResult = sendto(SendSocket,
        (char*)&move, sizeof(move), 0, (struct sockaddr*)&SendAddr, sizeof(SendAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
    }
}

void ToGui::ResetBoard() {
    char reset[] = { 1 };

    wprintf(L"Sending a reset to the GUI...\n");
    iResult = sendto(SendSocket,
        (char*)&reset, sizeof(reset), 0, (struct sockaddr*)&SendAddr, sizeof(SendAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
    }
}

void ToGui::operator()(int *localClientID, std::vector<Client*> *clients, bool *updateFlag) {
    while (1) {
        if (*updateFlag == true) {
            int x = 0, y = 0, z = 0;
            this->ResetBoard();
            for (int i = 0; i < clients->size(); i++) {
                x = clients->at(i)->getPosition().x;
                y = clients->at(i)->getPosition().y;
                if (clients->at(i)->getClientID() == *localClientID) {
                    z = 4;
                } else {
                    z = 2;
                }
                this->SendMoveToGui(x, y, z);
            }
            *updateFlag = false;
        }
    }
}