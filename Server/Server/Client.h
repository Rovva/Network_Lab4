#pragma once

#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif
#include <thread>

class Client {
private:
	int clientID;
	Coordinate position;
	SOCKET clientSocket;
public:
	Client(int id, SOCKET socket);
	int getClientID();
	Coordinate getPosition();
	void setPosition(Coordinate newPosition);
	SOCKET getSocket();
};