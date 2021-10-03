// This class handles all the data that is sent to all the clients from the server.

#include "ReaderThread.h"

// Constructor that takes and stores the socket.
ReaderThread::ReaderThread(SOCKET Socket) {
    ConnectSocket = Socket;
    std::cout << "Starting ReaderThread." << std::endl;
}

void ReaderThread::operator()(int *seq, int *localClientID, Coordinate *startPosition, std::vector<Client*> *clients, bool *updateFlag) {
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
            
            // Start by checking if the message is a "Join".
            if (msgHead->type == Join) {
                JoinMsg* joinMsg = (JoinMsg*)recvbuf;
                // If the local clients ID is -1 then store the ID sent from the server.
                if (*localClientID == -1) {
                    std::cout << "Joining server.\n";
                    *localClientID = joinMsg->head.id;
                    while (*localClientID == -1) {

                    }
                    // Store invalid positions for the client.
                    Coordinate cord;
                    cord.x = -200;
                    cord.y = -200;
                    // Create a client object and store the data in it.
                    Client* client;
                    client = new Client(joinMsg->head.id, cord);
                    clients->push_back(client);
                    std::cout << "Player ID is: " << *localClientID << std::endl;
                }
            // If the message from server is "Change".
            } else if (msgHead->type == Change) {
                std::cout << "Change detected.\n";
                ChangeMsg* chgMsg = (ChangeMsg*)msgHead;
                // If the "ChageType" is NewPlayer.
                if (chgMsg->type == NewPlayer) {
                    std::cout << "New player detected.\n";
                    NewPlayerMsg* newPlayer = (NewPlayerMsg*)chgMsg;

                    bool clientFound = false;

                    // If the client ID is not the local clients ID so go through all the clients to find it.
                    if (newPlayer->msg.head.id != *localClientID) {
                        std::cout << "Enemy has joined the server. ID: " << newPlayer->msg.head.id << "\n";
                        for (int i = 0; i < clients->size(); i++) {
                            if (newPlayer->msg.head.id == clients->at(i)->getClientID()) {
                                clientFound = true;
                                break;
                            }
                        }
                        // If the client ID cannot be found, then create a new client object and store it in
                        // the vector with other clients.
                        if (clientFound == false) {
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
                } else if (chgMsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected.\n";
                    NewPlayerPositionMsg* newPlayer = (NewPlayerPositionMsg*)recvbuf;

                    bool playerFound = false;

                    // Go through the vector with clients and check if the ID in the message exist.
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == newPlayer->msg.head.id) {
                            clients->at(i)->changeCoordinate(newPlayer->pos);
                            std::cout << "Player ID: " << clients->at(i)->getClientID() << " X: " << clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
                            playerFound = true;
                        }
                    }
                    // If the ID in the message is not found, then create a new client.
                    if (playerFound == false) {
                        Client* client;
                        client = new Client(newPlayer->msg.head.id, newPlayer->pos);
                        clients->push_back(client);
                    }
                    *updateFlag = true;
                // If "ChangeType" is PlayerLeave.
                } else if (chgMsg->type == PlayerLeave) {
                    std::cout << "Player with ID: " << chgMsg->head.id << " has left the game.\n";
                    // Go through the vector with clients and check if the ID in the message exist.
                    // If it does, delete the client.
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