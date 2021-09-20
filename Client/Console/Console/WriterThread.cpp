// Klassen för att skicka meddelanden till servern.

#include "WriterThread.h"

// Konstruktor som tar emot en Socket som lagras lokalt i objektet.
WriterThread::WriterThread(SOCKET socket) {
	ConnectSocket = socket;
}

// Funktion för att skicka ett Join-meddelande.
void WriterThread::sendJoin() {
    int sendMsg = 0;
    // Fylla meddelandet med data som behövs till ett Joinmeddelande.
    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
    // Skicka meddelandet.
    sendMsg = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Join message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Funktion för att skicka ett Leave meddelande.
void WriterThread::sendLeave(int id) {
    int sendMsg = 0;
    // Lagra den lokala klients ID i meddelandet.
    LeaveMsg leaveMsg;
    leaveMsg.head.id = id;
    leaveMsg.head.length = sizeof(leaveMsg);
    leaveMsg.head.type = Leave;

    sendMsg = send(ConnectSocket, (char*)&leaveMsg, sizeof(leaveMsg), 0);
    printf("Leave message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Funktion för att skicka en förfrågan om att byta position.
void WriterThread::sendMoveEvent(int id, Coordinate position, int *seq) {
    int sendMsg = 0, newSeq = 0;

    MoveEvent move;
    move.event.type = Move;
    // Lagra den lokala klientens ID och beräkna Seq som ska skickas
    move.event.head.id = id;
    move.event.head.length = sizeof(move);
    newSeq = *seq;
    move.event.head.seq_no = newSeq++;
    move.event.head.type = Event;
    // Lagra position och sätta direction till 0.
    move.pos = position;
    move.dir.x = 0;
    move.dir.y = 0;
    // Skicka meddelandet till servern.
    sendMsg = send(ConnectSocket, (char*)&move, sizeof(move), 0);
    printf("Move event message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}