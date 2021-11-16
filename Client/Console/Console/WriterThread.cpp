#include "WriterThread.h"

// Konstruktor som tar emot en socket och lagrar den lokalt.
WriterThread::WriterThread(SOCKET socket) {
	ConnectSocket = socket;
}

// Metoden som skickar ett "Join" meddelande.
void WriterThread::sendJoin() {
    int sendMsg = 0;
	// Fyll meddelandet med data som behövs.
    JoinMsg joinMsg;
    joinMsg.head.id = 0;
    joinMsg.head.length = sizeof(joinMsg);
    joinMsg.head.type = Join;
	
	// Skicka meddelandet till servern.
    sendMsg = send(ConnectSocket, (char*)&joinMsg, sizeof(joinMsg), 0);
    printf("Join message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Metoden som skickar ett "Leave" meddelande till servern.
void WriterThread::sendLeave(int id) {
    int sendMsg = 0;
	// Lagra det id som den lokala klienten har i meddelandet.
    LeaveMsg leaveMsg;
    leaveMsg.head.id = id;
    leaveMsg.head.length = sizeof(leaveMsg);
    leaveMsg.head.type = Leave;
	
	// Skicka meddelandet till servern.
    sendMsg = send(ConnectSocket, (char*)&leaveMsg, sizeof(leaveMsg), 0);
    printf("Leave message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}

// Function for sending a request to move the clients position.
// Metoden som skickar en "MoveEvent" till servern. (Skickar förfrågan om att förflytta klienten)
void WriterThread::sendMoveEvent(int id, Coordinate position, int *seq) {
    int sendMsg = 0, newSeq = 0;

    MoveEvent move;
    move.event.type = Move;
	// Lagra det id som den lokala klienten har i meddelandet och kalkylera sekvensnummret.
    move.event.head.id = id;
    move.event.head.length = sizeof(move);
    newSeq = *seq;
    move.event.head.seq_no = newSeq++;
    move.event.head.type = Event;
	
	// Lagra positionen som skickats som parameter och sätt "direction" till 0.
    move.pos = position;
    move.dir.x = 0;
    move.dir.y = 0;
	
	// Skicka meddelandet till servern.
    sendMsg = send(ConnectSocket, (char*)&move, sizeof(move), 0);
    printf("Move event message sent.\n");
    printf("Bytes Sent: %ld\n", sendMsg);
}