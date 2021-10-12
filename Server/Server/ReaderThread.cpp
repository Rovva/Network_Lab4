#include "ReaderThread.h"

// Constructor that takes a Socket as a parameter and stores it locally.
ReaderThread::ReaderThread(SOCKET socket) {
	RecvSocket = socket;
	clientID = -1;
}

// This function creates an unique id for the client.
int ReaderThread::createID(std::vector<Client*> *clients) {
	int id = 1;
	// If there is no clients then assign the client the id 1.
	if (clients->size() == 0) {
		return id;
	// If there already exists clients in the vector then go through all to
	// assign a unique id to the connected client.
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

// This function creates a position that is not occupied by going through all
// clients and check what position in x-direction is free.
Coordinate ReaderThread::createPosition(std::vector<Client*> *clients) {
	std::cout << "Creating startposition...\n";
	Coordinate pos;
	pos.x = -100;
	pos.y = -100;
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getPosition().x == pos.x && clients->at(i)->getPosition().y == pos.y &&
			clients->at(i)->getClientID() != clientID) {
			pos.x = pos.x + 1;
		}
	}
	return pos;
}

// Function that checks if a move is valid or not.
Coordinate ReaderThread::checkMove(int id, Coordinate newPosition, std::vector<Client*> *clients) {
	// Create a Coordinate variable and assign -100 in both x and y.
	Coordinate oldPosition;
	oldPosition.x = -100;
	oldPosition.y = -100;
	// Loop through all clients and check if the clients ID has any old coordinates.
	// If not, then assign -100, -100.
	bool empty = false;
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			if (clients->at(i)->getPosition().x == NULL) {
				empty = true;
			} else if (clients->at(i)->getPosition().y == NULL) {
				empty = true;
			} else {
				oldPosition = clients->at(i)->getPosition();
			}
		}
	}
	if (empty) {
		oldPosition = createPosition(clients);
	}
	// Check the coordinates so that they do not move outside the coordinates.
	if (newPosition.x < -100 || newPosition.y < -100) {
		std::cout << "Coordinates are less than -100.\n";
		return oldPosition;
	}
	if (newPosition.x > 100 || newPosition.y > 100) {
		std::cout << "Coordinates are greater than 100.\n";
	}
	// Then check if the new coordinates are already occupied by looping through
	// all the clients and if it is, then set isOccupied flag as true.
	bool isOccupied = false;
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getPosition().x == newPosition.x && clients->at(i)->getPosition().y == newPosition.y) {
			isOccupied = true;
			break;
		}
	}
	// If the spot is already occupied then send the old coordinates to clients.
	if (isOccupied) {
		return oldPosition;
	} else {
		return newPosition;
	}
}

// Loop through all the clients and assign new coordinates to the client with the id
// "id".
void updateClientPosition(int id, Coordinate pos, std::vector<Client*>* clients) {
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			clients->at(i)->setPosition(pos);
			std::cout << "Updated id: " << id << " with X: " << pos.x << " Y: " << pos.y << "\n";
		}
	}
}

// Remove a client with the id specified in the integer id.
void ReaderThread::removeClient(int id, std::vector<Client*> *clients) {
	for (int i = 0; i < clients->size(); i++) {
		if (clients->at(i)->getClientID() == id) {
			clients->erase(clients->begin() + i);
			std::cout << "Client id: " << id << " is removed.\n";
		}
	}
}

// The main loop in the ReadThread thread.
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

			// When a client joins, create id and position and send it to all the clients.
			if (msgHead->type == Join) {
				std::cout << "JOIN RECIEVED!\n";
				broad->incrementSeq();
				std::cout << "seq in readerthread is now: " << broad->getSeq();
				JoinMsg join;
				// Create a unique id.
				clientID = createID(broad->getClients());
				join.head.id = clientID;
				// Assign message type to the message.
				join.head.type = Join;
				broad->incrementSeq();
				join.head.seq_no = broad->getSeq();
				// Specify the length of the message.
				join.head.length = sizeof(join);

				// Create a new client object with the id.
				Client* tmpclient = new Client(clientID, RecvSocket);
				// Add it to the vector.
				broad->getClients()->push_back(tmpclient);
				std::cout << "Sending ID: " << join.head.id << "\n";
				std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";
				// Broadcast the message to all the clients.
				broad->SendMessageToAll((char*)&join, sizeof(join));

				// Create a new message that alerts all clients that there is a 
				// new one connected.
				ChangeMsg newPlayer;
				newPlayer.head.id = clientID;
				broad->incrementSeq();
				newPlayer.head.seq_no = broad->getSeq();
				newPlayer.head.type = Change;
				newPlayer.type = NewPlayer;

				std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";

				// Broadcast the New player message to everyone.
				broad->SendMessageToAll((char*)&newPlayer, sizeof(newPlayer));

				// Create a new message that specify the newly connected client
				// position.
				NewPlayerPositionMsg newPosition;
				// Create a unique position.
				newPosition.pos = createPosition(broad->getClients());
				std::cout << "Populating header...\n";
				// Populate headers with types and the clients id.
				newPosition.msg.type = NewPlayerPosition;
				newPosition.msg.head.id = clientID;
				broad->incrementSeq();
				newPosition.msg.head.seq_no = broad->getSeq();
				newPosition.msg.head.type = Change;
				std::cout << "Sending startposition to client id: " << clientID << " \n";
				// Update the client position in the clients vector.
				updateClientPosition(newPosition.msg.head.id, newPosition.pos, broad->getClients());
				
				std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";
				// Send the mesage to everyone connected.
				broad->SendMessageToAll((char*)&newPosition, sizeof(newPosition));

			} else if (msgHead->type == Leave) {
				std::cout << "LEAVE RECIEVED!\n";
				broad->incrementSeq();
				// When a client sends a leave message to the server then
				// create a message that the client has left the game.
				ChangeMsg playerLeave;
				playerLeave.type = PlayerLeave;
				playerLeave.head.id = msgHead->id;
				broad->incrementSeq();
				playerLeave.head.seq_no = broad->getSeq();
				playerLeave.head.type = Change;

				std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";
				broad->SendMessageToAll((char*)&playerLeave, sizeof(playerLeave));

				removeClient(msgHead->id, broad->getClients());
				std::cout << "Removed client id: " << msgHead->id << "\n";
			} else if (msgHead->type == Event) {
				std::cout << "EVENT RECIEVED!\n";
				broad->incrementSeq();
				// When a client sends a move event, the server will check the
				// new coordinates so that they are valid and then update the
				// position for the client in the clients vector and broadcast
				// the new coordinates to all other clients connected.
				MoveEvent* moveEvent;
				moveEvent = (MoveEvent*)recvbuf;
				
				NewPlayerPositionMsg newPosition;
				newPosition.pos = checkMove(moveEvent->event.head.id, moveEvent->pos, broad->getClients());
				newPosition.msg.type = NewPlayerPosition;
				newPosition.msg.head.id = moveEvent->event.head.id;
				broad->incrementSeq();
				newPosition.msg.head.seq_no = broad->getSeq();
				newPosition.msg.head.type = Change;

				std::cout << "Client id: " << newPosition.msg.head.id << " moves to X: " << newPosition.pos.x << " Y: " << newPosition.pos.y << "\n";
				updateClientPosition(newPosition.msg.head.id, newPosition.pos, broad->getClients());

				std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";
				broad->SendMessageToAll((char*)&newPosition, sizeof(newPosition));
			}
		} else if (iResult == 0) {
			printf("Connection closed\n");
		} else {
			// If the client closes the connection by exiting the clientprogram
			// the server will send a message to all other clients that the id
			// bound to that client has left the game.
			ChangeMsg playerLeave;
			playerLeave.type = PlayerLeave;
			playerLeave.head.id = clientID;
			broad->incrementSeq();
			playerLeave.head.seq_no = broad->getSeq();
			playerLeave.head.type = Change;

			std::cout << "SEQ in ReaderThread is now: " << broad->getSeq() << "\n";
			broad->SendMessageToAll((char*)&playerLeave, sizeof(playerLeave));

			// Remove the disconnected client from the clients vector.
			removeClient(clientID, broad->getClients());
			std::cout << "Removed client id: " << clientID << "\n";
		}
	} while (iResult > 0);
}