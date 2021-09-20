#include "WriterThread.h"

WriterThread::WriterThread(SOCKET socket) {
	ConnectSocket = socket;
}

void WriterThread::sendJoin() {
    int sendMsg = 0;
    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    
    sendMsg = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Join message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

void WriterThread::sendLeave(int id) {
    int sendMsg = 0;
    LeaveMsg leaveMsg;
    leaveMsg.head.id = id;
    leaveMsg.head.length = sizeof(leaveMsg);
    leaveMsg.head.type = Leave;

    sendMsg = send(ConnectSocket, (char*)&leaveMsg, sizeof(leaveMsg), 0);
    printf("Leave message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

void WriterThread::sendMoveEvent(int id, Coordinate position, int *seq) {
    int sendMsg = 0, newSeq = 0;
    MoveEvent move;
    move.event.type = Move;

    move.event.head.id = id;
    move.event.head.length = sizeof(move);
    newSeq = *seq;
    move.event.head.seq_no = newSeq++;
    move.event.head.type = Event;

    move.pos = position;
    move.dir.x = 0;
    move.dir.y = 0;

    sendMsg = send(ConnectSocket, (char*)&move, sizeof(move), 0);
    printf("Move event message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}