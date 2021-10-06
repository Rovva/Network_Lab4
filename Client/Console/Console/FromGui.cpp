#include "FromGui.h"
#define BUFLEN 512	//Max length of buffer
#define PORT 4445	//The port on which to listen for incoming data

FromGui::FromGui() {
	WSADATA wsa;

	slen = sizeof(si_other);

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

	si_other.sin6_family = AF_INET6;
	si_other.sin6_port = htons(PORT);
	inet_pton(PF_INET6, "::1", &si_other.sin6_addr);

	//Bind
	if (bind(RecvSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

void FromGui::operator()() {
	char buf[1024];
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(RecvSocket, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//printf("Received packet from %s:%d\n", inet_ntoa(si_other), ntohs(si_other.sin6_port));
		printf("Data: %s\n", buf);
	}
}