# Makefile for Writing Make Files Example
# efter # kommer kommentar
# variabler
CXX = g++
CXXFLAGS = -Wall -pthread -std=c++11 -I.
#-oterm –pthread
run: Server.o ConnectionThread.o Broadcaster.o ReaderThread.o SocketSetup.o Client.o
	$(CXX) $(CXXFLAGS) -o run Server.o ConnectionThread.o Broadcaster.o ReaderThread.o SocketSetup.o Client.o

clean:
	-rm *.o run
#Server.o: Server.cpp
#ConnectionThread.o: ConnectionThread.cpp ConnectionThread.h
#Broadcaster.o: Broadcaster.cpp Broadcaster.h
#ReaderThread.o: ReaderThread.cpp ReaderThread.h
#SocketSetup.o: SocketSetup.cpp SocketSetup.h
#Client.o: Client.cpp Client.h

