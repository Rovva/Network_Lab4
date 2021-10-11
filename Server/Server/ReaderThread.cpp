#include "ReaderThread.h"

ReaderThread::ReaderThread(SOCKET socket) {
	RecvSocket = socket;
}

int ReaderThread::createID(std::vector<Client*> clients) {
	int id = 1;
	if (clients.size() == 0) {
		return id;
	} else {
		bool idFound = true;
		while (idFound) {
			id++;
			idFound = false;
			for (int i = 0; i < clients.size(); i++) {
				if (clients.at(i)->getClientID() == id) {
					idFound = true;
				}
			}
		}
		return id;
	}
}

Coordinate ReaderThread::createPosition(std::vector<Client*> clients) {
	Coordinate pos;
	pos.x = -100;
	pos.y = -100;
	for (int i = 0; clients.size(); i++) {
		if (clients.at(i)->getPosition().x == pos.x && clients.at(i)->getPosition().y == pos.y) {
			pos.x = pos.x + 1;
			i = 0;
		}
	}
	return pos;
}

Coordinate ReaderThread::checkMove(int id, Coordinate newPosition, std::vector<Client*> clients) {
	Coordinate oldPosition;
	bool empty;
	for (int i = 0; i < clients.size(); i++) {
		if (clients.at(i)->getClientID() == id) {
			if (clients.at(i)->getPosition().x == NULL) {
				empty = true;
			}
			if (clients.at(i)->getPosition().y == NULL) {
				empty = true;
			}
		}
	}
	if (empty) {
		oldPosition = createPosition(clients);
	}

	bool isOccupied = false;
	for (int i = 0; i < clients.size(); i++) {
		if (clients.at(i)->getPosition().x == oldPosition.x && clients.at(i)->getPosition().x) {
			isOccupied = true;
			break;
		}
	}
	if (isOccupied) {
		return oldPosition;
	} else {
		return newPosition;
	}
}

void ReaderThread::operator()(Broadcaster *broad) {
	std::cout << "ReaderThread starting...\n";
	int recvbuflen = 1024;
	int iResult;

	char recvbuf[1024];
	do {
		iResult = recv(RecvSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			MsgHead* msgHead;
			printf("Bytes received: %d\n", iResult);
			msgHead = (MsgHead*)recvbuf;
			if (msgHead->type == Join) {
				std::cout << "JOIN RECIEVED!\n";
				JoinMsg join;
				join.head.id = createID(*broad->getClients());
				join.head.type = Join;
				join.head.length = sizeof(join);

				char* sendbuffer[sizeof(join)];

				memcpy(sendbuffer, (char*)&join, sizeof(join));
				Client* tmpclient = new Client(join.head.id, RecvSocket);
				broad->getClients()->push_back(tmpclient);
				std::cout << "Clients: " << broad->getClients()->size() << "\n";
				std::cout << "Sending ID: " << join.head.id << "\n";
				broad->SendMessageToAll((char*)&join, sizeof(join));

				ChangeMsg newPlayer;
				newPlayer.head.id = join.head.id;
				newPlayer.head.type = Change;
				newPlayer.type = NewPlayer;

				broad->SendMessageToAll((char*)&newPlayer, sizeof(newPlayer));

			} else if (msgHead->type == Leave) {
				std::cout << "LEAVE RECIEVED!\n";
			} else if (msgHead->type == Event) {
				std::cout << "EVENT RECIEVED!\n";
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
}