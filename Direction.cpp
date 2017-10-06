#ifndef DIRECTION_CPP_
#define DIRECTION_CPP_

struct Direction {
	public:
		int x, y;
	public:
		Direction inverse() {
			Direction resul;
			resul.x = (-1) * x;
			resul.y = (-1) * y;
			return resul;
		}
};

#endif
