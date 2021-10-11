#pragma once
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
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
	SOCKET RecvSocket;
	sockaddr_in server;
public:
	ConnectionThread(SOCKET *socket, sockaddr_in srv);
	void operator()(Broadcaster *broad);
};