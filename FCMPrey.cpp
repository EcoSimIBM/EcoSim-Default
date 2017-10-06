/*
 * FCM.cpp
 *  - The prey and predators fuzzy cognitive map
 *  /- Maintains data structures to represent concepts and cognitive charts
 */

#include <iostream>
#include "Ecosystem.h"

using namespace std;

vector<vector<float> > FCMPrey::fuzzyfication_params1;
vector<vector<float> > FCMPrey::fuzzyfication_params2;

short FCMPrey::nbSens;						//-- Number of concepts about direction
short FCMPrey::nbConcepts;					//-- Number of concepts about emotions
short FCMPrey::nbMoteursDep;
short FCMPrey::nbMoteursFix;
short FCMPrey::nbNodes;


void FCMPrey::static_init (short nbSens, short nbConcepts, short nbMoteursDep, short nbMoteursFix){
    FCMPrey::nbSens = nbSens;
    FCMPrey::nbConcepts = nbConcepts;
    FCMPrey::nbMoteursDep = nbMoteursDep;
    FCMPrey::nbMoteursFix = nbMoteursFix;
    FCMPrey::nbNodes = nbSens+nbConcepts+nbMoteursDep+nbMoteursFix;
    fuzzyfication_params1 = vector<vector<float> >(FCMPrey::nbNodes, vector<float>(2));
    fuzzyfication_params2 = vector<vector<float> >(FCMPrey::nbNodes, vector<float>(3));
}

FCMPrey::FCMPrey() {

}
FCMPrey::FCMPrey(vector<vector<FCM_TYPE> > * chart, vector<float> *activations, Prey *p) : FCM(chart, activations){
	prey = p;
}
FCMPrey::FCMPrey(Prey *p) : FCM(nbSens + nbConcepts + nbMoteursDep + nbMoteursFix, nbConcepts + nbMoteursDep + nbMoteursFix){
	prey = p;
}

FCMPrey::~FCMPrey() {

}

void FCMPrey::updateConcepts() {
	vector<float> activations_old=activations;

	for (size_t i = nbSens; i < get_rows(); i++) {
			activations[i] = standardizeContinuous(calcul_node_activation_pure(&activations_old, i), fuzzyfication_params2[i][0], fuzzyfication_params2[i][1], fuzzyfication_params2[i][2]);
	}
}


int FCMPrey::choose_action(Ecosystem *eco){
	//--new: removed
	float max = 0;
	short numMax = 0;
	Random *r = eco->getRandom();
	for (int i = nbSens + nbConcepts; i < nbSens + nbConcepts+nbMoteursDep + nbMoteursFix; i++) {				//-- nbMoteursDep=4, fix=3
		bool change_action = false;
		if (activations[i] > max)
			change_action = true;
		else if (activations[i] == max){
			float test = (float) (r->next(10000) / 10000.0);
			if (test<0.5)
				change_action = true;
		}

		if (change_action == true) {
			if (i == Reproduce) {					//-- Change it Armin
				//if (prey->getAge() >= eco->getAgeReprodPrey()) {		//-- If old enough to reproduce
				if (prey->getAge() >= prey->getRepAge() ){ // M.M because Rep Age now is opart of genome.
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


short FCMPrey::percept_prey(Ecosystem * eco) {

	vector<int> friends = prey->friendsClose;
	vector<int> friendsDistance = prey->friendsCloseDistance;
	vector<int> pred = prey->opponentsClose;
	vector<int> predDistance = prey->opponentsCloseDistance;
	vector<Position> food = prey->foodClose;
	vector<int> foodDistance = prey->foodCloseDistance;
	short v = prey->getVision();
	float e = prey->getEnergy();
	float em = prey->getEnergyMax();
	short friendsLocal = 0;

#ifdef TWO_RESOURCES
	vector<Position> food2 = prey->foodClose2;
	vector<int> foodDistance2 = prey->foodCloseDistance2;
	int mH2 = eco->getMaxGrass2();
#endif
	int mH = eco->getMaxGrass();
	int d = 0;
    if ( int (eco->getListCreature(*(prey->getPosition()), 1)->size()) > 0 ){ //-- If any predator exists in current cell
		activations[PredClose] = fuzzy1(0, (v / fuzzyfication_params1[PredClose][0]), 3 * (v / fuzzyfication_params1[PredClose][1]));
		activations[PredFar] = fuzzy2(0, (v / fuzzyfication_params1[PredFar][0]), 3 * (v / fuzzyfication_params1[PredFar][1]));
    } else {
        if ( int (predDistance.size()) > 0 ) {
			d = int (predDistance.size()) - 1; //-- Nearest local Predator (Enemies)
			activations[PredClose] = fuzzy1(float (predDistance[d]), (v / fuzzyfication_params1[PredClose][0]), 3 * (v / fuzzyfication_params1[PredClose][1]));
			activations[PredFar] = fuzzy2(float (predDistance[d]), (v / fuzzyfication_params1[PredFar][0]), 3 * (v / fuzzyfication_params1[PredFar][1]));
        } else {
			activations[PredClose] = 0;
			activations[PredFar] = 1;
        }
    }

	d = 0; //-- Farthest local Foods
    if ( int (foodDistance.size()) > 0 ) {
		activations[FoodClose] = fuzzy1(float (foodDistance[d]), (v / fuzzyfication_params1[FoodClose][0]), 3 * (v / fuzzyfication_params1[FoodClose][1]));
		activations[FoodFar] = fuzzy2(float (foodDistance[d]), (v / fuzzyfication_params1[FoodFar][0]), 3 * (v / fuzzyfication_params1[FoodFar][1]));
    } else {
		activations[FoodClose] = 0;
		activations[FoodFar] = 1;
    }


#ifdef TWO_RESOURCES
	d = 0; //-- Farthest local Foods
    if ( int (foodDistance2.size()) > 0 ) {
		activations[FoodClose2] = fuzzy1(float (foodDistance2[d]), (v / fuzzyfication_params1[FoodClose2][0]), 3 * (v / fuzzyfication_params1[FoodClose2][1]));
		activations[FoodFar2] = fuzzy2(float (foodDistance2[d]), (v / fuzzyfication_params1[FoodFar2][0]), 3 * (v / fuzzyfication_params1[FoodFar2][1]));
    } else {
		activations[FoodClose2] = 0;
		activations[FoodFar2] = 1;
    }

#endif

    d = 0; //-- Farthest local Friends
	if ( int (friendsDistance.size()) > 0 ) {
		activations[FriendClose] = fuzzy1((float)friendsDistance[d], (v / fuzzyfication_params1[FriendClose][0]), 3 * (v / fuzzyfication_params1[FriendClose][1]));
		activations[FriendFar] = fuzzy2((float)friendsDistance[d], (v / fuzzyfication_params1[FriendFar][0]), 3 * (v / fuzzyfication_params1[FriendFar][1]));
	} else {
		activations[FriendClose] = 0;
		activations[FriendFar] = 1;
	}


	// =====    A.G

	activations[StrengthPreyCellLocalLow] = fuzzy1(eco->getSumofStrength(*(prey->getPosition()),0), (fuzzyfication_params1[StrengthPreyCellLocalLow][0]), fuzzyfication_params1[StrengthPreyCellLocalLow][1]);
	activations[StrengthPreyCellLocalHigh] = fuzzy2(eco->getSumofStrength(*(prey->getPosition()),0), (fuzzyfication_params1[StrengthPreyCellLocalHigh][0]), fuzzyfication_params1[StrengthPreyCellLocalHigh][1]);

	activations[AgeLow] = fuzzy1(prey->getAge(), fuzzyfication_params1[AgeLow][0], fuzzyfication_params1[AgeLow][1]);
	activations[AgeHigh] = fuzzy2(prey->getAge(), fuzzyfication_params1[AgeHigh][0], fuzzyfication_params1[AgeHigh][1]);

	activations[StrengthStrongestPreyCellDistanceLow] = fuzzy1(prey->getStrengthStrongestCell(), (fuzzyfication_params1[StrengthStrongestPreyCellDistanceLow][0]), fuzzyfication_params1[StrengthStrongestPreyCellDistanceLow][1]);
	activations[StrengthStrongestPreyCellDistanceHigh] = fuzzy2(prey->getStrengthStrongestCell(), (fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][0]), fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][1]);
	//
	activations[StrengthStrongestPreyLocalLow] = fuzzy1(eco->getMaxStrength(prey->getGender(), *(prey->getPosition()), 0), (fuzzyfication_params1[StrengthStrongestPreyLocalLow][0]), fuzzyfication_params1[StrengthStrongestPreyLocalLow][1]);
	activations[StrengthStrongestPreyLocalHigh] = fuzzy2(eco->getMaxStrength(prey->getGender(), *(prey->getPosition()), 0), (fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][0]), fuzzyfication_params1[StrengthStrongestPreyCellDistanceHigh][1]);
	//
	activations[StrengthLow] = fuzzy1((prey->getStrength()), (fuzzyfication_params1[StrengthLow][0]), fuzzyfication_params1[StrengthLow][1]);
	activations[StrengthHigh] = fuzzy2((prey->getStrength()), (fuzzyfication_params1[StrengthHigh][0]), fuzzyfication_params1[StrengthHigh][1]);


	//====


	activations[EnergyLow] = fuzzy1(e, (em / fuzzyfication_params1[EnergyLow][0]), 3 * (em / fuzzyfication_params1[EnergyLow][1]));
	activations[EnergyHigh] = fuzzy2(e, (em / fuzzyfication_params1[EnergyHigh][0]), 3 * (em / fuzzyfication_params1[EnergyHigh][1]));


	activations[FoodLocalHigh] = fuzzy2(eco->getLevelGrass(*(prey->getPosition())), (mH / fuzzyfication_params1[FoodLocalHigh][0]), 3 * (mH / fuzzyfication_params1[FoodLocalHigh][1]));
    activations[FoodLocalLow] = fuzzy1(eco->getLevelGrass(*(prey->getPosition())), (mH / fuzzyfication_params1[FoodLocalLow][0]), 3 * (mH / fuzzyfication_params1[FoodLocalLow][1]));



#ifdef TWO_RESOURCES
	activations[FoodLocalHigh2] = fuzzy2(eco->getLevelGrass2(*(prey->getPosition())), (mH2 / fuzzyfication_params1[FoodLocalHigh2][0]), 3 * (mH2 / fuzzyfication_params1[FoodLocalHigh2][1]));
    activations[FoodLocalLow2] = fuzzy1(eco->getLevelGrass2(*(prey->getPosition())), (mH2 / fuzzyfication_params1[FoodLocalLow2][0]), 3 * (mH2 / fuzzyfication_params1[FoodLocalLow2][1]));
#endif


	friendsLocal = int (eco->getListCreature(*(prey->getPosition()), 0)->size()) - 1;
	activations[PartnerLocalYes] = fuzzy2(friendsLocal, (fuzzyfication_params1[PartnerLocalYes][0]), (fuzzyfication_params1[PartnerLocalYes][1]));
	activations[PartnerLocalNo] = fuzzy1(friendsLocal, (fuzzyfication_params1[PartnerLocalNo][0]), (fuzzyfication_params1[PartnerLocalNo][1]));


	for (int i = nbSens + nbConcepts; i < nbSens + nbConcepts + nbMoteursDep + nbMoteursFix; ++i) {
		activations[i] = 0; //-- Clear Activation level of Motors (wait, eat, reproduce)
	}
	//activations[ConceptIndex] = activations[ConceptIndex] * ConceptMultiplier; //-- effect of previous action on Activation level (Internal Concepts)



	for (int i = 0; i < 3; i++) {
		updateConcepts();	//-- Updates concepts [Fear][0] -> [26][0]
	}

	/*************
	 *Armin
	 */
	short numMax = choose_action(eco);

	return numMax;
}

void FCMPrey::set_prey(Prey *prey){
	this->prey = prey;
}

