#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <iostream>
class ReaderThread {
private:
	SOCKET ConnectSocket;
public:
	ReaderThread(SOCKET ConnectSocket);
	void operator()(int *seq);

};