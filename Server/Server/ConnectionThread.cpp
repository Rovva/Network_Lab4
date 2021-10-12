#include "ConnectionThread.h"

// Constructor takes a socket and sockaddr srv which are needed for connecting
// and accepting connections from clients.
ConnectionThread::ConnectionThread(SOCKET *socket, sockaddr_in srv) {
	RecvSocket = *socket;
	server = srv;
}

// This is the main thread that handles all the connections.
void ConnectionThread::operator()(Broadcaster *broad) {
	SOCKET tmpSocket;
	socklen_t addrSize = sizeof(server);

	while (1) {
		// When a connection is accepted, the thread creates a ReaderThread obeject
		// in a thread that handles this specific connection and input/output.
		if (tmpSocket = accept(RecvSocket, (struct sockaddr*)&server, &addrSize)) {
			std::thread clientThread(ReaderThread(tmpSocket), broad);
			clientThread.detach();
		}
	}
}