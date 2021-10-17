#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <thread>
#include "Broadcaster.h"
#include "Client.h"
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif

class ReaderThread {
private:
	int RecvSocket;
	int clientID;
public:
	ReaderThread(int socket);
	int createID(std::vector<Client*> *clients);
	Coordinate createPosition(std::vector<Client*> *clients);
	Coordinate checkMove(int id, Coordinate newPosition, std::vector<Client*> *clients);
	void removeClient(int id, std::vector<Client*> *clients);
	void operator()(Broadcaster *broad);
};
