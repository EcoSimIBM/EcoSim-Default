/*
 * Sim.cpp
 *  - Details about the simulation such as vision of prey,
 *    energie of prey and predators, etc.
 */

#include "Ecosystem.h"
#include "Sim.h"
#include "Position.h"
#include "PhysicalGenome.h"
#include <stdio.h>


Sim::Sim() {

}

Sim::~Sim() {
}
 
// for first generation individuals
Sim::Sim(short type, long ID, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, float energyMax, short speedMax, short ageMax, short vision, short repAge, float distanceEvol, float stateBirth, unsigned char nbGenes, short genderVal, float defence, float coopDefence, short persuasionVal) //Add Gender /MRE/
{
	this->type = type;
	this->id = ID;
	this->species = species;

	this->parent1ID = parent1ID;
	this->parent2ID = parent2ID;
	this->speciesParent1ID = speciesParent1ID;
	this->speciesParent2ID = speciesParent2ID;

	this->energyMax = energyMax;
	this->speedMax = speedMax;
	this->ageMax = ageMax;
	this->vision = vision;
	this->distanceEvol = distanceEvol;
	this->stateBirth = stateBirth;
	this->repAge = repAge;


	this->DistMating = 0;
	this->pos.x = 0;
	this->pos.y = 0;
	this->updated = 0;
	this->lastAction = 0;
	this->living = true;

	this->nbReprod = 0;

	//-- Meisam: Reasons of reproduction failed
	cntReprodFailedTries = 0;
	reproduceFailed1 = 0;
	reproduceFailed2 = 0;
	reproduceFailed3 = 0;
	reproduceFailed4 = 0;
	reproduceFailed5 = 0;
	//reproduceFailed6 = 0;
	//-- Meisam End


	// Genome initialization /M.M
	float genomeArr[nbGenes];
	genomeArr[0] = energyMax;
	genomeArr[1] = ageMax;
	genomeArr[2] = vision;
	genomeArr[3] = speedMax;
	genomeArr[4] = repAge;
	genomeArr[5] = stateBirth;
	if (nbGenes > 6)
	{
		genomeArr[6] = defence;   // prey defence
		genomeArr[7] = coopDefence;   // prey cooperative defence
	}
	vector<float> genome(genomeArr, genomeArr + sizeof(genomeArr) / sizeof(float));
	phGenome = PhysicalGenome(nbGenes, &genome);
	phGenome.setPersuasion((unsigned char)persuasionVal);
}

// for new born individuals
Sim::Sim(short type, long ID, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, float distanceEvol, unsigned char nbGenes, short gender){//Add Gender /MRE/

	this->type = type;
	this->id = ID;
	this->species = species;

	this->parent1ID = parent1ID;
	this->parent2ID = parent2ID;
	this->speciesParent1ID = speciesParent1ID;
	this->speciesParent2ID = speciesParent2ID;

	//energyMax; speedMax; ageMax; vision; stateBirth; are in the genome		 // M.M
	this->distanceEvol = distanceEvol;
	this->DistMating = 0;
	this->pos.x = 0;
	this->pos.y = 0;
	this->energy = 0;
	this->age = 0;
	this->speed = 0;
	this->updated = 0;
	this->lastAction = 0;
	this->living = true;

	this->nbReprod = 0;

	this->gender = gender;//Add Gender /MRE/

	//-- Meisam: Reasons of reproduction failed
	reproduceFailed1 = 0;
	reproduceFailed2 = 0;
	reproduceFailed3 = 0;
	reproduceFailed4 = 0;
	reproduceFailed5 = 0;
	//reproduceFailed6 = 0;
	cntReprodFailedTries = 0;
	//-- Meisam End

	// Genome initialization /M.M
	phGenome = PhysicalGenome(nbGenes); // 0.005 should be come from ecosystem. How ?!
}

short Sim::getAgeMax(bool addOffset) { // determine if the real ageMax or ageMax in genome is required. /M.M
	return (short)phGenome.getGene(1);   // M.M				// MaxAge in genome
}

void Sim::setEnergy(float e) {
	float maxEnergy = this->getEnergyMax();
	//MRE NewRandomGoodGene
	if (e < 0)
		energy = 0;

	if (e <= maxEnergy)
		energy = e;
	else
		energy = maxEnergy;
	calcStrength(type); // update the strength based on the new energy. / M.M
}

Position Sim::Movement(int x, int y, int Width, int Height) {
	Position pResult; //-- pos is member of class

	pResult.x = pos.x + x;
	pResult.y = pos.y + y;

	//-- Keep position within bounds
	pResult.x = pResult.x % Width;
	if (pResult.x < 0)
		pResult.x = pResult.x + Width;

	pResult.y = pResult.y % Height;
	if (pResult.y < 0)
		pResult.y = pResult.y + Height;

	return pResult;
}

//MRE change energy formula
float Sim::fixRange(float value, float start, float step)
{
	if (value >1)
		value += 0.05; // solve float precision problem
	return (float)((value - start) / step);
}

float Sim::calculationEnergyAction(float energy, int nbArc, float speed, float strength, float persuasion, Ecosystem * eco){

	//return ( energy - (float) ( ((float)nbArc / 4.0) + pow((double)speed, 1.4)) );  old version
	float maxEnergy, vision, maxSpeed, repAge, defence, coopDefence;

	maxEnergy = getEnergyMax();
	vision = (float)getVision();
	maxSpeed = (float)getSpeedMax();
	repAge = (float)getRepAge();
	float e = 0;

	float e_old = 0;

	if (phGenome.getGenomeSize() > 6){ // prey
		defence = (float)getDefence();
		coopDefence = (float)getCoopDefence();
		
		//e_old = energy - ((float)(nbArc - 100) / 3.1 + pow((double)speed, 1.5) + pow((double)maxEnergy / 18, 1.25) + pow((double)vision / 4, 2) + pow((double)maxSpeed / 5, 1.5) + pow((double)defence / 18, 1.7) + pow((double)coopDefence / 18, 1.7) + pow((double)max(0, 7 - repAge), 2.1));
		//e = energy - ((FC - 100) ^ 1.34) / 8 + (St*(S ^ 2)) / 40 + (ME / 18) ^ 1.34 + (V / 2) ^ 1.34 + (St*(MS^1.34)) / 200 + (D / 18) ^ 1.34 + (max(0, 7 - RA)) ^ 2.1;

		//New Energy Formula From Dr. Grass, June 2014
		float arcPenalty = 0;
		if (nbArc >= 101) arcPenalty = pow(((double)nbArc - 100), 0.75);
		else if (nbArc < 101) arcPenalty = 1;
		float energyToSpend = ((.8 * arcPenalty)  + ((strength*(pow((double)speed, 2))) / 10000.0) + pow((double)maxEnergy / 5.5, 0.75) + pow((double)vision * 5.0, 0.75) + (pow((double)maxSpeed * 5.0, 0.75)) + pow((double)defence * 100.0, 0.75) + pow((double)coopDefence * 75.0, 0.75) + pow((double)max(0.0, (double)(8 - repAge)), 2.3) + pow((double)persuasion * 5.0, 0.75));
		
		e = energy - energyToSpend;
	
		if (getGender() == 0){
			eco->statWorld.incEnergySpentMalePrey(energyToSpend);
		}
		else if (getGender() == 1){
			eco->statWorld.incEnergySpentFemalePrey(energyToSpend);
		}
		
	}
	else{ // predator
		//e_old = energy - ((float)(nbArc - 100) / 3.1 + pow((double)speed, 1.5) + pow((double)maxEnergy / 15, 1.4) + pow((double)vision / 4, 1.9) + pow((double)maxSpeed / 5, 1.5) + pow((double)max(0, 7 - repAge), 2.3));

		//e = energy - ((FC - 100) ^ 1.34) / 1 + (St*(S ^ 2)) / 56 + (ME / 18) ^ 1.34 + (V / 2) ^ 1.34 + (St*(MS^1.34)) / 200 + (max(0, 7 - RA)) ^ 2.3

		//New Energy Formula From Dr. Grass, June 2014
		
		float arcPenalty = 0;
		if (nbArc >= 131) arcPenalty = pow(((double)nbArc - 130), 0.75);
		else if (nbArc < 131) arcPenalty = 1;
		float energyToSpend = ((.8 * arcPenalty)  + ((strength*(pow((double)speed, 2))) / 11000.0) + pow((double)maxEnergy / 5.5, 0.75) + pow((double)vision * 5.0, 0.75) + pow((double)maxSpeed * 5.0, 0.75) + pow((double)max(0.0, (double)(7 - repAge)), 2.3) + pow((double)persuasion * 5.0, 0.75));
		e = energy - energyToSpend;
		if (getGender() == 0){
			eco->statWorld.incEnergySpentMalePred(energyToSpend);
		}
		else if (getGender() == 1){
			eco->statWorld.incEnergySpentFemalePred(energyToSpend);
		}
		
	}

	return e; // M.M return new energy

}

float Sim::calculationStateOfBirth(float sobMax, float sobParent, float ProbMut, float PercentMutHigh, Random *r){

	float sob; //-- State of Birth of the child
	float prob_sob_mut = float(r->next(32000) / 32000.0);

	if (prob_sob_mut <= ProbMut) {
		float sob_mut_rate = (float)(r->next((int)(2 * PercentMutHigh * 32000.0)) / 32000.0) - PercentMutHigh;
		sob = sobParent + sobParent * sob_mut_rate;
	}
	else {
		sob = sobParent;
	}

	if (sob > sobMax)
		sob = sobMax;
	//-- if sob < 0 it can make a problem

	return sob;
}

//MRE RandomGoodGene
float Sim::calculationEnergyNewBorn(float sobParent1, float EnergyMaxParent1, float sobParent2, float EnergyMaxParent2){

	float energy = (EnergyMaxParent1 * sobParent1 + EnergyMaxParent2 * sobParent2) / (float)100;
	//cout << "EnMain:" << energy << endl;
	return energy;
}


//MRE RandomGoodGene
float Sim::calculationEnergyNewParent(float energyParent1, float energyNewBorn, float sobParent1, float sobParent2){
	//Parent1 should have the same gender with bebe
	//1.05: waste energy for reproduction
	float energy = energyParent1 - (sobParent1 / (sobParent1 + sobParent2))*energyNewBorn*1.05;
	//cout << energy << endl;
	return energy;
}


//float Sim::calculationEnergyNewBorn(float sobParent1, float EnergyMaxParent1, float sobParent2, float EnergyMaxParent2){
//
//	//-- sob: State Of Birth
//	float energy = EnergyMaxParent1 * sobParent1 / (float)100;
//
//	return energy;
//}
//
//
//float Sim::calculationEnergyNewParent(float energyParent1, float energyNewBorn, float sobParent1, float sobParent2){
//
//	//-- sob: State Of Birth
//	float coefficient = 0.05 + (sobParent1 / 200);
//	return energyParent1 - coefficient * 650;
//}


// M.M based on current energy and MaxEnergy
void Sim::calcStrength(short type)
{
	GENETYPE repAge, maxAge;
	float maxEnergy = getEnergyMax();
	float energy = getEnergy();
	repAge = phGenome[4];
	maxAge = phGenome[1];
	switch (type){

	case 0: // prey
		if (energy >= (1.0 / 3.0) * maxEnergy){
			if ((age <= repAge) || (age >= maxAge - repAge)) // too young or too old
				strength = (2.0 / 3.0) * maxEnergy;
			else
				strength = maxEnergy;
		}
		else{
			if ((age <= repAge) || (age >= maxAge - repAge)) // too young or too old
				strength = (3.0 / 2.0) * energy + (1.0 / 4.0) * maxEnergy;
			else
				strength = (3.0 / 2.0) * energy + (1.0 / 2.0) * maxEnergy;
		}
		break;
	case 1: // predator
		if (energy >= (1.0 / 4.0) * maxEnergy){
			if ((age <= repAge) || (age >= maxAge - repAge)) // too young or too old
				strength = (2.5 / 3.0) * maxEnergy;
			else
				strength = maxEnergy;
		}
		else{
			if ((age <= repAge) || (age >= maxAge - repAge)) // too young or too old
				strength = 2.0 * energy + (1.0 / 4.0) * maxEnergy;
			else
				strength = 2.0 * energy + (1.0 / 2.0) * maxEnergy;
		}
		break;

	default:
		cout << "Error in individual type";
		exit(-1);
	}
	if (strength < 0) strength = 0;
}
