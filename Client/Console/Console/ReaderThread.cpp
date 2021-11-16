#include "ReaderThread.h"

ReaderThread::ReaderThread(SOCKET Socket) {
    ConnectSocket = Socket;
    std::cout << "Starting ReaderThread." << std::endl;
}

void ReaderThread::operator()(int *seq, int *localClientID, Coordinate *startPosition, std::vector<Client*> *clients, bool *updateFlag) {
    int recvbuflen = 1024;
    int iResult;

    char recvbuf[1024];
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            MsgHead* msgHead;
            printf("Bytes received: %d\n", iResult);
            msgHead = (MsgHead*)recvbuf;
            *seq = msgHead->seq_no;
            std::cout << "Seq is now: " << *seq << "\n";
			
			// Börja med att kolla om meddelandet är en "Join".
            if (msgHead->type == Join) {
                JoinMsg* joinMsg = (JoinMsg*)recvbuf;

				// Om den lokala klientens id är -1 så spara det id som skickats från servern.
                if (*localClientID == -1) {
                    std::cout << "Joining server.\n";
                    *localClientID = joinMsg->head.id;
                    while (*localClientID == -1) {

                    }
					
					// Lagra position som inte är tillåten.
                    Coordinate cord;
                    cord.x = -200;
                    cord.y = -200;

					// Skapa ett klientobjekt och lagra data i den.
                    Client* client;
                    client = new Client(joinMsg->head.id, cord);
                    clients->push_back(client);
                    std::cout << "Local player ID is: " << *localClientID << std::endl;
                }
				
			// Annars om meddelandet från servern är en "Change".
            } else if (msgHead->type == Change) {
                std::cout << "Change detected.\n";
                ChangeMsg* chgMsg = (ChangeMsg*)msgHead;


				// Om "ChangeType" i meddelandet är "NewPlayer".
                if (chgMsg->type == NewPlayer) {
                    std::cout << "New player detected.\n";
                    NewPlayerMsg* newPlayer = (NewPlayerMsg*)recvbuf;

                    bool clientFound = false;

					// Om klient-id i meddelandet från server inte är likadan som den lokala klientens id
					// så gå igenom alla klienterna för att kolla om den redan finns i clients vektorn.
                    if (newPlayer->msg.head.id != *localClientID) {
                        std::cout << "Enemy has joined the server. ID: " << newPlayer->msg.head.id << "\n";
                        for (int i = 0; i < clients->size(); i++) {
                            if (newPlayer->msg.head.id == clients->at(i)->getClientID()) {
                                clientFound = true;
                            }
                        }
						
						// Om klient-id i meddelandet inte kan hittas så skapa en ny klient och lagra 
						// det id.
                        if (clientFound == false) {
                            std::cout << "Couldnt find player, adding in NewPlayer.\n";
                            Coordinate cord;
                            cord.x = -200;
                            cord.y = -200;
                            Client* client;
                            client = new Client(newPlayer->msg.head.id, cord);
                            clients->push_back(client);
                        }
                        *updateFlag = true;
                    }
                // If "ChangeType" is NewPlayerPosition.
				// Om "ChangeType" i meddelandet är "NewPlayerPosition".
                } else if (chgMsg->type == NewPlayerPosition) {
                    NewPlayerPositionMsg* newPlayerPos = (NewPlayerPositionMsg*)recvbuf;
                    std::cout << "New player position detected for " << newPlayerPos->msg.head.id << " .\n";

                    if(newPlayerPos->msg.head.id != *localClientID) {
                        std::cout << "Enemy is moving.\n";
                    }
                    bool playerFound = false;

					// Loopa igenom vektorn med klienter och kolla om någon av klienterna har ett id
					// som finns i meddelandet.
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == newPlayerPos->msg.head.id) {
                            clients->at(i)->changeCoordinate(newPlayerPos->pos);
                            std::cout << "Player ID: " << clients->at(i)->getClientID() << " is at X: " << clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
                            std::cout << "Player ID: " << newPlayerPos->msg.head.id << " should be at X: " << newPlayerPos->pos.x << " Y: " << newPlayerPos->pos.y << "\n";
                            playerFound = true;
                        }
                    }
					
					// Om id från meddelandet inte hittas i vektorn, så skapa en ny klient och lagra i 
					// vektorn.
                    if (playerFound == false) {
                        std::cout << "Couldnt find player. Adding new player in NewPlayerPosition.\n";
                        Client* client;
                        client = new Client(newPlayerPos->msg.head.id, newPlayerPos->pos);
                        clients->push_back(client);
                    }
                    *updateFlag = true;
					
				// Om "ChangeType" i meddelandet är "PlayerLeave".
                } else if (chgMsg->type == PlayerLeave) {
                    std::cout << "Player with ID: " << chgMsg->head.id << " has left the game.\n";

					// Loopa igenom vektorn med klienter och se om en klient med samma id som i meddelandet
					// finns, om det finns så ta bort den klienten.
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == chgMsg->head.id) {
                            clients->erase(clients->begin() + i);
                            std::cout << "Player with ID: " << chgMsg->head.id << " has been removed from the list of players.\n";
                        }
                    }
                    *updateFlag = true;
                }
            } else if (msgHead->type == Event) {
                std::cout << "Event message detected.\n";
            } else if (msgHead->type == Leave) {
                LeaveMsg* playerLeave = (LeaveMsg*)recvbuf;
                for (int i = 0; i < clients->size(); i++) {
                    if (clients->at(i)->getClientID() == playerLeave->head.id) {
                        clients->erase(clients->begin() + i);
                        std::cout << "Client with ID: " << i << " has left the game.\n";
                    }
                }
                *updateFlag = true;
            }
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}