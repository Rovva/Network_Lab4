#include "FromGui.h"
#define BUFLEN 512	// Max length of buffer
#define PORT 4445	// The port on which to listen for incoming data

FromGui::FromGui() {
	WSADATA wsa;

	// Fill memory locations for server variable.
	ZeroMemory(&server, sizeof server);

	// Initialise winsock.
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Create a socket.
	if ((RecvSocket = socket(AF_INET6, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	// Prepare the sockaddr_in structure.
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(PORT);
	// Specify localhost in IPv6 address.
	inet_pton(PF_INET6, "::1", &server.sin6_addr);

	// Bind the socket.
	if (bind(RecvSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void FromGui::operator()(int* localClientID, int *seq, std::vector<Client*>* clients, WriterThread writer) {
	// char variable buf is used to store data from the GUI.
	char buf[1];
	// newPos and oldPos is used to calculate the new position of the client.
	Coordinate newPos, oldPos;
	oldPos.x = 0; oldPos.y = 0;
	newPos.x = 0; newPos.y = 0;
	while (1)
	{
		fflush(stdout);

		// Zero the memory location for the variable buf as it might contain old data.
		ZeroMemory(&buf, sizeof buf);

		// Listen for UDP traffic from localhost.
		if ((recv_len = recvfrom(RecvSocket, buf, BUFLEN, 0, (struct sockaddr*)&server, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		// Print the data recieved in the form of a integer for debugging purpose.
		printf("Data: %i\n", (char) buf[0]);

		// Go through all the clients in the vector to find the current position for the local client.
		for (int i = 0; i < clients->size(); i++) {
			std::cout << "Client: " << clients->at(i)->getClientID() << " is in the list. X: "
				<< clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
			if (clients->at(i)->getClientID() == *localClientID) {
				oldPos = clients->at(i)->getPosition();
			}
		}
		// 4 means left (numpad). Loop through all the clients and find the local client and
		// then calculate the new position to send to the server.
		if (buf[0] == 4) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x - 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		// 6 means right (numpad).
		} else if (buf[0] == 6) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x + 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		// 8 means up (numpad).
		} else if (buf[0] == 8) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x;
					newPos.y = oldPos.y + 1;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		// 2 means down (numpad).
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