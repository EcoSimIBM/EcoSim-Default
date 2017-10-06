#ifndef PREY_H_
#define PREY_H_

#include "FCMPrey.h"
#include "Sim.h"

class Ecosystem;

class Prey: public Sim {

private:

	static const float FirstPregnancyEneryWaste = 0.05; //MRE RandomGoodGene//mother spent more energy than child get in first pregnancy
	static const float MaxSOBFemale = 38.5;
	static const float MaxSOBMale = 38.5;
	Position StrengthStrongestCellPos;
	float StrengthStrongestCell;
	Position StrengthWeakestCellPos;
	float StrengthWeakestCell;
	float StrengthStrongestIndivClose;
	int StrengthStrongestIndivCloseIndex;

public:
	static const unsigned char nbGenes=8;
	FCMPrey model;							//-- Prey's map of concepts


	vector<int> friendsClose;				//-- Number of nearby prey
	vector<int> friendsCloseDistance;		//-- Distance to nearby prey
	vector<int> opponentsClose;				//-- Number of nearby predators
	vector<int> opponentsCloseDistance;	    //-- Distance to nearby predators
	vector<Position> foodClose;				//-- Location of nearby food
	vector<int> foodCloseDistance;			//-- Distance to nearby food

#ifdef TWO_RESOURCES
	vector<Position> foodClose2;			//-- Armin
	vector<int> foodCloseDistance2;			//-- Armin
#endif

	Prey();
	virtual ~Prey();

    // for first generation individuals
	Prey(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float energy, float energy_max, float speed, short speed_max, short age, short age_max, short vision, short repAge, float distanceEvol, float stateBirth, FCMPrey *model, short genderVal, float defence=0, float coopDefence=0, short persuasionVal = 0);//Add Gender /MRE/
	// for new born individuals
	Prey(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float speed, short age, float distanceEvol, FCMPrey *model, short genderVal);//Add Gender /MRE/

	FCMPrey * getFCM();

	float get_mutate_amount( Ecosystem * eco);
	void reprod(Prey *, vector<Prey> *, Ecosystem *);
	void updateNew(Ecosystem *);
	void act_driver(vector<Prey> *, Ecosystem *, short);
	Position * getStrengthStrongestCellPos() 	{ return &StrengthStrongestCellPos; };
	float getStrengthStrongestCell()			{ return StrengthStrongestCell; };
	Position * getStrengthWeakestCellPos()		{ return &StrengthWeakestCellPos; };
	float getStrengthWeakestCell()				{ return StrengthWeakestCell; };
	float getStrengthStrongestIndivClose()		{ return StrengthStrongestIndivClose; };
	int getStrengthStrongestIndivCloseIndex()	{ return StrengthStrongestIndivCloseIndex; };
	//
	void setStrengthStrongestCellPos(Position p) 		{ StrengthStrongestCellPos = p; };
	void setStrengthStrongestCell(float s)				{ StrengthStrongestCell = s; };
	void setStrengthWeakestCellPos(Position p)			{ StrengthWeakestCellPos = p; };
	void setStrengthWeakestCell(float s)				{ StrengthWeakestCell = s; };
	void setStrengthStrongestIndivClose(float s)		{ StrengthStrongestIndivClose = s; };
	void setStrengthStrongestIndivCloseIndex(int index)	{ StrengthStrongestIndivCloseIndex = index; };


#ifdef TWO_RESOURCES
	void getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco);
	void partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco);
	void partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco);
	void partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco);
	void partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco);
	void lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, int * nbFoods2, Ecosystem * eco, Position * pTemp);
#else
	void getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco);
	void lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco, Position * pTemp);
#endif

	Position act(vector<Prey> * , Ecosystem *, short );

};


#endif /* PREY_H_ */
