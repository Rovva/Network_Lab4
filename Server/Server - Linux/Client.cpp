#include "Client.h"

// Constructor for a client takes two parameters, id and socket,
// id is the clients id and socket is the newly created socket from
// ConnectionThread.
Client::Client(int id, int socket) {
	clientID = id;
	clientSocket = socket;
}

// Return the client id.
int Client::getClientID() {
	return clientID;
}

// Sets a new position with the given Coordinate in parameter.
void Client::setPosition(Coordinate newPosition) {
	position = newPosition;
}

// Returns the clients position in a Coordinate type variable.
Coordinate Client::getPosition() {
	return position;
}

// Returns the stored socket specific for this client.
int Client::getSocket() {
	return clientSocket;
}
