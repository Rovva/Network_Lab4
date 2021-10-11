#include "ReaderThread.h"

ReaderThread::ReaderThread(SOCKET socket) {
	RecvSocket = socket;
	clientID = -1;
}

int ReaderThread::createID(std::vector<Client*> *clients) {
	int id = 1;
	if (clients->size() == 0) {
		return id;
	} else {
		bool idFound = true;
		while (idFound) {
			id++;
			idFound = false;
			for (int i = 0; i < clients->size(); i++) {
				if (clients->at(i)->getClientID() == id) {
					idFound = true;
				}
			}
		}
		return id;
	}
}

Coordinate ReaderThread::createPosition(std::vector<Client*> *clients) {
	Coordinate pos;
	pos.x = -100;
	pos.y = -100;
	for (int i = 0; clients->size(); i++) {
		if (clients->at(i)->getPosition().x == pos.x && clients->at(i)->getPosition().y == pos.y) {
			pos.x = pos.x + 1;
			i = 0;
		}
	}
	return pos;
}

Coordinate ReaderThread::checkMove(int id, Coordinate newPosition, std::vector<Client*> *clients) {
	Coordinate oldPosition;
	oldPosition.x = -100;
	oldPosition.y = -100;
	bool empty = false;
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			if (clients->at(i)->getPosition().x == NULL) {
				empty = true;
			}
			if (clients->at(i)->getPosition().y == NULL) {
				empty = true;
			}
		}
	}
	if (empty) {
		oldPosition = createPosition(clients);
	}

	if (newPosition.x < -100 || newPosition.y < -100) {
		std::cout << "Coordinates are less than -100\n";
		return oldPosition;
	}
	bool isOccupied = false;
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getPosition().x == newPosition.x && clients->at(i)->getPosition().y == newPosition.y) {
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

void updateClientPosition(int id, Coordinate pos, std::vector<Client*>* clients) {
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			clients->at(i)->setPosition(pos);
			std::cout << "Updated id: " << id << " with X: " << pos.x << " Y: " << pos.y << "\n";
		}
	}
}

void ReaderThread::removeClient(int id, std::vector<Client*> *clients) {
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			clients->erase(clients->begin() + i);
			std::cout << "Client id: " << id << " is removed.\n";
		}
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
				clientID = createID(broad->getClients());
				join.head.id = clientID;
				join.head.type = Join;
				join.head.length = sizeof(join);

				char* sendbuffer[sizeof(join)];

				memcpy(sendbuffer, (char*)&join, sizeof(join));
				Client* tmpclient = new Client(clientID, RecvSocket);
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
				ChangeMsg playerLeave;
				playerLeave.type = PlayerLeave;
				playerLeave.head.id = msgHead->id;
				playerLeave.head.type = Change;

				broad->SendMessageToAll((char*)&playerLeave, sizeof(playerLeave));

				removeClient(msgHead->id, broad->getClients());
				std::cout << "Removed client id: " << msgHead->id << "\n";
			} else if (msgHead->type == Event) {
				std::cout << "EVENT RECIEVED!\n";
				//recvbuf
				MoveEvent* moveEvent;
				moveEvent = (MoveEvent*)recvbuf;
				
				NewPlayerPositionMsg newPosition;
				newPosition.pos = checkMove(moveEvent->event.head.id, moveEvent->pos, broad->getClients());
				newPosition.msg.type = NewPlayerPosition;
				newPosition.msg.head.id = moveEvent->event.head.id;
				newPosition.msg.head.type = Change;

				std::cout << "Client id: " << newPosition.msg.head.id << " moves to X: " << newPosition.pos.x << " Y: " << newPosition.pos.y << "\n";
				updateClientPosition(newPosition.msg.head.id, newPosition.pos, broad->getClients());

				broad->SendMessageToAll((char*)&newPosition, sizeof(newPosition));
			}
		} else if (iResult == 0) {
			printf("Connection closed\n");
		} else {
			ChangeMsg playerLeave;
			playerLeave.type = PlayerLeave;
			playerLeave.head.id = clientID;
			playerLeave.head.type = Change;

			broad->SendMessageToAll((char*)&playerLeave, sizeof(playerLeave));

			removeClient(clientID, broad->getClients());
			std::cout << "Removed client id: " << clientID << "\n";
		}
			//printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
}