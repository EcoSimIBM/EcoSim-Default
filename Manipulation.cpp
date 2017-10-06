/*
 * Manipulation.cpp
 *  -
 */

#include "Manipulation.h"

//#include <vector>


using namespace std;

Manipulation::~Manipulation() {

}

Manipulation::Manipulation() {

}


string Manipulation::vtos(float value) {
	char str[50]  = "";
	sprintf(str, "%.4f", value);
	return str;
}

string Manipulation::vtos(double value) {
	char str[50] = "";
	sprintf(str, "%.4lf", value);
	return str;
}

string Manipulation::vtos(short value) {
	char str[50]  = "";
	sprintf(str, "%hd", value);
	return str;
}

string Manipulation::vtos(int value) {
	char str[50]  = "";
	sprintf(str, "%d", value);
	return str;
}

string Manipulation::vtos(long int value) {
	char str[50]  = "";
	sprintf(str, "%ld", value);
	return str;
}

string Manipulation::vtos(unsigned long int value) {
	char str[50]  = "";
	sprintf(str, "%lu", value);
	return str;
}

string Manipulation::vtos(long double value) {
	char str[50]  = "";
	sprintf(str, "%Lf", value);
	return str;
}



void Manipulation::Tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}
