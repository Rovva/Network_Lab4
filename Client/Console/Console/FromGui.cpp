#include "FromGui.h"
#define BUFLEN 512	// Max length of buffer
#define PORT 4445	// The port on which to listen for incoming data

FromGui::FromGui() {
	WSADATA wsa;

	// Nulla minnet där "server" lagras.
	ZeroMemory(&server, sizeof server);

	// Initiera WinSock.
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Skapa en socket.
	if ((RecvSocket = socket(AF_INET6, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	server.sin6_family = AF_INET6;
	server.sin6_port = htons(PORT);
	inet_pton(PF_INET6, "::1", &server.sin6_addr);

	// Binda socketen som skapats tidigare.
	if (bind(RecvSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void FromGui::operator()(int* localClientID, int *seq, std::vector<Client*>* clients, WriterThread writer) {
	// Denna variabel lagrar det tecken som tas emot från GUI.
	char buf[1];
	// newPos och oldPos används för att beräkna den nya positionen för den lokala klienten.
	Coordinate newPos, oldPos;
	oldPos.x = 0; oldPos.y = 0;
	newPos.x = 0; newPos.y = 0;

	slen = sizeof(server);
	
	while (1)
	{
		fflush(stdout);

		// Nulla minnet där "buf" lagras eftersom det kan finns gammal data där.
		ZeroMemory(&buf, sizeof buf);

		// Lyssna efter UDP trafik från localhost.
		if ((recv_len = recvfrom(RecvSocket, buf, BUFLEN, 0, (struct sockaddr*)&server, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		// Skriva ut värdet som togs emot för att debugga.
		printf("Data: %i\n", (char) buf[0]);

		// Gå igenom alla klienter i vektorn för att hitta den position den lokala klienten har.
		for (int i = 0; i < clients->size(); i++) {
			std::cout << "Client: " << clients->at(i)->getClientID() << " is in the list. X: "
				<< clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
			if (clients->at(i)->getClientID() == *localClientID) {
				oldPos = clients->at(i)->getPosition();
			}
		}
		// 4 innebär vänster som på numpaden. Loopa igenom alla klienter för att hitta den
		// gammla positionen och beräkna den nya som skickas som ett meddelande till servern.
		if (buf[0] == 4) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x - 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
			
		// 6 innebär höger (numpad).
		} else if (buf[0] == 6) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x + 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		// 8 innebär upp (numpad).
		} else if (buf[0] == 8) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x;
					newPos.y = oldPos.y + 1;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		// 2 innebär neråt (numpad).
		} else if (buf[0] == 2) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x;
					newPos.y = oldPos.y - 1;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		}
	}
}