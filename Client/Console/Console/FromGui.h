#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#if (_WIN32_WINNT == 0x0500)
#include <tpipv6.h>
#endif
#pragma comment(lib, "Ws2_32.lib")

class FromGui {
private:
	SOCKET RecvSocket;
	sockaddr_in6 RecvAddr;
	struct sockaddr_in6 server, si_other;
	int slen, recv_len;
	int iResult;
public:
	FromGui();
	void operator()();
};
