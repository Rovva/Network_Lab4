// Denna klass har hand om all data som kommer från servern till klienterna.

#include "ReaderThread.h"

// Kontruktor som tar emot en Socket.
ReaderThread::ReaderThread(SOCKET Socket) {
    ConnectSocket = Socket;
    std::cout << "Starting ReaderThread." << std::endl;
}

void ReaderThread::operator()(int *seq, int *localClientID, Coordinate *startPosition, std::vector<Client*> *clients) {
    int recvbuflen = 512;
    int iResult;

    char recvbuf[512];
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            MsgHead* msgHead;
            printf("Bytes received: %d\n", iResult);
            msgHead = (MsgHead*)recvbuf;
            *seq = msgHead->seq_no;
            
            // Börjar med att kolla om meddelandet är Join.
            if (msgHead->type == Join) {
                JoinMsg* joinMsg = (JoinMsg*)recvbuf;
                // Om lockala klientens ID är -1 så sätt ID till vad som skickades från servern.
                if (*localClientID == -1) {
                    std::cout << "Joining server.\n";
                    *localClientID = joinMsg->head.id;
                    while (*localClientID == -1) {

                    }
                    // Ställ in positioner till ogiltligt värde.
                    Coordinate cord;
                    cord.x = -200;
                    cord.y = -200;
                    // Skapa ett client objekt och lagra data i det.
                    Client* client;
                    client = new Client(joinMsg->head.id, cord);
                    clients->push_back(client);
                    std::cout << "Player ID is: " << *localClientID << std::endl;
                }
            // Ifall meddelandet från servern är Change.
            } else if (msgHead->type == Change) {
                std::cout << "Change detected.\n";
                ChangeMsg* chgMsg = (ChangeMsg*)msgHead;
                // Om meddelandet är New Player.
                if (chgMsg->type == NewPlayer) {
                    std::cout << "New player detected.\n";
                    NewPlayerMsg* newPlayer = (NewPlayerMsg*)chgMsg;
                    // Boolean clientFound är false om klient ID inte redan finns i vektorn.
                    bool clientFound = false;
                    // Om klient ID inte är den lokala klientens ID så sök efter den.
                    if (newPlayer->msg.head.id != *localClientID) {
                        std::cout << "Enemy has joined the server. ID: " << newPlayer->msg.head.id << "\n";
                        for (int i = 0; i < clients->size(); i++) {
                            if (newPlayer->msg.head.id == clients->at(i)->getClientID()) {
                                clientFound = true;
                                break;
                            }
                        }
                        // Om klient ID inte hittas så skapa ett Client objekt och lagra i klientvektorn.
                        if (clientFound == false) {
                            Coordinate cord;
                            cord.x = -200;
                            cord.y = -200;
                            Client* client;
                            client = new Client(newPlayer->msg.head.id, cord);
                            clients->push_back(client);
                        }
                    }
                // Om meddelandet är New player position.
                } else if (chgMsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected.\n";
                    NewPlayerPositionMsg* newPlayer = (NewPlayerPositionMsg*)recvbuf;
                    // Boolean som är false om klientens ID inte hittas.
                    bool playerFound = false;
                    // Gå igenom vektorn med klienter och se ifall ID från meddelandet finns.
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == newPlayer->msg.head.id) {
                            clients->at(i)->changeCoordinate(newPlayer->pos);
                            std::cout << "Player ID: " << clients->at(i)->getClientID() << " X: " << clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
                            playerFound = true;
                        }
                    }
                    // Ifall klientens ID inte hittas så skapa en ny.
                    if (playerFound == false) {
                        Client* client;
                        client = new Client(newPlayer->msg.head.id, newPlayer->pos);
                        clients->push_back(client);
                    }
                // Ifall meddelandet är Player leave.
                } else if (chgMsg->type == PlayerLeave) {
                    std::cout << "Player with ID: " << chgMsg->head.id << " has left the game.\n";
                    // Leta reda på om klientens ID finns i vektorn med klienter och om den finns så ta bort klienten.
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == chgMsg->head.id) {
                            clients->erase(clients->begin() + i);
                            std::cout << "Player with ID: " << chgMsg->head.id << " has been removed from the list of players.\n";
                        }
                    }
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
            }
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}