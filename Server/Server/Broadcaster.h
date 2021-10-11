#pragma once
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <thread>
#include "Client.h"

class Broadcaster {
private:
	std::vector<Client*>* clients;
public:
	Broadcaster(std::vector<Client*>* clientVector);
	std::vector<Client*>* getClients();
	void SendMessageToAll(char* buffer, int size);
};
