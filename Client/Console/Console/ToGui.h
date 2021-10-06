#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Client.h"
#if (_WIN32_WINNT == 0x0500)
#include <tpipv6.h>
#endif
#pragma comment(lib, "Ws2_32.lib")

class ToGui {
private:
	SOCKET SendSocket;
	sockaddr_in6 SendAddr;
	int iResult;
public:
	ToGui();
	void SendMoveToGui(int x, int y, int z);
	void ResetBoard();
	void operator()(int *localClientID, std::vector<Client*> *clients, bool *updateFlag);
};