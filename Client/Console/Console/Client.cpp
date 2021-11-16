#include "Client.h"

Client::Client(int id, Coordinate position) {
	clientID = id;
	clientPosition = position;
}

int Client::getClientID() {
	return clientID;
}

Coordinate Client::getPosition() {
	return clientPosition;
}

void Client::changeCoordinate(Coordinate position) {
	clientPosition = position;
}