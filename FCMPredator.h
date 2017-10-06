#ifndef FCMPREDATOR_H_
#define FCMPREDATOR_H_

#include <vector>
#include "Position.h"
#include "FCM.h"
class Ecosystem;
class FCM;

class FCMPredator : public FCM {
public:


	const static int PreyClose 									= 0;
	const static int PreyFar 									= 1;
	const static int FoodClose 									= 2;
	const static int FoodFar 									= 3;
	const static int FriendClose 								= 4;
	const static int FriendFar 									= 5;
	const static int EnergyLow 									= 6;
	const static int EnergyHigh 								= 7;
	const static int FoodLocalHigh 								= 8;
	const static int FoodLocalLow 								= 9;
	const static int PartnerLocalYes 							= 10;
	const static int PartnerLocalNo 							= 11;
	const static int StrengthPreyCellLocalLow					= 12;	//A.G
	const static int StrengthPreyCellLocalHigh					= 13;	//A.G
	const static int AgeLow										= 14;	//A.G
	const static int AgeHigh									= 15;	//A.G
	const static int StrengthStrongestPreyCellDistanceLow		= 16;	//A.G
	const static int StrengthStrongestPreyCellDistanceHigh		= 17;	//A.G
	const static int StrengthStrongestPreyLocalLow				= 18;	//A.G
	const static int StrengthStrongestPreyLocalHigh				= 19;	//A.G
	const static int StrengthStrongestPredLocalLow				= 20;	//A.G
	const static int StrengthStrongestPredLocalHigh				= 21;	//A.G
	const static int StrengthLow								= 22;	//A.G
	const static int StrengthHigh								= 23;	//A.G
	const static int ChaseAway									= 24;
	const static int Hunger 									= 25;
	const static int SearchPartner 								= 26;
	const static int Curiosity		 							= 27;
	const static int Sedentary 									= 28;
	const static int Satisfaction 								= 29;
	const static int Nuisance 									= 30;
	const static int HuntPrey									= 31;
	const static int SearchFood 								= 32;
	const static int Socialize 									= 33;
	const static int Exploration 								= 34;
	const static int Wait 										= 35;
	const static int Eat 										= 36;
	const static int Reproduce 									= 37;
	const static int Move2StrongestPredator						= 38;	//A.G
	const static int Move2StrongestPreyDistance					= 39;	//A.G
	const static int Move2WeakestPreyCell						= 40;	//A.G
	const static int Move2WeakestPrey							= 41;	//A.G
//	const static int Decision 									= 38;

	const static int NbActions								= 20;

	const static int HuntPreyAction							= 1;
	const static int HuntPreyFailAction						= 2;
	const static int SearchFoodAction						= 3;
	const static int SearchFoodFailAction					= 4;
	const static int SocializeAction						= 5;
	const static int SocializeFailAction					= 6;
	const static int ExplorationAction						= 7;
	const static int WaitAction								= 8;
	const static int EatAction								= 9;
	const static int EatFailAction							= 10;
	const static int ReproduceAction						= 11;
	const static int ReproduceFailAction					= 12;
	const static int Move2StrongestPredatorAction 			= 13;
	const static int Move2StrongestPredatorFailAction 		= 14;
	const static int Move2StrongestPreyDistanceAction 		= 15;
	const static int Move2StrongestPreyDistanceFailAction 	= 16;
	const static int Move2WeakestPreyCellAction 			= 17;
	const static int Move2WeakestPreyCellFailAction 		= 18;
	const static int Move2WeakestPreyAction 				= 19;
	const static int Move2WeakestPreyFailAction 			= 20;

	static short nbSens;						//-- Number of concepts about direction
	static short nbConcepts;					//-- Number of concepts about emotions
	static short nbMoteursDep;
	static short nbMoteursFix;
	static short nbNodes;

	static vector<vector<float> > fuzzyfication_params1;
	static vector<vector<float> > fuzzyfication_params2;
	static void static_init (short nbSens, short nbConcepts, short nbMoteursDep, short nbMoteursFix);



	Predator * predator;				//-- Self-referencing pointer

	FCMPredator();
    FCMPredator(Predator *p);
    FCMPredator(vector<vector<FCM_TYPE> > * chart, vector<float> *activations, Predator *p);
	virtual ~FCMPredator();


	//FCMPredator (short n1, short n2, short n3, short n4, vector<vector<float> > cart, vector<vector<float> > concept, int n, Predator * me);
	void updateConcepts();
	int choose_action(Ecosystem *eco);

	short percept_predator (Ecosystem * eco);
	void set_predator(Predator *predator);

};
//vector<float> FCMPredator::action_number;	// Current concepts about the world
//vector<vector<float> > FCMPredator::fuzzyfication_params1;	// Current concepts about the world
//vector<vector<float> > FCMPredator::fuzzyfication_params2;	// Current concepts about the world

#endif /* FCMPREDATOR_H_ */

