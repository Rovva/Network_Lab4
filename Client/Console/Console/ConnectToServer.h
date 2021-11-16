/**
* @file ConnectToServer.h
* @brief Klassen som etablerar en anslutning till servern.
* @author Christoffer Rova
* @version 1.0
* @date 2021-10-26
*/
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
	/**
	*	Konstruktorn.
	*/
	ConnectToServer();
	/**
	*	Metoden som etablerar en anslutning till servern.
	*/
	int setupConnection();
	/**
	*	Den här metoden returnerar den socket som etablerats tidigare.
	*	@param[out]	ConnectSocket
	*/
	SOCKET getSocket();
};