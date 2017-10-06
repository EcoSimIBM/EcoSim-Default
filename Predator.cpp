/*
 * Predator.cpp
 *  - The predator objects
 */

#include "Predator.h"

#include <iostream>
#include "Ecosystem.h"
#include "FCMPredator.h"

#include <algorithm> //Reproduction based on gender /MRE/
using namespace std; //Reproduction based on gender /MRE/

Predator::Predator() {

}

Predator::~Predator() {

}
// for 1st generation individuals
Predator::Predator(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float energy, float energy_max, float speed, short speed_max, short age, short age_max, short vision, short repAge, float distanceEvol, float stateBirth, FCMPredator *model, short genderVal, short persuasionVal) : Sim(type, id, species, parent1ID, parent2ID, speciesParent1ID, speciesParent2ID, energy_max, speed_max, age_max, vision, repAge, distanceEvol, stateBirth, nbGenes, genderVal, 0, 0, persuasionVal) //Add Gender /MRE/
{
	setPosition(pos);
	//	calcWeight();
	setAge(age);
	setSpeed(speed);

	setGender(genderVal); //Add Gender /MRE/

	setUpdated(0);
	setLastAction(0);
	setLiving(true);
	setKilledInFight(false);

	StrengthWeakestPreyCellClose = 1000000;
	StrengthStrongestPreyCellClose = 0;
	StrengthStrongestPreyClose = 0;
	StrengthWeakestPreyClose = 100000;
	StrengthStrongestPredatorClose = 0;

	this->model = *model;
	this->model.set_predator(this);
	
	setEnergy(energy);
}
// for new born individuals /M.M
Predator::Predator(short type, long id, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, Position pos, float speed, short age, float distanceEvol, FCMPredator *model, short genderVal) : Sim(type, id, species, parent1ID, parent2ID, speciesParent1ID, speciesParent2ID, distanceEvol, nbGenes, genderVal) //Add Gender /MRE/
{
	setPosition(pos);
	//setEnergy(energy);  // M.M => because it should be calculated after specifying SOB
	setAge(age);
	setSpeed(speed);

	setUpdated(0);
	setLastAction(0);
	setLiving(true);
	setKilledInFight(false);

	StrengthWeakestPreyCellClose = 100000;
	StrengthStrongestPreyCellClose = 0;
	StrengthStrongestPreyClose = 0;
	StrengthWeakestPreyClose = 100000;
	StrengthStrongestPredatorClose = 0;

	this->model = *model;
	this->model.set_predator(this);
}

FCMPredator * Predator::getFCM() {
	return &model;
}


float Predator::get_mutate_amount(Ecosystem * eco){
	Random *r = eco->getRandom();
	double prob, test;
	prob = r->next(32000) / 32000.0;
	if (prob <= 0.05)
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.3;
		else
			test = -0.3;
	}
	else if (prob <= 0.3)
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.2;
		else
			test = -0.2;
	}
	else
	{
		prob = r->next(32000) / 32000.0;
		if (prob <= 0.5)
			test = 0.1;
		else
			test = -0.1;
	}
	return (float)test * 2; // M.M based on the changes in mutation probability (divided by 10)

}

void Predator::reprod(Predator * friends, vector<Predator> * newPred, Ecosystem * eco) {

	Random *r = eco->getRandom();
	short nSens = FCMPredator::nbSens;
	short nConcepts = FCMPredator::nbConcepts;
	short nMoteursDep = FCMPredator::nbMoteursDep;
	short nMoteursFix = FCMPredator::nbMoteursFix;

	short bebe_gender = eco->rand.next(0, 1); //Add Gender /MRE/ Female=1, Male=0

	FCMPredator bebe_fcm = FCMPredator(NULL);

	float distanceEvol = 0;

	Predator *selected_parent;
	float partner_select;

	// M.M
	// M.M commented because SOB is part of genome
	// After creating the child's genome we have sob and we can calculate energy

	// ------ Creation of FCM
	float pRemove = (float)0.0005;
	float prob;

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) { //-- Generate the new Proie's concepts and chart
		partner_select = float(r->next(10000) / 10000.0);

		if (i < nSens){//MRE RandomGoodGene //get sense features from mother or father (the one with same gender)
			if (this->getGender() == bebe_gender)//female
			{
				selected_parent = this;
			}
			else
			{
				selected_parent = friends;
			}
		}
		else{
			if (partner_select < 0.5)
				selected_parent = this;
			else
				selected_parent = friends;
		}

		for (int j = 0; j < nConcepts + nMoteursDep + nMoteursFix; ++j) {
			float do_mutate = float(r->next(32000) / 32000.0);

			if (selected_parent->model.get_chart(i, j) != 0){	//A.G
				float mutate_amount = 0.0;
				if (this->getAge() >(this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut_HighAge() + pRemove) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					if (do_mutate <= pRemove)
					{
						bebe_fcm.set_chart(i, j, 0);
					}
					else mutate_amount = (do_mutate <= eco->getProbaMut() + pRemove) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, selected_parent->model.get_chart(i, j) + (float)mutate_amount);
			}
			else {
				float mutate_amountLow = 0.0;
				if (this->getAge() > (this->getAgeMax(false) - this->getRepAge()))	//A.G
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow_HighAge()) ? get_mutate_amount(eco) : 0;	//A.G
				}
				else
				{
					mutate_amountLow = (do_mutate <= eco->getProbaMutLow()) ? get_mutate_amount(eco) : 0;
				}
				bebe_fcm.set_chart(i, j, (float)mutate_amountLow);
			}
		}
	}

	distanceEvol = bebe_fcm.calculDistanceEvol(eco->fcm_pred);

	for (int i = 0; i < nSens + nConcepts + nMoteursDep + nMoteursFix; ++i) {
		// bebe_fcm.set_activation(i, model.get_activations()->at(i));
		bebe_fcm.set_activation(i, 0); //-- Meisam: concepts should be zero?
	}


	//-- Create the new Pred
	short bebe_type = 1; //--Pred
	long bebe_id = eco->maxPredID;
	int bebe_species = 0;
	long bebe_parent1ID = getID();
	long bebe_parent2ID = friends->getID();
	int bebe_speciesParent1ID = getSpecies();
	int bebe_speciesParent2ID = friends->getSpecies();
	Position bebe_position = *(getPosition());
	float bebe_speed = 0;
	short bebe_age = 0;
	float bebe_distanceEvol = distanceEvol;

	// create new individual
	Predator * bebe = new Predator(bebe_type, bebe_id, bebe_species, bebe_parent1ID, bebe_parent2ID, bebe_speciesParent1ID, bebe_speciesParent2ID, bebe_position, bebe_speed, bebe_age, bebe_distanceEvol, &bebe_fcm, bebe_gender); //Add Gender /MRE/

	//-- species adjustment: For every new Pred, compare distanceEvol with parent species - add to closest species
	float distFromP1 = 0.0;
	float distFromP2 = 0.0;

	// physical gene crossover and mutation / M.M
	if (this->getGender() == 1)//female. first parameter should be mother //MRE RandomGoodGene
		bebe->phGenome.crossover(this->phGenome.getGenome(), friends->phGenome.getGenome(), bebe_gender, this->phGenome.persuasion);
	else
		bebe->phGenome.crossover(this->phGenome.getGenome(), friends->phGenome.getGenome(), bebe_gender, friends->phGenome.persuasion);

	bebe->phGenome.mutate(eco->getProbaMut(), eco);
	//GENETYPE distance=bebe->phGenome.calcDistance(eco->ini_PredGenome.getGenome()); // M.M calculate evolution distance
	float distance;
	if (bebe->getGender() == 1){//MRE RandomGoodGene
		distance = bebe->phGenome.calcDistance(&eco->ini_FemalePredGenome); // M.M calculate evolution distance
	}
	else{
		distance = bebe->phGenome.calcDistance(&eco->ini_MalePredGenome); // M.M calculate evolution distance
	}
	bebe->set_DistEvolution(distance + bebe->getDistanceEvol());  //  M.M update distance evolution

	int percent = r->next(-15, 15);

	//	float MaxSOB; //MRE RandomGoodGene
	//	if(bebe_gender  == 1)//1 means Gender::Female
	//	{
	//		MaxSOB = MaxSOBFemale;
	//	}else{ //baby is male
	//		MaxSOB = MaxSOBMale;
	//	}
	
	eco->statWorld.incAvgEnergyTransferredMalePred((friends->getStateBirth() * friends->getEnergyMax()) / 100);
	eco->statWorld.incAvgEnergyTransferredFemalePred((this->getStateBirth() * this->getEnergyMax()) / 100);
	bebe->setEnergy(calculationEnergyNewBorn(friends->getStateBirth(), friends->getEnergyMax(), this->getStateBirth(), this->getEnergyMax()));  // M.M
	//    bebe->calcWeight();
	float energy = bebe->getEnergy();
	
	float geneticDistFromP1, geneticDistFromP2;  //M.M
	distFromP1 = bebe->getFCM()->calculDistanceEvol(eco, getFCM());
	//geneticDistFromP1= bebe->phGenome.calcDistance(this->phGenome.getGenome()); // M.M
	geneticDistFromP1 = bebe->phGenome.calcDistance(&this->phGenome); // M.M
	distFromP1 += geneticDistFromP1;  //M.M

	distFromP2 = bebe->getFCM()->calculDistanceEvol(eco, friends->getFCM());
	//geneticDistFromP2= bebe->phGenome.calcDistance(friends->phGenome.getGenome()); // M.M
	geneticDistFromP2 = bebe->phGenome.calcDistance(&friends->phGenome); // M.M
	distFromP2 += geneticDistFromP2;  // M.M

	if (distFromP1 < distFromP2) {
		bebe->setSpecies(getSpecies());
		bebe->speciesPtr = this->speciesPtr;
	}
	else {
		bebe->setSpecies(friends->getSpecies());
		bebe->speciesPtr = friends->speciesPtr;
	}

	bebe->set_action_offset(-1);
	newPred->push_back(*bebe);	//-- Add the bebe to newPred

	eco->maxPredID++;
	eco->incNbPred();


	//-- Adjust the the parent's energy and other properties

	incNbReprod();
	friends->incNbReprod();
	friends->setUpdated(1);

	friends->setLastAction(FCMPredator::ReproduceAction); //-- Action of reproduction

	friends->model.ConceptIndex = FCMPredator::SearchPartner;       //-- shows Concept that should changes its' value after acting
	friends->model.ConceptMultiplier = 0;  //-- Based on the model.ConceptIndex, the value of this concpet should multipy to model.ConceptMultiplier

	friends->cntReprodFailedTries = 0;
	friends->reproduceFailed1 = 0;  //-- 1: Energy of prey is not enoug, 10: partner has not found, 11: both of them
	friends->reproduceFailed2 = 0;  //-- "energy of partner is not sufficient"
	friends->reproduceFailed3 = 0;  //-- "Partner has acted befor"
	friends->reproduceFailed4 = 0;  //-- "Partner has chosen another action"
	friends->reproduceFailed5 = 0;  //-- "Distance is far"
	//friends->reproduceFailed6 = 0; //-- "Same gender" //Since Females are initiating reproduction, it happens when mate is male //Reproduction based on gender /MRE/

	friends->set_DistMating(get_DistMating());    //-- Initial value for Distance Parents
	friends->setSpeed(0);

	eco->ParentPred[getID()].energy = getEnergy();						//-- Meisam: Information for Parents
	eco->ParentPred[friends->getID()].energy = friends->getEnergy();  //-- Meisam: Information for Parents

	// in first pregnancy mother spend more energy than child earn (5%)
	
	if (getNbReprod() == 1){
		this->setEnergy(calculationEnergyNewParent(this->getEnergy(), energy, this->getStateBirth(), friends->getStateBirth()) - FirstPregnancyEneryWaste*getEnergy());			//-- Losing Energy for reproducing
	}
	else{
		this->setEnergy(calculationEnergyNewParent(this->getEnergy(), energy, this->getStateBirth(), friends->getStateBirth()));			//-- Losing Energy for reproducing
	}
	friends->setEnergy(calculationEnergyNewParent(friends->getEnergy(), energy, friends->getStateBirth(), this->getStateBirth()));

	float newEnergy = friends->calculationEnergyAction(friends->getEnergy(), friends->model.getNbArc(), friends->getSpeed(), friends->getStrength(), 0, eco); //-- Losing Energy for acting
	friends->setEnergy(newEnergy);
	

	//-- clear the memory of Partner
	friends->friendsClose.clear();
	friends->friendsCloseDistance.clear();
	friends->opponentsClose.clear();
	friends->opponentsCloseDistance.clear();
	friends->foodClose.clear();
	friends->foodCloseDistance.clear();
	delete bebe;				//-- De-allocate

}

/*
 * Method updateNew
 * 	@Parameters
 * 	- num, the current pred's position in wolves data structure
 *  - *eco, pointer to the ecosystem object
 *  @Purpose
 *  - computes a list of the closet food, friends, and prey
 *    used to determine which action to take
 *  - predators looks in a spiral direction outward from its current position
 *  @Note
 *  - symantics (1, 2, 3, 4) == (North, East, South, West)
 *  - lookPermut[1..8] = {(1,2,3,4), (2,3,4,1), (3,4,1,2), (4,1,2,3),
 *  				  (1,4,3,2), (4,3,2,1), (3,2,1,4), (2,1,4,3)}
 */

void Predator::updateNew(Ecosystem * eco) {

	//-- Clear the current list of friends and distance to friends
	friendsClose.clear();
	friendsCloseDistance.clear();

	//-- Clear the current list of predators and distance to predators
	opponentsClose.clear();
	opponentsCloseDistance.clear();

	//-- Clear the current food and distance to food
	foodClose.clear();
	foodCloseDistance.clear();

	// A.G
	// clear variables
	StrengthStrongestPredatorCloseIndex = -1;
	StrengthStrongestPredatorClose = 0;

	StrengthStrongestPreyCloseIndex = -1;
	StrengthStrongestPreyClose = 0;

	StrengthWeakestPreyCloseIndex = -1;
	StrengthWeakestPreyClose = 10000;

	StrengthWeakestPreyCellClose = 100000;
	StrengthWeakestPreyCellClosePos.x = -1;
	StrengthWeakestPreyCellClosePos.y = -1;

	StrengthStrongestPreyCellClose = 0;
	StrengthStrongestPreyCellClosePos.x = -1;
	StrengthStrongestPreyCellClosePos.y = -1;



	//-- Select a random direction for looking to start looking
	int direction = eco->rand.next(1, 8);
	//	eco->numRandoms++;

	//-- First distance to look (0= present cell) but these functins can't calculate "distance = 0"
	int distance = 1;

	//-- Number of Local Friends, Enemies & Foods
	const int nbList = 5;

	//-- Counters
	int nbFriends = 0;
	int nbOpponents = 0;
	int nbFoods = 0;

	getListFriends(nbList, &distance, direction, &nbFriends, &nbOpponents, &nbFoods, eco);
	cout << flush;

	//-- Action: 0-escape, 1-rsearchFood, 2-socialize, 3-explore, 4-stay, 5-eat, 6-reproduce
	short na = model.percept_predator(eco);
	set_action_offset(na);
}

/*
 * Uses distance and direction to look around itself in the world
 * Finds the closest friends and adds them to the vector friendsClose
 */
void Predator::getListFriends(int nbList, int * distance, int direction, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {

	while ((*distance < getVision()) && (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)) {
		switch (direction) {
		case 1: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 2: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 3: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 4: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 5: {
					partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 6: {
					partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 7: {
					partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		case 8: {
					partialCompute2(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute1(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute4(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					if (*nbFriends < nbList || *nbOpponents < nbList || *nbFoods < nbList)
						partialCompute3(nbList, distance, nbFriends, nbOpponents, nbFoods, eco);
					break;
		}

		default:
			break;
		} //-- Endof switch

		(*distance)++;

	} //-- End of while

}

/*
 * Finds the friends in the north direction
 */

void Predator::partialCompute1(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {
	Position pTemp;

	//-- Look in one row
	for (int x = -(*distance - 1); x <= *distance; x++) {

		//-- ChangePosition :
		//--		pTemp.x = position->x + x;
		//--		pTemp.y = position->y - *distance;

		pTemp = Movement(x, -*distance, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
	}
}

/*
 * Finds the friends in the east direction
 */
void Predator::partialCompute2(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {

	Position pTemp;

	//-- Look in one column
	for (int y = -(*distance - 1); y <= *distance; y++) {

		//-- ChangePosition :
		//--		pTemp.x = position->x + *distance;
		//--		pTemp.y = position->y + y;

		pTemp = Movement(*distance, y, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
	}
}

/*
 * Finds the friends in the south direction
 */
void Predator::partialCompute3(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {

	Position pTemp;

	//-- Look in one row
	for (int x = (*distance - 1); x >= (-*distance); x--) {

		//-- ChangePosition :
		//--		pTemp.x = position->x + x;
		//--		pTemp.y = position->y + *distance;;

		pTemp = Movement(x, *distance, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
	}
}

/*
 * Finds the friends in the west direction
 */
void Predator::partialCompute4(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco) {

	Position pTemp;

	//-- Look in one column
	for (int y = (*distance - 1); y >= (-*distance); y--) {

		//-- ChangePosition :
		//--		pTemp.x = position->x - *distance;
		//--		pTemp.y = position->y + y;

		pTemp = Movement(-*distance, y, eco->getWidth(), eco->getHeight());

		//-- Look at this location for finding local frieds, opponents and foods
		lookat(nbList, distance, nbFriends, nbOpponents, nbFoods, eco, &pTemp);
	}
}

void Predator::lookat(int nbList, int * distance, int * nbFriends, int * nbOpponents, int * nbFoods, Ecosystem * eco, Position * pTemp) {

	int FriendType = 1;   //-- Predator
	int OpponentType = 0; //-- Prey
	float FoodAvailability = eco->getLevelMeat(*pTemp);

	if (*nbFriends < nbList) {
		//-- Get list of Friends in the Cell pTemp
		vector<int> nearbyFriends = *(eco->getListCreature(*pTemp, FriendType));

		//-- Continue until (1) found nbList friends, or (2) ran out of nearby friends -> add to list of friends
		while (*nbFriends < nbList && int(nearbyFriends.size()) > 0) {
			//-- Choose random friend to add to list
			int fID = eco->rand.next(0, int(nearbyFriends.size()) - 1);
			//-- Add distance value of friend in to the Global list friendsCloseDistance
			friendsCloseDistance.insert(friendsCloseDistance.begin(), *distance);
			//-- Add position of friend based on the distance in to Global list friendsClose
			friendsClose.insert(friendsClose.begin(), nearbyFriends.at(fID));
			//-- Remove the friend from the list of available friends
			nearbyFriends.erase(nearbyFriends.begin() + fID);
			*nbFriends += 1;
		}
	}

	if (*nbOpponents < nbList) {
		//-- Get list of Opponents in the Cell pTemp
		vector<int> nearbyOpponents = *(eco->getListCreature(*pTemp, OpponentType));

		//-- Continue until (1) found nbList Opponents, or (2) ran out of nearby Opponents -> add to list of Opponents
		while (*nbOpponents < nbList && int(nearbyOpponents.size()) > 0) {
			int fID = eco->rand.next(0, int(nearbyOpponents.size()) - 1);
			//-- Add distance value of Opponent into the Global list OpponentsCloseDistance
			opponentsCloseDistance.insert(opponentsCloseDistance.begin(), *distance);
			//-- Add position of Opponent into the Global list OpponentsClose
			opponentsClose.insert(opponentsClose.begin(), nearbyOpponents.at(fID));
			//-- Remove the Opponent from the list of available Opponents
			nearbyOpponents.erase(nearbyOpponents.begin() + fID);
			*nbOpponents += 1;
		}
	}

	if (*nbFoods < nbList && FoodAvailability >= 1) {
		//-- Add distance value of food into the Global list foodCloseDistance
		foodCloseDistance.insert(foodCloseDistance.begin(), *distance);
		//-- Add position of food into the Global list foodClose
		foodClose.insert(foodClose.begin(), *pTemp);
		*nbFoods += 1;
	}

	// =============	A.G
	int gender_temp;
	if (getGender() == 0)
		gender_temp = 1;
	else
		gender_temp = 0;

	// =============	A.G //MRE RandomGoodGene //Move2StrongestPred
	if (*nbFriends > 0)
	{
		//-- Get list of Friends in the Cell pTemp
		vector<int> nearbyFriends = *(eco->getListCreature(*pTemp, FriendType));
		//float sum = 0;
		for (int cnt = 0; cnt < (int)nearbyFriends.size(); cnt++)
		{
			//sum = sum + eco->rabbits[nearbyFriends[cnt]].getStrength();
			if (eco->wolves[nearbyFriends[cnt]].getStrength() > StrengthStrongestPredatorClose && ((this->getGender() + 1) % 2) == eco->wolves[nearbyFriends[cnt]].getGender())
			{
				StrengthStrongestPredatorCloseIndex = nearbyFriends[cnt]; //Move2StrongestPred
				StrengthStrongestPredatorClose = eco->wolves[nearbyFriends[cnt]].getStrength();
			}
		}
	}
	//------------------ //MRE RandomGoodGene //StrengthStrongestPredLocal
	if (*nbOpponents > 0)
	{
		vector<int> nearbyOpponents = *(eco->getListCreature(*pTemp, OpponentType));
		float sum = 0;
		for (int cnt = 0; cnt < (int)nearbyOpponents.size(); cnt++)
		{
			sum = sum + eco->rabbits[nearbyOpponents[cnt]].getStrength();
			if (eco->rabbits[nearbyOpponents[cnt]].getStrength() > StrengthStrongestPreyClose)
			{
				StrengthStrongestPreyCloseIndex = nearbyOpponents[cnt];
				StrengthStrongestPreyClose = eco->rabbits[nearbyOpponents[cnt]].getStrength();
			}
			if (eco->rabbits[nearbyOpponents[cnt]].getStrength() < StrengthWeakestPreyClose)
			{
				StrengthWeakestPreyCloseIndex = nearbyOpponents[cnt];
				StrengthWeakestPreyClose = eco->rabbits[nearbyOpponents[cnt]].getStrength();
			}
		}
		if (sum < StrengthWeakestPreyCellClose)
		{
			StrengthWeakestPreyCellClose = sum;
			StrengthWeakestPreyCellClosePos = *pTemp;
		}
		if (sum > StrengthStrongestPreyCellClose)
		{
			StrengthStrongestPreyCellClose = sum;
			StrengthStrongestPreyCellClosePos = *pTemp;
		}
	}
	// =========

}


void Predator::act_driver(vector<Predator> * newPred, Ecosystem * eco, short MatingMode) {
	Position newPos;

	setUpdated(1);

	eco->ParentPred[getID()].energy = getEnergy(); //-- Meisam: Information for Parents

	newPos = act(newPred, eco, MatingMode);
	setPosition(newPos);
	if (this->getGender() == 0){
		float newEnergy = calculationEnergyAction(getEnergy(), model.getNbArc(), getSpeed(), getStrength(), 0, eco);  //-- Losing Energy for acting //RS males don't carry persuasion penalty
		setEnergy(newEnergy);
	}
	else{
		float newEnergy = calculationEnergyAction(getEnergy(), model.getNbArc(), getSpeed(), getStrength(), getPersuasion(), eco);  //-- Losing Energy for acting	//RS females do carry persuasion penalty
		setEnergy(newEnergy);
	}

	//-- clear the memory
	friendsClose.clear();
	friendsCloseDistance.clear();
	opponentsClose.clear();
	opponentsCloseDistance.clear();
	foodClose.clear();
	foodCloseDistance.clear();
	// A.G
	StrengthStrongestPredatorCloseIndex = -1;
	StrengthStrongestPredatorClose = 0;

	StrengthStrongestPreyCloseIndex = -1;
	StrengthStrongestPreyClose = 0;

	StrengthWeakestPreyCloseIndex = -1;
	StrengthWeakestPreyClose = 10000;

	StrengthWeakestPreyCellClose = 100000;
	StrengthWeakestPreyCellClosePos.x = -1;
	StrengthWeakestPreyCellClosePos.y = -1;

	StrengthStrongestPreyCellClose = 0;
	StrengthStrongestPreyCellClosePos.x = -1;
	StrengthStrongestPreyCellClosePos.y = -1;
}

Position Predator::act(vector<Predator> * nouveauNe, Ecosystem * eco, short MatingMode) {
	//-- Meisam, Feb 2011: Hunt, Search, Socialize, Explore & Reproduce have changed.
	//-- Lists of friendsClose, pred, ... do not contain individuals in a current cell (distance=0)
	Random *r = eco->getRandom();

	cout << flush;

	Position posNouvelle;
	posNouvelle.x = 0;
	posNouvelle.y = 0;
	Position posRelative;
	posRelative.x = 0;
	posRelative.y = 0;

	Direction dir;
	dir.x = 0;
	dir.y = 0;

	short VisionMe = 5;// (getVision() / 2) + 1;
	float e = getEnergy();
	float distance;

	model.ConceptIndex = 0;       //-- shows Concept that should changes its' value after acting
	model.ConceptMultiplier = 1;  //-- Based on the model.ConceptIndex, the value of this concpet should multipy to model.ConceptMultiplier

	cntReprodFailedTries = 0; //MRE RandomGoodGene
	reproduceFailed1 = 0;  //-- 1: Energy of prey is not enoug, 10: partner has not found, 11: both of them
	reproduceFailed2 = 0;  //-- "energy of partner is not sufficient"
	reproduceFailed3 = 0;  //-- "Partner has acted befor"
	reproduceFailed4 = 0;  //-- "Partner has chosen another action"
	reproduceFailed5 = 0;  //-- "Distance is far"
	//reproduceFailed6 = 0; //-- "Same gender" //Since Females are initiating reproduction, it happens when mate is male //Reproduction based on gender /MRE/
	set_DistMating(0);    //-- Initial value for Distance Parents

	int ao = get_action_offset();
	switch (ao) {

	case FCMPredator::HuntPrey: { //-- hunting prey (chase prey)
									//-- Meisam, Feb 2011: 1)was rewrited.
									
									//Ryan's hunting fix
									vector<int> v = *(eco->getListCreature(*(getPosition()), 0));
									bool weHaveLivingPrey = false;
									for (int xyz = 0; xyz < v.size(); xyz++)
									{
										if (eco->rabbits[v[xyz]].getLiving() && eco->rabbits[v[xyz]].getEnergy() > 0) weHaveLivingPrey = true;
									}
									
									vector<int> v2 = opponentsClose;
									bool weHaveLivingOpponentsClose = false;
									for (int xyz = 0; xyz < v2.size(); xyz++)
									{
										if (eco->rabbits[v2[xyz]].getLiving() && eco->rabbits[v2[xyz]].getEnergy() > 0) weHaveLivingOpponentsClose = true;
									}
									
									int modifier = 0;
									if (weHaveLivingPrey || !weHaveLivingOpponentsClose)
										distance = 0;
									else { //-- There is at least one prey in vicinity
										for (int xyz = 0; xyz < opponentsClose.size(); xyz++)
										{
											if ((eco->rabbits[opponentsClose[int(opponentsClose.size()) - (1 + modifier)]].getLiving()) && (eco->rabbits[opponentsClose[int(opponentsClose.size()) - (1 + modifier)]].getEnergy() > 0)) break;
											modifier ++;
										}
										
										if (modifier >= opponentsClose.size()) modifier = 0; //no living opponents close, doesn't matter where we move we will fail at hunting
										dir = getPosition()->calculDirection(*(eco->rabbits[opponentsClose[int(opponentsClose.size()) - (1 + modifier)]].getPosition()), eco->getWidth(), eco->getHeight());
										posRelative.x = dir.x;
										posRelative.y = dir.y;
										distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

										//-- Meisam
										float ActionActivation = model.get_activations()->at(ao);
										if (fabs(ActionActivation) > 0){
											distance = distance / (ActionActivation * getSpeedMax());
										}
										else {
											distance = 0;
										}
									}

									if (distance > 1) {
										posRelative.x = int(posRelative.x / distance);
										posRelative.y = int(posRelative.y / distance);

										posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

										setLastAction(FCMPredator::HuntPreyFailAction); //-- Hunt prey is failed because the distance is far
									}
									else { //-- distance < 1
										if (weHaveLivingPrey || weHaveLivingOpponentsClose) {
											//-- exists a pray in cuurent cell or vicinty with distance < 1

											vector<int>  HuntList, HuntTmp;			//-- list of preies in the current cell && distance < 1
											if (weHaveLivingOpponentsClose) { //-- vicinity with distance < 1
												posNouvelle.x = eco->rabbits[opponentsClose[int(opponentsClose.size()) - (1 + modifier)]].getPosition()->x;
												posNouvelle.y = eco->rabbits[opponentsClose[int(opponentsClose.size()) - (1 + modifier)]].getPosition()->y;

												HuntTmp = *eco->getListCreature(posNouvelle, 0);
												while ((int)HuntTmp.size() > 0) { //-- make randome order
													int fID = eco->rand.next(0, int(HuntTmp.size()) - 1);
													if (eco->rabbits[HuntTmp.at(fID)].getLiving() && eco->rabbits[HuntTmp.at(fID)].getEnergy() > 0) HuntList.insert(HuntList.begin(), HuntTmp.at(fID));
													HuntTmp.erase(HuntTmp.begin() + fID);
												}
											}
											if (weHaveLivingPrey) { //-- current cell
												posNouvelle.x = getPosition()->x;
												posNouvelle.y = getPosition()->y;

												HuntTmp = *eco->getListCreature(*(getPosition()), 0);
												while ((int)HuntTmp.size() > 0) { //-- make randome order
													int fID = eco->rand.next(0, int(HuntTmp.size()) - 1);
													if (eco->rabbits[HuntTmp.at(fID)].getLiving() && eco->rabbits[HuntTmp.at(fID)].getEnergy() > 0) HuntList.insert(HuntList.begin(), HuntTmp.at(fID));
													HuntTmp.erase(HuntTmp.begin() + fID);
												}
											}

											bool cont = true;
											int k = 0;
											vector<int> currcell_Preys;
											bool coopDefended = false;
											while (cont && k < int(HuntList.size())) {
												if (eco->rabbits[HuntList[k]].getLiving() && eco->rabbits[HuntList[k]].getEnergy() > 0) {
													posNouvelle.x = eco->rabbits[HuntList[k]].getPosition()->x; //-- Meisam: added for more accuracy
													posNouvelle.y = eco->rabbits[HuntList[k]].getPosition()->y;
													//-- Meisam End
													currcell_Preys = *eco->getListCreature(posNouvelle, 0); // get the all preys available in this cell
													//-- calculate energy penalty for predator. M.M
													float predatorPenalty = 0;   // M.M
													int preysWithCoopDefence = 0; // M.M
													// count the number of alive prey in the currecnt cell except the target prey, with cooperative defence capability
													for (unsigned char p = 0; p < currcell_Preys.size(); p++)  // M.M
														if (eco->rabbits[HuntList[k]].getID() != eco->rabbits[currcell_Preys[p]].getID() && eco->rabbits[currcell_Preys[p]].getEnergy() > 0 && eco->rabbits[currcell_Preys[p]].getLiving() && eco->rabbits[currcell_Preys[p]].getCoopDefence() > 0) {// not the same prey, only other prey in the same cell
															preysWithCoopDefence++;
														}
													//--  calculate the selected prey energy after fight
													// %80 of predatore's strength if there is another prey with cooperative defence capability in the current cell else the whole strength of predator is on the selected prey
													float prey_energy = 0;
													if (preysWithCoopDefence > 0 && !coopDefended){
														prey_energy = eco->rabbits[HuntList[k]].getEnergy() - 0.8 * this->getStrength();
														for (unsigned char p = 0; p < currcell_Preys.size(); p++)  // M.M
														{
															// if this prey can involve in the cooperative defence
															if (eco->rabbits[HuntList[k]].getID() != eco->rabbits[currcell_Preys[p]].getID() && eco->rabbits[currcell_Preys[p]].getLiving() && eco->rabbits[currcell_Preys[p]].getEnergy() > 0 && eco->rabbits[currcell_Preys[p]].getCoopDefence() > 0) {// not the same prey, only other prey in the same cell
																predatorPenalty += eco->rabbits[currcell_Preys[p]].getDefence() * eco->rabbits[currcell_Preys[p]].getCoopDefence() * eco->rabbits[currcell_Preys[p]].getStrength(); // chack also they are alive
																eco->rabbits[currcell_Preys[p]].setEnergy(eco->rabbits[currcell_Preys[p]].getEnergy() - 0.2 * this->getStrength() / preysWithCoopDefence); // penalty for preys take part in the fight.
																
																if (eco->rabbits[currcell_Preys[p]].getEnergy() <= 0){
																	//RS NEED TO ADD DEATH FOR COOP DEFENCE PREY IF THEY DIE!!
																	eco->rabbits[currcell_Preys[p]].setLiving(false);
																	eco->DeadInd.species = eco->rabbits[currcell_Preys[p]].getSpecies();
																	eco->DeadInd.id = eco->rabbits[currcell_Preys[p]].getID();
																	eco->DeadInd.age = eco->rabbits[currcell_Preys[p]].getAge();
																	eco->DeadInd.energy = eco->rabbits[currcell_Preys[p]].getEnergy();
																	eco->DeadInd.lastAction = eco->rabbits[currcell_Preys[p]].getLastAction();
																	eco->DeadInd.evolDist = eco->rabbits[currcell_Preys[p]].getDistanceEvol();
																	eco->DeadInd.speed = eco->rabbits[currcell_Preys[p]].getSpeed();
																	eco->DeadInd.deathType = 3;
																	eco->DeadPrey.push_back(eco->DeadInd);
																	if (eco->rabbits[currcell_Preys[p]].getGender() == 1){
																		eco->getStat()->incAgeAvgDeadFemalePrey(eco->rabbits[currcell_Preys[p]].getAge());
																		eco->getStat()->incNbDeadFemalePreyK();
																	}
																	else{
																		eco->getStat()->incAgeAvgDeadMalePrey(eco->rabbits[currcell_Preys[p]].getAge());
																		eco->getStat()->incNbDeadMalePreyK();
																	}
																}
															}
														}
														coopDefended = true;
													}
													else{
														prey_energy = eco->rabbits[HuntList[k]].getEnergy() - this->getStrength();
													}

													predatorPenalty += eco->rabbits[HuntList[k]].getDefence() * eco->rabbits[HuntList[k]].getStrength();  // M.M
		
													if (prey_energy <= 0) {// Predator killed the Prey
														// add the strength of prey before attack to the predator energy and update strength M.M
														float predatorReward = (eco->rabbits[HuntList[k]].getStrength());	// M.M //RS FOOD
														if (predatorReward < 0) predatorReward = 0;
														float pred_energy = this->getEnergy() + predatorReward - predatorPenalty; // M.M

														eco->rabbits[HuntList[k]].setLiving(false);
														eco->decNbPrey();
														if (eco->rabbits[HuntList[k]].getGender() == 1){
															eco->getStat()->incAgeAvgDeadFemalePrey(eco->rabbits[HuntList[k]].getAge());
															eco->getStat()->incNbDeadFemalePreyK();
														}
														else{
															eco->getStat()->incAgeAvgDeadMalePrey(eco->rabbits[HuntList[k]].getAge());
															eco->getStat()->incNbDeadMalePreyK();
														}

														//eco->incLevelMeat1(posNouvelle); /M.M
														if (pred_energy > this->getEnergyMax()) // if still something remained /M.M
															eco->incLevelMeatByStrength(posNouvelle, pred_energy - this->getEnergyMax());
														this->setEnergy(pred_energy); // M.M
														eco->rabbits[HuntList[k]].setEnergy(0);   // M.M
														//setEnergy(e + eco->getValuePrey()); //M.M
														//-- Meisam
														model.ConceptIndex = FCMPredator::ChaseAway;
														model.ConceptMultiplier = float(1 / 2.0);
														model.ConceptIndex = FCMPredator::Hunger;
														model.ConceptMultiplier = float(1 / 5.0);
														setLastAction(FCMPredator::HuntPreyAction);

														//-- Meisam
														eco->DeadInd.species = eco->rabbits[HuntList[k]].getSpecies();
														eco->DeadInd.id = eco->rabbits[HuntList[k]].getID();
														eco->DeadInd.age = eco->rabbits[HuntList[k]].getAge();
														eco->DeadInd.energy = eco->rabbits[HuntList[k]].getEnergy();
														eco->DeadInd.lastAction = eco->rabbits[HuntList[k]].getLastAction();
														eco->DeadInd.evolDist = eco->rabbits[HuntList[k]].getDistanceEvol();
														eco->DeadInd.speed = eco->rabbits[HuntList[k]].getSpeed();
														eco->DeadInd.deathType = 3;
														eco->DeadPrey.push_back(eco->DeadInd);
														//-- Meisam End
													}
													else{ // just update the energy M.M failed to kill prey
														this->setEnergy(this->getEnergy() - predatorPenalty); // M.M
														eco->rabbits[HuntList[k]].setEnergy(prey_energy);   // M.M
														setLastAction(FCMPredator::HuntPreyFailAction);
													}
													cont = false;
													if (this->getEnergy() <= 0){ // M.M not only failed to kill prey but lost fight and died
														setKilledInFight(true);
														setLastAction(FCMPredator::HuntPreyFailAction);
													}
												}
												k++;
											}

											if (cont)
												setLastAction(FCMPredator::HuntPreyFailAction);

											HuntList.clear();
										}
										else { //-- stay at your position because there isn't any opponentsClose in your vicinity for chasing
											posNouvelle.x = getPosition()->x;
											posNouvelle.y = getPosition()->y;
											setLastAction(FCMPredator::HuntPreyFailAction); //-- Hunt prey is failed because not any preys exist in the view
										}
									}
									break;
	}

	case FCMPredator::SearchFood: { //-- search food
									  //-- Meisam, Feb 2011: 1)was rewrited. 2)Random shearch was changed

									  if (int(foodClose.size()) > 0){
										  dir = getPosition()->calculDirection(foodClose[int(foodClose.size()) - 1], eco->getWidth(), eco->getHeight());
									  }
									  else{ //-- There isn't any food in the vicinity -> random walk
										  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25));: random search
										  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25));
									  }
									  posRelative.x = dir.x;
									  posRelative.y = dir.y;
									  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

									  //-- Meisam
									  float ActionActivation = model.get_activations()->at(ao);
									  if (fabs(ActionActivation) > 0)
										  distance = distance / (ActionActivation * getSpeedMax());
									  else
										  distance = 0;

									  if (distance > 1){
										  posRelative.x = int(posRelative.x / distance);
										  posRelative.y = int(posRelative.y / distance);
										  setLastAction(FCMPredator::SearchFoodFailAction);  //-- Search for food is failed because the distance is far
									  }
									  else
									  if (int(foodClose.size()) > 0)
										  setLastAction(FCMPredator::SearchFoodAction);
									  else
									  if (distance > 0){// --if ( (int)food.size() == 0) -> No food then random walk
										  posRelative.x = int(posRelative.x / distance);
										  posRelative.y = int(posRelative.y / distance);
										  setLastAction(FCMPredator::SearchFoodFailAction); //-- Search for food is failed
									  }
									  else {
										  posRelative.x = 0;
										  posRelative.y = 0;
										  setLastAction(FCMPredator::SearchFoodFailAction); //-- Search for food is failed
									  }

									  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

									  break;
	}

	case FCMPredator::Socialize: { //-- sociabilisation
									 //-- Meisam, Feb 2011: 1)was rewrited. 2)Random walk was changed

									 if (int(friendsClose.size()) > 0) { //-- goes toward friendsClose
										 dir = getPosition()->calculDirection(*(eco->wolves[friendsClose[int(friendsClose.size()) - 1]].getPosition()), eco->getWidth(), eco->getHeight());
									 }
									 else { //-- There isn't any friendsClose in the vicinity
										 dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25)); : random socialize
										 dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(30));
									 }
									 posRelative.x = dir.x;
									 posRelative.y = dir.y;
									 distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

									 //-- Meisam
									 float ActionActivation = model.get_activations()->at(ao);
									 if (fabs(ActionActivation) > 0)
										 distance = distance / (ActionActivation * getSpeedMax());
									 else
										 distance = 0;

									 if (distance > 1){
										 posRelative.x = int(posRelative.x / distance);
										 posRelative.y = int(posRelative.y / distance);
										 setLastAction(FCMPredator::SocializeFailAction); //-- Socialization failed because the distance is far
									 }
									 else
									 if (int(friendsClose.size()) > 0){
										 model.ConceptIndex = FCMPredator::SearchPartner;	 //-- Meisam
										 model.ConceptMultiplier = float(1 / 3.0);			 //-- Meisam
										 setLastAction(FCMPredator::SocializeAction);
									 }
									 else
									 if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friend then random walk
										 posRelative.x = int(posRelative.x / distance);
										 posRelative.y = int(posRelative.y / distance);
										 setLastAction(FCMPredator::SocializeFailAction); //-- Socialization failed
									 }
									 else { //-- distance == 0
										 posRelative.x = 0;
										 posRelative.y = 0;
										 setLastAction(FCMPredator::SocializeFailAction); //-- Socialization failed
									 }

									 posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

									 break;
	}

		// ========================= A.G
	case FCMPredator::Move2StrongestPredator: {
												  //-- 1)was rewrited. 2)Random walk was changed

												  if (StrengthStrongestPredatorCloseIndex != -1) { //-- goes toward friendsClose
													  dir = getPosition()->calculDirection(*(eco->wolves[StrengthStrongestPredatorCloseIndex].getPosition()), eco->getWidth(), eco->getHeight());
												  }
												  else { //-- There isn't any friendsClose in the vicinity
													  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25)); : random
													  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(30));
												  }
												  posRelative.x = dir.x;
												  posRelative.y = dir.y;
												  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

												  float ActionActivation = model.get_activations()->at(ao);
												  if (fabs(ActionActivation) > 0)
													  distance = distance / (ActionActivation * getSpeedMax());
												  else
													  distance = 0;

												  if (distance > 1){
													  posRelative.x = int(posRelative.x / distance);
													  posRelative.y = int(posRelative.y / distance);
													  setLastAction(FCMPredator::Move2StrongestPredatorFailAction); //-- Socialization failed because the distance is far
												  }
												  else
												  if (StrengthStrongestPredatorCloseIndex != -1){
													  model.ConceptIndex = FCMPredator::SearchPartner;
													  model.ConceptMultiplier = float(1 / 3.0);
													  setLastAction(FCMPredator::Move2StrongestPredatorAction);
												  }
												  else
												  if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friend then random walk
													  posRelative.x = int(posRelative.x / distance);
													  posRelative.y = int(posRelative.y / distance);
													  setLastAction(FCMPredator::Move2StrongestPredatorFailAction); //-- Socialization failed
												  }
												  else { //-- distance == 0
													  posRelative.x = 0;
													  posRelative.y = 0;
													  setLastAction(FCMPredator::Move2StrongestPredatorFailAction); //-- Socialization failed
												  }

												  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

												  break;
	}

	case FCMPredator::Move2StrongestPreyDistance: {
													  //-- 1)was rewrited. 2)Random walk was changed

													  if (StrengthStrongestPreyCloseIndex != -1) { //-- goes toward friendsClose
														  dir = getPosition()->calculDirection(*(eco->rabbits[StrengthStrongestPreyCloseIndex].getPosition()), eco->getWidth(), eco->getHeight());
													  }
													  else { //-- There isn't any friendsClose in the vicinity
														  dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25)); : random socialize
														  dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(30));
													  }
													  posRelative.x = dir.x;
													  posRelative.y = dir.y;
													  distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

													  float ActionActivation = model.get_activations()->at(ao);
													  if (fabs(ActionActivation) > 0)
														  distance = distance / (ActionActivation * getSpeedMax());
													  else
														  distance = 0;

													  if (distance > 1){
														  posRelative.x = int(posRelative.x / distance);
														  posRelative.y = int(posRelative.y / distance);
														  setLastAction(FCMPredator::Move2StrongestPreyDistanceFailAction); //-- Socialization failed because the distance is far
													  }
													  else
													  if (StrengthStrongestPreyCloseIndex != -1){
														  model.ConceptIndex = FCMPredator::ChaseAway;
														  model.ConceptMultiplier = float(1 / 3.0);
														  setLastAction(FCMPredator::Move2StrongestPreyDistanceAction);
													  }
													  else
													  if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friend then random walk
														  posRelative.x = int(posRelative.x / distance);
														  posRelative.y = int(posRelative.y / distance);
														  setLastAction(FCMPredator::Move2StrongestPreyDistanceFailAction); //-- Socialization failed
													  }
													  else { //-- distance == 0
														  posRelative.x = 0;
														  posRelative.y = 0;
														  setLastAction(FCMPredator::Move2StrongestPreyDistanceFailAction); //-- Socialization failed
													  }

													  posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

													  break;
	}

	case FCMPredator::Move2WeakestPreyCell: {
												//-- 1)was rewrited. 2)Random walk was changed

												if (StrengthWeakestPreyCellClosePos.x != -1) { //-- goes toward friendsClose
													dir = getPosition()->calculDirection(StrengthWeakestPreyCellClosePos, eco->getWidth(), eco->getHeight());
												}
												else { //-- There isn't any friendsClose in the vicinity
													dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25)); : random socialize
													dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(30));
												}
												posRelative.x = dir.x;
												posRelative.y = dir.y;
												distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

												float ActionActivation = model.get_activations()->at(ao);
												if (fabs(ActionActivation) > 0)
													distance = distance / (ActionActivation * getSpeedMax());
												else
													distance = 0;

												if (distance > 1){
													posRelative.x = int(posRelative.x / distance);
													posRelative.y = int(posRelative.y / distance);
													setLastAction(FCMPredator::Move2WeakestPreyCellFailAction); //-- Socialization failed because the distance is far
												}
												else
												if (StrengthWeakestPreyCellClosePos.x != -1){
													model.ConceptIndex = FCMPredator::ChaseAway;
													model.ConceptMultiplier = float(1 / 3.0);
													setLastAction(FCMPredator::Move2WeakestPreyCellAction);
												}
												else
												if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friend then random walk
													posRelative.x = int(posRelative.x / distance);
													posRelative.y = int(posRelative.y / distance);
													setLastAction(FCMPredator::Move2WeakestPreyCellFailAction); //-- Socialization failed
												}
												else { //-- distance == 0
													posRelative.x = 0;
													posRelative.y = 0;
													setLastAction(FCMPredator::Move2WeakestPreyCellFailAction); //-- Socialization failed
												}

												posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

												break;
	}

	case FCMPredator::Move2WeakestPrey: {
											//-- 1)was rewrited. 2)Random walk was changed

											if (StrengthWeakestPreyCloseIndex != -1) { //-- goes toward friendsClose
												dir = getPosition()->calculDirection(*(eco->rabbits[StrengthWeakestPreyCloseIndex].getPosition()), eco->getWidth(), eco->getHeight());
											}
											else { //-- There isn't any friendsClose in the vicinity
												dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(25)); : random socialize
												dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 + r->next(30));
											}
											posRelative.x = dir.x;
											posRelative.y = dir.y;
											distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

											float ActionActivation = model.get_activations()->at(ao);
											if (fabs(ActionActivation) > 0)
												distance = distance / (ActionActivation * getSpeedMax());
											else
												distance = 0;

											if (distance > 1){
												posRelative.x = int(posRelative.x / distance);
												posRelative.y = int(posRelative.y / distance);
												setLastAction(FCMPredator::Move2WeakestPreyFailAction); //-- Socialization failed because the distance is far
											}
											else
											if (StrengthWeakestPreyCloseIndex != -1){
												model.ConceptIndex = FCMPredator::ChaseAway;
												model.ConceptMultiplier = float(1 / 3.0);
												setLastAction(FCMPredator::Move2WeakestPreyAction);
											}
											else
											if (distance > 0){//-- if ( (int)friendsClose.size() == 0) -> No friend then random walk
												posRelative.x = int(posRelative.x / distance);
												posRelative.y = int(posRelative.y / distance);
												setLastAction(FCMPredator::Move2WeakestPreyFailAction); //-- Socialization failed
											}
											else { //-- distance == 0
												posRelative.x = 0;
												posRelative.y = 0;
												setLastAction(FCMPredator::Move2WeakestPreyFailAction); //-- Socialization failed
											}

											posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

											break;
	}

		//========================
	case FCMPredator::Exploration: { //-- explore
									   //-- Meisam, Feb 2011: 1)Condition "if (distance > 1)" was added 2)Random walk was changed

									   dir.x = VisionMe - r->next(2 * VisionMe + 1); //-- (13 - r->next(30)); : random explore
									   dir.y = VisionMe - r->next(2 * VisionMe + 1); //-- (13 - r->next(30));

									   posRelative.x = dir.x;
									   posRelative.y = dir.y;
									   distance = posNouvelle.distance(posRelative, eco->getWidth(), eco->getHeight());

									   //-- Meisam
									   float ActionActivation = model.get_activations()->at(ao);
									   if (fabs(ActionActivation) > 0)
										   distance = distance / (ActionActivation * getSpeedMax());
									   else
										   distance = 0;

									   if (distance > 0) {
										   posRelative.x = int(posRelative.x / distance);
										   posRelative.y = int(posRelative.y / distance);
									   }
									   else {
										   posRelative.x = 0;
										   posRelative.y = 0;
									   }

									   posNouvelle = Movement(posRelative.x, posRelative.y, eco->getWidth(), eco->getHeight());

									   model.ConceptIndex = FCMPredator::Curiosity;	//-- Meisam
									   model.ConceptMultiplier = float(1 / 1.5);		//-- Meisam
									   setLastAction(FCMPredator::ExplorationAction);
									   break;
	}

	case FCMPredator::Wait: { //-- stay
								posNouvelle.x = getPosition()->x;
								posNouvelle.y = getPosition()->y;
								setLastAction(FCMPredator::WaitAction);
								break;
	}

	case FCMPredator::Eat: { //-- eat
							   posNouvelle.x = getPosition()->x;
							   posNouvelle.y = getPosition()->y;
							   float ava_energy = eco->getLevelMeat(posNouvelle); // M.M
							   if (ava_energy > 0) {
								   float req_energy = this->getEnergyMax() - this->getEnergy(); //M.M The energy that pradator needs
								   if (ava_energy >= req_energy) {// available energy more than required energy  /M.M //RS FOOD
									   eco->decLevelMeat(posNouvelle, req_energy);
									   setEnergy(this->getEnergyMax());
								   }
								   else {// available energy is not enough but still better than nothing   /M.M
									   eco->decLevelMeat(posNouvelle, ava_energy);
									   setEnergy(e + ava_energy);	//RS FOOD
								   }
								   //eco->decLevelMeat(posNouvelle);
								   // setEnergy(e + eco->getValuePrey()); M.M
								   setLastAction(FCMPredator::EatAction);
								   model.ConceptIndex = FCMPredator::Hunger;  //-- Meisam
								   model.ConceptMultiplier = float(1 / 5.0);   //-- Meisam
							   }
							   else
								   setLastAction(FCMPredator::EatFailAction);  //-- Eating failed
							   break;
	}

	case FCMPredator::Reproduce: { //-- reproduce
									 //-- Meisam, Feb 2011: 1) was rewrited. 2) Mating based on the all individuals in the current cell
									 posNouvelle.x = getPosition()->x;
									 posNouvelle.y = getPosition()->y;

									 //vector<int> localFriends = *(eco->getListCreature(*(getPosition()), 1)); //-- list of individuals in the current cell
									 //int NBfriends1 = (int)localFriends.size() - 1;
									 //cout << endl << "NBfriends1: " << NBfriends1;

									 vector<int> localMaleFriends = *(eco->getListMaleCreature(*(getPosition()), 1)); //-- list of individuals in the current cell
									 int NBfriends = (int)localMaleFriends.size() - 1;

									 //cout << endl << "NBfriends: " << NBfriends;
									
									 cntReprodFailedTries = 0; //MRE RandomGoodGene
									 reproduceFailed1 = 0;	//-- 1: Energy of prey is not enoug, 10: partner has not found, 11: both of them
									 reproduceFailed2 = 0;  //-- "energy of partner is not sufficient"
									 reproduceFailed3 = 0;  //-- "Partner has acted befor"
									 reproduceFailed4 = 0;  //-- "Partner has chosen another action"
									 reproduceFailed5 = 0;  //-- "Distance is far"
									 //reproduceFailed6 = (float)NBfriends; //-- "Same gender" //Since Females are initiating reproduction, it happens when mate is male//Reproduction based on gender /MRE/

									 reproduceFailed1 = reproduceFailed1 | ((NBfriends <= 0) ? 10 : 00); //-- partner has not found

									 bool pasFait = true;
									 if (getEnergy() < (this->getStateBirth() * this->getEnergyMax() / 100)) {
										 reproduceFailed1 = reproduceFailed1 | 1;
									 } //-- Energy of prey is not enough
									 else{

										 if (MatingMode == 0){ //Random Mating

											 int k;

											 while (pasFait && int(localMaleFriends.size()) > 0) {
												 k = eco->rand.next(0, int(localMaleFriends.size()) - 1);   //-- make randomomize order

												 if (getID() != eco->wolves[localMaleFriends[k]].getID()) {    //-- Preventing for mating with itself

													 if (eco->wolves[localMaleFriends[k]].getUpdated() == 0) { //partner has not acted

														 if ((eco->wolves[localMaleFriends[k]]).getAge() >= (eco->wolves[localMaleFriends[k]]).getRepAge() && eco->wolves[localMaleFriends[k]].getFCM()->ReviseReproductionDecision(getPersuasion(), eco)){ //partner want to reproduce

															 if (eco->wolves[localMaleFriends[k]].getEnergy() > (eco->wolves[localMaleFriends[k]].getEnergyMax() * eco->wolves[localMaleFriends[k]].getStateBirth() / 100)) { //-- Energy of partner is enough

																 float distance = this->phGenome.calcDistance(eco->wolves[localMaleFriends[k]].getPhGenome()); // M.M
																 if (distance + model.calculDistanceEvol(eco, eco->wolves[localMaleFriends[k]].getFCM()) < eco->getDistanceSpeciesPred()) { //-- Distance is not far
																	 //DO REPRODUCTION
																	 this->setEnergy(this->getEnergy() - cntReprodFailedTries * 2); //MRE decrease 2 unit of female energy for unsuccessful mating tries
																	 float distance = this->phGenome.calcDistance(eco->wolves[localMaleFriends[k]].getPhGenome());  // M.M
																	 set_DistMating(distance + model.calculDistanceEvol(eco, eco->wolves[localMaleFriends[k]].getFCM()));  // M.M modified
																	 reprod(&eco->wolves[localMaleFriends[k]], nouveauNe, eco); //-- mePredator reproduces with wolves[localMaleFriends[k]]
																	 pasFait = false;
																	 setLastAction(FCMPredator::ReproduceAction);
																	 model.ConceptIndex = FCMPredator::SearchPartner;
																	 model.ConceptMultiplier = 0;
																	 eco->wolves[localMaleFriends[k]].getFCM()->ConceptIndex = FCMPredator::SearchPartner;
																	 eco->wolves[localMaleFriends[k]].getFCM()->ConceptMultiplier = 0;

																 }
																 else{//-- Distance is far!
																	 reproduceFailed5++;
																	 cntReprodFailedTries++;
																 }

															 }
															 else{// Energy of partner is not enough
																 reproduceFailed2++;
																 cntReprodFailedTries++;
															 }

														 }
														 else{ //Partner has chosen another action
															 reproduceFailed4++;
															 cntReprodFailedTries++;
														 }

													 }
													 else{ //partner has acted!
														 reproduceFailed3++;
														 cntReprodFailedTries++;
													 }
												 }

												 localMaleFriends.erase(localMaleFriends.begin() + k);
											 }

										 }
										 else if (MatingMode == 1){///Good Gene Mating

											 //int k; //Reproduction based on gender /MRE/ Changed the logic of k, beacuse the list is not random anymore. we need a sorted list
											 int cntrLocalFriends = 0;//MRE

											 while (pasFait && cntrLocalFriends <= int(localMaleFriends.size()) - 1) {
												 //k = eco->rand.next(0, int (localMaleFriends.size()) - 1);   //-- make randomomize order

												 if (getID() != eco->wolves[localMaleFriends[cntrLocalFriends]].getID()) {    //-- Preventing for mating with itself

													 if (eco->wolves[localMaleFriends[cntrLocalFriends]].getUpdated() == 0) { //partner has not acted

														 if ((eco->wolves[localMaleFriends[cntrLocalFriends]]).getAge() >= (eco->wolves[localMaleFriends[cntrLocalFriends]]).getRepAge() && eco->wolves[localMaleFriends[cntrLocalFriends]].getFCM()->ReviseReproductionDecision(getPersuasion(), eco)){ //partner want to reproduce

															 if (eco->wolves[localMaleFriends[cntrLocalFriends]].getEnergy() > (eco->wolves[localMaleFriends[cntrLocalFriends]].getEnergyMax() * eco->wolves[localMaleFriends[cntrLocalFriends]].getStateBirth() / 100)) { //-- Energy of partner is enough
																 float distance = this->phGenome.calcDistance(eco->wolves[localMaleFriends[cntrLocalFriends]].getPhGenome()); // M.M																 
																 if (distance + model.calculDistanceEvol(eco, eco->wolves[localMaleFriends[cntrLocalFriends]].getFCM()) < eco->getDistanceSpeciesPred()) { //-- Distance is not far
																	 //DO REPRODUCTION
																	 this->setEnergy(this->getEnergy() - (cntReprodFailedTries * 2 + 2)); //MRE decrease 2 unit of female energy for unsuccessful mating tries + 2
																	 float distance = this->phGenome.calcDistance(eco->wolves[localMaleFriends[cntrLocalFriends]].getPhGenome());  // M.M
																	 set_DistMating(distance + model.calculDistanceEvol(eco, eco->wolves[localMaleFriends[cntrLocalFriends]].getFCM()));  // M.M modified
																	 reprod(&eco->wolves[localMaleFriends[cntrLocalFriends]], nouveauNe, eco); //-- mePredator reproduces with wolves[localMaleFriends[k]]
																	 pasFait = false;
																	 setLastAction(FCMPredator::ReproduceAction);
																	 model.ConceptIndex = FCMPredator::SearchPartner;
																	 model.ConceptMultiplier = 0;
																	 eco->wolves[localMaleFriends[cntrLocalFriends]].getFCM()->ConceptIndex = FCMPredator::SearchPartner;
																	 eco->wolves[localMaleFriends[cntrLocalFriends]].getFCM()->ConceptMultiplier = 0;
																 }
																 else{//-- Distance is far!
																	 reproduceFailed5++;
																	 cntReprodFailedTries++;
																 }

															 }
															 else{// Energy of partner is not enough
																 reproduceFailed2++;
																 cntReprodFailedTries++;
															 }

														 }
														 else{ //Partner has chosen another action
															 reproduceFailed4++;
															 cntReprodFailedTries++;
														 }

													 }
													 else{ //partner has acted!
														 reproduceFailed3++;
														 cntReprodFailedTries++;
													 }

												 }
												 cntrLocalFriends += 1;
											 }// -- End of while

										 }
										 else if (MatingMode == 2){ //intermediate mating

											 vector<int> LocalMaleFriends;
											 float sumOfSize = 0;
											 for (unsigned int i = 0; i < localMaleFriends.size(); i++) {
												 //if ((short)eco->wolves[localMaleFriends[i]].getGender() == 0){ //Male
												 cout << flush;
												 sumOfSize += (float)eco->wolves[localMaleFriends[i]].getEnergy();
												 LocalMaleFriends.push_back(localMaleFriends[i]);
												 //}
											 }

											 if (sumOfSize < 1)
												 sumOfSize = 1;

											 ////debug info
											 //cout << endl << "--------------NEXT INDIVIDUAL-----------------" << endl << "LocalMaleFriends.size: " << LocalMaleFriends.size() << " || localMaleFriends.size: " << localMaleFriends.size() << endl;

											 ////debug info
											 //cout << "sumOfSize: " << sumOfSize << endl;


											 int cntrLocalFriends = 0;//MRE
											 int sumOfFailedTry = 0;

											 while (pasFait && cntrLocalFriends <= int(LocalMaleFriends.size()) - 1) {
												 //generate a random number,
												 float rnd = eco->rand.next(sumOfSize);
												 int selectedRandomMateIndex = -1;

												 ////debug info
												 //cout << " rnd: " << rnd << endl;

												 for (int i = 0; i < LocalMaleFriends.size(); i++) {
													 float currentPreySize = (float)eco->wolves[LocalMaleFriends[i]].getEnergy();

													 ////debug info
													 //cout << " currentPreySize: " << currentPreySize << endl;

													 if (rnd < currentPreySize)
													 {
														 rnd -= currentPreySize;
														 LocalMaleFriends.erase(LocalMaleFriends.begin() + i); //remove the item from array - avoid selecting as next male candidate
														 selectedRandomMateIndex = i;
														 break;
													 }
													 else{
														 rnd -= currentPreySize;
														 ////debug info
														 //cout << " rnd is not less than currentPreySize. new rnd value is: " << rnd << endl;
													 }
												 }

												 ////debug info
												 //cout << " selected male id is: " << selectedRandomMateIndex << endl;


												 if (getID() != eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getID()) {    //-- Preventing for mating with itself

													 if (eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getUpdated() == 0) { //partner has not acted

														 if ((eco->wolves[LocalMaleFriends[selectedRandomMateIndex]]).getAge() >= (eco->wolves[LocalMaleFriends[selectedRandomMateIndex]]).getRepAge() && eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getFCM()->ReviseReproductionDecision(getPersuasion(), eco)){ //partner want to reproduce

															 if (eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getEnergy() > (eco->wolves[localMaleFriends[selectedRandomMateIndex]].getEnergyMax() * eco->wolves[localMaleFriends[selectedRandomMateIndex]].getStateBirth() / 100)) { //-- Energy of partner is enough
																 float distance = this->phGenome.calcDistance(eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getPhGenome()); // M.M																 
																 if (distance + model.calculDistanceEvol(eco, eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getFCM()) < eco->getDistanceSpeciesPred()) { //-- Distance is not far
																	 //DO REPRODUCTION
																	 this->setEnergy(this->getEnergy() - (cntReprodFailedTries * 2 + 1)); //MRE decrease 2 unit of female energy for unsuccessful mating tries + 1
																	 float distance = this->phGenome.calcDistance(eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getPhGenome());  // M.M
																	 set_DistMating(distance + model.calculDistanceEvol(eco, eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getFCM()));  // M.M modified
																	 reprod(&eco->wolves[LocalMaleFriends[selectedRandomMateIndex]], nouveauNe, eco); //-- mePredator reproduces with wolves[LocalMaleFriends[k]]
																	 pasFait = false;
																	 setLastAction(FCMPredator::ReproduceAction);
																	 model.ConceptIndex = FCMPredator::SearchPartner;
																	 model.ConceptMultiplier = 0;
																	 eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getFCM()->ConceptIndex = FCMPredator::SearchPartner;
																	 eco->wolves[LocalMaleFriends[selectedRandomMateIndex]].getFCM()->ConceptMultiplier = 0;

																 }
																 else{//-- Distance is far!
																	 reproduceFailed5++;
																	 cntReprodFailedTries++;
																 }

															 }
															 else{// Energy of partner is not enough
																 reproduceFailed2++;
																 cntReprodFailedTries++;
															 }

														 }
														 else{ //Partner has chosen another action
															 reproduceFailed4++;
															 cntReprodFailedTries++;
														 }

													 }
													 else{ //partner has acted!
														 reproduceFailed3++;
														 cntReprodFailedTries++;
													 }

												 }
												 cntrLocalFriends += 1;
											 }// -- End of while

											 LocalMaleFriends.clear();
										 }

									 }
									 localMaleFriends.clear();

									 if (pasFait){
										 setLastAction(FCMPredator::ReproduceFailAction); //-- Reproduction failed
									 }
									 break;
	}

	default:
		break;
	}//-- Endof switch

	float speedCurent = getPosition()->distance(posNouvelle, eco->getWidth(), eco->getHeight());
	setSpeed(speedCurent);
	return posNouvelle;
}

