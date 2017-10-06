/*
 * StatSpeciesPrey.cpp
 *  -
 */
#include "StatSpeciesPrey.h"
#include "Ecosystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;


StatSpeciesPrey::~StatSpeciesPrey() {

}

StatSpeciesPrey::StatSpeciesPrey() {

}

StatSpeciesPrey::StatSpeciesPrey(int row, int col) {

	SIZE_activ = FCMPrey::nbNodes;			 //--  nbSensPrey + nbConceptsPrey + nbMoteursDepPrey + nbMoteursFixPrey
	activations.resize(SIZE_activ);
	actions.resize(FCMPrey::NbActions);

	fcm_row = row; //-- eco.fcm_prey.get_rows();
	fcm_col = col; //-- eco.fcm_prey.get_cols();

	fcm_avg.resize(fcm_row, vector<float>(fcm_col));
	fcm_var.resize(fcm_row, vector<float>(fcm_col));
}

void StatSpeciesPrey::fileName(string gender, string *Prey, string *PreyFcm, int gen) {

	ostringstream out1, out2;
#ifdef LinuxSystem
	out1 << "./" << "//Results_SpeciesPrey//";
	mkdir(out1.str().c_str(), 0777);
#endif
	out1 << "Result_Prey_" + gender << (int)(gen / 1000) << ".csv";
	*Prey = out1.str();

#ifdef LinuxSystem
	out2 << "./" << "//Results_SpeciesPrey//";
#endif
	out2 << "Result_PreyFCM_" + gender << (int)(gen / 1000) << ".csv";
	*PreyFcm = out2.str();
}

void StatSpeciesPrey::write_Header(string Prey, string PreyFCM, int gen) {

	ofstream statFile;

	if ((gen == 1) || ((gen % 1000) == 0)) {

		statFile.open(Prey.c_str(), ios::app);

		statFile << "generation, nbSpecies";
		statFile << ", speciesParentID, speciesID, nbIndividual, individualRatio";

		statFile << ", birthRatio, interBreedingRatio";
		statFile << ", deadRatio, deadAgeRatio, deadEnergyRatio, deadKilledRatio, deadAge, deadEnergy";
		statFile << ", entropy, diversitySpatial, diversitySpatialRatio";
		statFile << ", distEvol, stateOFbirth, age, energy, strength, speed";
		statFile << ", compactness, nbArc";

#ifdef TWO_RESOURCES
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SearchFood2Ratio, act_SearchFoodFailed2Ratio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_Eat2Ratio, act_EatFailed2Ratio, act_ReproduceRatio, act_ReproduceFailedRatio";
#else
		statFile << ", act_EscapeRatio, act_SearchFoodRatio, act_SearchFoodFailedRatio, act_SocializeRatio, act_SocializeFailedRatio, act_ExplorationRatio, act_WaitRatio, act_EatRatio, act_EatFailedRatio, act_ReproduceRatio, act_ReproduceFailedRatio, act_Move2StrongestPreyRatio, act_Move2StrongestPreyFailedRatio, act_Move2StrongestPreyCellRatio, act_Move2StrongestPreyCellFailedRatio, act_Move2WeakestPreyCellRatio, act_Move2WeakestPreyCellFailedRatio";
#endif

		statFile << ", reprodFailed_age, reprodFailed_energy";

		statFile << ", parent1_reproductionAge, parent1_reproductionEnergy";
		statFile << ", parent2_reproductionAge, parent2_reproductionEnergy";
		statFile << ", DistMating, pregnancyDeathRatio";

		statFile << ", reasonReproduceFailed_Energy, reasonReproduceFailed_NoPartner";
		statFile << ", reasonReproduceFailed_PartnerEnerg, reasonReproduceFailed_PartnerActed";
		statFile << ", reasonReproduceFailed_PartnerAction, reasonReproduceFailed_PartnerDist";

#ifdef TWO_RESOURCES
		statFile << ", concept_predClose, concept_predFar, concept_foodClose, concept_foodFar, concept_foodClose2, concept_foodFar2, concept_friendClose, concept_friendFar, concept_energyLow, concept_energyHigh, concept_foodLocalHigh, concept_foodLocalLow, concept_foodLocalHigh2, concept_foodLocalLow2, concept_partnerLocalYes, concept_partnerLocalNo";
		statFile << ", concept_fear, concept_hunger, concept_searchPartner, concept_curiosity, concept_sedentary, concept_satisfaction, concept_nuisance";
		statFile << ", concept_escape, concept_searchFood, concept_searchFood2, concept_socialize, concept_exploration, concept_wait, concept_eat, concept_eat2, concept_reproduce";
#else
		statFile << ", concept_predClose, concept_predFar, concept_foodClose, concept_foodFar, concept_friendClose, concept_friendFar, concept_energyLow, concept_energyHigh, concept_foodLocalHigh, concept_foodLocalLow, concept_partnerLocalYes, concept_partnerLocalNo, concept_strengthPreyCellLocalLow, concept_strengthPreyCellLocalHigh, concept_ageLow, concept_ageHigh, concept_strengthStrongestPreyCellDistanceLow, concept_StrengthStrongestPreyCellDistanceHigh, concept_strengthStrongestPreyLocalLow, concept_strengthStrongestPreyLocalHigh, concept_strengthLow, concept_strengthHigh";
		statFile << ", concept_fear, concept_hunger, concept_searchPartner, concept_curiosity, concept_sedentary, concept_satisfaction, concept_nuisance";
		statFile << ", concept_escape, concept_searchFood, concept_socialize, concept_exploration, concept_wait, concept_eat, concept_reproduce, concept_move2StrongestPrey, concept_move2StrongestPreyCell, concept_move2WeakestPreyCell\n";
#endif

		statFile.close();
	}


	if ((gen == 1) || ((gen % 1000) == 0)) {
		int row, col;

		statFile.open(PreyFCM.c_str(), ios::app);

		statFile << "generation, nbSpecies, speciesParentID, speciesID, individualNB, ";

		for (row = 0; row < fcm_row; row++)
		{
			for (col = 0; col < fcm_col; col++)
			{
				statFile << "Favg" << row << "." << col << ", ";
			}
		}

		for (row = 0; row < fcm_row; row++)
		{
			for (col = 0; col < fcm_col; col++)
			{
				statFile << "Fvar" << row << "." << col << ", ";
			}
		}
		statFile << "MaxEnergy, MaxAge, Vision, MaxSpeed, RepAge, StateBirth, Defence, CoopDefence\n "; // M.M Physical genome header
		statFile.close();
	}

}

void StatSpeciesPrey::reset() {
	//-- Initialization
	nbSameGenderInds = 0;

	nbBirth = 0;
	distEvol = 0;
	stateOfBirth = 0;
	age = 0;
	energy = 0;
	strength = 0; //M.M
	speed = 0;
	nbCase = 0;
	nbArc = 0;

	for (int i = 0; i < SIZE_activ; i++) {
		activations[i] = 0;
	}
	for (int i = 0; i < FCMPrey::NbActions; i++) {
		actions[i] = 0;
	}

	reprodFailedA = 0;
	reprodFailedE = 0;

	parent1_reproduceA = 0;
	parent1_reproduceE = 0;
	nbParent1 = 0;
	parent2_reproduceA = 0;
	parent2_reproduceE = 0;
	nbParent2 = 0;
	DistMating = 0;
	PregnantyDeath = 0;

	nbInterbreeding = 0;

	reproductionAttempts = 0;
	reasonReproduceFailed_Energy = 0;
	reasonReproduceFailed_NoPartner = 0;
	reasonReproduceFailed_PartnerEnergy = 0;
	reasonReproduceFailed_PartnerActed = 0;
	reasonReproduceFailed_PartnerAction = 0;
	reasonReproduceFailed_PartnerDist = 0;
	reasonReproduceFailed_SameGender = 0; //MRE

	for (int row = 0; row < fcm_row; row++)
	{
		for (int col = 0; col < fcm_col; col++)
		{
			fcm_avg[row][col] = fcm_var[row][col] = 0;
		}
	}
}

void StatSpeciesPrey::Death(Ecosystem * eco, list<Species>::iterator species, int gender){

	DeathRatio = 0;
	nbDeadA = 0;
	nbDeadE = 0;
	nbDeadK = 0;
	DeadA = 0;
	DeadE = 0;

	for (size_t i = 0; i < eco->DeadPrey.size(); i++){
		if (eco->DeadPrey[i].species == species->getNum()){

			if (eco->DeadPrey[i].gender != gender && gender != 2){
				continue;
			}

			if (eco->DeadPrey[i].deathType == 1){ nbDeadA++; }
			if (eco->DeadPrey[i].deathType == 2){ nbDeadE++; }
			if (eco->DeadPrey[i].deathType == 3){ nbDeadK++; }
			DeadA += eco->DeadPrey[i].age;
			DeadE += eco->DeadPrey[i].energy;
		}
	}
	DeadA = nbDeadA + nbDeadE + nbDeadK > 0 ? (float)DeadA / (float)(nbDeadA + nbDeadE + nbDeadK) : 0;
	DeadE = nbDeadA + nbDeadE + nbDeadK > 0 ? (float)DeadE / (float)(nbDeadA + nbDeadE + nbDeadK) : 0;
	DeathRatio = (float)(nbDeadA + nbDeadE + nbDeadK) / (float)(nbSameGenderInds + nbDeadA + nbDeadE + nbDeadK);
	DeadAratio = nbDeadA + nbDeadE + nbDeadK > 0 ? (float)nbDeadA / (float)(nbDeadA + nbDeadE + nbDeadK) : 0;
	DeadEratio = nbDeadA + nbDeadE + nbDeadK > 0 ? (float)nbDeadE / (float)(nbDeadA + nbDeadE + nbDeadK) : 0;
	DeadKratio = nbDeadA + nbDeadE + nbDeadK > 0 ? (float)nbDeadK / (float)(nbDeadA + nbDeadE + nbDeadK) : 0;
}

void StatSpeciesPrey::write(ofstream *statFile) {

	*statFile << Manip.vtos(gen) << ", " << Manip.vtos(nbSpecies);
	*statFile << ", " << Manip.vtos(speciesParentID) << ", " << Manip.vtos(speciesID) << ", " << Manip.vtos(nbSameGenderInds) << ", " << Manip.vtos(individualRatio);

	*statFile << ", " << Manip.vtos((float)nbBirth / (float)nbSameGenderInds) << ", " << (nbBirth > 0 ? Manip.vtos((float)nbInterbreeding / (float)nbBirth) : Manip.vtos(0));
	*statFile << ", " << Manip.vtos(DeathRatio) << ", " << Manip.vtos(DeadAratio) << ", " << Manip.vtos(DeadEratio) << ", " << Manip.vtos(DeadKratio) << ", " << Manip.vtos(DeadA) << ", " << Manip.vtos(DeadE);
	*statFile << ", " << Manip.vtos(entropy) << ", " << Manip.vtos(diversityS) << ", " << Manip.vtos(diversitySratio);
	*statFile << ", " << Manip.vtos(distEvol / (float)nbSameGenderInds) << ", " << Manip.vtos(stateOfBirth / (float)nbSameGenderInds) << ", " << Manip.vtos(age / (float)nbSameGenderInds) << ", " << Manip.vtos(energy / (float)nbSameGenderInds) << ", " << Manip.vtos(strength / (float)nbSameGenderInds) << ", " << Manip.vtos(speed / (float)nbSameGenderInds);
	*statFile << ", " << Manip.vtos((float)nbSameGenderInds / nbCase) << ", " << Manip.vtos((float)nbArc / (float)nbSameGenderInds);

	int nbActed = 0;
	for (int j = 0; j < FCMPrey::NbActions; j++) {
		nbActed += actions[j];
	}
	
	for (int j = 0; j < FCMPrey::NbActions; j++) {
		*statFile << ", " << Manip.vtos((float)actions[j] / (float)nbActed);
	}

	*statFile << ", " << (actions[FCMPrey::ReproduceFailAction - 1] > 0 ? Manip.vtos((float)reprodFailedA / (float)actions[FCMPrey::ReproduceFailAction - 1]) : Manip.vtos(0)) << ", " << (actions[FCMPrey::ReproduceFailAction - 1] > 0 ? Manip.vtos((float)reprodFailedE / (float)(actions[FCMPrey::ReproduceFailAction - 1])) : Manip.vtos(0));

	*statFile << ", " << (nbParent1 > 0 ? Manip.vtos((float)parent1_reproduceA / (float)(nbParent1)) : Manip.vtos(0)) << ", " << (nbParent1 > 0 ? Manip.vtos(parent1_reproduceE / (float)(nbParent1)) : Manip.vtos(0));
	*statFile << ", " << (nbParent2 > 0 ? Manip.vtos((float)parent2_reproduceA / (float)(nbParent2)) : Manip.vtos(0)) << ", " << (nbParent2 > 0 ? Manip.vtos(parent2_reproduceE / (float)(nbParent2)) : Manip.vtos(0));
	*statFile << ", " << (actions[FCMPrey::ReproduceAction - 1] > 0 ? Manip.vtos(DistMating / (float)(actions[FCMPrey::ReproduceAction - 1])) : Manip.vtos(0)) << ", " << (nbParent1 + nbParent2 > 0 ? Manip.vtos((float)PregnantyDeath / (float)(nbParent1 + nbParent2)) : Manip.vtos(0));

	*statFile << ", " << Manip.vtos((float)reasonReproduceFailed_Energy) << ", " << Manip.vtos((float)reasonReproduceFailed_NoPartner);
	*statFile << ", " << Manip.vtos((float)reasonReproduceFailed_PartnerEnergy) << ", " << Manip.vtos((float)reasonReproduceFailed_PartnerActed);
	*statFile << ", " << Manip.vtos((float)reasonReproduceFailed_PartnerAction) << ", " << Manip.vtos((float)reasonReproduceFailed_PartnerDist);
	//*statFile << ", " << (actions[FCMPrey::ReproduceFailAction - 1] > 0 ? Manip.vtos((float)reasonReproduceFailed_SameGender / (float)(actions[FCMPrey::ReproduceFailAction - 1])) : Manip.vtos(0)); 

	for (int j = 0; j < SIZE_activ; j++) {
		*statFile << ", " << Manip.vtos(activations[j] / (float)nbActed);
	}
	*statFile << "\n";
}

void StatSpeciesPrey::write_FCM(ofstream *statFile) {

	int row, col;

	*statFile << Manip.vtos(gen) << ", " << Manip.vtos(nbSpecies) << ", " << Manip.vtos(speciesParentID) << ", " << Manip.vtos(speciesID) << ", " << Manip.vtos(nbSameGenderInds) << ", ";

	// FCM sFCM=FCM(fcm_row,fcm_col);  // M.M

	// change the map to char and write in file M.M
	for (row = 0; row < fcm_row; row++)
	{
		for (col = 0; col < fcm_col; col++)
		{
			//sFCM.set_chart(row,col, fcm_avg[row][col]);  // M.M
			//*statFile << Manip.vtos(sFCM.getchar_chart(row,col)) << ", ";   // M.M
			*statFile << Manip.vtos(fcm_avg[row][col]) << ", ";   // M.M
		}
	}
	// -- write variance of FCM values
	//sFCM.reset_chart();
	for (row = 0; row < fcm_row; row++)
	{
		for (col = 0; col < fcm_col; col++)
		{
			//sFCM.set_chart(row,col, fcm_var[row][col]);  // M.M
			//*statFile << Manip.vtos(sFCM.getchar_chart(row,col)) << ", ";   // M.M
			*statFile << Manip.vtos(fcm_var[row][col]) << ", ";   // M.M
		}
	}

	// write genome at the end.

	for (size_t j = 0; j < Prey::nbGenes; j++){  // M.M
		*statFile << Manip.vtos(genetic_avg.getGene(j)) << ", ";
	}

	*statFile << "\n";
}

void StatSpeciesPrey::writeSpeciesStat(Ecosystem * eco, int gender) {

	string strGender = "";

	if (gender == 1){
		strGender = "female";
	}
	else if (gender == 0){
		strGender = "male";
	}
	else{
		strGender = "";
	}

	gen = eco->generation;
	pop = (int)eco->rabbits.size();
	nbSpecies = (int)eco->speciesPrey.size();

	cout.clear();
	cout << " Writing results per Species of Prey " + strGender << flush;
	time(&eco->start);

	string fPrey = "", fPreyFcm = "";


	fileName(strGender, &fPrey, &fPreyFcm, gen);
	write_Header(fPrey, fPreyFcm, gen);

	ofstream statFile1, statFile2;
	statFile1.open(fPrey.c_str(), ios::app);
	statFile2.open(fPreyFcm.c_str(), ios::app);

	for (list<Species>::iterator it = eco->speciesPrey.begin(); it != eco->speciesPrey.end(); ++it) {

		reset(); //-- Initialization
		speciesID = (*it).getNum();
		speciesParentID = (*it).getParent();
		nbIndividuals = (int)(*it).getNbMembers();
		diversityS = DiversitySpatial(it, gender);
		entropy = Entropy(it, gender);

		//-- Calculation
		for (int i = 0; i < nbIndividuals; i++){

			if ((*it).preyMembers.at(i)->getGender() != gender && gender != 2){ //will continue for all individuals if gender = 2
				continue;
			}

			nbSameGenderInds++;

			distEvol += (*it).preyMembers.at(i)->getDistanceEvol();
			stateOfBirth += (*it).preyMembers.at(i)->getStateBirth();
			age += (*it).preyMembers.at(i)->getAge();
			energy += (*it).preyMembers.at(i)->getEnergy();
			strength += (*it).preyMembers.at(i)->getStrength();
			speed += (*it).preyMembers.at(i)->getSpeed();

			//-- Compactness;
			int nbSize = 0;
			vector<int> LocalInidv = *(eco->getListCreature(*((*it).preyMembers.at(i)->getPosition()), 0));

			for (int cnt = 0; cnt < (int)LocalInidv.size(); cnt++)
				if ((*it).preyMembers.at(i)->getSpecies() == eco->rabbits[LocalInidv[cnt]].getSpecies()){
					nbSize++;
			}
			LocalInidv.clear();
			nbCase += nbSize > 0 ? (float) 1.0 / (float)nbSize : 0;
			//--

			nbArc += (*it).preyMembers.at(i)->getFCM()->getNbArc();

			if ((*it).preyMembers.at(i)->getLastAction() != 0) { //-- The individual is not new born

				actions[(*it).preyMembers.at(i)->getLastAction() - 1]++;

				for (int j = 0; j < SIZE_activ; j++) {
					activations[j] += (*it).preyMembers.at(i)->getFCM()->get_activation(j);
				}
			}
			else { //-- The individual is new born
				nbBirth++;
				if ((*it).preyMembers.at(i)->getSpeciesParent1ID() != (*it).preyMembers.at(i)->getSpeciesParent2ID()) {
					nbInterbreeding++;
				}

				if ((*it).preyMembers.at(i)->getSpeciesParent1ID() == (*it).preyMembers.at(i)->getSpecies()) {
					parent1_reproduceE += eco->ParentPrey[(*it).preyMembers.at(i)->getParent1ID()].energy;
					parent1_reproduceA += eco->ParentPrey[(*it).preyMembers.at(i)->getParent1ID()].age;
					PregnantyDeath += eco->ParentPrey[(*it).preyMembers.at(i)->getParent1ID()].PregnancyDeath;
					nbParent1++;
				}

				if ((*it).preyMembers.at(i)->getSpeciesParent2ID() == (*it).preyMembers.at(i)->getSpecies()) {
					parent2_reproduceE += eco->ParentPrey[(*it).preyMembers.at(i)->getParent2ID()].energy;
					parent2_reproduceA += eco->ParentPrey[(*it).preyMembers.at(i)->getParent2ID()].age;
					PregnantyDeath += eco->ParentPrey[(*it).preyMembers.at(i)->getParent2ID()].PregnancyDeath;
					nbParent2++;
				}
			}

			if ((*it).preyMembers.at(i)->getLastAction() == FCMPrey::ReproduceAction){
				DistMating += (*it).preyMembers.at(i)->get_DistMating();
			}

			if ((*it).preyMembers.at(i)->getLastAction() == FCMPrey::ReproduceFailAction){

				reprodFailedA += (*it).preyMembers.at(i)->getAge();
				reprodFailedE += (*it).preyMembers.at(i)->getEnergy();

				if ((*it).preyMembers.at(i)->reproduceFailed1 == 1)
					reasonReproduceFailed_Energy++;
				if ((*it).preyMembers.at(i)->reproduceFailed1 == 10)
					reasonReproduceFailed_NoPartner++;
				if ((*it).preyMembers.at(i)->reproduceFailed1 == 11)
				{
					reasonReproduceFailed_Energy++;
					reasonReproduceFailed_NoPartner++;
				}
				reasonReproduceFailed_PartnerEnergy += (*it).preyMembers.at(i)->reproduceFailed2;
				reasonReproduceFailed_PartnerActed += (*it).preyMembers.at(i)->reproduceFailed3;
				reasonReproduceFailed_PartnerAction += (*it).preyMembers.at(i)->reproduceFailed4;
				reasonReproduceFailed_PartnerDist += (*it).preyMembers.at(i)->reproduceFailed5;
				reproductionAttempts += (*it).preyMembers.at(i)->cntReprodFailedTries;
				//reasonReproduceFailed_SameGender += (*it).preyMembers.at(i)->reproduceFailed6; 
			}
		}
		
		individualRatio = (float)nbSameGenderInds / (float)pop; //needs to be calculated after we count nbSameGenderInds...
		
		Death(eco, it, gender);			//-- Information about Death

		write(&statFile1);		//-- Writhing in the file

		AverageFCM(it, &fcm_avg, &fcm_var, gender);

		write_FCM(&statFile2);  //-- Writhing in the file of FCM

	}

	statFile1.close();

	statFile2.close();

	time(&eco->end);
	eco->printTime(eco->start, eco->end);
}


double StatSpeciesPrey::ArcTAN(double delX, double delY)
{
#define PI 3.14159265358979324
	if (fabs(delX) < 1e-6)
	{
		if (delY > 0)
			return PI / 2;
		else return 3 * PI / 2;
	}

	if (delY >= 0)
	{
		if (delX > 0)
			return atan(delY / delX);
		else
			return PI - atan(delY / -delX);
	}
	else
	{
		if (delX > 0)
			return 2 * PI - atan(-delY / delX);
		else
			return PI + atan(-delY / -delX);
	}
}

StatSpeciesPrey::SpatialDiversityParameters StatSpeciesPrey::CalcSpatialDiversityParams(list<Species>::iterator species, int gender)
{
	int i, num_indiv, num_indiv_sameGender = 0;

	double *x_dim, *y_dim;
	const double pi = 2 * acos(0.0);
	const double two_pi = 2 * pi;
	double sum_x, sum_y, factor = two_pi / 999.0;
	SpatialDiversityParameters dparams = { -1.0, -1.0, -1.0, -1.0 };

	num_indiv = species->getNbMembers();

	x_dim = new double[num_indiv];
	y_dim = new double[num_indiv];

	memset(x_dim, 0, num_indiv*sizeof(double));
	memset(y_dim, 0, num_indiv*sizeof(double));

	for (i = 0; i < num_indiv; i++){

		if (species->preyMembers.at(i)->getGender() != gender && gender != 2){
			continue;
		}

		num_indiv_sameGender++;

		x_dim[i] = (double)species->preyMembers.at(i)->pos.x - 1;  //-- transform: [1, 1000] --> [0,999]
		y_dim[i] = (double)species->preyMembers.at(i)->pos.y - 1;
	}

	//-- scale to [0, 2pi]
	for (i = 0; i < num_indiv; i++)
	{
		if (species->preyMembers.at(i)->getGender() != gender && gender != 2){
			continue;
		}

		x_dim[i] *= factor;
		y_dim[i] *= factor;
	}

	//-- find the variance of circular X
	sum_x = sum_y = 0.0;
	for (i = 0; i < num_indiv; i++)
	{
		if (species->preyMembers.at(i)->getGender() != gender && gender != 2){
			continue;
		}

		sum_x += cos(x_dim[i]);
		sum_y += sin(x_dim[i]);
	}

	dparams.m_center_x = ArcTAN(sum_x, sum_y) / factor + 1.0;
	dparams.m_variance_x = num_indiv_sameGender - sqrt(sum_x*sum_x + sum_y*sum_y); //-- var(x) = n - ||R||

	//-- find the variance of circular Y
	sum_x = sum_y = 0.0;
	for (i = 0; i < num_indiv; i++)
	{
		if (species->preyMembers.at(i)->getGender() != gender && gender != 2){
			continue;
		}

		sum_x += cos(y_dim[i]);
		sum_y += sin(y_dim[i]);
	}

	dparams.m_center_y = ArcTAN(sum_x, sum_y) / factor + 1.0;
	dparams.m_variance_y = num_indiv_sameGender - sqrt(sum_x*sum_x + sum_y*sum_y); //-- var(y) = n - ||R||

	delete[] x_dim;
	delete[] y_dim;

	return dparams;
}

float StatSpeciesPrey::DiversitySpatial(list<Species>::iterator species, int gender){

	long long cur_sqr_dist, total_sqr_dist;
	int dx, dy, x_min, y_min, cx, cy;
	SpatialDiversityParameters dparams;

	dparams = CalcSpatialDiversityParams(species, gender);
	cx = int(dparams.m_center_x + 0.5);
	cy = int(dparams.m_center_y + 0.5);

	diversitySratio = 0.0;
	cur_sqr_dist = total_sqr_dist = 0;

	int num_indiv = species->getNbMembers();
	int num_indiv_sameGender = 0;

	for (int i = 0; i < num_indiv; i++){

		if (species->preyMembers.at(i)->getGender() != gender && gender != 2){
			continue;
		}

		num_indiv_sameGender++;

		dx = cx - species->preyMembers.at(i)->pos.x;
		dy = cy - species->preyMembers.at(i)->pos.y;
		if (dx < 0) dx = -dx;
		if (dy < 0) dy = -dy;
		x_min = min(dx, 1000 - dx);
		y_min = min(dy, 1000 - dy);
		cur_sqr_dist = x_min*x_min + y_min*y_min;	//-- square of actual distance
		total_sqr_dist += cur_sqr_dist;
		//-- Morteza
		if (cur_sqr_dist > 0)
			diversitySratio += (float)sqrt(double(cur_sqr_dist));
		//-- Morteza End
	}

	if (num_indiv_sameGender == 0){
		diversitySratio = 0;
		return 0;
	}
	else{
		diversitySratio /= (float)num_indiv_sameGender; //-- Morteza
		return (float)sqrt(double(total_sqr_dist) / double(num_indiv_sameGender));
	}

}


//-- Fix Number of bins
float StatSpeciesPrey::Entropy(list<Species>::iterator species, int gender)
{

	const float BIN_WIDTH = (float)0.1;
	const int NUM_BIN = 1000;
	int col, row, indiv;

	int num_indiv, bin, num_indiv_sameGender = 0;
	int cur_freq, freqs[NUM_BIN];

	float probability;

	num_indiv = species->getNbMembers();

	entropy = 0;

	for (row = 0; row < fcm_row; row++)
	{
		for (col = 0; col < fcm_col; col++)
		{
			for (int k = 0; k < NUM_BIN; k++)
				freqs[k] = 0;

			for (indiv = 0; indiv < num_indiv; ++indiv)
			{

				if (species->preyMembers.at(indiv)->getGender() != gender && gender != 2){
					continue;
				}

				num_indiv_sameGender++;

				bin = int((species->preyMembers.at(indiv)->getFCM()->get_chart(row, col) + 10) / BIN_WIDTH);
				++freqs[abs(bin)];
			}

			for (int i = 0; i < NUM_BIN; i++)
			{
				cur_freq = freqs[i];
				if (cur_freq > 0)
				{

					if (num_indiv_sameGender == 0){
						probability = 0;
					}
					else{
						probability = float(cur_freq) / (float)num_indiv_sameGender;
					}

					if (probability != 0){
#ifdef LinuxSystem
						entropy += (-probability*log2(probability));
#else
						entropy += (-probability*log(probability));
#endif
					}
				}
			}
		}
	}

	return entropy;
}

void StatSpeciesPrey::AverageFCM(list<Species>::iterator species, vector<vector<float> > * fcm_avg, vector<vector<float> > * fcm_var, int gender)
{
	int col, row, indiv;

	//vector<vector<float> > cur_fcm;
	vector<vector<unsigned char> > cur_fcm;

	vector<vector<float> > avg = vector<vector<float> >(fcm_row, vector<float>(fcm_col, 0));
	vector<vector<float> > var = vector<vector<float> >(fcm_row, vector<float>(fcm_col, 0));
	vector <float> genome_sum = vector<float>(Prey::nbGenes, 0);  // M.M
	vector <float> genome_avg = vector<float>(Prey::nbGenes, 0);  // M.M

	int num_indiv = species->getNbMembers();

	int num_indiv_sameGender = 0;

	//-- Average of FCM
	for (indiv = 0; indiv < num_indiv; ++indiv)
	{
		if (species->preyMembers.at(indiv)->getGender() != gender && gender != 2){
			continue;
		}

		num_indiv_sameGender++;

		//cur_fcm = species->preyMembers.at(indiv)->getFCM()->get_chart_float();  M.M
		cur_fcm = species->preyMembers.at(indiv)->getFCM()->get_char_chart();  // M.M
		for (row = 0; row < fcm_row; row++)
		{
			for (col = 0; col < fcm_col; col++)
			{
				avg[row][col] += FCM::char2float(cur_fcm[row][col]);
			}
		}

		// M.M
		PhysicalGenome* genome_temp = species->preyMembers.at(indiv)->getPhGenome();
		for (size_t j = 0; j < Prey::nbGenes; j++){ // M.M
			//genome_sum[j] = genome_sum[j] + (*genome_temp)[j];
			genome_sum[j] += genome_temp->getGene(j);
		}
	}

	for (row = 0; row < fcm_row; row++)
	{
		for (col = 0; col < fcm_col; col++)
		{
			if (num_indiv_sameGender == 0){
				avg[row][col] = 0;
			}
			else{
				avg[row][col] /= (float)num_indiv_sameGender;
			}
		}
	}
	for (size_t j = 0; j < Prey::nbGenes; j++){ // M.M
		if (num_indiv_sameGender == 0){
			genome_avg[j] = 0;
		}
		else{
			genome_avg[j] = (float)(genome_sum[j] / num_indiv_sameGender);
		}
	}

	this->genetic_avg = PhysicalGenome(Prey::nbGenes, &genome_avg); // M.M

	*fcm_avg = avg;
	//-- End of Average of FCM


	//-- Variance of FCM
	float VAR;
	for (indiv = 0; indiv < num_indiv; ++indiv)
	{
		if (species->preyMembers.at(indiv)->getGender() != gender && gender != 2){
			continue;
		}

		// cur_fcm = species->preyMembers.at(indiv)->getFCM()->get_chart_float();  M.M
		cur_fcm = species->preyMembers.at(indiv)->getFCM()->get_char_chart();  // M.M
		for (row = 0; row < fcm_row; row++)
		{
			for (col = 0; col < fcm_col; col++)
			{
				VAR = FCM::char2float(avg[row][col]) - FCM::char2float(cur_fcm[row][col]);
				var[row][col] += VAR * VAR;
			}
		}
	}

	for (row = 0; row < fcm_row; row++)
	{
		for (col = 0; col < fcm_col; col++)
		{
			if (num_indiv_sameGender == 0){
				var[row][col] = 0;
			}
			else{
				var[row][col] /= (float)num_indiv_sameGender;
			}
		}
	}

	*fcm_var = var;
	//-- End of Variance of FCM
}


void StatSpeciesPrey::RemoveEnd(Ecosystem *eco, int gender){
	int gen = eco->generation;

	string strGender = "";

	if (gender == 1){
		strGender = "female";
	}
	else if (gender == 0){
		strGender = "male";
	}
	else{
		strGender = "";
	}

	cout.clear();
	cout << " Remove End of ResultSpecies Prey file" << flush;
	time(&eco->start);

	string Info;
	int Result = 1;

	ifstream statFileInPrey, statFileInPreyFCM;
	ofstream statFileOutPrey, statFileOutPreyFCM;

	ostringstream input, output;


#ifdef LinuxSystem
	input << "./" << "//Results_SpeciesPrey//";
	output << "./" << "//Results_SpeciesPrey//";
	mkdir(input.str().c_str(), 0777);
#endif
	input << "Result_Prey_" + strGender << (int)(gen / 1000) << ".csv";
	output << "Result_PreyTMP_" + strGender << (int)(gen / 1000) << ".csv" << ".bk";

	statFileInPrey.open(input.str().c_str());   //-- Try to open ResultsSpecies Prey file
	if (!statFileInPrey.is_open()) {
		cout << "Error! ResultSpeciesPrey file for gen= " << gen << "not found" << endl;
		return;
	}


	statFileOutPrey.open(output.str().c_str(), ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOutPrey.is_open()){
		statFileInPrey.close();
		cout << "Error! New ResultSpeciesPrey cannot create" << endl;
		return;
	}


	while (!statFileInPrey.eof()){
		getline(statFileInPrey, Info);

		if (int(Info.find(", ")) > -1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOutPrey << Info << endl;
	}

	statFileInPrey.close();
	statFileOutPrey.close();

	remove(input.str().c_str());
	rename(output.str().c_str(), input.str().c_str());


	//-- FCM part
	input.clear();
	input.str("");
	output.clear();
	output.str("");
#ifdef LinuxSystem
	input << "./" << "//Results_SpeciesPrey//";
	output << "./" << "//Results_SpeciesPrey//";
#endif

	input << "Result_PreyFCM_" + strGender << (int)(gen / 1000) << ".csv";
	output << "Result_PreyFCMTMP_" + strGender << (int)(gen / 1000) << ".csv" << ".bk";

	statFileInPreyFCM.open(input.str().c_str());   //-- Try to open ResultsSpecies PreyFCM file
	if (!statFileInPreyFCM.is_open()) {
		cout << "Error! ResultSpeciesPreyFCM file for gen= " << gen << "not found" << endl;
		return;
	}

	statFileOutPreyFCM.open(output.str().c_str(), ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
	if (!statFileOutPreyFCM.is_open()){
		statFileInPreyFCM.close();
		cout << "Error! New ResultSpeciesPreyFCM cannot create" << endl;
		return;
	}

	while (!statFileInPreyFCM.eof()){
		getline(statFileInPreyFCM, Info);

		if (int(Info.find(", ")) > -1){
			Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
		}


		if ((gen - 1) < Result) //-- Remove after this generation
			break;
		else if (Info.size() > 0)
			statFileOutPreyFCM << Info << endl;
	}

	statFileInPreyFCM.close();
	statFileOutPreyFCM.close();

	remove(input.str().c_str());
	rename(output.str().c_str(), input.str().c_str());

	time(&eco->end);
	eco->printTime(eco->start, eco->end);
}
