#include "ReaderThread.h"
#include <stdio.h>
#include <iostream>

ReaderThread::ReaderThread(SOCKET Socket) {
    ConnectSocket = Socket;
}

void ReaderThread::operator()(int *seq, int *localClientID, Coordinate *startPosition) {
    int recvbuflen = 512;
    int iResult;

    char recvbuf[512];
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            MsgHead* msghead;
            printf("Bytes received: %d\n", iResult);
            msghead = (MsgHead*)recvbuf;
            *seq = msghead->seq_no;
            if (msghead->type == Change) {
                std::cout << "Change detected\n";
                ChangeMsg* chgmsg = (ChangeMsg*)msghead;

                if (chgmsg->type == NewPlayer) {
                    std::cout << "New player detected\n";
                }
                if (chgmsg->type == NewPlayerPosition) {
                    std::cout << "New player position detected\n";
                    NewPlayerPositionMsg* newplayer = (NewPlayerPositionMsg*)chgmsg;
                    std::cout << "X: " << newplayer->pos.x << "Y: " << newplayer->pos.y << "\n";
                    *localClientID = newplayer->msg.head.id;
                    *startPosition = newplayer->pos;
                }
            }
            //clientId = msghead->id;
            std::cout << "ID: " << msghead->id << "\n";
            std::cout << "Length: " << msghead->length << "\n";
            std::cout << "SEQ: " << msghead->seq_no << "\n";
            std::cout << "Type: " << msghead->type << "\n";
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}