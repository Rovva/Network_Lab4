/**
* @file ReaderThread.h
* @brief Klassen som tar emot och hanterar meddelanden från servern.
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
#include <thread>
#include <iostream>
#include <vector>
#ifndef MESSAGES_H

#define MESSAGES_H

#include "Messages.h"

#endif

#include "Client.h"

class ReaderThread {
private:
	SOCKET ConnectSocket;
public:
	/**
	*	Konstruktor som tar emot en socket som lagras lokalt.
	*/
	ReaderThread(SOCKET ConnectSocket);
	/**
	*	Funktionen som ska köras om och om igen i en tråd och analyserar alla meddelanden från servern.
	*	@param[in]	*seq	Variabeln som lagrar vilket sekvensnummer för meddelanden.
	*	@param[in]	*localClientID	Variabel som lagrar den lokala klientens ID.
	*	@param[in]	*startPosition	Startposition för den lokala klienten.
	*	@param[in]	*clients		Vektorn som lagrar alla klienterna.
	*	@param[in]	*updateFlag		Variabeln som används för att signalera när uppdatering behövs.
	*/
	void operator()(int *seq, int *localClientID, Coordinate *startPosition, std::vector<Client*> *clients, bool *updateFlag);

};