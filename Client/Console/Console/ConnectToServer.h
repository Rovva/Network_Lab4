#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
class ConnectToServer {
private:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	int iResult;
public:
	ConnectToServer();
	int setupConnection();
	SOCKET getSocket();
};