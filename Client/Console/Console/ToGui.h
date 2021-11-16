/**
* @file ToGui.h
* @brief Klassen som skickar data till GUI.
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
	/**
	 * Konstruktor.
	*/
	ToGui();

	/**
	*	Metoden som skickar ett "MoveEvent" meddelande till servern.
	*	@param[in]	x	X-koordinat.
	*	@param[in]	y	Y-koordinat.
	*	@param[in]	z	Färg.
	*/
	void SendMoveToGui(int x, int y, int z);
	
	/**
	*	Den här metoden skickar kommando till GUI om att rensa hela spelplanen.
	*/
	void ResetBoard();
	
	/**
	*	Metoden som körs om och om igen i en tråd och har som uppgift att uppdatera spelplanen med klienternas position och färg (lokala klienten är blå och alla andra är röda).
	*	@param[in]	*localClientID	Den lokala klientens id.
	*	@param[in]	*clients		Vektorn som innehåller alla klienter.
	*	@param[in]	*updateFlag		En bool som används för att veta när det är dags att uppdatera spelplanen.
	*/
	void operator()(int *localClientID, std::vector<Client*> *clients, bool *updateFlag);
};