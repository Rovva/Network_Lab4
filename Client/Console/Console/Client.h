#pragma once
#ifndef MESSAGES_H

#define MESSAGES_H

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