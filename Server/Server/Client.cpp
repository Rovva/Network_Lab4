#include "Client.h"

Client::Client(int id, SOCKET socket) {
	clientID = id;
	//position = pos;
	clientSocket = socket;
}

int Client::getClientID() {
	return clientID;
}

void Client::setPosition(Coordinate newPosition) {
	position = newPosition;
}

Coordinate Client::getPosition() {
	return position;
}

SOCKET Client::getSocket() {
	return clientSocket;
}