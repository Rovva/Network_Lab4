#include "ReaderThread.h"

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

            //std::cout << "id " << msgHead->id << " length " << msgHead->length << " seq " << msgHead->seq_no << std::endl;
            if (msgHead->type == Join) {
                JoinMsg* joinMsg = (JoinMsg*)recvbuf;
                if (*localClientID == -1) {
                    std::cout << "Joining server.\n";
                    *localClientID = joinMsg->head.id;
                    while (*localClientID == -1) {

                    }
                    Coordinate cord;
                    cord.x = -200;
                    cord.y = -200;
                    Client* client;
                    client = new Client(joinMsg->head.id, cord);
                    clients->push_back(client);
                    std::cout << "Player ID is: " << *localClientID << std::endl;
                }
                
            } else if (msgHead->type == Change) {
                std::cout << "Change detected.\n";
                ChangeMsg* chgMsg = (ChangeMsg*)msgHead;

                if (chgMsg->type == NewPlayer) {
                    std::cout << "New player detected.\n";
                    NewPlayerMsg* newPlayer = (NewPlayerMsg*)chgMsg;
                    bool clientFound = false;
                    if (newPlayer->msg.head.id != *localClientID) {
                        std::cout << "Enemy has joined the server. ID: " << newPlayer->msg.head.id << "\n";
                        for (int i = 0; i < clients->size(); i++) {
                            if (newPlayer->msg.head.id == clients->at(i)->getClientID()) {
                                clientFound = true;
                                break;
                            }
                        }
                        if (clientFound == false) {
                            Coordinate cord;
                            cord.x = -200;
                            cord.y = -200;
                            Client* client;
                            client = new Client(newPlayer->msg.head.id, cord);
                            clients->push_back(client);
                        }
                    }
                } else if (chgMsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected.\n";
                    NewPlayerPositionMsg* newPlayer = (NewPlayerPositionMsg*)recvbuf;
                        //std::cout << "Player ID: " << newPlayer->msg.head.id << " X: " << newPlayer->pos.x << "Y: " << newPlayer->pos.y << "\n";
                        //*localClientID = newplayer->msg.head.id;
                        //*startPosition = newplayer->pos;
                    for (int i = 0; i < clients->size(); i++) {
                        if (clients->at(i)->getClientID() == newPlayer->msg.head.id) {
                            clients->at(i)->changeCoordinate(newPlayer->pos);
                            std::cout << "Player ID: " << clients->at(i)->getClientID() << " X: " << clients->at(i)->getPosition().x << " Y: " << clients->at(i)->getPosition().y << "\n";
                        }
                    }
                }
            }
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}