#include "Broadcaster.h"

Broadcaster::Broadcaster(std::vector<Client*>* clientVector) {
	clients = clientVector;
}

std::vector<Client*>* Broadcaster::getClients() {
	return clients;
}

void Broadcaster::SendMessageToAll(char* buffer, int size) {
	std::cout << "Clients: " << clients->size();
	for (int i = 0; i < clients->size(); i++) {
		std::cout << "Sending message to ID: " << i+1 << "\n";
		send(clients->at(i)->getSocket(), buffer, size, 0);
	}
}