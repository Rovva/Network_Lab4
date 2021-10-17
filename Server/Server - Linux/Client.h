#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif
#include <thread>

class Client {
private:
	int clientID;
	Coordinate position;
	int clientSocket;
public:
	Client(int id, int socket);
	int getClientID();
	Coordinate getPosition();
	void setPosition(Coordinate newPosition);
	int getSocket();
};
