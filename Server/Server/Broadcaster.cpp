#include "Broadcaster.h"

Broadcaster::Broadcaster(std::vector<Client*>* clientVector) {
	clients = clientVector;
}

// Return the vector clients.
std::vector<Client*>* Broadcaster::getClients() {
	return clients;
}

// This function goes through all the clients in the vector clients
// and sends the message given in the parameters to the sockets
// stored for each client in the vector.
void Broadcaster::SendMessageToAll(char* buffer, int size) {
	for (int i = 0; i < clients->size(); i++) {
		std::cout << "Sending message to ID: " << i+1 << "\n";
		send(clients->at(i)->getSocket(), buffer, size, 0);
	}
}