#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#ifndef MESSAGES_H

#define MESSAGES_H

#include "Messages.h"

#endif 

class ReaderThread {
private:
	SOCKET ConnectSocket;
public:
	ReaderThread(SOCKET ConnectSocket);
	void operator()(int *seq, int *localClientID, Coordinate *startPosition);

};