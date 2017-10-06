#ifndef PREDATOR_H_
#define PREDATOR_H_

#include "FCMPredator.h"
#include "Sim.h"

class Ecosystem;

class Predator: public Sim {

private:

	static const float FirstPregnancyEneryWaste = 0.05; //MRE RandomGoodGene//mother spent more energy than child get in first pregnancy
	static const float MaxSOBFemale = 50;
	static const float MaxSOBMale = 50;
	bool killedInFight;
public:
	static const unsigned char nbGenes=6;

	FCMPredator model;					//-- Predator's concepts map

	Position StrengthStrongestPreyCellClosePos;
	float StrengthStrongestPreyCellClose;
	//
	Position StrengthWeakestPreyCellClosePos;
	float StrengthWeakestPreyCellClose;
	// For move to strongest prey
	float StrengthStrongestPreyClose;
	int StrengthStrongestPreyCloseIndex;
	// For move to weakest prey
	float StrengthWeakestPreyClose;
	int StrengthWeakestPreyCloseIndex;
	// For move to strongest predator
	float StrengthStrongestPredatorClose;
	int StrengthStrongestPredatorCloseIndex;

	vector<int> friendsClose;			//-- Number of nearby predators
	vector<int> friendsCloseDistance;	//-- Distance to nearby predators
	vector<int> opponentsClose;			//-- Number of nearby prey
	vector<int> opponentsCloseDistance;	//-- Distance to nearby prey
	vector<Position> foodClose;			//-- Location of nearby food
	vector<int> foodCloseDistance;		//-- Distance to nearby food

	Predator();
	virtual ~Predator();
    // 1st generation individuals
	Predator(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float energy, float energy_max, float speed, short speed_max, short age, short age_max, short vision, short repAge, float distanceEvol, float stateBirth, FCMPredator *model, short genderVal, short persuasionVal = 0); //Add Gender /MRE/
	// new born individuals
	Predator(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float speed, short age, float distanceEvol, FCMPredator *model, short genderVal); //Add Gender /MRE/

	FCMPredator * getFCM();
	Position act(vector<Predator> * nouveauNe, Ecosystem * eco, short);

	float get_mutate_amount( Ecosystem * eco);
	void reprod(Predator *, vector<Predator> *, Ecosystem *);
	void updateNew(Ecosystem *);
	void act_driver(vector<Predator> *, Ecosystem *, short);

	void getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco, Position * pTemp);

	bool getKilledInFight() 		{ return killedInFight ;};
	void setKilledInFight(bool b) 	{ killedInFight = b ;};


};

#endif /* PREDATOR_H_ */
