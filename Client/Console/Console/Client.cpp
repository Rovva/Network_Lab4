// Klassen för att hålla reda på ID och position för en klient.

#include "Client.h"

Client::Client(int id, Coordinate position) {
	clientID = id;
	clientPosition = position;
}
// Funktion för att returnera klientens ID.
int Client::getClientID() {
	return clientID;
}

// Returnera positionen som är lagrad.
Coordinate Client::getPosition() {
	return clientPosition;
}

// Ändra klientens position.
void Client::changeCoordinate(Coordinate position) {
	clientPosition = position;
}