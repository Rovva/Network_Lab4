/**
* @file FromGui.h
* @brief Klassen som tar emot och hanterar data från GUI.
* @author Christoffer Rova
* @version 1.0
* @date 2021-10-26
*/

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

#include "WriterThread.h"
#include <vector>
#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

class FromGui {
private:
	SOCKET RecvSocket; ///< Variabel som lagrar en socket
	sockaddr_in6 RecvAddr;
	sockaddr_in6 server, si_other;
	int slen, recv_len;
	int iResult;
public:
	/**
	*	Konstruktor för FromGui.
	*/
	FromGui();
	
	/**
	*	Funktionen som ska köras om och om igen i en tråd.
		@param[in]	*localClientID	Vilket id som den lokala klienten har.
		@param[in]	*seq			Variabeln som lagrar vilket sekvensnummer som är giltligt.
		@param[in]	clients			Vektorn som lagrar alla klienter.
		@param[in]	writer			Referens till WriterThread objektet som skickar meddelanden.
	*/
	void operator()(int *localClientID, int *seq, std::vector<Client*>* clients, WriterThread writer);
};
