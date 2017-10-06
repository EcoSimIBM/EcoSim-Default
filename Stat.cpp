/*
* Stat.cpp
*  -
*/

#include "Stat.h"
#include "Ecosystem.h"
#include "Species.h"
#include "Ecosystem.h"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

Stat::Stat() {

}

Stat::~Stat() {

}

Stat::Stat(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, Ecosystem * eco) {
	nbActionPreyMale.resize(FCMPrey::NbActions);
	nbActionPreyFemale.resize(FCMPrey::NbActions);
	nbActionPredMale.resize(FCMPredator::NbActions);
	nbActionPredFemale.resize(FCMPredator::NbActions);
	nbSensPrey = n1;
	nbConceptsPrey = n2;
	nbMoteursDepPrey = n3;
	nbMoteursFixPrey = n4;
	nbSensPred = n5;
	nbConceptsPred = n6;
	nbMoteursDepPred = n7;
	nbMoteursFixPred = n8;
	activations_prey_male.resize(n1 + n2 + n3 + n4 + 1);
	activations_prey_female.resize(n1 + n2 + n3 + n4 + 1);
	activations_pred_male.resize(n5 + n6 + n7 + n8 + 1);
	activations_pred_female.resize(n5 + n6 + n7 + n8 + 1);

	writeHeader(eco);

}


void Stat::reset() {
	for (int i = 0; i < FCMPrey::NbActions; i++) {
		nbActionPreyMale[i] = 0;
		nbActionPreyFemale[i] = 0;
	}
	for (int i = 0; i < FCMPredator::NbActions; i++) {
		nbActionPredMale[i] = 0;
		nbActionPredFemale[i] = 0;
	}
	for (int i = 0; i < nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey + 1; i++) {
		activations_prey_male[i] = 0;
		activations_prey_female[i] = 0;
	}
	for (int i = 0; i < nbSensPred + nbConceptsPred + nbMoteursDepPred + nbMoteursFixPred + 1; i++) {
		activations_pred_male[i] = 0;
		activations_pred_female[i] = 0;
	}
	
	persuasionTotalMalePrey = 0;
	persuasionTotalMalePred = 0;
	persuasionTotalFemalePrey = 0;
	persuasionTotalFemalePred = 0;
	
	nbMalePrey = 0;
	nbFemalePrey = 0;
	nbMalePred = 0;
	nbFemalePred = 0;
	
	energyAvgMalePrey = 0;
	energyAvgMalePred = 0;
	strengthAvgMalePrey = 0; //M.M
	strengthAvgMalePred = 0; // M.M
	nbBirthMalePrey = 0;
	nbBirthMalePred = 0;
	avgDistEvMalePrey = 0;
	avgDistEvMalePred = 0;

	energyAvgFemalePrey = 0;
	energyAvgFemalePred = 0;
	strengthAvgFemalePrey = 0; //M.M
	strengthAvgFemalePred = 0; // M.M
	nbBirthFemalePrey = 0;
	nbBirthFemalePred = 0;
	avgDistEvFemalePrey = 0;
	avgDistEvFemalePred = 0;
	
	
	avgEnergySpentMalePrey = 0;
	avgEnergySpentFemalePrey = 0;
	avgEnergySpentMalePred = 0;
	avgEnergySpentFemalePred = 0;
	
	avgEnergyTransferredMalePrey = 0;
	avgEnergyTransferredFemalePrey = 0;
	avgEnergyTransferredMalePred = 0;
	avgEnergyTransferredFemalePred = 0;

	avgDistMatingPrey = 0;
	avgDistMatingPred = 0;
	ageAvgDeadMalePrey = 0;
	ageAvgDeadMalePred = 0;
	ageAvgDeadFemalePrey = 0;
	ageAvgDeadFemalePred = 0;
	speedAvgMalePrey = 0;
	speedAvgMalePred = 0;
	speedAvgFemalePrey = 0;
	speedAvgFemalePred = 0;
	ageMalePrey = 0;
	ageMalePred = 0;
	nbDeadMalePreyA = 0;
	nbDeadMalePreyE = 0;
	nbDeadMalePreyK = 0;
	nbDeadMalePredA = 0;
	nbDeadMalePredE = 0;
	nbDeadMalePredF = 0;  // M.M

	ageFemalePrey = 0;
	ageFemalePred = 0;
	nbDeadFemalePreyA = 0;
	nbDeadFemalePreyE = 0;
	nbDeadFemalePreyK = 0;
	nbDeadFemalePredA = 0;
	nbDeadFemalePredE = 0;
	nbDeadFemalePredF = 0;  // M.M

	stateAvgMalePrey = 0;
	stateAvgMalePred = 0;

	stateAvgFemalePrey = 0;
	stateAvgFemalePred = 0;

	nbPreyByCase = 0;
	nbPredByCase = 0;
	nbCasePrey = 0;
	nbCasePred = 0;
	nbTotalGrass = 0;
#ifdef TWO_RESOURCES
	nbTotalGrass2 = 0; //-- Armin
#endif
	nbTotalMeat = 0;
	nbArcAvgMalePrey = 0;
	nbArcAvgMalePred = 0;

	nbArcAvgFemalePrey = 0;
	nbArcAvgFemalePred = 0;

	sumPreyMaleGenome = vector<double>(Prey::nbGenes, 0);  // M.M
	sumPredMaleGenome = vector<double>(Predator::nbGenes, 0);  // M.M
	
	sumPreyFemaleGenome = vector<double>(Prey::nbGenes, 0);  // M.M
	sumPredFemaleGenome = vector<double>(Predator::nbGenes, 0);  // M.M
}


void Stat::globalEntropy(Ecosystem * eco)
{

	const float BIN_WIDTH = (float)0.1;
	const int NUM_BIN = 200;
	float  probability;

	int i;
	int num_indiv, bin, maxbins;
	int cur_freq, *freqs;

	freqs = new int[NUM_BIN];

	num_indiv = eco->rabbits.size();
	gEntropy = 0.0;
	maxEntropy = 0.0;

	int numcol, numrow;
	numrow = eco->fcm_prey.get_rows();
	numcol = eco->fcm_prey.get_cols();

	for (int row = 0; row < numrow; row++){
		for (int col = 0; col < numcol; col++){

			for (int k = 0; k<NUM_BIN; k++)
				freqs[k] = 0;

			maxbins = 0;
			int n;

			for (n = 0; n < (int)eco->rabbits.size(); n++){
				bin = int((eco->rabbits[n].getFCM()->get_chart(row, col) + 10) / BIN_WIDTH);

				++freqs[abs(bin)];
			}

			for (i = 0; i < NUM_BIN; i++){
				cur_freq = freqs[i];
				if (cur_freq > 0){
					maxbins++;
					probability = float(cur_freq) / num_indiv;
					if (probability != 0){
#ifdef LinuxSystem
						gEntropy += (-probability*log2(probability));
#else
						gEntropy += (-probability*log(probability));
#endif
					}
				}
			}

#ifdef LinuxSystem
			maxEntropy += log2((float)maxbins);
#else
			maxEntropy += log((float)maxbins);
#endif
		}
	}
	delete[] freqs;
}

void Stat::writeHeader(Ecosystem * eco) {
	ofstream statFile;

	if (eco->generation == 1) {

		//-- Prey male
		statFile.open("Results_Prey_Male.csv", ios::app);

		statFile << "generation";
		statFile << ", nbGrass";
#ifdef TWO_RESOURCES
		statFile << ", nbGrass2";
#endif
		statFile << ", nbMeat";
		statFile << ", total population";
		statFile << ", population";
		statFile << ", nbSpecies, speciesRate, extinctionRate";
		statFile << ", globalEntropy, maxEntropy";

		statFile << ", speed, energy, strength, nbPerCell";
		statFile << ", birthRatio, deathRatio";
		statFile << ", age, ageDeath";
		statFile << ", deadOldAgeRatio, deadEnergyRatio, deadKilledRatio";
		statFile << ", stateOFbirth, arcs, dist_Evol, distMating";


#ifdef TWO_RESOURCES
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SearchFood2Ratio, act_SearchFoodFailed2Ratio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_Eat2Ratio, act_EatFailed2Ratio, act_ReproduceRatio, act_ReproduceFailedRatio";
#else
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_ReproduceRatio, act_ReproduceFailedRatio, act_Move2StrongestPreyRatio, act_Move2StrongestPreyFailedRatio, act_Move2StrongestPreyCellRatio, act_Move2StrongestPreyCellFailedRatio, act_Move2WeakestPreyCellRatio, act_Move2WeakestPreyCellFailedRatio";
#endif

		for (int i = 0; i < nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey; i++) {
			int START = (*eco->getNameConceptsPrey())[i].find_last_of("_") + 1;
			int END = (*eco->getNameConceptsPrey())[i].find(":");
			statFile << ", concept_" << (*eco->getNameConceptsPrey())[i].substr(START, END - START);
		}
		statFile << ", MaxEnergy, MaxAge, Vision, MaxSpeed, RepAge, StateBirth, Defence, CoopDefence, Persuasion, EnergySpent, EnergyTransferred"; // M.M Physical genome header
		statFile << "\n";
		statFile.close();

		//-- Prey female
		statFile.open("Results_Prey_Female.csv", ios::app);

		statFile << "generation";
		statFile << ", nbGrass";
#ifdef TWO_RESOURCES
		statFile << ", nbGrass2";
#endif
		statFile << ", nbMeat";
		statFile << ", total population";
		statFile << ", population";
		statFile << ", nbSpecies, speciesRate, extinctionRate";
		statFile << ", globalEntropy, maxEntropy";

		statFile << ", speed, energy, strength, nbPerCell";
		statFile << ", birthRatio, deathRatio";
		statFile << ", age, ageDeath";
		statFile << ", deadOldAgeRatio, deadEnergyRatio, deadKilledRatio";
		statFile << ", stateOFbirth, arcs, dist_Evol, distMating";


#ifdef TWO_RESOURCES
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SearchFood2Ratio, act_SearchFoodFailed2Ratio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_Eat2Ratio, act_EatFailed2Ratio, act_ReproduceRatio, act_ReproduceFailedRatio";
#else
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_ReproduceRatio, act_ReproduceFailedRatio, act_Move2StrongestPreyRatio, act_Move2StrongestPreyFailedRatio, act_Move2StrongestPreyCellRatio, act_Move2StrongestPreyCellFailedRatio, act_Move2WeakestPreyCellRatio, act_Move2WeakestPreyCellFailedRatio";
#endif

		for (int i = 0; i < nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey; i++) {
			int START = (*eco->getNameConceptsPrey())[i].find_last_of("_") + 1;
			int END = (*eco->getNameConceptsPrey())[i].find(":");
			statFile << ", concept_" << (*eco->getNameConceptsPrey())[i].substr(START, END - START);
		}
		statFile << ", MaxEnergy, MaxAge, Vision, MaxSpeed, RepAge, StateBirth, Defence, CoopDefence, Persuasion, EnergySpent, EnergyTransferred"; // M.M Physical genome header
		statFile << "\n";
		statFile.close();

		//-- Pred male
		statFile.open("Results_Pred_Male.csv", ios::app);

		statFile << "generation";
		statFile << ", total population";
		statFile << ", population";
		statFile << ", nbSpecies, speciesRate, extinctionRate";

		statFile << ", speed, energy, strength, nbPerCell";
		statFile << ", birthRatio, deathRatio";
		statFile << ", age, ageDeath";
		statFile << ", deadOldAgeRatio, deadEnergyRatio, deadFightRatio "; // M.M modified
		statFile << ", stateOFbirth, arcs, dist_Evol, distMating";

		statFile << ", act_HuntRatio, act_HuntFailRatio, act_SearchFoodRatio, act_SearchFoodFailRatio";
		statFile << ", act_SocializeRatio, act_SocializeFailRatio";
		statFile << ", act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailRatio";
		statFile << ", act_ReproduceRatio, act_ReproduceFailRatio";
		statFile << ", act_Move2StrongestPredatorRatio, act_Move2StrongestPredatorFailRatio, act_Move2StrongestPreyDistanceRatio, act_Move2StrongestPreyDistanceFailRatio,  act_Move2WeakestPreyCellRatio, act_Move2WeakestPreyCellFailRatio,  act_Move2WeakestPreyRatio, act_Move2WeakestPreyFailRatio";

		for (int i = 0; i < nbSensPred + nbConceptsPred + nbMoteursDepPred + nbMoteursFixPred; i++) {
			int START = (*eco->getNameConceptsPred())[i].find_last_of("_") + 1;
			int END = (*eco->getNameConceptsPred())[i].find(":");
			statFile << ", concept_" << (*eco->getNameConceptsPred())[i].substr(START, END - START);
		}

		statFile << ", MaxEnergy, MaxAge, Vision, MaxSpeed, RepAge, StateBirth, Persuasion, EnergySpent, EnergyTransferred"; // M.M Physical genome header
		statFile << "\n";
		statFile.close();

		//-- Pred female
		statFile.open("Results_Pred_Female.csv", ios::app);

		statFile << "generation";
		statFile << ", total population";
		statFile << ", population";
		statFile << ", nbSpecies, speciesRate, extinctionRate";

		statFile << ", speed, energy, strength, nbPerCell";
		statFile << ", birthRatio, deathRatio";
		statFile << ", age, ageDeath";
		statFile << ", deadOldAgeRatio, deadEnergyRatio, deadFightRatio "; // M.M modified
		statFile << ", stateOFbirth, arcs, dist_Evol, distMating";

		statFile << ", act_HuntRatio, act_HuntFailRatio, act_SearchFoodRatio, act_SearchFoodFailRatio";
		statFile << ", act_SocializeRatio, act_SocializeFailRatio";
		statFile << ", act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailRatio";
		statFile << ", act_ReproduceRatio, act_ReproduceFailRatio";
		statFile << ", act_Move2StrongestPredatorRatio, act_Move2StrongestPredatorFailRatio, act_Move2StrongestPreyDistanceRatio, act_Move2StrongestPreyDistanceFailRatio,  act_Move2WeakestPreyCellRatio, act_Move2WeakestPreyCellFailRatio,  act_Move2WeakestPreyRatio, act_Move2WeakestPreyFailRatio";

		for (int i = 0; i < nbSensPred + nbConceptsPred + nbMoteursDepPred + nbMoteursFixPred; i++) {
			int START = (*eco->getNameConceptsPred())[i].find_last_of("_") + 1;
			int END = (*eco->getNameConceptsPred())[i].find(":");
			statFile << ", concept_" << (*eco->getNameConceptsPred())[i].substr(START, END - START);
		}

		statFile << ", MaxEnergy, MaxAge, Vision, MaxSpeed, RepAge, StateBirth, Persuasion, EnergySpent, EnergyTransferred"; // M.M Physical genome header
		statFile << "\n";
		statFile.close();
	}

}


void Stat::writeStat(Ecosystem * eco) {

	time(&eco->start);
	ofstream statFile;

	int gen = eco->generation;


	int nbPrey = (int)eco->rabbits.size();
	int nbPred = (int)eco->wolves.size();

	//MRE //RandomGoodGene
	
	int nbMalePreyActed = 0;
	int nbFemalePreyActed = 0;
	int nbMalePredActed = 0;
	int nbFemalePredActed = 0;


	// M.M start  => calculate average genome per generation
	vector<double> avgPreyMaleGenome = vector <double>(Prey::nbGenes);
	vector<double> avgPreyFemaleGenome = vector <double>(Prey::nbGenes);
	
	vector<double> avgPredFemaleGenome = vector <double>(Predator::nbGenes);
	vector<double> avgPredMaleGenome = vector <double>(Predator::nbGenes);
	
	vector <double> preyMaleGenome;					//-- keep the average of genetome M.M
	vector <double> predMaleGenome;					//-- keep the average of genetome M.M
	
	vector <double> preyFemaleGenome;					//-- keep the average of genetome M.M
	vector <double> predFemaleGenome;					//-- keep the average of genetome M.M

	preyMaleGenome = vector<double>(Prey::nbGenes, 0);
	predMaleGenome = vector<double>(Predator::nbGenes, 0);

	preyFemaleGenome = vector<double>(Prey::nbGenes, 0);
	predFemaleGenome = vector<double>(Predator::nbGenes, 0);

	
	for (int i = 0; i < (int)nbActionPreyFemale.size(); i++)
	{
		nbFemalePreyActed += nbActionPreyFemale[i];
	}
	for (int i = 0; i < (int)nbActionPreyMale.size(); i++)
	{
		nbMalePreyActed += nbActionPreyMale[i];
	}
	for (int i = 0; i < (int)nbActionPredFemale.size(); i++)
	{
		nbFemalePredActed += nbActionPredFemale[i];
	}
	for (int i = 0; i < (int)nbActionPredMale.size(); i++)
	{
		nbMalePredActed += nbActionPredMale[i];
	}

	
	
	for (int i = 0; i< nbPrey; i++){
		for (size_t j = 0; j<Prey::nbGenes; j++){ // M.M
			PhysicalGenome* genome_temp = eco->rabbits[i].getPhGenome();
			//sumPreyGenome[j] = sumPreyGenome[j] + (*genome_temp)[j];
			if (eco->rabbits[i].getGender() == 0){ //male
				sumPreyMaleGenome[j] += genome_temp->getGene(j); //real value of the gene
			}
			else if (eco->rabbits[i].getGender() == 1){ //female
				sumPreyFemaleGenome[j] += genome_temp->getGene(j); //real value of the gene
			}
		}
	}
	
	for (size_t j = 0; j<Prey::nbGenes; j++){ // M.M
		avgPreyMaleGenome[j] = (double)(sumPreyMaleGenome[j] / nbMalePrey);
		avgPreyFemaleGenome[j] = (double)(sumPreyFemaleGenome[j] / nbFemalePrey);
	}


	for (int i = 0; i < nbPred; i++){
		for (size_t j = 0; j<Predator::nbGenes; j++){ // M.M
			PhysicalGenome* genome_temp = eco->wolves[i].getPhGenome();
			//sumPredGenome[j] = sumPredGenome[j] + (*genome_temp)[j];
			if (eco->wolves[i].getGender() == 0){ //male
				sumPredMaleGenome[j] += genome_temp->getGene(j);
			}
			if (eco->wolves[i].getGender() == 1){ //female
				sumPredFemaleGenome[j] += genome_temp->getGene(j);
			}
		}
	}
	
	for (size_t j = 0; j<Predator::nbGenes; j++){ // M.M{
		avgPredMaleGenome[j] = (double)(sumPredMaleGenome[j] / nbMalePred);
		avgPredFemaleGenome[j] = (double)(sumPredFemaleGenome[j] / nbFemalePred);
	}
	// M.M end


	if (nbPrey < 1) nbPrey = 1;		 	 //-- For divide by zero
	if (nbPred < 1) nbPred = 1;			 //-- For divide by zero

	globalEntropy(eco);

	cout.clear();
	cout << " Writing results Prey" << flush;

	//writing male stat prey //MRE
	statFile.open("Results_Prey_Male.csv", ios::app);

	statFile << Manip.vtos(gen);
	statFile << ", " << Manip.vtos(nbTotalGrass);
#ifdef TWO_RESOURCES
	statFile << ", " << Manip.vtos(nbTotalGrass2);
#endif
	statFile << ", " << Manip.vtos(nbTotalMeat);
	statFile << ", " << Manip.vtos(nbMalePrey + nbFemalePrey);
	statFile << ", " << Manip.vtos(nbMalePrey);//MRE RandomGoodGene
	statFile << ", " << Manip.vtos((int)eco->getSpeciesPrey()->size()) << ", " << Manip.vtos(SpeciesRatioPrey) << ", " << Manip.vtos(ExtinctionRatioPrey); //-- Meisam
	statFile << ", " << Manip.vtos(gEntropy) << ", " << Manip.vtos(maxEntropy);  //-- Marwa Global entropy for prey population //MRE_HERE

	statFile << ", " << Manip.vtos((double)speedAvgMalePrey / (float)nbMalePreyActed);
	statFile << ", " << Manip.vtos((double)energyAvgMalePrey / (float)nbMalePrey);
	statFile << ", " << Manip.vtos((double)strengthAvgMalePrey / (float)nbMalePrey); // M.M Very wierd use of type-casting
	statFile << ", " << (nbCasePrey > 0 ? Manip.vtos((double)nbPreyByCase / (float)nbCasePrey) : Manip.vtos(0));//MRE_HERE

	statFile << ", " << Manip.vtos((double)nbBirthMalePrey / (float)nbMalePreyActed); //(float)(nbMalePrey - nbBirthMalePrey + nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK));
	statFile << ", " << Manip.vtos((double)(nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK) / (float)nbMalePreyActed); //(float)(nbMalePrey - nbBirthMalePrey + nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK));

	statFile << ", " << Manip.vtos((double)ageMalePrey / (float)nbMalePrey);
	statFile << ", " << (nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK > 0 ? Manip.vtos(ageAvgDeadMalePrey / (double)(nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK)) : Manip.vtos(0));

	statFile << ", " << (nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK > 0 ? Manip.vtos((double)nbDeadMalePreyA / (float)(nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK)) : Manip.vtos(0));
	statFile << ", " << (nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK > 0 ? Manip.vtos((double)nbDeadMalePreyE / (float)(nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK)) : Manip.vtos(0));
	statFile << ", " << (nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK > 0 ? Manip.vtos((double)nbDeadMalePreyK / (float)(nbDeadMalePreyA + nbDeadMalePreyE + nbDeadMalePreyK)) : Manip.vtos(0));

	statFile << ", " << Manip.vtos(stateAvgMalePrey / (float)nbMalePrey);
	statFile << ", " << Manip.vtos((double)nbArcAvgMalePrey / (float)nbMalePrey);
	statFile << ", " << Manip.vtos(avgDistEvMalePrey / (float)nbMalePrey);
	statFile << ", " << (nbActionPreyMale.at(FCMPrey::ReproduceAction - 1) > 0 ? Manip.vtos(avgDistMatingPrey / (float)nbActionPreyMale.at(FCMPrey::ReproduceAction - 1)) : Manip.vtos(0));

	for (int i = 0; i < (int)nbActionPreyMale.size(); i++) {
		statFile << ", " << (nbMalePreyActed > 0 ? Manip.vtos((double)nbActionPreyMale[i] / (float)nbMalePreyActed) : Manip.vtos(0));
	}

	for (int i = 0; i < nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey; i++) {
		statFile << ", " << (nbMalePreyActed > 0 ? Manip.vtos(activations_prey_male[i] / (float)nbMalePreyActed) : Manip.vtos(0));
	}

	for (short p = 0; p < Prey::nbGenes; p++)  // M.M
		statFile << ", " << avgPreyMaleGenome[p];
		
	statFile << ", " << (nbMalePrey > 0 ? Manip.vtos(persuasionTotalMalePrey / (float)nbMalePrey) : Manip.vtos(0));
	
	statFile << ", " << (nbMalePreyActed > 0 ? Manip.vtos(avgEnergySpentMalePrey / (float)nbMalePreyActed) : Manip.vtos(0));
	statFile << ", " << (nbActionPreyMale.at(FCMPrey::ReproduceAction - 1) > 0 ? Manip.vtos(avgEnergyTransferredMalePrey / (float)(nbActionPreyMale.at(FCMPrey::ReproduceAction - 1))) : Manip.vtos(0));
	
	statFile << "\n";
	statFile.close();



	//Writing Female stat prey
	statFile.open("Results_Prey_Female.csv", ios::app);

	statFile << Manip.vtos(gen);
	statFile << ", " << Manip.vtos(nbTotalGrass);
#ifdef TWO_RESOURCES
	statFile << ", " << Manip.vtos(nbTotalGrass2);
#endif
	statFile << ", " << Manip.vtos(nbTotalMeat);
	statFile << ", " << Manip.vtos(nbMalePrey + nbFemalePrey);
	statFile << ", " << Manip.vtos(nbFemalePrey);//MRE RandomGoodGene
	statFile << ", " << Manip.vtos((int)eco->getSpeciesPrey()->size()) << ", " << Manip.vtos(SpeciesRatioPrey) << ", " << Manip.vtos(ExtinctionRatioPrey); //-- Meisam
	statFile << ", " << Manip.vtos(gEntropy) << ", " << Manip.vtos(maxEntropy);  //-- Marwa Global entropy for prey population

	statFile << ", " << Manip.vtos((double)speedAvgFemalePrey / (float)nbFemalePreyActed);
	statFile << ", " << Manip.vtos((double)energyAvgFemalePrey / (float)nbFemalePrey);
	statFile << ", " << Manip.vtos((double)strengthAvgFemalePrey / (float)nbFemalePrey); // M.M Very wierd use of type-casting
	statFile << ", " << (nbCasePrey> 0 ? Manip.vtos((double)nbPreyByCase / (float)nbCasePrey) : Manip.vtos(0));

	statFile << ", " << Manip.vtos((double)nbBirthFemalePrey / (float)nbFemalePreyActed);//nbFemalePrey - nbBirthFemalePrey + nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK));
	statFile << ", " << Manip.vtos((double)(nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK) / (float)nbFemalePreyActed);//(float)(nbFemalePrey - nbBirthFemalePrey + nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK));

	statFile << ", " << Manip.vtos((double)ageFemalePrey / (float)nbFemalePrey);
	statFile << ", " << (nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK > 0 ? Manip.vtos(ageAvgDeadFemalePrey / (float)(nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK)) : Manip.vtos(0));

	statFile << ", " << (nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK > 0 ? Manip.vtos((double)nbDeadFemalePreyA / (float)(nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK)) : Manip.vtos(0));
	statFile << ", " << (nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK > 0 ? Manip.vtos((double)nbDeadFemalePreyE / (float)(nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK)) : Manip.vtos(0));
	statFile << ", " << (nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK > 0 ? Manip.vtos((double)nbDeadFemalePreyK / (float)(nbDeadFemalePreyA + nbDeadFemalePreyE + nbDeadFemalePreyK)) : Manip.vtos(0));

	statFile << ", " << Manip.vtos(stateAvgFemalePrey / (float)nbFemalePrey);
	statFile << ", " << Manip.vtos((double)nbArcAvgFemalePrey / (float)nbFemalePrey);
	statFile << ", " << Manip.vtos(avgDistEvFemalePrey / (float)nbFemalePrey);
	statFile << ", " << (nbActionPreyFemale.at(FCMPrey::ReproduceAction - 1) > 0 ? Manip.vtos(avgDistMatingPrey / (float)nbActionPreyFemale.at(FCMPrey::ReproduceAction - 1)) : Manip.vtos(0));

	for (int i = 0; i < (int)nbActionPreyFemale.size(); i++) {
		statFile << ", " << (nbFemalePreyActed > 0 ? Manip.vtos((double)nbActionPreyFemale[i] / (float)nbFemalePreyActed) : Manip.vtos(0));
	}

	for (int i = 0; i < nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey; i++) {
		statFile << ", " << (nbFemalePreyActed > 0 ? Manip.vtos(activations_prey_female[i] / (float)nbFemalePreyActed) : Manip.vtos(0));
	}

	for (short p = 0; p < Prey::nbGenes; p++)  // M.M
		statFile << ", " << avgPreyFemaleGenome[p];
		
	statFile << ", " << (nbFemalePrey > 0 ? Manip.vtos(persuasionTotalFemalePrey / (float)nbFemalePrey) : Manip.vtos(0));
	
	statFile << ", " << (nbFemalePreyActed > 0 ? Manip.vtos(avgEnergySpentFemalePrey / (float)nbFemalePreyActed) : Manip.vtos(0));
	statFile << ", " << (nbActionPreyFemale.at(FCMPrey::ReproduceAction - 1) > 0 ? Manip.vtos(avgEnergyTransferredFemalePrey / (float)(nbActionPreyFemale.at(FCMPrey::ReproduceAction - 1))) : Manip.vtos(0));
	
	statFile << "\n";
	statFile.close();
	//MRE END


	cout.clear();
	cout << " Writing results Pred" << flush;

	statFile.open("Results_Pred_Male.csv", ios::app);

	statFile << gen;
	statFile << ", " << Manip.vtos(nbFemalePred + nbMalePred);
	statFile << ", " << Manip.vtos(nbMalePred);//MRE RandomGoodGene
	statFile << ", " << Manip.vtos((int)eco->getSpeciesPred()->size()) << ", " << Manip.vtos(SpeciesRatioPred) << ", " << Manip.vtos(ExtinctionRatioPred); //-- Meisam

	statFile << ", " << Manip.vtos(speedAvgMalePred / (float)nbMalePredActed);
	statFile << ", " << Manip.vtos(energyAvgMalePred / (float)nbMalePred);
	statFile << ", " << Manip.vtos(strengthAvgMalePred / (float)nbMalePred); // M.M
	statFile << ", " << (nbCasePred > 0 ? Manip.vtos((double)nbPredByCase / (float)nbCasePred) : Manip.vtos(0));

	statFile << ", " << Manip.vtos((double)nbBirthMalePred / (float)nbMalePredActed);//(float)(nbMalePred - nbBirthMalePred + nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF));
	statFile << ", " << Manip.vtos((double)(nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF) / (float)nbMalePredActed);//(float)(nbMalePred - nbBirthMalePred + nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF)); // M.M modified

	statFile << ", " << Manip.vtos((double)ageMalePred / (float)nbMalePred);
	statFile << ", " << (nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF > 0 ? Manip.vtos(ageAvgDeadMalePred / (float)(nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF)) : Manip.vtos(0));  // M.M modified

	statFile << ", " << (nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF > 0 ? Manip.vtos((double)nbDeadMalePredA / (float)(nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF)) : Manip.vtos(0)); // M.M modified
	statFile << ", " << (nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF > 0 ? Manip.vtos((double)nbDeadMalePredE / (float)(nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF)) : Manip.vtos(0)); // M.M modified
	statFile << ", " << (nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF > 0 ? Manip.vtos((double)nbDeadMalePredF / (float)(nbDeadMalePredA + nbDeadMalePredE + nbDeadMalePredF)) : Manip.vtos(0)); // M.M

	statFile << ", " << Manip.vtos(stateAvgMalePred / (float)nbMalePred);
	statFile << ", " << Manip.vtos((double)nbArcAvgMalePred / (float)nbMalePred);
	statFile << ", " << Manip.vtos(avgDistEvMalePred / (float)nbMalePred);
	statFile << ", " << (nbActionPredMale.at(FCMPredator::ReproduceAction - 1) > 0 ? Manip.vtos(avgDistMatingPred / (float)nbActionPredMale.at(FCMPredator::ReproduceAction - 1)) : Manip.vtos(0));

	for (int i = 0; i < (int)nbActionPredMale.size(); i++) {
		statFile << ", " << (nbMalePredActed > 0 ? Manip.vtos((double)nbActionPredMale[i] / (float)nbMalePredActed) : Manip.vtos(0));
	}

	for (int i = 0; i < nbSensPred + nbConceptsPred + nbMoteursDepPred + nbMoteursFixPred; i++) {
		statFile << ", " << (nbMalePredActed > 0 ? Manip.vtos(activations_pred_male[i] / (float)nbMalePredActed) : Manip.vtos(0));
	}

	for (short p = 0; p < Predator::nbGenes; p++)  // M.M
		statFile << ", " << avgPredMaleGenome[p];

	statFile << ", " << (nbMalePred > 0 ? Manip.vtos(persuasionTotalMalePred / (float)nbMalePred) : Manip.vtos(0));
	
	statFile << ", " << (nbMalePredActed > 0 ? Manip.vtos(avgEnergySpentMalePred / (float)nbMalePredActed) : Manip.vtos(0));
	statFile << ", " << (nbActionPredMale.at(FCMPredator::ReproduceAction - 1) > 0 ? Manip.vtos(avgEnergyTransferredMalePred / (float)(nbActionPredMale.at(FCMPredator::ReproduceAction - 1))) : Manip.vtos(0));

	statFile << "\n";
	statFile.close();
	//MRE Female Pred


	statFile.open("Results_Pred_Female.csv", ios::app);

	statFile << gen;
	statFile << ", " << Manip.vtos(nbMalePred + nbFemalePred);
	statFile << ", " << Manip.vtos(nbFemalePred);//MRE RandomGoodGene
	statFile << ", " << Manip.vtos((int)eco->getSpeciesPred()->size()) << ", " << Manip.vtos(SpeciesRatioPred) << ", " << Manip.vtos(ExtinctionRatioPred); //-- Meisam

	statFile << ", " << Manip.vtos(speedAvgFemalePred / (float)nbFemalePredActed);
	statFile << ", " << Manip.vtos(energyAvgFemalePred / (float)nbFemalePred);
	statFile << ", " << Manip.vtos(strengthAvgFemalePred / (float)nbFemalePred); // M.M
	statFile << ", " << (nbCasePred > 0 ? Manip.vtos((double)nbPredByCase / (float)nbCasePred) : Manip.vtos(0));

	statFile << ", " << Manip.vtos((double)nbBirthFemalePred / (float)nbFemalePredActed);//(float)(nbFemalePred - nbBirthFemalePred + nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF));
	statFile << ", " << Manip.vtos((double)(nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF) / (float)nbFemalePredActed);//(float)(nbFemalePred - nbBirthFemalePred + nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF)); // M.M modified

	statFile << ", " << Manip.vtos((double)ageFemalePred / (float)nbFemalePred);
	statFile << ", " << (nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF > 0 ? Manip.vtos(ageAvgDeadFemalePred / (float)(nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF)) : Manip.vtos(0));  // M.M modified

	statFile << ", " << (nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF > 0 ? Manip.vtos((double)nbDeadFemalePredA / (float)(nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF)) : Manip.vtos(0)); // M.M modified
	statFile << ", " << (nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF > 0 ? Manip.vtos((double)nbDeadFemalePredE / (float)(nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF)) : Manip.vtos(0)); // M.M modified
	statFile << ", " << (nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF > 0 ? Manip.vtos((double)nbDeadFemalePredF / (float)(nbDeadFemalePredA + nbDeadFemalePredE + nbDeadFemalePredF)) : Manip.vtos(0)); // M.M

	statFile << ", " << Manip.vtos(stateAvgFemalePred / (float)nbFemalePred);
	statFile << ", " << Manip.vtos((double)nbArcAvgFemalePred / (float)nbFemalePred);
	statFile << ", " << Manip.vtos(avgDistEvFemalePred / (float)nbFemalePred);
	statFile << ", " << (nbActionPredFemale.at(FCMPredator::ReproduceAction - 1) > 0 ? Manip.vtos(avgDistMatingPred / (float)nbActionPredFemale.at(FCMPredator::ReproduceAction - 1)) : Manip.vtos(0));

	for (int i = 0; i < (int)nbActionPredFemale.size(); i++) {
		statFile << ", " << (nbFemalePredActed > 0 ? Manip.vtos((double)nbActionPredFemale[i] / (float)nbFemalePredActed) : Manip.vtos(0));
	}

	for (int i = 0; i < nbSensPred + nbConceptsPred + nbMoteursDepPred + nbMoteursFixPred; i++) {
		statFile << ", " << (nbFemalePredActed > 0 ? Manip.vtos(activations_pred_female[i] / (float)nbFemalePredActed) : Manip.vtos(0));
	}

	for (short p = 0; p < Predator::nbGenes; p++)  // M.M
		statFile << ", " << avgPredFemaleGenome[p];
		
	statFile << ", " << (nbFemalePred > 0 ? Manip.vtos(persuasionTotalFemalePred / (float)nbFemalePred) : Manip.vtos(0));
	
	statFile << ", " << (nbFemalePredActed > 0 ? Manip.vtos(avgEnergySpentFemalePred / (float)nbFemalePredActed) : Manip.vtos(0));
	statFile << ", " << (nbActionPredFemale.at(FCMPredator::ReproduceAction - 1) > 0 ? Manip.vtos(avgEnergyTransferredFemalePred / (float)(nbActionPredFemale.at(FCMPredator::ReproduceAction - 1))) : Manip.vtos(0));

	statFile << "\n";
	statFile.close();



	// statFile << "Nb Average Reproduce By Prey: " << (float)nbAvgReprodPrey / nbPrey << endl;
	// statFile << "Nb Reproduce Max Prey: " << nbReprodMaxPrey << endl;
	// statFile << "Nb Average Reproduce By Pred: " << (float)nbAvgReprodPred / nbPred << endl;
	// statFile << "Nb Reproduce Max Pred: " << nbReprodMaxPred << endl;

	/*
	statFile << "Pyramid Old Ages Prey: ";
	for (int i = 0; i < 50; i++) {
	statFile << distribAge[0][i] << " ";
	}
	statFile << "\n";
	statFile << "Pyramid Old Ages Predators: ";
	for (int i = 0; i < 50; i++) {
	statFile << distribAge[1][i] << " ";
	}
	statFile << "\n";
	*/
	time(&eco->end);
	eco->printTime(eco->start, eco->end);
}


void Stat::RemoveEnd(Ecosystem *eco){


	int gen = eco->generation;

	cout.clear();
	cout << " Remove End of Result file" << flush;
	time(&eco->start);

	string Info;
	int Result = 1;


	ifstream statFileIn;
	ofstream statFileOut;

	//-- Prey //MER Male
	statFileIn.open("Results_Prey_Male.csv");   //-- Try to open Results file
	if (!statFileIn.is_open()) {
		cout << "Error! ResultPrey file not found" << endl;
		exit(-1);
	}

	statFileOut.open("Results_Prey_Male.csv.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOut.is_open()){
		statFileIn.close();
		cout << "Error! New ResultPreyMale cannot create" << endl;
		return;
	}

	while (!statFileIn.eof()){
		getline(statFileIn, Info);

		if (int(Info.find(", "))>-1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOut << Info << endl;
	}


	statFileIn.close();
	statFileOut.close();

	remove("Results_Prey_Male.csv");
	rename("Results_Prey_Male.csv.bak", "Results_Prey_Male.csv");

	//MRE
	//-- Prey
	statFileIn.open("Results_Prey_Female.csv");   //-- Try to open Results file
	if (!statFileIn.is_open()) {
		cout << "Error! ResultPreyFemale file not found" << endl;
		exit(-1);
	}

	statFileOut.open("Results_Prey_Female.csv.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOut.is_open()){
		statFileIn.close();
		cout << "Error! New ResultPreyFemale cannot create" << endl;
		return;
	}

	while (!statFileIn.eof()){
		getline(statFileIn, Info);

		if (int(Info.find(", "))>-1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOut << Info << endl;
	}


	statFileIn.close();
	statFileOut.close();

	remove("Results_Prey_Female.csv");
	rename("Results_Prey_Female.csv.bak", "Results_Prey_Female.csv");

	//-- Pred //MRE Male

	statFileIn.open("Results_Pred_Male.csv");   //-- Try to open Results file
	if (!statFileIn.is_open()) {
		cout << "Error! ResultPredMale file not found" << endl;
		exit(-1);
	}

	statFileOut.open("Results_Pred_Male.csv.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOut.is_open()){
		statFileIn.close();
		cout << "Error! New ResultPredMale cannot create" << endl;
		return;
	}

	while (!statFileIn.eof()){
		getline(statFileIn, Info);

		if (int(Info.find(", "))>-1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOut << Info << endl;
	}

	statFileIn.close();
	statFileOut.close();

	remove("Results_Pred_Male.csv");
	rename("Results_Pred_Male.csv.bak", "Results_Pred_Male.csv");

	//MRE Female
	//-- Pred

	statFileIn.open("Results_Pred_Female.csv");   //-- Try to open Results file
	if (!statFileIn.is_open()) {
		cout << "Error! ResultPredFemale file not found" << endl;
		exit(-1);
	}

	statFileOut.open("Results_Pred_Female.csv.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOut.is_open()){
		statFileIn.close();
		cout << "Error! New ResultPredFemale cannot create" << endl;
		return;
	}

	while (!statFileIn.eof()){
		getline(statFileIn, Info);

		if (int(Info.find(", "))>-1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOut << Info << endl;
	}

	statFileIn.close();
	statFileOut.close();

	remove("Results_Pred_Female.csv");
	rename("Results_Pred_Female.csv.bak", "Results_Pred_Female.csv");

	time(&eco->end);
	eco->printTime(eco->start, eco->end);
}
