#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif

class SocketSetup {
private:
	int RecvSocket;
	sockaddr_in server;
public:
	SocketSetup();
	int getSocket();
	struct sockaddr_in getServer();
};
