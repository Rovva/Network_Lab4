// This class handles sending messages to the server.

#include "WriterThread.h"

// Constructor that takes and stores the socket needed for communicating with the server.
WriterThread::WriterThread(SOCKET socket) {
	ConnectSocket = socket;
}

// The function for sending a Join message.
void WriterThread::sendJoin() {
    int sendMsg = 0;
    // Fill the message with the data needed for a Join message.
    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    // Send the message to the server.
    sendMsg = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Join message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Function for sending a Leave message.
void WriterThread::sendLeave(int id) {
    int sendMsg = 0;
    // Store the local clients ID in the Leave message.
    LeaveMsg leaveMsg;
    leaveMsg.head.id = id;
    leaveMsg.head.length = sizeof(leaveMsg);
    leaveMsg.head.type = Leave;
    // Send the message to the server.
    sendMsg = send(ConnectSocket, (char*)&leaveMsg, sizeof(leaveMsg), 0);
    printf("Leave message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Function for sending a request to move the clients position.
void WriterThread::sendMoveEvent(int id, Coordinate position, int *seq) {
    int sendMsg = 0, newSeq = 0;

    MoveEvent move;
    move.event.type = Move;
    // Store the local clients ID in the Move message and calculate the sequence number.
    move.event.head.id = id;
    move.event.head.length = sizeof(move);
    newSeq = *seq;
    move.event.head.seq_no = newSeq++;
    move.event.head.type = Event;
    // Store the position and set direction to 0.
    move.pos = position;
    move.dir.x = 0;
    move.dir.y = 0;
    // Send the message to the server.
    sendMsg = send(ConnectSocket, (char*)&move, sizeof(move), 0);
    printf("Move event message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}