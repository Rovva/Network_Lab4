#include "ReaderThread.h"
#include <stdio.h>
#include <iostream>
#include <vector>

ReaderThread::ReaderThread(SOCKET Socket) {
    ConnectSocket = Socket;
    std::cout << "Starting ReaderThread." << std::endl;
}

void ReaderThread::operator()(int *seq, int *localClientID, Coordinate *startPosition) {
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

            std::cout << "id " << msgHead->id << " length " << msgHead->length << " seq " << msgHead->seq_no << std::endl;
            if (msgHead->type == Join) {
                JoinMsg* joinMsg = (JoinMsg*)recvbuf;
                if (*localClientID == -1) {
                    std::cout << "Joining server.\n";
                    *localClientID = joinMsg->head.id;
                    std::cout << "Player ID is: " << *localClientID << std::endl;
                }
            }
            if (msgHead->type == Change) {
                std::cout << "Change detected.\n";
                ChangeMsg* chgMsg = (ChangeMsg*)msgHead;

                if (chgMsg->type == NewPlayer) {
                    std::cout << "New player detected.\n";
                    NewPlayerMsg* newPlayer = (NewPlayerMsg*)chgMsg;
                    
                }
                
                if (chgMsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected.\n";
                    NewPlayerPositionMsg* newplayer = (NewPlayerPositionMsg*)chgMsg;
                    if (newplayer->msg.head.id == *localClientID) {
                        std::cout << "X: " << newplayer->pos.x << "Y: " << newplayer->pos.y << "\n";
                        *localClientID = newplayer->msg.head.id;
                        *startPosition = newplayer->pos;
                    }
                }
            }
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}