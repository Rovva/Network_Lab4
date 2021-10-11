#include "ConnectionThread.h"

ConnectionThread::ConnectionThread(SOCKET *socket, sockaddr_in srv) {
	RecvSocket = *socket;
	server = srv;
}

void ConnectionThread::operator()(Broadcaster *broad) {
	SOCKET tmpSocket;
	socklen_t addrSize = sizeof(server);

	while (1) {
		if (tmpSocket = accept(RecvSocket, (struct sockaddr*)&server, &addrSize)) {
			std::cout << "ACCEPTED\n";
			std::thread test(ReaderThread(tmpSocket), broad);
			test.detach();
			//ReaderThread* reader = new ReaderThread(tmpSocket);
			//std::thread readerThread(&reader);
			//Client* client = new Client(id, pos, tmpSocket);
			//clients->push_back(client);
		}
	}
}