#ifndef FCMPREY_H_
#define FCMPREY_H_

//-- If uncomment TWO_RESOURCES -> world has to kind of Grass
//#define TWO_RESOURCES

#include "FCM.h"

class Ecosystem;
class Predator;
class Prey;

using namespace std;

class FCMPrey :  public FCM{
public:

	/*******
	*New FCM
	********/
#ifdef TWO_RESOURCES

	const static int PredClose 									= 0;
	const static int PredFar 									= 1;
	const static int FoodClose 									= 2;
	const static int FoodFar 									= 3;
	const static int FoodClose2 								= 4;
	const static int FoodFar2 									= 5;
	const static int FriendClose 								= 6;
	const static int FriendFar 									= 7;
	const static int EnergyLow 									= 8;
	const static int EnergyHigh 								= 9;
	const static int FoodLocalHigh 								= 10;
	const static int FoodLocalLow 								= 11;
	const static int FoodLocalHigh2 							= 12;
	const static int FoodLocalLow2 								= 13;
	const static int PartnerLocalYes 							= 14;
	const static int PartnerLocalNo 							= 15;
	const static int StrengthPreyCellLocalHigh					= 16;	//A.G
	const static int StrengthPreyCellLocalLow					= 17;	//A.G
	const static int AgeLow										= 18;	//A.G
	const static int AgeHigh									= 19;	//A.G
	const static int StrengthStrongestPreyCellDistanceLow		= 20;	//A.G
	const static int StrengthStrongestPreyCellDistanceHigh		= 21;	//A.G
	const static int StrengthStrongestPreyLocalLow				= 22;	//A.G
	const static int StrengthStrongestPreyLocalHigh				= 23;	//A.G
	const static int StrengthLow								= 24;	//A.G
	const static int StrengthHigh								= 25;	//A.G

	const static int Fear 										= 26;
	const static int Hunger 									= 27;
	const static int SearchPartner 								= 28;
	const static int Curiosity       							= 29;
	const static int Sedentary 									= 30;
	const static int Satisfaction 								= 31;
	const static int Nuisance 									= 32;
	const static int Escape 									= 33;
	const static int SearchFood 								= 34;
	const static int SearchFood2 								= 35;
	const static int Socialize 									= 36;
	const static int Exploration 								= 37;
	const static int Wait 										= 38;
	const static int Eat 										= 39;
	const static int Eat2 										= 40;
	const static int Reproduce 									= 41;
	const static int Move2StrongestPrey							= 42;	//A.G
	const static int Move2StrongestPreyCell						= 43;	//A.G
	const static int Move2WeakestPreyCell						= 44;	//A.G
	//const static int Decision 			= 32;

	const static int NbActions		    = 21;

	const static int EscapeAction						= 1;
	const static int SearchFoodAction					= 2;
	const static int SearchFoodFailAction				= 3;
	const static int SearchFood2Action					= 4;
	const static int SearchFoodFail2Action				= 5;
	const static int SocializeAction					= 6;
	const static int SocializeFailAction				= 7;
	const static int ExplorationAction					= 8;
	const static int WaitAction							= 9;
	const static int EatAction							= 10;
	const static int EatFailAction						= 11;
	const static int Eat2Action							= 12;
	const static int EatFail2Action						= 13;
	const static int ReproduceAction					= 14;
	const static int ReproduceFailAction				= 15;
	const static int Move2StrongestPreyAction			= 16;	//A.G
	const static int Move2StrongestPreyFailAction		= 17;	//A.G
	const static int Move2StrongestPreyCellAction		= 18;	//A.G
	const static int Move2StrongestPreyCellFailAction	= 19;	//A.G
	const static int Move2WeakestPreyCellAction			= 20;	//A.G
	const static int Move2WeakestPreyCellFailAction		= 21;	//A.G


#else

	/*******
	*OLD FCM
	********/

	const static int PredClose 									= 0;
	const static int PredFar 									= 1;
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
	const static int StrengthLow								= 20;	//A.G
	const static int StrengthHigh								= 21;	//A.G
	const static int Fear 										= 22;
	const static int Hunger 									= 23;
	const static int SearchPartner 								= 24;
	const static int Curiosity	 	   						    = 25;
	const static int Sedentary 									= 26;
	const static int Satisfaction 								= 27;
	const static int Nuisance 									= 28;
	const static int Escape 									= 29;
	const static int SearchFood 								= 30;
	const static int Socialize 									= 31;
	const static int Exploration 								= 32;
	const static int Wait 										= 33;
	const static int Eat 										= 34;
	const static int Reproduce 									= 35;
	const static int Move2StrongestPrey							= 36;	//A.G
	const static int Move2StrongestPreyCell						= 37;	//A.G
	const static int Move2WeakestPreyCell						= 38;	//A.G
//	const static int Decision 			= 26;

	const static int NbActions			= 17;

	const static int EscapeAction						= 1;
	const static int SearchFoodAction					= 2;
	const static int SearchFoodFailAction				= 3;
	const static int SocializeAction					= 4;
	const static int SocializeFailAction				= 5;
	const static int ExplorationAction					= 6;
	const static int WaitAction							= 7;
	const static int EatAction							= 8;
	const static int EatFailAction						= 9;
	const static int ReproduceAction					= 10;
	const static int ReproduceFailAction				= 11;
	const static int Move2StrongestPreyAction			= 12;	//A.G
	const static int Move2StrongestPreyFailAction		= 13;	//A.G
	const static int Move2StrongestPreyCellAction		= 14;	//A.G
	const static int Move2StrongestPreyCellFailAction	= 15;	//A.G
	const static int Move2WeakestPreyCellAction			= 16;	//A.G
	const static int Move2WeakestPreyCellFailAction		= 17;	//A.G

#endif

	static short nbSens;						//-- Number of concepts about direction
	static short nbConcepts;					//-- Number of concepts about emotions
	static short nbMoteursDep;
	static short nbMoteursFix;
	static short nbNodes;

	static vector<vector<float> > fuzzyfication_params1;
	static vector<vector<float> > fuzzyfication_params2;
	static void static_init (short nbSens, short nbConcepts, short nbMoteursDep, short nbMoteursFix);

	Prey * prey;

	FCMPrey();
    FCMPrey(Prey *p);
    FCMPrey(vector<vector<FCM_TYPE> > * chart, vector<float> *activations, Prey *p);
	virtual ~FCMPrey();

	void updateConcepts();
	int choose_action(Ecosystem *eco);

	short percept_prey(Ecosystem * eco);
	void set_prey(Prey *prey);

};


#endif /* FCMPREY_H_ */
