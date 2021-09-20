// Klassen f�r att h�lla reda p� ID och position f�r en klient.

#include "Client.h"

Client::Client(int id, Coordinate position) {
	clientID = id;
	clientPosition = position;
}
// Funktion f�r att returnera klientens ID.
int Client::getClientID() {
	return clientID;
}

// Returnera positionen som �r lagrad.
Coordinate Client::getPosition() {
	return clientPosition;
}

// �ndra klientens position.
void Client::changeCoordinate(Coordinate position) {
	clientPosition = position;
}