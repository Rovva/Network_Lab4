#pragma once
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif 

class SocketSetup {
private:
	WSADATA wsa;
	SOCKET RecvSocket;
	sockaddr_in server;
public:
	SocketSetup();
	SOCKET getSocket();
	struct sockaddr_in getServer();
};