#pragma once
#ifndef MESSAGES_H

#define MESSAGES_H

#include "Messages.h"

#endif 
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <iostream>

class WriterThread {
private:
	SOCKET ConnectSocket;
public:
	WriterThread(SOCKET socket);
	void operator()();
	void sendJoin();
	void sendLeave(int id);
	void sendMoveEvent(int id, Coordinate position, int *seq);
};