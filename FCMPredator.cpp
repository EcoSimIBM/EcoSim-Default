/*
 * FCM.cpp
 *  - The prey and predators fuzzy cognitive map
 *  - Maintains data structures to represent concepts and cognitive charts
 */

#include <iostream>
#include "Ecosystem.h"

using namespace std;

vector<vector<float> > FCMPredator::fuzzyfication_params1;
vector<vector<float> > FCMPredator::fuzzyfication_params2;

short FCMPredator::nbSens;						//-- Number of concepts about direction
short FCMPredator::nbConcepts;					//-- Number of concepts about emotions
short FCMPredator::nbMoteursDep;
short FCMPredator::nbMoteursFix;
short FCMPredator::nbNodes;


void FCMPredator::static_init (short nbSens, short nbConcepts, short nbMoteursDep, short nbMoteursFix){
    FCMPredator::nbSens = nbSens;
    FCMPredator::nbConcepts = nbConcepts;
    FCMPredator::nbMoteursDep = nbMoteursDep;
    FCMPredator::nbMoteursFix = nbMoteursFix;

    FCMPredator::nbNodes = nbSens+nbConcepts+nbMoteursDep+nbMoteursFix;
    fuzzyfication_params1 = vector<vector<float> >(FCMPredator::nbNodes, vector<float>(2));
    fuzzyfication_params2 = vector<vector<float> >(FCMPredator::nbNodes, vector<float>(3));
}


FCMPredator::FCMPredator() {

}
FCMPredator::FCMPredator(vector<vector<FCM_TYPE> > * chart, vector<float> *activations, Predator *p) : FCM(chart, activations){
	predator = p;
}
FCMPredator::FCMPredator(Predator *p) : FCM(nbSens + nbConcepts + nbMoteursDep + nbMoteursFix, nbConcepts + nbMoteursDep + nbMoteursFix){
	predator = p;
}

FCMPredator::~FCMPredator() {

}
void FCMPredator::updateConcepts() {
	vector<float> activations_old=activations;
	float ff, gg;
	for (size_t i = nbSens; i < get_rows(); i++) {
			activations[i] = standardizeContinuous(calcul_node_activation_pure(&activations_old, i), fuzzyfication_params2[i][0], fuzzyfication_params2[i][1], fuzzyfication_params2[i][2]);
			ff = activations[i];
			gg = 0;

	}
}


int FCMPredator::choose_action(Ecosystem *eco){
	//--new: removed
	Random *r = eco->getRandom();
	float max = 0;
	short numMax = 0;
	for (int i = nbSens + nbConcepts; i < nbSens + nbConcepts + nbMoteursDep + nbMoteursFix; i++) {				//-- nbMoteursDep=4, fix=3
		bool change_action = false;
		if (activations[i] > max)
			change_action = true;
		else if (activations[i] == max){
			float test = (float) (r->next(10000) / 10000.0);
			if (test<0.5)
				change_action = true;
		}
		if (change_action == true) {				//-- nbSens+nbConcepts=19
			if (i ==Reproduce) {					//-- Change it Armin
				//if (predator->getAge() >= eco->getAgeReprodPred()) {		//-- If old enough to reproduce
				if (predator->getAge() >= predator->getRepAge()){   // M.M comes from genetic
					max = activations[i];
					numMax = (short)i;
				}
			} else {
				max = activations[i];
				numMax = (short)i;
			}
		}
	}
	SetLargestActivation(max);
	SetLargestActivationIndex(numMax);
	return numMax;
}

short FCMPredator::percept_predator(Ecosystem * eco) {

	vector<int> friends = predator->friendsClose;
	vector<int> friendsDistance = predator->friendsCloseDistance;
	vector<int> prey = predator->opponentsClose;
	vector<int> preyDistance = predator->opponentsCloseDistance;
	vector<Position> food = predator->foodClose;
	vector<int> foodDistance = predator->foodCloseDistance;

	short v = predator->getVision();
	float e = predator->getEnergy();
	float em = predator->getEnergyMax();
	short friendsLocal = 0;
	int mV = eco->getMaxMeat();
	int d;

	if (int(eco->getListCreature(*(predator->getPosition()), 0)->size()) > 0) {  //-- If any prey exists in current cell
		activations[PreyClose] = fuzzy1(0, (v / fuzzyfication_params1[PreyClose][0]), 3 * (v / fuzzyfication_params1[PreyClose][1]));
		activations[PreyFar] = fuzzy2(0, (v / fuzzyfication_params1[PreyFar][0]), 3 * (v / fuzzyfication_params1[PreyFar][1]));
	}
	else {
		if (int(preyDistance.size()) > 0) {
			d = int(preyDistance.size()) - 1; //-- Nearest local Prays
			activations[PreyClose] = fuzzy1((float)preyDistance[d], (v / fuzzyfication_params1[PreyClose][0]), 3 * (v / fuzzyfication_params1[PreyClose][1]));
			activations[PreyFar] = fuzzy2((float)preyDistance[d], (v / fuzzyfication_params1[PreyFar][0]), 3 * (v / fuzzyfication_params1[PreyFar][1]));
		}
		else {
			activations[PreyClose] = 0;
			activations[PreyFar] = 1;
		}
	}

	d = 0; //-- Farthest local Foods
	if (int(foodDistance.size()) > 0) {
		activations[FoodClose] = fuzzy1((float)foodDistance[d], (v / fuzzyfication_params1[FoodClose][0]), 3 * (v / fuzzyfication_params1[FoodClose][1]));
		activations[FoodFar] = fuzzy2((float)foodDistance[d], (v / fuzzyfication_params1[FoodFar][0]), 3 * (v / fuzzyfication_params1[FoodFar][1]));
	}
	else {
		activations[FoodClose] = 0;
		activations[FoodFar] = 1;
	}

	d = 0; //-- Farthest local Friends
	if (int(friendsDistance.size()) > 0) {
		activations[FriendClose] = fuzzy1((float)friendsDistance[d], (v / fuzzyfication_params1[FriendClose][0]), 3 * (v / fuzzyfication_params1[FriendClose][1]));
		activations[FriendFar] = fuzzy2((float)friendsDistance[d], (v / fuzzyfication_params1[FriendFar][0]), 3 * (v / fuzzyfication_params1[FriendFar][1]));
	}
	else {
		activations[FriendClose] = 0;
		activations[FriendFar] = 1;
	}

	//
	activations[AgeLow] = fuzzy1(predator->getAge(), fuzzyfication_params1[AgeLow][0], fuzzyfication_params1[AgeLow][1]);
	activations[AgeHigh] = fuzzy2(predator->getAge(), fuzzyfication_params1[AgeHigh][0], fuzzyfication_params1[AgeHigh][1]);

	//-- Cumulative prey strength locally //RS
	if (int(prey.size()) > 0) {
		activations[StrengthPreyCellLocalLow] = fuzzy1(eco->getSumOfStrengthAndDefence(*(predator->getPosition())), (fuzzyfication_params1[StrengthPreyCellLocalLow][0]), fuzzyfication_params1[StrengthPreyCellLocalLow][1]); //RS
		activations[StrengthPreyCellLocalHigh] = fuzzy2(eco->getSumOfStrengthAndDefence(*(predator->getPosition())), (fuzzyfication_params1[StrengthPreyCellLocalHigh][0]), fuzzyfication_params1[StrengthPreyCellLocalHigh][1]); //RS
	}
	else{
		activations[StrengthPreyCellLocalLow] = 1;
		activations[StrengthPreyCellLocalHigh] = 0;
	}


	//-- Distance to strongest local opponent //RS
	if (int(preyDistance.size()) > 0) {
		Position tempPosition;
		tempPosition.x = predator->getPosition()->x;
		tempPosition.y = predator->getPosition()->y;
		activations[StrengthStrongestPreyCellDistanceLow] = fuzzy1(eco->getDistanceMaxStrengthAndDefencePrey(tempPosition), (v / fuzzyfication_params1[StrengthStrongestPreyCellDistanceLow][0]), 3 * (v / fuzzyfication_params1[StrengthStrongestPreyCellDistanceLow][1]));
		activations[StrengthStrongestPreyCellDistanceHigh] = fuzzy2(eco->getDistanceMaxStrengthAndDefencePrey(tempPosition), (v / fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][0]), 3 * (v / fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][1]));
	}
	else {
		activations[StrengthStrongestPreyCellDistanceLow] = 0;
		activations[StrengthStrongestPreyCellDistanceHigh] = 1;
	}

	//-- Strength of strongest local opponent //RS
	activations[StrengthStrongestPreyLocalLow] = fuzzy1(eco->getMaxStrengthAndDefencePrey(*(predator->getPosition())), (fuzzyfication_params1[StrengthStrongestPreyLocalLow][0]), fuzzyfication_params1[StrengthStrongestPreyLocalLow][1]);
	activations[StrengthStrongestPreyLocalHigh] = fuzzy2(eco->getMaxStrengthAndDefencePrey(*(predator->getPosition())), (fuzzyfication_params1[StrengthStrongestPreyLocalHigh][0]), fuzzyfication_params1[StrengthStrongestPreyLocalHigh][1]);

	//
	activations[StrengthStrongestPredLocalLow] = fuzzy1(eco->getMaxStrength(predator->getGender(), *(predator->getPosition()), 1), (fuzzyfication_params1[StrengthStrongestPredLocalLow][0]), fuzzyfication_params1[StrengthStrongestPredLocalLow][1]);
	activations[StrengthStrongestPredLocalHigh] = fuzzy2(eco->getMaxStrength(predator->getGender(), *(predator->getPosition()), 1), (fuzzyfication_params1[StrengthStrongestPredLocalHigh][0]), fuzzyfication_params1[StrengthStrongestPredLocalHigh][1]);
	//
	activations[StrengthLow] = fuzzy1((predator->getStrength()), (fuzzyfication_params1[StrengthLow][0]), fuzzyfication_params1[StrengthLow][1]);
	activations[StrengthHigh] = fuzzy2((predator->getStrength()), (fuzzyfication_params1[StrengthHigh][0]), fuzzyfication_params1[StrengthHigh][1]);
	//=====================
	
	activations[EnergyLow] = fuzzy1(e, (em / fuzzyfication_params1[EnergyLow][0]), 2.5 * (em / fuzzyfication_params1[EnergyLow][1]));		//RS
	activations[EnergyHigh] = fuzzy2(e, (em / fuzzyfication_params1[EnergyHigh][0]), 2.5 * (em / fuzzyfication_params1[EnergyHigh][1]));  //RS
	
	activations[FoodLocalHigh] = fuzzy2(eco->getLevelMeat(*(predator->getPosition())), (mV / fuzzyfication_params1[FoodLocalHigh][0]), 3 * (mV / fuzzyfication_params1[FoodLocalHigh][1]));
	activations[FoodLocalLow] = fuzzy1(eco->getLevelMeat(*(predator->getPosition())), (mV / fuzzyfication_params1[FoodLocalLow][0]), 3 * (mV / fuzzyfication_params1[FoodLocalLow][1]));

	friendsLocal = int(eco->getListCreature(*(predator->getPosition()), 1)->size()) - 1;
	activations[PartnerLocalYes] = fuzzy2(friendsLocal, (fuzzyfication_params1[PartnerLocalYes][0]), fuzzyfication_params1[PartnerLocalYes][1]);
	activations[PartnerLocalNo] = fuzzy1(friendsLocal, (fuzzyfication_params1[PartnerLocalNo][0]), fuzzyfication_params1[PartnerLocalNo][1]);

	for (int i = nbSens + nbConcepts; i < nbSens + nbConcepts + nbMoteursDep + nbMoteursFix; ++i) {
		activations[i] = 0; //-- Clear Activation level of Motors (wait, eat, reproduce)
	}
	
	for (int i = 0; i < 3; i++) {
		updateConcepts();
	}

	short numMax = choose_action(eco);

	// Action: 0-fruite, 1-researchFood, 2-socialize, 3-explore, 4-stay, 5-eat, 6-reproduce
	return numMax;
}

void FCMPredator::set_predator (Predator *predator){
	this->predator = predator;
}

