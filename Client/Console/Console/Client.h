/**
* @file Client.h
* @brief Klassen för att hålla reda på ID och position för en klient.
* @author Christoffer Rova
* @version 1.0
* @date 2021-10-26
*/

#pragma once
#ifndef MESSAGES_H

#define MESSAGES_H

#include "Messages.h"

#endif
 
class Client {
private:
	int clientID;
	Coordinate clientPosition;
public:
	/**
	*	Konstruktorn för Client.
	*	@param[in]	id Vad för id som Client objektet ska ha.
	*	@param[in]	position Vilken position objektet ska ha.
	*/
	Client(int id, Coordinate position);
	/**
	* 	Returnera ID för klienten.
	* 	@param[out]	clientID
	*/
	int getClientID();
	/**
	*	Returnera Coordinate värdet klienten har.
	*	@param[out]	clientPosition
	*/
	Coordinate getPosition();
	/**
	*	Ändra klientens Coordinate värde.
	*	@param[in]	position
	*/
	void changeCoordinate(Coordinate position);
};