#include "Broadcaster.h"

Broadcaster::Broadcaster(std::vector<Client*>* clientVector) {
	clients = clientVector;
	seq = 0;
}

// Return the vector clients.
std::vector<Client*>* Broadcaster::getClients() {
	return clients;
}

int Broadcaster::getSeq() {
	return seq;
}

void Broadcaster::incrementSeq() {
	seq++;
}

// This function goes through all the clients in the vector clients
// and sends the message given in the parameters to the sockets
// stored for each client in the vector.
void Broadcaster::SendMessageToAll(char* buffer, int size) {
	//std::cout << " seq in Broadcast is: " << seq;
	for (int i = 0; i < clients->size(); i++) {
		std::cout << "Sending message to ID: " << i+1 << "\n";
		send(clients->at(i)->getSocket(), buffer, size, 0);
	}
}