#ifndef POSITION_H_
#define POSITION_H_

#include <list>
#include "Direction.cpp"

class Ecosystem;

using namespace std;

class Position {
public:

	int x, y;				// (x, y) coordinates in the environment

	Position();
	virtual ~Position();
	float distance (Position, int , int );
	Direction calculDirection (Position, int , int );
};
#endif /* POSITION_H_ */
