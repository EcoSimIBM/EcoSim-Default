#ifndef SPECIES_H_
#define SPECIES_H_

#include "FCM.h"
#include "PhysicalGenome.h"

class Predator;
class Prey;

using namespace std;

class Species {
private:
	int num;										//-- Species number
	int nbMembers;									//-- Number of individuals in this species
	FCM FCMAvg;										//-- Average FCM in this species
	PhysicalGenome phGenomeAvg;						//-- Average Genome in this species
	short parent;

public:
	static int numSpeciesPredMax;					//-- Max number of predator species
	static int numSpeciesPreyMax;					//-- Max number of prey species

	vector<Prey *> preyMembers;
	vector<Predator *> predMembers;

	Species();
	virtual ~Species();

	Species(int n, int nb, FCM *fcm);
	Species(int n, int nb, FCM *fcm, PhysicalGenome *genome);  // M.M

	int getNbMembers();

	FCM * getFCMAvg();
	PhysicalGenome* getGenomeAvg();

	int getNum();
	int getParent();

	void setNbMembers(int);
	void incNbMembers();
	void setNum(int);
	void setParent(int);

};

#endif /* SPECIES_H_ */
