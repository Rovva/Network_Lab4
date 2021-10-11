#pragma once
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include "Broadcaster.h"
#include "Client.h"
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif 


#pragma comment(lib,"ws2_32.lib") //Winsock Library

class ReaderThread {
private:
	SOCKET RecvSocket;
public:
	ReaderThread(SOCKET socket);
	int createID(std::vector<Client*> clients);
	Coordinate createPosition(std::vector<Client*> clients);
	Coordinate checkMove(int id, Coordinate newPosition, std::vector<Client*> clients);
	void operator()(Broadcaster *broad);
};