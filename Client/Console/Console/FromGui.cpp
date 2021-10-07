#include "FromGui.h"
#define BUFLEN 512	//Max length of buffer
#define PORT 4445	//The port on which to listen for incoming data

FromGui::FromGui() {
	WSADATA wsa;

	slen = sizeof(server);
	ZeroMemory(&server, sizeof server);
	ZeroMemory(&si_other, sizeof si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//Create a socket
	if ((RecvSocket = socket(AF_INET6, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Prepare the sockaddr_in structure
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(PORT);
	inet_pton(PF_INET6, "::1", &server.sin6_addr);

	//si_other.sin6_family = AF_INET6;
	//si_other.sin6_port = htons(PORT);
	//server.sin6_addr = in6addr_any;

	//Bind
	if (bind(RecvSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void FromGui::operator()(int* localClientID, int *seq, std::vector<Client*>* clients, WriterThread writer) {
	char buf[1];
	Coordinate newPos, oldPos;
	oldPos.x = 0; oldPos.y = 0;
	newPos.x = 0; newPos.y = 0;
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		//memset(buf, '\0', BUFLEN);
		ZeroMemory(&buf, sizeof buf);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(RecvSocket, buf, BUFLEN, 0, (struct sockaddr*)&server, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//printf("Received packet from %s:%d\n", inet_ntoa(si_other), ntohs(si_other.sin6_port));
		printf("Data: %i\n", (char) buf[0]);

		// Go through all the clients in the vector to find the current position for the local client.
		for (int i = 0; i < clients->size(); i++) {
			std::cout << "Client: " << clients->at(i)->getClientID() << " is in the list. X: "
				<< clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
			if (clients->at(i)->getClientID() == *localClientID) {
				oldPos = clients->at(i)->getPosition();
			}
		}

		if (buf[0] == 4) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x - 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		}
		else if (buf[0] == 6) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x + 1;
					newPos.y = oldPos.y;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		}
		else if (buf[0] == 8) {
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == *localClientID) {
					newPos.x = oldPos.x;
					newPos.y = oldPos.y + 1;
					writer.sendMoveEvent(*localClientID, newPos, seq);
				}
			}
		}
		else if (buf[0] == 2) {
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