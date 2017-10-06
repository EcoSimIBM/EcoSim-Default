#ifndef MANIPULATION_H_
#define MANIPULATION_H_

#include <sstream>
#include <vector>
#include <stdio.h>

using namespace std;

class Manipulation {
private:

public:

	Manipulation();
	virtual ~Manipulation();

	string vtos(float value);
	string vtos(double value);
	string vtos(short value);
	string vtos(int value);
	string vtos(long int value);
	string vtos(unsigned long int value);
	string vtos(long double value);
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");
};

#endif /* MANIPULATION_H_ */
