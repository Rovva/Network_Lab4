#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#ifndef MY_HEADER_FILE_IS_INCLUDED

#define MY_HEADER_FILE_IS_INCLUDED

#include "Messages.h"

#endif 

class ReaderThread {
private:
	SOCKET ConnectSocket;
public:
	ReaderThread(SOCKET ConnectSocket);
	void operator()(int *seq, int *localClientID, Coordinate *startPosition);

};