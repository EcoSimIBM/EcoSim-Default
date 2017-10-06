/*
 * Position.cpp
 *  - Provides each individual with an (x, y)
 *    position in the environment and methods for calculating
 *    the distance between points
 */

#include <cmath>
#include "Position.h"
#include "Ecosystem.h"

using namespace std;

Position::Position() {

}

Position::~Position() {

}

/*
 * Returns the distance between the calling Position and Position 'p'
 */
float Position::distance(Position p, int Width, int Height) {
	int diffxs, diffys, diffxd, diffyd;

	if (x > p.x) {
		diffxs = x - p.x;
		diffxd = x - p.x - Width;
	} else {
		diffxs = p.x - x;
		diffxd = p.x - x - Width;
	}
	if (y > p.y) {
		diffys = y - p.y;
		diffyd = y - p.y - Height;
	} else {
		diffys = p.y - y;
		diffyd = p.y - y - Height;
	}
	if (abs(diffxs) > abs(diffxd)) {
		diffxs = diffxd;
	}
	if (abs(diffys) > abs(diffyd)) {
		diffys = diffyd;
	}

	float res = float (diffxs * diffxs + diffys * diffys);
	res = sqrt(res);
	return res;
}

/*
 * Returns the direction of movement between the calling Position
 * and Position 'arrivee'
 */
Direction Position::calculDirection(Position arrivee, int Width, int Height) {

	//-- Meisam 11 March 2011 -> Relative position with preserving the diretion
	Direction resul;
	int diffxs, diffys;

	diffxs = arrivee.x - x;
	diffys = arrivee.y - y;

	if (diffxs > 0){
		if ( diffxs > (Width - diffxs) )
			diffxs = -(Width - diffxs);
	}else
		if ( diffxs < (- Width - diffxs) )
			diffxs = - Width - diffxs;

	if (diffys > 0){
		if ( diffys > (Height - diffys) )
			diffys = -(Height - diffys);
	}else
		if ( diffys < (- Height - diffys) )
			diffys = - Height - diffys;

	resul.x = diffxs;
	resul.y = diffys;
	return resul;

}

