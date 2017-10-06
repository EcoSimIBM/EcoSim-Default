/*
 * Species.cpp
 *  -
 */

#include <cmath>

#include "Species.h"
#include "FCM.h"

int Species::numSpeciesPredMax = 0;					//-- Max number of predator species
int Species::numSpeciesPreyMax = 0;					//-- Max number of prey species

using namespace std;


Species::~Species() {

}

Species::Species() {

}

Species::Species(int n, int nb, FCM *fcm) {
	num = n;
	nbMembers = nb;
	FCMAvg = *fcm;
}

// M.M
Species::Species(int n, int nb, FCM *fcm, PhysicalGenome* phGenome) {
	num = n;
	nbMembers = nb;
	FCMAvg = *fcm;
	phGenomeAvg = *phGenome;
}

FCM * Species::getFCMAvg() {
	return &FCMAvg;
}

// M.M
PhysicalGenome* Species::getGenomeAvg(){
	return &phGenomeAvg;
}

int Species::getNbMembers() {
	return nbMembers;
}

int Species::getNum() {
	return num;
}

int Species::getParent() {
	return parent;
}

void Species::setNbMembers(int input) {
	nbMembers = input;
}

void Species::setNum(int input) {
	num = input;
}

void Species::setParent(int input) {
	parent = input;
}

void Species::incNbMembers() {
	nbMembers++;
}

