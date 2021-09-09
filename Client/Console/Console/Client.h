#pragma once
#ifndef MY_HEADER_FILE_IS_INCLUDED

#define MY_HEADER_FILE_IS_INCLUDED

#include "Messages.h"

#endif 
class Client {
private:
	int clientID;
	Coordinate clientPosition;
public:
	Client(int id, Coordinate position);
	int getClientID();
	Coordinate getPosition();
	void changeCoordinate(Coordinate position);
};