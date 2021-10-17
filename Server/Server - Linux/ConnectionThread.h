#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <thread>
#include "Client.h"
#include "ReaderThread.h"
#include "Broadcaster.h"
#ifndef MESSAGES_H
#define MESSAGES_H
#include "Messages.h"
#endif

class ConnectionThread {
private:
	int RecvSocket;
	sockaddr_in server;
public:
	ConnectionThread(int *socket, sockaddr_in srv);
	void operator()(Broadcaster *broad, int *seq);
};
