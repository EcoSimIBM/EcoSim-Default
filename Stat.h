#ifndef STAT_H_
#define STAT_H_

#include "Species.h"
#include "FCMPrey.h"
#include "Manipulation.h"

class Ecosystem;

using namespace std;

class Stat {
private:

	int nbMalePrey;
	int nbFemalePrey;
	int nbMalePred;
	int nbFemalePred;
	int nbSensPrey;
	int nbConceptsPrey;
	int nbMoteursDepPrey;
	int nbMoteursFixPrey;
	int nbSensPred;
	int nbConceptsPred;
	int nbMoteursDepPred;
	int nbMoteursFixPred;
	//MRE RandomGoodGene
	double energyAvgMalePrey;					//-- Average energy of all prey
	double strengthAvgMalePrey;					//-- Average strength of all prey
	double energyAvgMalePred;					//-- Average energy of all predators
	double strengthAvgMalePred;					//-- Average strength of all pred
	int nbBirthMalePrey;						//-- Number of new prey born
	int nbBirthMalePred;						//-- Number of new predators born
	double ageAvgDeadMalePrey;					//-- Average age of the prey that died
	double ageAvgDeadMalePred;					//-- Average age of the predators that died
	double avgDistEvMalePrey;					//-- Average distance evolved for the prey
	double avgDistEvMalePred;					//-- Average distance evolved for the predators
	
	double avgEnergySpentMalePrey;
	double avgEnergySpentFemalePrey;
	double avgEnergySpentMalePred;
	double avgEnergySpentFemalePred;
	
	double avgEnergyTransferredMalePrey;
	double avgEnergyTransferredFemalePrey;
	double avgEnergyTransferredMalePred;
	double avgEnergyTransferredFemalePred;

	double energyAvgFemalePrey;					//-- Average energy of all prey
	double strengthAvgFemalePrey;					//-- Average strength of all prey
	double energyAvgFemalePred;					//-- Average energy of all predators
	double strengthAvgFemalePred;					//-- Average strength of all pred
	int nbBirthFemalePrey;						//-- Number of new prey born
	int nbBirthFemalePred;						//-- Number of new predators born
	double ageAvgDeadFemalePrey;					//-- Average age of the prey that died
	double ageAvgDeadFemalePred;					//-- Average age of the predators that died
	double avgDistEvFemalePrey;					//-- Average distance evolved for the prey
	double avgDistEvFemalePred;					//-- Average distance evolved for the predators

	double avgDistMatingPrey;
	double avgDistMatingPred;

	vector <double> sumPreyMaleGenome;					//-- keep the sum of genetome M.M
	vector <double> sumPredMaleGenome;					//-- keep the sum of genetome M.M
	vector <double> sumPreyFemaleGenome;					//-- keep the sum of genetome M.M
	vector <double> sumPredFemaleGenome;					//-- keep the sum of genetome M.M

	int nbDeadMalePreyA;						//-- Number of prey that died because of age
	int nbDeadMalePreyE;						//-- Number of prey that died because energy <= 0
	int nbDeadMalePreyK;						//-- Number of prey that died because they were eaten by predator
	int nbDeadMalePredA;						//-- Number of pred that died because of age
	int nbDeadMalePredE;						//-- Number of pred that died because energy <= 0
	int nbDeadMalePredF;						//-- Number of predator that died because of fighting with preys  M.M

	int nbDeadFemalePreyA;						//-- Number of prey that died because of age
	int nbDeadFemalePreyE;						//-- Number of prey that died because energy <= 0
	int nbDeadFemalePreyK;						//-- Number of prey that died because they were eaten by predator
	int nbDeadFemalePredA;						//-- Number of pred that died because of age
	int nbDeadFemalePredE;						//-- Number of pred that died because energy <= 0
	int nbDeadFemalePredF;						//-- Number of predator that died because of fighting with preys  M.M

	double stateAvgMalePrey;						//
	double stateAvgMalePred;						//
	double stateAvgFemalePrey;						//
	double stateAvgFemalePred;						//
	
	double persuasionTotalMalePrey;
	double persuasionTotalMalePred;
	double persuasionTotalFemalePrey;
	double persuasionTotalFemalePred;

	double speedAvgMalePrey;						//-- Average speed of the prey
	double speedAvgMalePred;						//-- Average speed of the predators
	double speedAvgFemalePrey;						//-- Average speed of the prey
	double speedAvgFemalePred;						//-- Average speed of the predators

	int ageMalePrey;							//
	int ageMalePred;							//

	int ageFemalePrey;							//
	int ageFemalePred;							//

	//int nbAvgReprodPrey;					//-- Average number of reproduced prey
	//int nbAvgReprodPred;					//-- Average number of reproduced predators
	//int nbReprodMaxPrey;					//-- Max number of prey reproduced
	//int nbReprodMaxPred;					//-- Max number of predator reproduced
	int nbPreyByCase;						//
	int nbPredByCase;						//
	int nbCasePrey;							//
	int nbCasePred;							//

	int nbArcAvgMalePrey;						//-- Average number of arcs for the MalePrey
	int nbArcAvgMalePred;						//-- Average number of arcs for the MalePredators

	int nbArcAvgFemalePrey;						//-- Average number of arcs for the FemalePrey
	int nbArcAvgFemalePred;						//-- Average number of arcs for the FemalePredators

	long double nbTotalGrass;						//-- Total grass in the world
#ifdef TWO_RESOURCES
	double nbTotalGrass2;					//-- Armin
#endif
	long double nbTotalMeat;						//-- Total meat in the world

	//vector<vector<int> > distribAge;		//-- Distribution of ages

	vector<int> nbActionPreyMale;				//-- Actions of the prey
	vector<int> nbActionPreyFemale;				//-- Actions of the prey
	vector<int> nbActionPredMale;				//-- Actions of the predators
	vector<int> nbActionPredFemale;				//-- Actions of the predators
	vector<float> activations_prey_male;			//-- Prey's concepts
	vector<float> activations_prey_female;			//-- Prey's concepts
	vector<float> activations_pred_male;			//-- Predator's concepts
	vector<float> activations_pred_female;			//-- Predator's concepts

	double gEntropy;						    //-- Marwa: Global entropy for prey population
	double maxEntropy;						//-- Marwa: Maximum global entropy for prey population

	float SpeciesRatioPrey;					//-- Meisam
	float SpeciesRatioPred;					//-- Meisam
	float ExtinctionRatioPrey;				//-- Meisam
	float ExtinctionRatioPred;				//-- Meisam

	Manipulation Manip;							//-- Class Instances: Manipulating the input

public:

	Stat();
	virtual ~Stat();

	Stat(int, int, int, int, int, int, int, int, Ecosystem *);

	void setSpeciesRatioPrey(float s)		{ SpeciesRatioPrey = s; };	//-- Meisam
	void setSpeciesRatioPred(float s)		{ SpeciesRatioPred = s; };	//-- Meisam
	void setExtinctionRatioPrey(float s)  	{ ExtinctionRatioPrey = s; };	//-- Meisam
	void setExtinctionRatioPred(float s)	{ ExtinctionRatioPred = s; };	//-- Meisam

	void incNbMalePrey()					{ nbMalePrey++; };	//-- RS
	void incNbFemalePrey()					{ nbFemalePrey++; };	//-- RS
	void incNbMalePred()  					{ nbMalePred++; };	//-- RS
	void incNbFemalePred()					{ nbFemalePred++; };	//-- RS

	void reset();
	void incNbArcAvgMalePrey(int a)				{ nbArcAvgMalePrey += a; };
	void incNbArcAvgMalePred(int a) 			{ nbArcAvgMalePred += a; };

	void incNbPreyByCase(int n)  			{ nbPreyByCase += n; };
	void incNbPredByCase(int n)    			{ nbPredByCase += n; };
	void incNbCasePrey()   					{ nbCasePrey++; };
	void incNbCasePred()  					{ nbCasePred++; };

	void incPersuasionMalePrey(float a)				{ persuasionTotalMalePrey += a; };
	void incPersuasionMalePred(float a)				{ persuasionTotalMalePred += a; };
	void incPersuasionFemalePrey(float a)				{ persuasionTotalFemalePrey += a; };
	void incPersuasionFemalePred(float a)				{ persuasionTotalFemalePred += a; };
	
	void incNbArcAvgFemalePrey(int a)				{ nbArcAvgFemalePrey += a; };
	void incNbArcAvgFemalePred(int a) 			{ nbArcAvgFemalePred += a; };
	
	void incEnergySpentMalePrey(float a)				{ avgEnergySpentMalePrey += a; };
	void incEnergySpentFemalePrey(float a) 			{ avgEnergySpentFemalePrey += a; };
	void incEnergySpentMalePred(float a)				{ avgEnergySpentMalePred += a; };
	void incEnergySpentFemalePred(float a) 			{ avgEnergySpentFemalePred += a; };
	
	
	void incAvgEnergyTransferredMalePrey(float a)				{ avgEnergyTransferredMalePrey += a; };
	void incAvgEnergyTransferredFemalePrey(float a) 			{ avgEnergyTransferredFemalePrey += a; };
	void incAvgEnergyTransferredMalePred(float a)				{ avgEnergyTransferredMalePred += a; };
	void incAvgEnergyTransferredFemalePred(float a) 			{ avgEnergyTransferredFemalePred += a; };

	void incNbTotalGrass(float n) 			{ nbTotalGrass += (double) n; };
#ifdef TWO_RESOURCES
	void incNbTotalGrass2(float n) 			{ nbTotalGrass2 += (double) n; };
#endif
	void incNbTotalMeat(float n) 			{ nbTotalMeat += n; };
	void incStateAvgMalePred(float e) 			{ stateAvgMalePred += e; };
	void incStateAvgMalePrey(float e) 			{ stateAvgMalePrey += e; };
	void incStateAvgFemalePred(float e) 			{ stateAvgFemalePred += e; };
	void incStateAvgFemalePrey(float e) 			{ stateAvgFemalePrey += e; };
	void incNbActionPreyMale(int i)  			{ nbActionPreyMale.at(i)++; };
	void incNbActionPreyFemale(int i)  			{ nbActionPreyFemale.at(i)++; };
	void incNbActionPredMale(int i)  			{ nbActionPredMale.at(i)++; };
	void incNbActionPredFemale(int i)  			{ nbActionPredFemale.at(i)++; };

	void incNbDeadMalePreyA()  					{ nbDeadMalePreyA++; };
	void incNbDeadMalePreyE()  					{ nbDeadMalePreyE++; };
	void incNbDeadMalePreyK()  					{ nbDeadMalePreyK++; };
	void incNbDeadMalePredA()  					{ nbDeadMalePredA++; };
	void incNbDeadMalePredE()  					{ nbDeadMalePredE++; };
	void incnbDeadMalePredF() 					{ nbDeadMalePredF++; };  		// M.M

	void incNbDeadFemalePreyA()  					{ nbDeadFemalePreyA++; };
	void incNbDeadFemalePreyE()  					{ nbDeadFemalePreyE++; };
	void incNbDeadFemalePreyK()  					{ nbDeadFemalePreyK++; };
	void incNbDeadFemalePredA()  					{ nbDeadFemalePredA++; };
	void incNbDeadFemalePredE()  					{ nbDeadFemalePredE++; };
	void incnbDeadFemalePredF() 					{ nbDeadFemalePredF++; };  		// M.M

	void inc_activation_prey_male(int i, float c){ activations_prey_male.at(i) += c; };
	void inc_activation_prey_female(int i, float c){ activations_prey_female.at(i) += c; };
	void inc_activation_pred_male(int i, float c){ activations_pred_male.at(i) += c; };
	void inc_activation_pred_female(int i, float c){ activations_pred_female.at(i) += c; };
	void incEnergyAvgMalePrey(float e)  		{ energyAvgMalePrey += e; };
	void incStrengthAvgMalePrey(float s) 		{ strengthAvgMalePrey += s; };  // M.M
	void setEnergyAvgMalePrey(float e) 			{ energyAvgMalePrey = e; };
	void setStrengthAvgMalePrey(float s) 		{ strengthAvgMalePrey = s; };   // M.M
	void incEnergyAvgMalePred(float e) 			{ energyAvgMalePred += e; };
	void incStrengthAvgMalePred(float s) 		{ strengthAvgMalePred += s; };  // M.M
	void setEnergyAvgMalePred(float e) 			{ energyAvgMalePred = e; };
	void setStrengthAvgMalePred(float s) 		{ strengthAvgMalePred = s; };   // M.M
	void incNbBirthMalePrey() 					{ nbBirthMalePrey++; };
	void incNbBirthMalePred() 				{ nbBirthMalePred++; };

	void incEnergyAvgFemalePrey(float e)  		{ energyAvgFemalePrey += e; };
	void incStrengthAvgFemalePrey(float s) 		{ strengthAvgFemalePrey += s; };  // M.M
	void setEnergyAvgFemalePrey(float e) 			{ energyAvgFemalePrey = e; };
	void setStrengthAvgFemalePrey(float s) 		{ strengthAvgFemalePrey = s; };   // M.M
	void incEnergyAvgFemalePred(float e) 			{ energyAvgFemalePred += e; };
	void incStrengthAvgFemalePred(float s) 		{ strengthAvgFemalePred += s; };  // M.M
	void setEnergyAvgFemalePred(float e) 			{ energyAvgFemalePred = e; };
	void setStrengthAvgFemalePred(float s) 		{ strengthAvgFemalePred = s; };   // M.M
	void incNbBirthFemalePrey() 					{ nbBirthFemalePrey++; };
	void incNbBirthFemalePred() 					{ nbBirthFemalePred++; };
	void incSpeedAvgMalePrey(float e) 		{ speedAvgMalePrey += e; };//MRE RandomGoodGene
	void setSpeedAvgFemalePrey(float e)  	{ speedAvgFemalePrey = e; };//MRE RandomGoodGene
	void incSpeedAvgMalePred(float e) 		{ speedAvgMalePred += e; };//MRE RandomGoodGene
	void setSpeedAvgFemalePred(float e) 	{ speedAvgFemalePred = e; };//MRE RandomGoodGene
	void incSpeedAvgFemalePrey(float e) 	{ speedAvgFemalePrey += e; };//MRE RandomGoodGene
	void setSpeedAvgMalePrey(float e)  		{ speedAvgMalePrey = e; };//MRE RandomGoodGene
	void incSpeedAvgFemalePred(float e) 	{ speedAvgFemalePred += e; };//MRE RandomGoodGene
	void setSpeedAvgMalePred(float e) 		{ speedAvgMalePred = e; };//MRE RandomGoodGene
	void incAgeAvgDeadMalePrey(int a) 			{ ageAvgDeadMalePrey += a; };
	void incAgeAvgDeadMalePred(int a) 			{ ageAvgDeadMalePred += a; };
	void incAgeMalePrey(int a) 					{ ageMalePrey += a; };
	void incAgeMalePred(int a) 					{ ageMalePred += a; };
	void incAvgDistEvMalePrey(float g) 			{ avgDistEvMalePrey += g; };
	void incAvgDistEvMalePred(float g) 			{ avgDistEvMalePred += g; };
	void incAgeAvgDeadFemalePrey(int a) 			{ ageAvgDeadFemalePrey += a; };
	void incAgeAvgDeadFemalePred(int a) 			{ ageAvgDeadFemalePred += a; };
	void incAgeFemalePrey(int a) 					{ ageFemalePrey += a; };
	void incAgeFemalePred(int a) 					{ ageFemalePred += a; };
	void incAvgDistEvFemalePrey(float g) 			{ avgDistEvFemalePrey += g; };
	void incAvgDistEvFemalePred(float g) 			{ avgDistEvFemalePred += g; };
	void incAvgDistMatingPrey(float g) 		{ avgDistMatingPrey += g; };	    //-- Meisma: Distance Parents
	void incAvgDistMatingPred(float g) 		{ avgDistMatingPred += g; };		//-- Meisma: Distance Parents

	void writeHeader(Ecosystem *);
	void writeStat(Ecosystem *);
	void RemoveEnd(Ecosystem *);		//-- Meisam: Remove additional generations after Restoring

	void globalEntropy(Ecosystem *);    //-- Marwa: added for calculating global entropy  and maximum entropy for prey population
};

#endif /* STAT_H_ */
