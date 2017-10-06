/*
 * Ecosystem.cpp
 *  - Maintains lists of the prey and predator objects
 *    and details about the environment such height, width,
 *    ageMaxPrey, probabilityMutation, etc.f
 */

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>


#include "EasyBMP.h"
#include "Ecosystem.h"
#include "Species.h"
#include "Prey.h"
#include "Predator.h"
#include "Stat.h"
#include "Position.h"
#include "Random.h"

using namespace std;


//-- new: make this function member methods
bool aTuerSpeciesObj(Species e) {
	return  e.getNbMembers() == 0; //-- Armin
}

bool aDeadPrey(Prey p) {
	if (p.getLiving()) {
		return false;
	}
	else {
		return true;
	}
}

bool aDeadPred(Predator p) {
	if (p.getLiving()) {
		return false;
	}
	else {
		return true;
	}
}


//=========================================
//+++++++++++++++++++++++++++++++++++++++++
//=========================================

void Ecosystem::removeDeadPrey() {
	vector<Prey>::iterator newEnd = remove_if(rabbits.begin(), rabbits.end(), aDeadPrey);
	rabbits.erase(newEnd, rabbits.end());
}

void Ecosystem::removeDeadPred() {
	vector<Predator>::iterator newEnd = remove_if(wolves.begin(), wolves.end(), aDeadPred);
	wolves.erase(newEnd, wolves.end());
}

void Ecosystem::removeDeadSpeciesPrey() {
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		if ((*S).getNbMembers() == 0)
		{
			ofstream preyPhylogeny;
			preyPhylogeny.open ("PreyPhylogeny.txt", ios::app);
			preyPhylogeny << generation << "," << (*S).getNum() << ",Extinction" << "\n";
			preyPhylogeny.close();
		}
	}
	speciesPrey.remove_if(aTuerSpeciesObj);
}

void Ecosystem::removeDeadSpeciesPred() {
	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		if ((*S).getNbMembers() == 0)
		{
			ofstream predPhylogeny;
			predPhylogeny.open ("PredPhylogeny.txt", ios::app);
			predPhylogeny << generation << "," << (*S).getNum() << ",Extinction" << "\n";
			predPhylogeny.close();
		}
	}
	speciesPred.remove_if(aTuerSpeciesObj);
}

Ecosystem::Ecosystem() {

	int initNbPrey, initNbPredator, perClusterPrey, perClusterPredator, RadiusCluster;
	float energyMaxPrey, energyMaxPredator;
	float stateBirthPreyMale, stateBirthPredMale;
	float stateBirthPreyFemale, stateBirthPredFemale;
	short speedMaxPrey, speedMaxPredator, vsionPrey, visionPredator;

	short nSensPrey, nConceptsPrey, nMoteurDepPrey, nMoteurFixPrey;
	short nSensPredator, nConceptsPredator, nMoteurDepPredator, nMoteurFixPredator;

	preyPix.Blue=0; preyPix.Red=0; preyPix.Green=255; preyPix.Alpha=0;
	predPix.Blue=0; predPix.Red=255; predPix.Green=0; predPix.Alpha=0;
	bkground.Blue=0; bkground.Red=0; bkground.Green=0; bkground.Alpha=0;
	grassPix.Blue = 0; grassPix.Red = 0; grassPix.Green = 255; grassPix.Alpha = 0;
	meatPix.Blue = 0; meatPix.Red=255; meatPix.Green=0; meatPix.Alpha=0;

	snapShot.SetBitDepth(32);
	snapShot.SetSize(5000,5000);
	snapShot2.SetBitDepth(32);
	snapShot2.SetSize(5000,5000);
	snapShot3.SetBitDepth(32);
	snapShot3.SetSize(5000,5000);

	maxPreyID = 1;
	maxPredID = 1;

	string entreInfo;
	vector<string> words;
#ifdef TWO_RESOURCES
	ifstream paramMonde ("Parameters2.txt");		//-- Define parameters file
#else
	ifstream paramMonde("Parameters1.txt");		//-- Define parameters file
#endif
	nbPrey = 0;
	nbPredator = 0;

	nameConceptsPrey.resize(100);
	nameConceptsPred.resize(100);

	if (!paramMonde.is_open()) {		//-- Try to open parameters file
		cout << "Error! Parameter file not found" << endl;
		exit(-1);
	}
	else {
#ifdef TWO_RESOURCES
		nbResources = 4;	// AG:  grass1, meat, grass2, fertilizer
#else
		nbResources = 3;	// AG:  grass, meat, fertilizer
#endif
		getline(paramMonde, entreInfo); //-- Comment, General and Environmental Parameters		getline (paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		HDF5 = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); //-- Blank

		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		width = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		height = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		valueGrass = (float)atof(words[1].c_str()); words.clear();
#ifdef TWO_RESOURCES
		getline (paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Armin
		valueGrass2 = (float) atof(words[1].c_str()); words.clear(); //-- Armin
#endif
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		maxGrass = atoi(words[1].c_str()); words.clear();
#ifdef TWO_RESOURCES
		getline (paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		maxGrass2 = atoi(words[1].c_str()); words.clear();
#endif
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		speedGrowGrass = (float)atof(words[1].c_str()); words.clear();
#ifdef TWO_RESOURCES
		getline (paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		speedGrowGrass2 = (float) atof(words[1].c_str()); words.clear(); //-- Armin
#endif
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		probaInitialGrass = (float)atof(words[1].c_str()); words.clear();
#ifdef TWO_RESOURCES
		getline (paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Armin
		probaInitialGrass2 = (float) atof(words[1].c_str()); words.clear(); //-- Armin
#endif
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		probaGrowGrass = (float)atof(words[1].c_str()); words.clear();
#ifdef TWO_RESOURCES
		getline (paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);     //-- Armin
		probaGrowGrass2 = (float) atof(words[1].c_str()); words.clear(); //-- Armin
#endif
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);	//-- Armin
		maxMeat = atoi(words[1].c_str()); //words.clear();
		wastedMeatPerGen = atoi(words[2].c_str()); words.clear();  // M.M
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		probaMutation = (float)atof(words[1].c_str()); //words.clear();
		probaMutation_HighAge = (float)atof(words[2].c_str()); words.clear();	// Abbas
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		probaMutationLow = (float)atof(words[1].c_str()); //words.clear();
		probaMutationLow_HighAge = (float)atof(words[2].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		percentMutation = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		percentMutationHigh = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		RadiusCluster = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		distanceMin = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		maxSaveFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		minSaveFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		minSaveCompressedFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		worldSaveFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		tarMinSaveWorldFlag = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		perspeciesPreyFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		perspeciesPredFlag = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		restore = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		MatingMode = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		IsWithoutPredator = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		FertilizerDivideTo = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		Visualizations = atoi(words[1].c_str()); words.clear();
		
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		fluctuatingResources = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		fluctuationAmplitudeRatio = atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		fluctuationMinimumRatio = atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		fluctuationCycle = atoi(words[1].c_str()); words.clear();
		
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		circularFoodGrowth = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		foodCircleDiameter = (float) atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		foodCircleMaxRatio = (float) atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		foodCircleMinimumRatio = (float) atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		persuasionHappens = atoi(words[1].c_str()); words.clear();
		
		
		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Comment, Prey Parameters
		getline(paramMonde, entreInfo); //-- Blank

		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		initNbPrey = atoi(words[1].c_str()); words.clear();
		
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		valuePrey = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		ageMaxPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		energyMaxPrey = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		speedMaxPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		ageReprodPrey = atoi(words[1].c_str()); words.clear();  //M.M
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		stateBirthPreyMale = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		stateBirthPreyFemale = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		vsionPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		perClusterPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		distanceSpeciesPrey = (float)atof(words[1].c_str()); words.clear();

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Comment, Predator Parameters
		getline(paramMonde, entreInfo); //-- Blank

		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		initNbPredator = atoi(words[1].c_str()); words.clear();
		if (IsWithoutPredator==1)
			initNbPredator = 0;
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		ageMaxPredator = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		energyMaxPredator = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		speedMaxPredator = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		ageReprodPredator = atoi(words[1].c_str()); words.clear(); // M.M
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		stateBirthPredMale = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		stateBirthPredFemale = (float)atof(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		visionPredator = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		perClusterPredator = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		distanceSpeciesPred = (float)atof(words[1].c_str()); words.clear();

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Comment, FCM Parameters for Prey
		getline(paramMonde, entreInfo); //-- Blank

		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		nSensPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		nConceptsPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		nMoteurDepPrey = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		nMoteurFixPrey = atoi(words[1].c_str()); words.clear();

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Header

		FCMPrey::static_init(nSensPrey, nConceptsPrey, nMoteurDepPrey, nMoteurFixPrey);
		fcm_prey = FCMPrey(NULL);

		//MRE RandomGoodGene
		float  genomeArrMalePrey[] = { energyMaxPrey, ageMaxPrey, vsionPrey, speedMaxPrey, ageReprodPrey, stateBirthPreyMale, .05, .05 };  // M.M
		vector<float> male_prey_genome(genomeArrMalePrey, genomeArrMalePrey + sizeof(genomeArrMalePrey) / sizeof(float));

		float  genomeArrFemalePrey[] = { energyMaxPrey, ageMaxPrey, vsionPrey, speedMaxPrey, ageReprodPrey, stateBirthPreyFemale, .05, .05 };  // M.M
		vector<float> female_prey_genome(genomeArrFemalePrey, genomeArrFemalePrey + sizeof(genomeArrFemalePrey) / sizeof(float));

		float genomeArrMalePred[] = { energyMaxPredator, ageMaxPredator, visionPredator, speedMaxPredator, ageReprodPredator, stateBirthPredMale };  // M.M
		vector<float> male_pred_genome(genomeArrMalePred, genomeArrMalePred + sizeof(genomeArrMalePred) / sizeof(float));

		float genomeArrFemalePred[] = { energyMaxPredator, ageMaxPredator, visionPredator, speedMaxPredator, ageReprodPredator, stateBirthPredFemale };  // M.M
		vector<float> female_pred_genome(genomeArrFemalePred, genomeArrFemalePred + sizeof(genomeArrFemalePred) / sizeof(float));

		ini_MalePreyGenome = PhysicalGenome(Prey::nbGenes, &male_prey_genome);  // M.M
		ini_MalePredGenome = PhysicalGenome(Predator::nbGenes, &male_pred_genome);  // M.M

		ini_FemalePreyGenome = PhysicalGenome(Prey::nbGenes, &female_prey_genome);  // M.M
		ini_FemalePredGenome = PhysicalGenome(Predator::nbGenes, &female_pred_genome);  // M.M


		//-- chart
		for (size_t i = 0; i < fcm_prey.get_rows(); ++i) {
			getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words, "\t");
			for (size_t j = 0; j < fcm_prey.get_cols(); ++j) {
				fcm_prey.set_chart(i, j, (float)atof(words[j + 1].c_str()));
			}
			words.clear();
		}

		fcm_prey.calculate_nb_arc();
		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); words.clear(); //-- Header
		//-- Sense parameters
		for (int i = 0; i < FCMPrey::nbSens; ++i) {
			getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words, "\t");
			nameConceptsPrey[i] = words[0];
			fcm_prey.set_activation(i, (float)atof(words[1].c_str()));
			FCMPrey::fuzzyfication_params1[i][0] = (float)atof(words[2].c_str());
			FCMPrey::fuzzyfication_params1[i][1] = (float)atof(words[3].c_str());
			words.clear();
		}

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); words.clear(); //-- Header
		//-- Concept + Motor parameters
		for (int i = FCMPrey::nbSens; i < (int)fcm_prey.get_rows(); ++i) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words, "\t");
			nameConceptsPrey[i] = words[0];
			fcm_prey.set_activation(i, (float)atof(words[1].c_str()));
			FCMPrey::fuzzyfication_params2[i][0] = (float)atof(words[2].c_str());
			FCMPrey::fuzzyfication_params2[i][1] = (float)atof(words[3].c_str());
			FCMPrey::fuzzyfication_params2[i][2] = (float)atof(words[4].c_str());
		}

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Comment, FCM Parameters for Predator
		getline(paramMonde, entreInfo); //-- Blank

		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		nSensPredator = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		nConceptsPredator = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		nMoteurDepPredator = atoi(words[1].c_str());
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		nMoteurFixPredator = atoi(words[1].c_str());

		FCMPredator::static_init(nSensPredator, nConceptsPredator, nMoteurDepPredator, nMoteurFixPredator);
		fcm_pred = FCMPredator(NULL);

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Header
		//-- Predator chart
		for (size_t i = 0; i < fcm_pred.get_rows(); ++i) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words, "\t");
			for (size_t j = 0; j < fcm_pred.get_cols(); ++j) {
				fcm_pred.set_chart(i, j, (float)atof(words[j + 1].c_str()));
			}
		}

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Header
		//-- Predator Sense parameters
		for (int i = 0; i < FCMPredator::nbSens; ++i) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words, "\t");
			nameConceptsPred[i] = words[0];
			fcm_pred.set_activation(i, (float)atof(words[1].c_str()));
			FCMPredator::fuzzyfication_params1[i][0] = (float)atof(words[2].c_str());
			FCMPredator::fuzzyfication_params1[i][1] = (float)atof(words[3].c_str());
		}

		getline(paramMonde, entreInfo); //-- Blank
		getline(paramMonde, entreInfo); //-- Header
		//-- Predator Concept+Motor parameters
		for (int i = FCMPredator::nbSens; i < (int)fcm_pred.get_rows(); ++i) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words, "\t");
			nameConceptsPred[i] = words[0];
			fcm_pred.set_activation(i, (float)atof(words[1].c_str()));
			FCMPredator::fuzzyfication_params2[i][0] = (float)atof(words[2].c_str());
			FCMPredator::fuzzyfication_params2[i][1] = (float)atof(words[3].c_str());
			FCMPredator::fuzzyfication_params2[i][2] = (float)atof(words[4].c_str());
		}

		world = vector<vector<vector<float> > >(height, vector<vector<float> >(width, vector<float>(nbResources))); //-- Armin

		initializeMaxGrass();
		
		if (restore == 0) {

#ifdef LinuxSystem
			ifstream existing_min("Min0//MinSave1.bin");
			ifstream existing_world("World0//World1.bin");
			if (existing_min.is_open() || existing_world.is_open()){
				cout << "\nThe risk of overwriting an exisitng run...\n\n";
				exit(-1);
			}
#endif

			cout << "Creating ecosystem" << endl;

			generation = 1;

			float energy;
			int percent;
			short age, ageMax;
			short type; //-- Type of individual: 0->prey and 1->predator
			long ID;
			float DistEvolInitial, speed;
			int species, SpeciesParent1ID, SpeciesParent2ID;
			long Parent1ID, Parent2ID;
			Position position, positionCluster;
			short genderVal; //Add Gender /MRE/ Female=1, Male=0

			//+++++++++++++++++++++++++++++++++++++++++++++++++++//
			cout << " - Creating prey" << endl;

			if (perClusterPrey < 1) perClusterPrey = 1; //-- Cluster addjustment

			type = 0; //-- prey
			species = 1;
			DistEvolInitial = 0;
			Parent1ID = 0;
			Parent2ID = 0;
			SpeciesParent1ID = 0;
			SpeciesParent2ID = 0;
			speed = 0;

			for (int i = 0; i < (initNbPrey / perClusterPrey); ++i) {		//-- Creates Prey
				positionCluster.x = rand.next(width);
				positionCluster.y = rand.next(height);

				for (int j = 1; j <= perClusterPrey; j++) {
					position.x = (int)(positionCluster.x + rand.next(RadiusCluster * 2) - RadiusCluster) % width;
					position.y = (int)(positionCluster.y + rand.next(RadiusCluster * 2) - RadiusCluster) % height;
					if (position.x < 0) { position.x = (width + position.x) % width; }
					if (position.y < 0) { position.y = (height + position.y) % height; }

					percent = rand.next(60);
					energy = energyMaxPrey - (energyMaxPrey * percent) / 100;
					percent = rand.next(-25, 25);
					age = rand.next(1, getAgeReprodPrey() + 19);
					ageMax = ageMaxPrey;
					ID = maxPreyID;
					double genderProb = (double)RandGen::next(100) / 100.0;
					if (genderProb > 0.50) genderVal = 0;
					else genderVal = 1;
					//genderVal = rand.next(0, 1); //Add Gender /MRE/ Female=1, Male=0
					// M.M modified

					//MRE RandomGoodGene
					Prey * newPrey;
					if (genderVal == 1)//1 means Gender::Female
					{
						newPrey = new Prey(type, ID, species, Parent1ID, Parent2ID, SpeciesParent1ID, SpeciesParent2ID, position, energy, energyMaxPrey, speed, speedMaxPrey, age, ageMax, vsionPrey, ageReprodPrey, DistEvolInitial, stateBirthPreyFemale, &fcm_prey, genderVal, 0.05, 0.05); //Add Gender /MRE/
					}
					else
					{ //baby is male
						newPrey = new Prey(type, ID, species, Parent1ID, Parent2ID, SpeciesParent1ID, SpeciesParent2ID, position, energy, energyMaxPrey, speed, speedMaxPrey, age, ageMax, vsionPrey, ageReprodPrey, DistEvolInitial, stateBirthPreyMale, &fcm_prey, genderVal, 0.05, 0.05); //Add Gender /MRE/
					}


					maxPreyID++;
					rabbits.push_back(*newPrey);

					delete newPrey;
					rabbits.back().getFCM()->prey = &(rabbits.back());
					nbPrey++;

				}
			}

			for (size_t i = 0; i < rabbits.size(); ++i) {
				rabbits[i].getFCM()->prey = &rabbits[i];
			}

			/*
			 * Initially all prey have the same FCM
			 * so they belong to the same species
			 */

			FCMPrey s_fcm_prey;
			PhysicalGenome s_genome_prey = PhysicalGenome(Prey::nbGenes); // M.M
			Prey * pe = NULL;
			s_fcm_prey = FCMPrey(fcm_prey.get_chart(), NULL, NULL);

			Species * newPreySpecies = new Species(1, nbPrey, &s_fcm_prey, &s_genome_prey);   // M.M modified
			newPreySpecies->setParent(0);
			speciesPrey.push_back(*newPreySpecies);
			delete newPreySpecies;
			delete pe;
			Species::numSpeciesPreyMax = 1;

			//-- For the single prey species, add pointers to its members
			for (size_t i = 0; i < rabbits.size(); i++) {
				rabbits[i].setSpecies(1);
				rabbits[i].speciesPtr = &(speciesPrey.front());
				speciesPrey.front().preyMembers.push_back(&rabbits[i]);
			}

			//+++++++++++++++++++++++++++++++++++++++++++++++++++//
			cout << " - Creating predator" << endl;

			if (perClusterPredator < 1) perClusterPredator = 1; //-- Cluster addjustment

			type = 1; //-- predator
			species = 1;
			DistEvolInitial = 0;
			Parent1ID = 0;
			Parent2ID = 0;
			SpeciesParent1ID = 0;
			SpeciesParent2ID = 0;

			for (int i = 0; i < (initNbPredator / perClusterPredator); ++i) {	//-- Creates Predator
				positionCluster.x = rand.next(width);
				positionCluster.y = rand.next(height);

				for (int j = 1; j <= perClusterPredator; ++j) {
					position.x = (positionCluster.x + rand.next(RadiusCluster * 2) - RadiusCluster) % width;
					position.y = (positionCluster.y + rand.next(RadiusCluster * 2) - RadiusCluster) % height;
					if (position.x < 0) { position.x = (width + position.x) % width; }
					if (position.y < 0) { position.y = (height + position.y) % height; }

					percent = rand.next(60);
					energy = energyMaxPredator - (energyMaxPredator * percent) / 100;
					percent = rand.next(-15, 15);
					//short ageMaxOffset = (ageMaxPredator * percent) / 100;  // M.M
					age = rand.next(1, getAgeReprodPred() + 30);
					ageMax = ageMaxPredator;
					
					double genderProb = (double)RandGen::next(100) / 100.0;
					if (genderProb > 0.50) genderVal = 0;
					else genderVal = 1;
					//genderVal = rand.next(0, 1); //Add Gender /MRE/ Female=1, Male=0
					ID = maxPredID;
					// M.M modified

					//MRE RandomGoodGene
					Predator * newPred;
					if (genderVal == 1)//1 means Gender::Female
					{
						newPred = new Predator(type, ID, species, Parent1ID, Parent2ID, SpeciesParent1ID, SpeciesParent2ID, position, energy, energyMaxPredator, speed, speedMaxPredator, age, ageMax, visionPredator, ageReprodPredator, DistEvolInitial, stateBirthPredFemale, &fcm_pred, genderVal); //Add Gender /MRE/Error
					}
					else{ //baby is male
						newPred = new Predator(type, ID, species, Parent1ID, Parent2ID, SpeciesParent1ID, SpeciesParent2ID, position, energy, energyMaxPredator, speed, speedMaxPredator, age, ageMax, visionPredator, ageReprodPredator, DistEvolInitial, stateBirthPredMale, &fcm_pred, genderVal); //Add Gender /MRE/Error
					}

					maxPredID++;
					wolves.push_back(*newPred);

					delete newPred;
					nbPredator++;
				}
			}

			for (size_t i = 0; i < wolves.size(); ++i) {
				wolves[i].getFCM()->predator = &wolves[i];
			}

			PhysicalGenome s_genome_pred = PhysicalGenome(Predator::nbGenes);  // M.M
			FCMPredator s_fcm_pred;
			Predator * pd = NULL;
			s_fcm_pred = FCMPredator(fcm_pred.get_chart(), NULL, NULL);
			Species * newPredSpecies = new Species(1, nbPredator, &s_fcm_pred, &s_genome_pred);  // M.M modified

			newPredSpecies->setParent(0);
			speciesPred.push_back(*newPredSpecies);
			delete newPredSpecies;
			delete pd;
			Species::numSpeciesPredMax = 1;

			//-- For the single predator species, add pointers to its members
			for (size_t i = 0; i < wolves.size(); i++) {
				wolves[i].setSpecies(1);
				wolves[i].speciesPtr = &(speciesPred.front());
				speciesPred.front().predMembers.push_back(&wolves[i]);
			}

			cout << " - Creating world" << endl;

			for (int i = 0; i < width; ++i) {
				for (int j = 0; j < height; ++j) {
					world[i][j][2] = 0;   // There is no fertilizer at the beginning
					float pr = (float)(rand.next(1000)) / (float)1000.0;
					if (pr <= probaInitialGrass) {
						float pi = 3.14159265;
						if (fluctuatingResources == 1) maxModifiedGrass = ((cos((generation * pi) / (fluctuationCycle * 0.5)) + 1) * maxGrass * fluctuationAmplitudeRatio + fluctuationMinimumRatio * maxGrass);
						else if (fluctuatingResources == 0) maxModifiedGrass = maxGrass;
						int n = rand.next(1, (int)(((float)worldMaxGrassArray[i][j] * maxModifiedGrass) + 0.5));
						world[i][j][0] = (float)n;
					}
#ifdef TWO_RESOURCES
					pr = (float)(rand.next(1000)) / (float)1000.0;
					if (pr <= probaInitialGrass2) {
						int n = rand.next(1, maxGrass2);
						world[i][j][2] = (float)n;
					}
#endif
					//-- </Armin>
				}
			}

			//int cntr = 0;
			for (size_t i = 0; i < rabbits.size(); ++i) {
				addListCreature(*(rabbits[i].getPosition()), 0, i);
				if (rabbits[i].getGender() == 0){ //MALE
					addListMaleCreature(*(rabbits[i].getPosition()), 0, i);
					//cntr++;
				}
			}

			//cntr = 0;
			for (size_t i = 0; i < wolves.size(); ++i) {
				addListCreature(*(wolves[i].getPosition()), 1, i);
				if (wolves[i].getGender() == 0){ //MALE
					addListMaleCreature(*(wolves[i].getPosition()), 1, i);
					//cntr++;
				}
			}

		}
		else {	//-- Else, user is restoring a state
			// HDF5

#ifdef HDF5_COMPILE
			restoreState_HDF5();
#else
			restoreState();
#endif
			//--	
		}

	}			//-- ends "else" paramMonde.txt is available

	//-- Initialization of Diffrent results Files
	statWorld = Stat(nSensPrey, nConceptsPrey, nMoteurDepPrey, nMoteurFixPrey, nSensPredator, nConceptsPredator, nMoteurDepPredator, nMoteurFixPredator, this);
	statWorld.reset();
	statSpeciesPrey = new StatSpeciesPrey(fcm_prey.get_rows(), fcm_prey.get_cols());  //-- Initilization of class StateSpecies
	statSpeciesPred = new StatSpeciesPred(fcm_pred.get_rows(), fcm_pred.get_cols());  //-- Initilization of class StateSpecies


}

Ecosystem::~Ecosystem() {
	//	genomeFile.close();

}


vector<int> * Ecosystem::getListMaleCreature(Position p, int type) {
	if (type == 0) {
		return &(worldMalePreyArray[p.x][p.y]);
	}
	else {
		return &(worldMalePredArray[p.x][p.y]);
	}
}

vector<int> * Ecosystem::getListCreature(Position p, int type) {
	if (type == 0) {
		return &(worldPreyArray[p.x][p.y]);
	}
	else {
		return &(worldPredArray[p.x][p.y]);
	}
}

void Ecosystem::addListMaleCreature(Position p, int type, int s) {
	if (type == 0) {
		vector<int>::iterator it = worldMalePreyArray[p.x][p.y].begin();
		worldMalePreyArray[p.x][p.y].insert(it, s);
	}
	else {
		vector<int>::iterator it = worldMalePredArray[p.x][p.y].begin();
		worldMalePredArray[p.x][p.y].insert(it, s);
	}
}

void Ecosystem::addListCreature(Position p, int type, int s) {
	if (type == 0) {
		vector<int>::iterator it = worldPreyArray[p.x][p.y].begin();
		worldPreyArray[p.x][p.y].insert(it, s);
	}
	else {
		vector<int>::iterator it = worldPredArray[p.x][p.y].begin();
		worldPredArray[p.x][p.y].insert(it, s);
	}
}

float Ecosystem::addFertilizerFunc(float p, float begin, float end) {
	if (p < begin) {
		return 0;
	}
	else {
		if (p > end) {
			return 20000;
		}
		else {
			return (p - begin);
		}
	}
}


void Ecosystem::addFertilizer(Position p)
{
	vector<int> rabbitList = *(getListCreature(p, 0));
	float sum = 0;
	float FertilizerAdded;
	if (world[p.x][p.y][2] > 0)
	{
		world[p.x][p.y][2] = world[p.x][p.y][2] - (0.1 * world[p.x][p.y][2]);
	}
	if (world[p.x][p.y][2] < 0)
		world[p.x][p.y][2] = 0;
	if ((int)rabbitList.size() > 0)
	{
		for (int cnt = 0; cnt < (int)rabbitList.size(); cnt++)
		{
			sum = sum + rabbits[rabbitList[cnt]].getEnergyMax();
		}
		sum = sum / FertilizerDivideTo;
		FertilizerAdded = addFertilizerFunc(sum, 1, 20000);
		world[p.x][p.y][2] = world[p.x][p.y][2] + FertilizerAdded;

	}

	// Predators
	vector<int> wolvesList = *(getListCreature(p, 1));
	sum = 0;
	if ((int)wolvesList.size() > 0)
	{
		for (int cnt = 0; cnt < (int)wolvesList.size(); cnt++)
		{
			sum = sum + wolves[wolvesList[cnt]].getEnergyMax();
		}
		sum = sum / FertilizerDivideTo;
		FertilizerAdded = addFertilizerFunc(sum, 1, 20000);
		world[p.x][p.y][2] = world[p.x][p.y][2] + FertilizerAdded;
	}

}

float Ecosystem::findSpeedGrass(float p, float begin, float end) {
	if (p < begin) {
		return speedGrowGrass;
	}
	else {
		if (p >(3 * speedGrowGrass)) {
			return (3 * speedGrowGrass);
		}
		else {
			return p;
		}
	}
}

void Ecosystem::incLevelGrass(Position p) {
	addFertilizer(p);
	float speedGrowthGrassNew = findSpeedGrass(world[p.x][p.y][2], speedGrowGrass, 3 * speedGrowGrass);
	if (world[p.x][p.y][0] > 0) {
		world[p.x][p.y][0] += speedGrowthGrassNew;
	}
	else {
		Position p1;
		float pr = (float)rand.next(32000) / (float) 32000.0;
		bool test = false;
		if (pr < probaGrowGrass) {
			p1.x = p.x;
			p1.y = (p.y + 1) % height;
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = p.x;
			p1.y = (p.y - 1) % height;
			if (p1.y < 0) {
				p1.y = (height + p1.y) % height;
			}
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.y = p.y;
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][0] > 0) {
				test = true;
			}
		}
		if (test) {
			world[p.x][p.y][0] += speedGrowGrass;
		}
	}
	if (world[p.x][p.y][0] > (float)worldMaxGrassArray[p.x][p.y] * (float)maxModifiedGrass) {
		world[p.x][p.y][0] = (float)worldMaxGrassArray[p.x][p.y] * (float)maxModifiedGrass;
	}
}

//RS Start
float Ecosystem::getSumOfStrengthAndDefence(Position p) {
	float sum = 0;
	vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
	for (int cnt = 0; cnt < (int)ListOfPrey.size(); cnt++)
	{
		sum = sum + (rabbits[ListOfPrey[cnt]].getStrength() * (1.0 + rabbits[ListOfPrey[cnt]].getDefence()));
	}
	return sum;
}

float Ecosystem::getMaxStrengthAndDefencePrey(Position p) {
	float max_strength = 0;
	vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
	for (int cnt = 0; cnt < (int)ListOfPrey.size(); cnt++)
	{
		if ((rabbits[ListOfPrey[cnt]].getStrength() * (1 + rabbits[ListOfPrey[cnt]].getDefence())) > max_strength)
		{
			max_strength = (rabbits[ListOfPrey[cnt]].getStrength() * (1 + rabbits[ListOfPrey[cnt]].getDefence()));
		}
	}
	return max_strength;
}

float Ecosystem::getDistanceMaxStrengthAndDefencePrey(Position p) {
	float distance;
	float max_strength = 0;
	vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
	for (int cnt = 0; cnt < (int)ListOfPrey.size(); cnt++)
	{
		if ((rabbits[ListOfPrey[cnt]].getStrength() * (1 + rabbits[ListOfPrey[cnt]].getDefence())) > max_strength)
		{
			max_strength = (rabbits[ListOfPrey[cnt]].getStrength() * (1 + rabbits[ListOfPrey[cnt]].getDefence()));
			Position newPosition;
			newPosition.x = rabbits[ListOfPrey[cnt]].getPosition()->x;
			newPosition.y = rabbits[ListOfPrey[cnt]].getPosition()->y;
			distance = p.distance(newPosition, getWidth(), getHeight());
		}
	}
	return distance;
}
//RS End

// ======= A.G
float Ecosystem::getSumofStrength(Position p, int type) {
	float sum = 0;
	if (type == 0) {
		vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
		for (int cnt = 0; cnt < (int)ListOfPrey.size(); cnt++)
		{
			sum = sum + rabbits[ListOfPrey[cnt]].getStrength();
		}
	}
	else {
		vector<int> ListOfPred(worldPredArray[p.x][p.y]);
		for (int cnt = 0; cnt < (int)ListOfPred.size(); cnt++)
		{
			sum = sum + wolves[ListOfPred[cnt]].getStrength();
		}
	}
	return sum;
}





//StrengthStrongestPredLocal //StrengthStrongestPreyLocal //MRE RandomGoodGene
float Ecosystem::getMaxStrength(short Gender, Position p, int type) {
	float max_strength = 0;
	if (type == 0) {
		vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
		for (int cnt = 0; cnt < (int)ListOfPrey.size(); cnt++)
		{
			if (rabbits[ListOfPrey[cnt]].getStrength() > max_strength && rabbits[ListOfPrey[cnt]].getGender() != Gender)
			{
				max_strength = rabbits[ListOfPrey[cnt]].getStrength();
			}
		}
	}
	else {
		vector<int> ListOfPred(worldPredArray[p.x][p.y]);
		for (int cnt = 0; cnt < (int)ListOfPred.size(); cnt++)
		{
			if (wolves[ListOfPred[cnt]].getStrength() > max_strength && wolves[ListOfPred[cnt]].getGender() != Gender)
			{
				max_strength = wolves[ListOfPred[cnt]].getStrength();
			}
		}
	}
	return max_strength;

}
//=====
#ifdef TWO_RESOURCES
void Ecosystem::incLevelGrass2(Position p) {
	if (world[p.x][p.y][2] > 0) {
		world[p.x][p.y][2] += speedGrowGrass2;
		if (world[p.x][p.y][2] > (float)maxGrass2) {
			world[p.x][p.y][2] = (float)maxGrass2;
		}
	} else {
		Position p1;
		float pr = (float) rand.next(32000) / (float) 32000.0;
		bool test = false;
		if (pr < probaGrowGrass2) {
			p1.x = p.x;
			p1.y = (p.y + 1) % height;
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = p.x;
			p1.y = (p.y - 1) % height;
			if (p1.y < 0) {
				p1.y = (height + p1.y) % height;
			}
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.y = p.y;
			p1.x = (p.x + 1) % width;
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
			p1.x = (p.x - 1) % width;
			if (p1.x < 0) {
				p1.x = (width + p1.x) % width;
			}
			if (world[p1.x][p1.y][2] > 0) {
				test = true;
			}
		}
		if (test) {
			world[p.x][p.y][2] += speedGrowGrass2;
		}
	}
}
#endif

// M.M commented
//void Ecosystem::incLevelMeat1(Position p) {
//	world[p.x][p.y][1] += 1;
//	if (world[p.x][p.y][1] > (float)maxMeat) {
//		world[p.x][p.y][1] = (float)maxMeat;
//	}
//}
//
//void Ecosystem::incLevelMeat2(Position p) {
//	world[p.x][p.y][1] += 2;
//	if (world[p.x][p.y][1] > (float)maxMeat) {
//		world[p.x][p.y][1] = (float)maxMeat;
//	}
//}

// M.M => new changes in amount of meat
void Ecosystem::incLevelMeatByStrength(Position p, float preyStrength) {
	world[p.x][p.y][1] += preyStrength;
	if (world[p.x][p.y][1] > (float)maxMeat) {
		world[p.x][p.y][1] = (float)maxMeat;
	}
}

// M.M commented
//void Ecosystem::decLevelGrass(Position p) {
//	//world[p.x][p.y][0]--;
//	world[p.x][p.y][0]-= valueGrass;  // M.M
//	if (world[p.x][p.y][0] < 0) {
//		world[p.x][p.y][0] = 0;
//	}
//}

void Ecosystem::decLevelGrass(Position p, float value) {
	world[p.x][p.y][0] -= value;
	if (world[p.x][p.y][0] < 0) {
		world[p.x][p.y][0] = 0;
	}
}


#ifdef TWO_RESOURCES
void Ecosystem::decLevelGrass2(Position p) {
	world[p.x][p.y][2]--;
	if (world[p.x][p.y][2] < 0) {
		world[p.x][p.y][2] = 0;
	}
}
#endif

// M.M commented
//void Ecosystem::decLevelMeat(Position p) {
//	world[p.x][p.y][1] -= (float)1.0;
//	if (world[p.x][p.y][1] < 0) {
//		world[p.x][p.y][1] = 0;
//	}
//}
//
//void Ecosystem::decLevelMeat2(Position p) {
//	world[p.x][p.y][1] -= (float)0.8;
//	if (world[p.x][p.y][1] < 0) {
//		world[p.x][p.y][1] = 0;
//	}
//}

// M.M
void Ecosystem::decLevelMeat(Position p, float value) {
	world[p.x][p.y][1] -= value;
	if (world[p.x][p.y][1] < 0) {
		world[p.x][p.y][1] = 0;
	}
}

/*
 * updateEco - occurs once for each time step
 */
void Ecosystem::updateEco() {
#ifdef HDF5_COMPILE
	if ((generation - 1) % maxSaveFlag == 0 && this->minSaveFlag != 0){
		sr = new SaveResults();
		sr->CreateFile("MinWorld", generation / maxSaveFlag); // create output file for 1-99 gen
		delete sr;
		cout << "File MinWorld" << generation / maxSaveFlag << ".h5 created.";

	}

	if ((generation - 1) % maxSaveFlag == 0 && this->minSaveCompressedFlag != 0){
		sr = new SaveResults();
		sr->CreateFile("MinWorld_C", generation / maxSaveFlag); // create output file for 1-99 gen
		delete sr;
		cout << "File MinWorld_C" << generation / maxSaveFlag << ".h5 created.";

	}
#endif // HDF5
	//--
	cout << "\nGEN: " << generation << endl;

	time(&gStart);

	//-- Updating new numbers of Prey and Predators

	if ((int)rabbits.size() > 0) {
		//lastGenPreyID = rabbits[((int) rabbits.size()-1)].getID();
		lastGenPreyID = (*std::max_element(rabbits.begin(), rabbits.end(), greaterID)).getID(); //M.M
	}
	if ((int)wolves.size() > 0) {
		//lastGenPredID = wolves[((int) wolves.size()-1)].getID();
		lastGenPredID = (*std::max_element(wolves.begin(), wolves.end(), greaterID)).getID(); //M.M
	}
	if (generation == 1) {
		prevSpeciesPrey.clear();
	}
	else if ((int)speciesPrey.size() > 0) {
		prevSpeciesPrey = speciesPrey;
	}
	if (generation == 1) {
		prevSpeciesPred.clear();
	}
	else if ((int)speciesPred.size() > 0) {
		prevSpeciesPred = speciesPred;
	}

	vector<Prey> newPrey;				//-- New prey born
	vector<Predator> newPredator;		//-- New predator born

	//-- Meisam
	DeadPrey.clear();
	DeadPred.clear();
	ParentPrey.clear();
	ParentPred.clear();
	//-- Meisam End
	preyChangedSp.clear();  //-- Meisam: Transfered from MinSave to UpdateEco()
	predChangedSp.clear();  //-- Meisam: Transfered from MinSave to UpdateEco()

	cout << " Individuals updating" << endl;



	/*
	* 1. PREY UPDATE THEIR CONCEPTS
	*/
	cout << "  - Prey seeing the world" << flush;
	time(&start);

	for (size_t i = 0; i < rabbits.size(); ++i) {
		rabbits[i].updateNew(this);
		//		rabbits[i].calcWeight();
	}

	time(&end);
	printTime(start, end);


	/*
	* 2. PREY ACT
	*/
	cout << "  - Prey acting" << flush;
	time(&start);

	//Reproduction based on gender /MRE/START
	//First female should select her mate
	femaleRabbitsIndexes.clear();
	for (unsigned int i = 0; i < rabbits.size(); i++) { // Act based on strength M.M; who has more strength act first
		cout << flush;
		rabbits[i].incAge();
		if (rabbits[i].getGender() == 1)//1 means Gender::Female
		if (rabbits[i].get_action_offset() == FCMPrey::Reproduce)
			//femaleRabbits.push_back((Prey) rabbits[i]);
			femaleRabbitsIndexes.push_back(i);
	}
	
	for (unsigned int i = 0; i < femaleRabbitsIndexes.size(); i++) {
		cout << flush;
		if (rabbits[femaleRabbitsIndexes[i]].getUpdated() == 0) {
			rabbits[femaleRabbitsIndexes[i]].act_driver(&newPrey, this, MatingMode);
		}
	}
	//Reproduction based on gender /MRE/END

	//for (int i = (int)rabbits.size() - 1; i >= 0; i--) { //-- Meisam: Act based on reverse age
	for (unsigned int i = 0; i < rabbits.size(); i++) { // Act based on strength M.M; who has more strength act first
		cout << flush;
		if (rabbits[i].getUpdated() == 0) {
			if (rabbits[i].get_action_offset() == FCMPrey::Reproduce){ //this is for the males who want to reproduce. since there is no female anymore this will fail...
				rabbits[i].setUpdated(1);
				rabbits[i].setLastAction(FCMPrey::ReproduceFailAction);
				//RS HERE
				float newEnergyPreyActed = rabbits[i].calculationEnergyAction(rabbits[i].getEnergy(), rabbits[i].model.getNbArc(), rabbits[i].getSpeed(), rabbits[i].getStrength(), 0, this);  //-- Losing Energy for acting //RS males don't carry persuasion penalty
				rabbits[i].setEnergy(newEnergyPreyActed);
				//RS END
			}
			else{
				rabbits[i].act_driver(&newPrey, this, MatingMode);
			}
		}

		//-- Meisam
		ParentPrey[rabbits[i].getID()].age = rabbits[i].getAge();
		ParentPrey[rabbits[i].getID()].PregnancyDeath = rabbits[i].getEnergy() > 0 ? 0 : 1;
		//-- Meisam End

		if (rabbits[i].getLastAction() != 0) {
			if (rabbits[i].getGender() == 0) //male
			{
				statWorld.incNbActionPreyMale(rabbits[i].getLastAction() - 1);
			}
			else
			{
				statWorld.incNbActionPreyFemale(rabbits[i].getLastAction() - 1);
			}
			if (rabbits[i].getLastAction() == FCMPrey::ReproduceAction)
				statWorld.incAvgDistMatingPrey(rabbits[i].get_DistMating());  //-- Meisam
		}
		if (rabbits[i].getEnergy() <= 0 || rabbits[i].getAge() > rabbits[i].getAgeMax(true)) {
			rabbits[i].setLiving(false);
			rabbits[i].setEnergy(0); // M.M to update strength
			//incLevelMeat2(*(rabbits[i].getPosition())); M.M
			incLevelMeatByStrength(*(rabbits[i].getPosition()), rabbits[i].getStrength());  // M.M
			if (rabbits[i].getGender() == 1)
				statWorld.incAgeAvgDeadFemalePrey(rabbits[i].getAge());
			else
				statWorld.incAgeAvgDeadMalePrey(rabbits[i].getAge());


			nbPrey--;
			//-- Meisam
			DeadInd.id = rabbits[i].getSpecies();
			DeadInd.id = rabbits[i].getID();
			DeadInd.age = rabbits[i].getAge();
			DeadInd.energy = ((rabbits[i].getEnergy() > 0) ? rabbits[i].getEnergy() : 0);
			DeadInd.lastAction = rabbits[i].getLastAction();
			DeadInd.evolDist = rabbits[i].getDistanceEvol();
			DeadInd.speed = rabbits[i].getSpeed();
			//-- Meisam End

			if (rabbits[i].getEnergy() <= 0) {
				if (rabbits[i].getGender() == 1)
					statWorld.incNbDeadFemalePreyE();
				else
					statWorld.incNbDeadMalePreyE();


				DeadInd.deathType = 2;   //-- Meisam
			}
			else {
				if (rabbits[i].getGender() == 1)
					statWorld.incNbDeadFemalePreyA();
				else
					statWorld.incNbDeadMalePreyA();

				DeadInd.deathType = 1;   //-- Meisam
			}
			DeadPrey.push_back(DeadInd); //-- Meisam
		}
		else {
			if (rabbits[i].getEnergy() > rabbits[i].getEnergyMax()) {
				rabbits[i].setEnergy(rabbits[i].getEnergyMax());
			}
		}
		//statWorld.incNbAvgReprodPrey(rabbits[i].getNbReprod());
		//statWorld.updateNbReprodMaxPrey(rabbits[i].getNbReprod());
	}
	
	time(&end);
	printTime(start, end);
	
	for (size_t i = 0; i < rabbits.size(); ++i) {
		if (rabbits[i].getGender() == 1){
			statWorld.incSpeedAvgFemalePrey(rabbits[i].getSpeed());
		}
		else
		{
			statWorld.incSpeedAvgMalePrey(rabbits[i].getSpeed());
		}
	}


	for (size_t i = 0; i < rabbits.size(); ++i) {
		if (rabbits[i].getGender() == 1){
			FCM * temp = rabbits[i].getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
				statWorld.inc_activation_prey_female(j, (*temp->get_activations())[j]);
			}
		}
		else{
			FCM * temp = rabbits[i].getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
				statWorld.inc_activation_prey_male(j, (*temp->get_activations())[j]);
			}
		}
	}
	/*
	* 3. Dead prey removed
	*/
	cout << "  - Dead Prey Removed" << flush;
	time(&start);
	
	removeDeadPrey(); //-- removed prey which was dead by age or energy
	
	time(&end);
	printTime(start, end);

	/*
	* 4. Add new prey to rabbits
	*/
	cout << "  - New Prey Add to list" << flush;
	time(&start);

	for (size_t i = 0; i < newPrey.size(); ++i) {
		rabbits.push_back(newPrey[i]);
		//rabbits.insert(rabbits.end(), newPrey[i]);
		if (newPrey[i].getGender() == 1)
			statWorld.incNbBirthFemalePrey();
		else
			statWorld.incNbBirthMalePrey();


	}
	newPrey.clear();

	time(&end);
	printTime(start, end);

	// sort the rabbits vector based on strength // M.M
	//if (generation > 100 )
	//sort(rabbits.begin(), rabbits.end());   // M.M

	/*
	* 5. Prey Rearranged
	*/
	cout << "  - Prey Rearranged" << flush;
	time(&start);

	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		(*S).preyMembers.clear();
		(*S).setNbMembers(0);
	}
	for (size_t i = 0; i < rabbits.size(); i++) {
		rabbits[i].speciesPtr->preyMembers.push_back(&rabbits[i]);
		rabbits[i].speciesPtr->incNbMembers();
	}
	for (size_t i = 0; i < rabbits.size(); ++i) {
		rabbits[i].getFCM()->prey = &rabbits[i];
	}

	//-- 1. All prey in worldCreature are cleared
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			worldPreyArray[i][j].clear();

			worldMalePreyArray[i][j].clear();
		}
	}

	//-- 2. All prey are added back to worldCreature
	//int cntr = 0;
	for (size_t i = 0; i < rabbits.size(); ++i) {
		addListCreature(*(rabbits[i].getPosition()), 0, (int)i);
		if (rabbits[i].getGender() == 0){ //MALE
			addListMaleCreature(*(rabbits[i].getPosition()), 0, i);
			//cntr++;
		}
	}

	//-- 3. Updating the Prey information
	for (size_t i = 0; i < rabbits.size(); ++i) {
		rabbits[i].setUpdated(0);
		//statWorld.incDistribAge(0, rabbits[i].getAge());
	}

	time(&end);
	printTime(start, end);


	/*
	* 6. PREDATORS UPDATE CONCEPTS
	*/
	cout << "  - Predator seeing the world" << flush;
	time(&start);

	for (size_t i = 0; i < wolves.size(); ++i) {
		wolves[i].updateNew(this);
		//wolves[i].calcWeight();
	}

	time(&end);
	printTime(start, end);


	/*
	* 7. PREDATORS ACT
	*/
	cout << "  - Predator acting" << flush;
	time(&start);

	//Reproduction based on gender /MRE/START
	//First female should select her mate,
	//reproduction will be successful depending on mate action, energy, and distance
	//(they should be in the same cell)
	femaleWolvesIndexes.clear();
	for (unsigned int i = 0; i < wolves.size(); i++) {
		wolves[i].incAge();
		cout << flush;
		if (wolves[i].getGender() == 1)//1 means Gender::Female
		if (wolves[i].get_action_offset() == FCMPredator::Reproduce)
			femaleWolvesIndexes.push_back(i);
	}

	for (unsigned int i = 0; i < femaleWolvesIndexes.size(); i++) {
		cout << flush;
		if (wolves[femaleWolvesIndexes[i]].getUpdated() == 0) {
			wolves[femaleWolvesIndexes[i]].act_driver(&newPredator, this, MatingMode);
		}
	}
	//Reproduction based on gender /MRE/END

	//for (int i = (int)wolves.size() - 1; i >= 0; i--) { //-- Meisam: Act based on reverse age
	for (unsigned int i = 0; i< wolves.size(); i++) { // Act based on strength  M.M
		if (wolves[i].getUpdated() == 0) {
			if (wolves[i].get_action_offset() == FCMPredator::Reproduce){ //this is for the males who want to reproduce. since there is no female anymore this will fail...
				wolves[i].setUpdated(1);
				wolves[i].setLastAction(FCMPredator::ReproduceFailAction);
				//RS HERE
				float newEnergyPredActed = wolves[i].calculationEnergyAction(wolves[i].getEnergy(), wolves[i].model.getNbArc(), wolves[i].getSpeed(), wolves[i].getStrength(), 0, this);  //-- Losing Energy for acting //RS males don't carry persuasion penalty
				wolves[i].setEnergy(newEnergyPredActed);
				//RS END
			}
			else{
				wolves[i].act_driver(&newPredator, this, MatingMode);
			}
		}

		//-- Meisam
		ParentPred[wolves[i].getID()].age = wolves[i].getAge();
		ParentPred[wolves[i].getID()].PregnancyDeath = wolves[i].getEnergy() > 0 ? 0 : 1;
		//-- Meisam End

		if (wolves[i].getLastAction() != 0) {
			if (wolves[i].getGender() == 0) //male
			{
				statWorld.incNbActionPredMale(wolves[i].getLastAction() - 1);
			}
			else
			{
				statWorld.incNbActionPredFemale(wolves[i].getLastAction() - 1);
			}
			if (wolves[i].getLastAction() == FCMPredator::ReproduceAction)
				statWorld.incAvgDistMatingPred(wolves[i].get_DistMating());  //-- Meisam
		}

		if (wolves[i].getEnergy() <= 0 || wolves[i].getAge() > wolves[i].getAgeMax(true)) {
			wolves[i].setLiving(false);
			if (wolves[i].getGender() == 1)
				statWorld.incAgeAvgDeadFemalePred(wolves[i].getAge());
			else
				statWorld.incAgeAvgDeadMalePred(wolves[i].getAge());

			nbPredator--;
			//-- Meisam
			DeadInd.id = wolves[i].getSpecies();
			DeadInd.id = wolves[i].getID();
			DeadInd.age = wolves[i].getAge();
			DeadInd.energy = ((wolves[i].getEnergy() > 0) ? wolves[i].getEnergy() : 0);
			DeadInd.lastAction = wolves[i].getLastAction();
			DeadInd.evolDist = wolves[i].getDistanceEvol();
			DeadInd.speed = wolves[i].getSpeed();
			DeadInd.gender = wolves[i].getGender();
			if (wolves[i].getEnergy() <= 0) {
				if (wolves[i].getKilledInFight()){ // M.M
					if (wolves[i].getGender() == 1)
						statWorld.incnbDeadFemalePredF();	  // M.M
					else
						statWorld.incnbDeadMalePredF();	  // M.M


					DeadInd.deathType = 3;		  // M.M	 killed in fight
				}
				else {
					if (wolves[i].getGender() == 1)
						statWorld.incNbDeadFemalePredE();	  // M.M
					else
						statWorld.incNbDeadMalePredE();


					DeadInd.deathType = 2;  // energy matter
				}
			}
			else {
				if (wolves[i].getGender() == 1)
					statWorld.incNbDeadFemalePredA();	  // M.M
				else
					statWorld.incNbDeadMalePredA();

				DeadInd.deathType = 1;   // age
			}

			DeadPred.push_back(DeadInd);
			//-- Meisam End
		}
		else {
			if (wolves[i].getEnergy() > wolves[i].getEnergyMax()) {
				wolves[i].setEnergy(wolves[i].getEnergyMax());
			}
		}
		//statWorld.incNbAvgReprodPred(wolves[i].getNbReprod());
		//statWorld.updateNbReprodMaxPred(wolves[i].getNbReprod());
	}

	time(&end);
	printTime(start, end);

	for (size_t i = 0; i < wolves.size(); ++i) {
		if (wolves[i].getGender() == 1)
		{
			statWorld.incSpeedAvgFemalePred(wolves[i].getSpeed());
		}
		else
		{
			statWorld.incSpeedAvgMalePred(wolves[i].getSpeed());	
		}
	}

	for (size_t i = 0; i < wolves.size(); ++i) {
		if (wolves[i].getGender() == 1){
			FCM * temp = wolves[i].getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
				statWorld.inc_activation_pred_female(j, (*temp->get_activations())[j]);
			}
		}
		else{
			FCM * temp = wolves[i].getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
				statWorld.inc_activation_pred_male(j, (*temp->get_activations())[j]);
			}
		}
	}

	/*
	* 8. New Pred Add to list
	*/
	cout << "  - New Pred Add to list" << flush;
	time(&start);

	for (size_t i = 0; i < newPredator.size(); ++i) {
		wolves.push_back(newPredator[i]);
		//wolves.insert(wolves.end(), newPredator[i]);

		if (newPredator[i].getGender() == 1)
			statWorld.incNbBirthFemalePred();
		else
			statWorld.incNbBirthMalePred();


	}
	newPredator.clear();

	for (size_t i = 0; i < wolves.size(); ++i) {
		wolves[i].getFCM()->predator = &wolves[i];
	}

	time(&end);
	printTime(start, end);

	/*
	* 9. Dead Pred & killed Prey Removed
	*/
	cout << "  - Dead Pred & killed Prey Removed" << flush;
	time(&start);

	removeDeadPred();  //-- removed Dead Predator
	removeDeadPrey();  //-- removed prey killed by Predator

	time(&end);
	printTime(start, end);

	/*
	* 9. Pred and Prey Rearranged
	*/
	cout << "  - Pred & Prey Rearranged" << flush;
	time(&start);

	sort(rabbits.begin(), rabbits.end());   // M.M
	sort(wolves.begin(), wolves.end());   // M.M
	
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		(*S).preyMembers.clear();
		(*S).setNbMembers(0);
	}
	for (size_t i = 0; i < rabbits.size(); i++) {
		rabbits[i].speciesPtr->preyMembers.push_back(&rabbits[i]);
		rabbits[i].speciesPtr->incNbMembers();
	}
	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		(*S).predMembers.clear();
		(*S).setNbMembers(0);
	}
	for (size_t i = 0; i < wolves.size(); i++) {
		wolves[i].speciesPtr->predMembers.push_back(&wolves[i]);
		wolves[i].speciesPtr->incNbMembers();
	}
	// RS HERE
	//-- 3. Updating the Prey information and Resultfile
	for (size_t i = 0; i < rabbits.size(); ++i) {
		if (rabbits[i].getGender() == 1){
			statWorld.incNbFemalePrey();
			statWorld.incNbArcAvgFemalePrey(rabbits[i].getFCM()->getNbArc());
			statWorld.incAgeFemalePrey(rabbits[i].getAge());
			statWorld.incEnergyAvgFemalePrey(rabbits[i].getEnergy());
			statWorld.incStrengthAvgFemalePrey(rabbits[i].getStrength()); // M.M
			statWorld.incStateAvgFemalePrey(rabbits[i].getStateBirth());
			statWorld.incAvgDistEvFemalePrey(rabbits[i].getDistanceEvol());
			statWorld.incPersuasionFemalePrey(rabbits[i].getPersuasion());
		}
		else{
			statWorld.incNbMalePrey();
			statWorld.incNbArcAvgMalePrey(rabbits[i].getFCM()->getNbArc());
			statWorld.incAgeMalePrey(rabbits[i].getAge());
			statWorld.incEnergyAvgMalePrey(rabbits[i].getEnergy());
			statWorld.incStrengthAvgMalePrey(rabbits[i].getStrength()); // M.M
			statWorld.incStateAvgMalePrey(rabbits[i].getStateBirth());
			statWorld.incAvgDistEvMalePrey(rabbits[i].getDistanceEvol());
			statWorld.incPersuasionMalePrey(rabbits[i].getPersuasion());
		}
	}

	//-- Updating the Prad information and Resultfile
	for (size_t i = 0; i < wolves.size(); ++i) {
		wolves[i].setUpdated(0);
		//statWorld.incDistribAge(1, wolves[i].getAge());
		if (wolves[i].getGender() == 1){
			statWorld.incNbFemalePred();
			statWorld.incNbArcAvgFemalePred(wolves[i].getFCM()->getNbArc());
			statWorld.incAgeFemalePred(wolves[i].getAge());
			statWorld.incEnergyAvgFemalePred(wolves[i].getEnergy());
			statWorld.incStrengthAvgFemalePred(wolves[i].getStrength()); // M.M
			statWorld.incStateAvgFemalePred(wolves[i].getStateBirth());
			statWorld.incAvgDistEvFemalePred(wolves[i].getDistanceEvol());
			statWorld.incPersuasionFemalePred(wolves[i].getPersuasion());
		}
		else{
			statWorld.incNbMalePred();
			statWorld.incNbArcAvgMalePred(wolves[i].getFCM()->getNbArc());
			statWorld.incAgeMalePred(wolves[i].getAge());
			statWorld.incEnergyAvgMalePred(wolves[i].getEnergy());
			statWorld.incStrengthAvgMalePred(wolves[i].getStrength()); // M.M
			statWorld.incStateAvgMalePred(wolves[i].getStateBirth());
			statWorld.incAvgDistEvMalePred(wolves[i].getDistanceEvol());
			statWorld.incPersuasionMalePred(wolves[i].getPersuasion());
		}
	}

	time(&end);
	printTime(start, end);

	cout << flush;

	/*
	* 10. World is updated
	*/
	cout << " World updating" << flush;
	time(&start);

	//-- World's list of creatures is cleared
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			worldPreyArray[i][j].clear();
			worldPredArray[i][j].clear();

			worldMalePreyArray[i][j].clear();
			worldMalePredArray[i][j].clear();

		}
	}

	//-- World's list of creatures is updated
	//cntr = 0;
	for (size_t i = 0; i < rabbits.size(); i++) {
		addListCreature(*(rabbits[i].getPosition()), 0, (int)i);
		if (rabbits[i].getGender() == 0){ //MALE
			addListMaleCreature(*(rabbits[i].getPosition()), 0, i);
			//cntr++;
		}
	}

	//cntr = 0;
	for (size_t i = 0; i < wolves.size(); i++) {
		addListCreature(*(wolves[i].getPosition()), 1, (int)i);
		if (wolves[i].getGender() == 0){ //MALE
			addListMaleCreature(*(wolves[i].getPosition()), 1, i);
			//cntr++;
		}
	}

	time(&end);
	printTime(start, end);


	/*
	* 11. Prey Speciation
	*/
	cout << "  - Prey speciation" << flush;
	time(&start);

	removeDeadSpeciesPrey();	 //-- Meisam: Changed the line of code/ Remove extinct species
	TwoMeanPreySpeciation();
	//removeDeadSpeciesPrey();	 //-- Meisam: uncommented/ Remove extinct species

	time(&end);
	printTime(start, end);

	/*
	* 12. Predator Speciation
	*/
	cout << "  - Predator speciation" << flush;
	time(&start);

	removeDeadSpeciesPred();	 //-- Meisam: Changed the line of code
	TwoMeanPredSpeciation();
	//removeDeadSpeciesPred();	 //-- Meisam: uncommented

	time(&end);
	printTime(start, end);


	/*
	* 13. UPDATE THE WORLD
	*/
	Position position;
	cout << "  - Adjusting world resources" << flush;
	time(&start);

	float pi = 3.14159265;
	if (fluctuatingResources == 1) maxModifiedGrass = ((cos((generation * pi) / (fluctuationCycle * 0.5)) + 1) * maxGrass * fluctuationAmplitudeRatio + fluctuationMinimumRatio * maxGrass);
	else if (fluctuatingResources == 0) maxModifiedGrass = maxGrass;
	//-- Adjust levels of grass and meat
	for (int i = 0; i < width; ++i) {
		position.x = i;
		for (int j = 0; j < height; ++j) {
			position.y = j;
			incLevelGrass(position);
#ifdef TWO_RESOURCES
			incLevelGrass2(position); //-- Armin
#endif
			// decLevelMeat2(position); M.M
			decLevelMeat(position, wastedMeatPerGen); // M.M new meat policy / put 300 in parameter file
			statWorld.incNbTotalGrass(getLevelGrass(position) / (width * height)); //here
#ifdef TWO_RESOURCES
			statWorld.incNbTotalGrass2(getLevelGrass2(position) / (width * height)); //-- Armin //here
#endif
			statWorld.incNbTotalMeat(getLevelMeat(position) / (width * height));
			if ((int)getListCreature(position, 0)->size() > 0) {
				statWorld.incNbPreyByCase((int)getListCreature(position, 0)->size());
				statWorld.incNbCasePrey();
			}
			if ((int)getListCreature(position, 1)->size() > 0) {
				statWorld.incNbPredByCase((int)getListCreature(position, 1)->size());
				statWorld.incNbCasePred();
			}
		}
	}

	time(&end);
	printTime(start, end);

	/*
	* 14. Update pointers
	*/
	cout << "  - Update Pointers" << flush;
	time(&start);

	for (size_t i = 0; i < rabbits.size(); ++i) {
		rabbits[i].getFCM()->prey = &rabbits[i];
	}

	for (size_t i = 0; i < wolves.size(); ++i) {
		wolves[i].getFCM()->predator = &wolves[i];
	}

	/*
	* Reassign members lists for species
	*/
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		(*S).preyMembers.clear();
	}
	for (size_t i = 0; i < rabbits.size(); i++) {
		rabbits[i].speciesPtr->preyMembers.push_back(&rabbits[i]);
	}

	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		(*S).predMembers.clear();
	}
	for (size_t i = 0; i < wolves.size(); ++i) {
		wolves[i].speciesPtr->predMembers.push_back(&wolves[i]);
	}

	time(&end);
	printTime(start, end);

	SpeciesStatistics(); //-- Meisam: Just for Results file
	snap();
}

#ifdef HDF5_COMPILE
void Ecosystem::maxSave_HDF5() {

	int gen = generation;

	cout << " Saving max state" << flush;

	time(&start);

	//-- Backup previous save in case current save fails

	if (gen > 1) {
		ostringstream oldname, newname;
		oldname << "MaxSave.h5";
		newname << "MaxSave.h5.bak";
		string sold = oldname.str();
		string snew = newname.str();
		rename(sold.c_str(), snew.c_str());
	}

	sr = new SaveResults();
	sr->CreateMaxSave("MaxSave.h5");
	sr->CreateMaxSaveGroups();

	delete sr;

	sr = new SaveResults();
	sr->OpenMaxSave("MaxSave.h5");

	//-- Generation
	Gen g = { gen, "GEN: " };
	sr->WrGen(g);


	//-- Next available IDs
	SpecExt maxSpec[1] = { { maxPreyID, "MaxPreyId", maxPredID, "MaxPredId" } };
	string tmpstr = "/G0_MaxIndvId";
	sr->WrSpecExt(maxSpec, 1, tmpstr.c_str());

	//-- Next available species IDs
	maxSpec[0].v1 = Species::numSpeciesPreyMax; maxSpec[0].st1 = "numSpeciesPreyMax";
	maxSpec[0].v2 = Species::numSpeciesPredMax; maxSpec[0].st2 = "numSpeciesPredMax";
	tmpstr = "/G1_MaxSpecNum";
	sr->WrSpecExt(maxSpec, 1, tmpstr.c_str());

	//-- Prey info
	tmpstr = "/G2_Prey";
	if (rabbits.size() > 0)
		sr->WrIndv(rabbits, tmpstr.c_str());

	//-- Pred Info
	tmpstr = "/G3_Pred";
	if (wolves.size() > 0)
		sr->WrIndv(wolves, tmpstr.c_str());
	cout << "Wr Wolves\n";

	//--World
	tmpstr = "/G4_World";
	sr->WrWorld(world, tmpstr.c_str());
	cout << "Wr World\n";

	//-- Species
	SpecExt se1[(int)speciesPrey.size()];
	int cc = 0;
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		se1[cc].v1 = (*S).getNum(); se1[cc].st1 = "Prey Species: ";
		se1[cc].v2 = (*S).getParent(); se1[cc].st2 = "Parent Species: ";
		cc++;
	}
	if (cc > 0)
		sr->WrSpecExt(se1, cc, "/G5_PreySpec");

	cc = 0;
	SpecExt se2[(int)speciesPred.size()];
	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		se2[cc].v1 = (*S).getNum(); se2[cc].st1 = "Pred Species: ";
		se2[cc].v2 = (*S).getParent(); se2[cc].st2 = "Parent Species: ";
		cc++;
	}
	if (cc > 0)
		sr->WrSpecExt(se2, cc, "/G6_PredSpec");



	if (gen > 1) {
		remove("MaxSave.h5.bak");
	}
	time(&end);
	printTime(start, end);
	delete sr;

}
#endif // HDF5
void Ecosystem::minSave() {

	long PrevRabbitsLastID, PrevWolvesLastID;						//-- Index used for MinSave
	int gen = generation;

	cout << " Saving min state" << flush;
	//	genomeFile << gen ; genomeFile << "\n";
	time(&start);

	//-- Open file for writing
	ostringstream o;
#ifdef LinuxSystem
	o << "./" << "//Min" << (int)(gen / 1000) << "//";
	mkdir(o.str().c_str(), 0777);
#endif
	o << "MinSave" << gen << ".bin";
	string s = o.str();

	ofstream stateFile;
	stateFile.open(s.c_str(), ios::out | ios::trunc | ios::binary);

	//-- For fewer write operations
	string buffer;

	//-- In the first generation the rabbits and the wolves of the previous generation does not exit, so it is set to 0
	PrevRabbitsLastID = (gen == 1) ? 0 : lastGenPreyID;
	PrevWolvesLastID = (gen == 1) ? 0 : lastGenPredID;

	//-- For next available species IDs
	stateFile << Manip.vtos(Species::numSpeciesPreyMax) << " " << Manip.vtos(Species::numSpeciesPredMax) << endl;

	//-- Extinct Species Prey
	bool extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			stateFile << "Extinct Species Prey " << Manip.vtos((*prevSp).getNum()) << " Parent " << Manip.vtos((prevSp)->getParent()) << endl;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Prey
	bool newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			stateFile << "New Species Prey " << Manip.vtos((*species).getNum()) << " Parent " << Manip.vtos((species)->getParent()) << endl;
		}
		newSpeciesflag = true;
	}


	//-- Extinct Species Pred
	extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			stateFile << "Extinct Species Pred " << Manip.vtos((*prevSp).getNum()) << " Parent " << Manip.vtos((prevSp)->getParent()) << endl;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Pred
	newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			stateFile << "New Species Pred " << Manip.vtos((*species).getNum()) << " Parent " << Manip.vtos((species)->getParent()) << endl;
		}
		newSpeciesflag = true;
	}

	buffer.clear();

	//-- Prey
	stateFile << "Prey: " << Manip.vtos((int)rabbits.size()) << endl;

	//-- Details of already created preys
	size_t i;									//-- index of previous individuals
	for (i = 0; i < rabbits.size(); i++) {

		if (rabbits[i].getID() <= PrevRabbitsLastID) {
			stateFile << Manip.vtos(rabbits[i].getID()) << endl; //-- Prey number

			//-- Details
			//EnergyMax, AgeMax, Vision, StateBirth, SpeedMax, Age and also prey's chart won't change after the initial creation
			buffer += Manip.vtos(rabbits[i].getPosition()->x) + " " + Manip.vtos(rabbits[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(rabbits[i].getEnergy()) + "\n";
			buffer += Manip.vtos(rabbits[i].getAge()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeed()) + "\n";
			buffer += Manip.vtos(rabbits[i].getDistanceEvol()) + "\n";
			buffer += Manip.vtos(rabbits[i].getLastAction()) + "\n";

			if (rabbits[i].getLastAction() == FCMPrey::ReproduceFailAction) //-- Meisam: Reproduction failed
			{
				buffer += Manip.vtos(rabbits[i].reproduceFailed1) + "\n";
				buffer += Manip.vtos(rabbits[i].reproduceFailed2) + "\n";
				buffer += Manip.vtos(rabbits[i].reproduceFailed3) + "\n";
				buffer += Manip.vtos(rabbits[i].reproduceFailed4) + "\n";
				buffer += Manip.vtos(rabbits[i].reproduceFailed5) + "\n";
				//buffer += Manip.vtos(rabbits[i].reproduceFailed6) + "\n"; //MRE
			}

			buffer += Manip.vtos(rabbits[i].getSpecies()) + " ";
			for (list<long>::iterator it = preyChangedSp.begin(); it != preyChangedSp.end(); ++it) {
				if ((*it) == rabbits[i].getID()) {
					buffer += "T";
				}
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();

			//-- Concepts
			buffer = "";
			for (int j = 0; j < (int)rabbits[i].getFCM()->get_rows(); j++) {
				buffer += Manip.vtos(rabbits[i].getFCM()->get_activation(j)) + " ";
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}

	//-- NEWLY ADDED
	stateFile << "NW PY:" << endl;
	//		genomeFile << "Prey : \n";
	//-- Details for newly created preys
	for (i = 0; i < rabbits.size(); i++) {
		if (rabbits[i].getID() > PrevRabbitsLastID) {
			stateFile << Manip.vtos(rabbits[i].getID()) << endl; //-- Prey number

			//-- Details
			buffer += Manip.vtos(rabbits[i].getPosition()->x) + " " + Manip.vtos(rabbits[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(rabbits[i].getEnergy()) + "\n";
			buffer += Manip.vtos(rabbits[i].getEnergyMax()) + "\n";
			buffer += Manip.vtos(rabbits[i].getAge()) + "\n";
			buffer += Manip.vtos(rabbits[i].getAgeMax(true)) + "\n";
			buffer += Manip.vtos(rabbits[i].getStateBirth()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeed()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeedMax()) + "\n";
			buffer += Manip.vtos(rabbits[i].getDistanceEvol()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpecies()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeciesParent1ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeciesParent2ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getParent1ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getParent2ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getGender()) + "\n"; //Add Gender /MRE/
			stateFile << buffer;
			buffer.clear();

			//-- Chart
			buffer = "";
			for (size_t j = 0; j < rabbits[i].getFCM()->get_rows(); j++) {
				for (size_t k = 0; k < rabbits[i].getFCM()->get_cols(); k++) {
					buffer += Manip.vtos(rabbits[i].getFCM()->getchar_chart(j, k)) + " ";  // M.M
				}
				buffer += "\n";
			}

			//-- Physical genome /M.M
			PhysicalGenome* genome_temp = rabbits[i].getPhGenome();       // M.M
			for (size_t j = 0; j < Prey::nbGenes; j++) {					//M.M
				buffer += Manip.vtos((*genome_temp)[j]) + " ";           // M.M
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}

	//preyChangedSp.clear();  //-- Meisam: Transfered to UpdateEco()

	//-- Meisam: Dead Prey
	stateFile << "\n" << "Dead Prey: " << Manip.vtos(int(DeadPrey.size())) << endl;

	for (i = 0; i < DeadPrey.size(); i++) {
		buffer += Manip.vtos(DeadPrey[i].species) + "\n";
		buffer += Manip.vtos(DeadPrey[i].id) + "\n";
		buffer += Manip.vtos(DeadPrey[i].deathType) + "\n";
		buffer += Manip.vtos(DeadPrey[i].lastAction) + "\n";
		buffer += Manip.vtos(DeadPrey[i].age) + "\n";
		buffer += Manip.vtos(DeadPrey[i].energy) + "\n";
		buffer += Manip.vtos(DeadPrey[i].evolDist) + "\n";
		buffer += Manip.vtos(DeadPrey[i].speed) + "\n";
		stateFile << buffer;
		buffer.clear();
	}

	//-- Pred
	stateFile << "\n" << "Pred: " << Manip.vtos((int)wolves.size()) << endl;

	//-- Details for already created predators
	for (i = 0; i < wolves.size(); i++) {
		if (wolves[i].getID() <= PrevWolvesLastID){
			stateFile << Manip.vtos(wolves[i].getID()) << endl; //-- Pred number

			//-- Details
			//EnergyMax, AgeMax, StateBirth, Vision, SpeedMax, Age and also predator's chart won't change after the initial creation
			buffer += Manip.vtos(wolves[i].getPosition()->x) + " " + Manip.vtos(wolves[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(wolves[i].getEnergy()) + "\n";
			buffer += Manip.vtos(wolves[i].getAge()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeed()) + "\n";
			buffer += Manip.vtos(wolves[i].getDistanceEvol()) + "\n";
			buffer += Manip.vtos(wolves[i].getLastAction()) + "\n";
			if (wolves[i].getLastAction() == FCMPredator::ReproduceFailAction) //-- Reproduction failed
			{
				buffer += Manip.vtos(wolves[i].reproduceFailed1) + "\n";
				buffer += Manip.vtos(wolves[i].reproduceFailed2) + "\n";
				buffer += Manip.vtos(wolves[i].reproduceFailed3) + "\n";
				buffer += Manip.vtos(wolves[i].reproduceFailed4) + "\n";
				buffer += Manip.vtos(wolves[i].reproduceFailed5) + "\n";
				//buffer += Manip.vtos(wolves[i].reproduceFailed6) + "\n";//Reproduction based on gender /MRE/
			}

			buffer += Manip.vtos(wolves[i].getSpecies()) + " ";
			for (list<long>::iterator it = predChangedSp.begin(); it != predChangedSp.end(); ++it) {
				if ((*it) == wolves[i].getID()) {
					buffer += "T";
				}
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();

			//-- Concepts
			buffer = "";
			for (int j = 0; j < (int)wolves[i].getFCM()->get_rows(); j++) {
				buffer += Manip.vtos(wolves[i].getFCM()->get_activation(j)) + " ";
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}


	//-- Newly Added Pred
	stateFile << "NW PD:" << endl;

	//-- Details for newly created predators
	//	genomeFile << "Pred : \n" ;  // => to test the genome.
	for (i = 0; i < wolves.size(); i++) {
		if (wolves[i].getID() > PrevWolvesLastID){
			stateFile << Manip.vtos(wolves[i].getID()) << endl; //-- Pred number

			//-- Details
			buffer += Manip.vtos(wolves[i].getPosition()->x) + " " + Manip.vtos(wolves[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(wolves[i].getEnergy()) + "\n";
			buffer += Manip.vtos(wolves[i].getEnergyMax()) + "\n";
			buffer += Manip.vtos(wolves[i].getAge()) + "\n";
			buffer += Manip.vtos(wolves[i].getAgeMax(true)) + "\n";
			buffer += Manip.vtos(wolves[i].getStateBirth()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeed()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeedMax()) + "\n";
			buffer += Manip.vtos(wolves[i].getDistanceEvol()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpecies()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeciesParent1ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeciesParent2ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getParent1ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getParent2ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getGender()) + "\n"; //Add Gender /MRE/
			stateFile << buffer;
			buffer.clear();

			//-- Chart
			buffer = "";
			for (size_t j = 0; j < wolves[i].getFCM()->get_rows(); j++) {
				for (size_t k = 0; k < wolves[i].getFCM()->get_cols(); k++) {
					buffer += Manip.vtos(wolves[i].getFCM()->getchar_chart(j, k)) + " "; // M.M
				}
				buffer += "\n";
			}

			//-- Physical genome /M.M
			PhysicalGenome* genome_temp = wolves[i].getPhGenome(); // M.M
			for (size_t j = 0; j < Predator::nbGenes; j++){		 //M.M
				buffer += Manip.vtos((*genome_temp)[j]) + " ";    // M.M
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}
	//predChangedSp.clear();  //-- Meisam: Transfered to UpdateEco()

	//-- Meisam: Dead Pred
	stateFile << "\n" << "Dead Pred: " << Manip.vtos(int(DeadPred.size())) << endl;

	for (i = 0; i < DeadPred.size(); i++) {
		buffer += Manip.vtos(DeadPred[i].species) + "\n";
		buffer += Manip.vtos(DeadPred[i].id) + "\n";
		buffer += Manip.vtos(DeadPred[i].deathType) + "\n";
		buffer += Manip.vtos(DeadPred[i].lastAction) + "\n";
		buffer += Manip.vtos(DeadPred[i].age) + "\n";
		buffer += Manip.vtos(DeadPred[i].energy) + "\n";
		buffer += Manip.vtos(DeadPred[i].evolDist) + "\n";
		buffer += Manip.vtos(DeadPred[i].speed) + "\n";
		stateFile << buffer;
		buffer.clear();
	}


	//prevSpeciesPred = speciesPred;  //-- Meisam: removed
	//prevSpeciesPrey = speciesPrey;  //-- Meisam: removed

	stateFile.close();

	//-- Zip the file
	ostringstream syscom;            //-- add syscom variable to store gzip system command
	syscom << "gzip -f " << o.str();    //-- build gzip command, o.str contains file name
	string stsyscom = syscom.str();  //-- generate string
	system(stsyscom.c_str());        //-- carry out the gzip operation, right after closing the file

	time(&end);
	printTime(start, end);
}

void Ecosystem::maxSave() {

	int gen = generation;

	cout << " Saving max state" << flush;

	time(&start);

	//-- Backup previous save in case current save fails
	if (gen > 1) {
		ostringstream oldname, newname;
		oldname << "MaxSave.txt";
		newname << "MaxSave.txt.bak";
		string sold = oldname.str();
		string snew = newname.str();
		rename(sold.c_str(), snew.c_str());
	}

	//-- Open file for writing
	ostringstream o;
	o << "MaxSave.txt";
	string s = o.str();
	ofstream stateFile;
	stateFile.open(s.c_str(), ios::out | ios::trunc | ios::binary);

	//-- For fewer write operations
	string buffer;

	FCM * fTemp;

	//-- Title
	stateFile << "GEN: " << Manip.vtos(gen) << endl;

	//-- Next available IDs
	stateFile << Manip.vtos(maxPreyID) << " " << Manip.vtos(maxPredID) << endl;

	//-- Next available species IDs
	stateFile << Manip.vtos(Species::numSpeciesPreyMax) << " " << Manip.vtos(Species::numSpeciesPredMax) << "\n" << endl;

	//-- Prey
	stateFile << "Prey: " << Manip.vtos((int)rabbits.size()) << endl;
	for (size_t i = 0; i < rabbits.size(); i++) {

		//-- Prey number
		stateFile << "\n" << Manip.vtos(rabbits[i].getID()) << endl;

		//-- Details
		buffer += "1: " + Manip.vtos(rabbits[i].getPosition()->x) + " " + Manip.vtos(rabbits[i].getPosition()->y) + "\n";
		buffer += "2: " + Manip.vtos(rabbits[i].getEnergy()) + "\n";
		buffer += "3: " + Manip.vtos(rabbits[i].getEnergyMax()) + "\n";
		buffer += "4: " + Manip.vtos(rabbits[i].getAge()) + "\n";
		buffer += "5: " + Manip.vtos(rabbits[i].getAgeMax(false)) + "\n";
		buffer += "6: " + Manip.vtos(rabbits[i].getStateBirth()) + "\n";
		buffer += "7: " + Manip.vtos(rabbits[i].getSpeed()) + "\n";
		buffer += "8: " + Manip.vtos(rabbits[i].getDistanceEvol()) + "\n";
		buffer += "9: " + Manip.vtos(rabbits[i].getLastAction()) + "\n";
		buffer += "10: " + Manip.vtos(rabbits[i].get_action_offset()) + "\n";
		buffer += "11: " + Manip.vtos(rabbits[i].getSpecies()) + "\n";
		buffer += "12: " + Manip.vtos(rabbits[i].getSpeedMax()) + "\n";
		buffer += "13: " + Manip.vtos(rabbits[i].getVision()) + "\n";
		buffer += "14: " + Manip.vtos(rabbits[i].getSpeciesParent1ID()) + "\n";
		buffer += "15: " + Manip.vtos(rabbits[i].getSpeciesParent2ID()) + "\n";
		buffer += "16: " + Manip.vtos(rabbits[i].getParent1ID()) + "\n";
		buffer += "17: " + Manip.vtos(rabbits[i].getParent2ID()) + "\n";
		buffer += "18: " + Manip.vtos(rabbits[i].getGender()) + "\n"; //Add Gender /MRE/
		buffer += "19: " + Manip.vtos((short)rabbits[i].getPersuasionChar()) + "\n";
		stateFile << buffer;
		buffer.clear();

		//-- Concepts and old Concepts
		buffer.clear();
		stateFile << "" << endl;
		fTemp = rabbits[i].getFCM();
		for (size_t j = 0; j < fTemp->get_rows(); j++) {
			buffer += Manip.vtos(fTemp->get_activation(j)) + " ";
		}

		buffer += "\n" + Manip.vtos(fTemp->ConceptIndex) + " " + Manip.vtos(fTemp->ConceptMultiplier) + "\n"; //-- Meisam

		stateFile << buffer;
		buffer.clear();

		//-- Chart
		buffer.clear();
		stateFile << "" << endl;
		for (size_t j = 0; j < fTemp->get_rows(); j++) {
			for (size_t k = 0; k < fTemp->get_cols(); k++) {
				buffer += Manip.vtos(fTemp->getchar_chart(j, k)) + " ";  // M.M modified
			}
			buffer += "\n";
		}
		//-- Physical Genome
		for (size_t j = 0; j < Prey::nbGenes; j++)					      //M.M
			buffer += Manip.vtos(rabbits[i].getPhGenome()->getGene(j)) + " "; // M.M  real values of genes
		buffer += "\n";
		stateFile << buffer;  // M.M
		buffer.clear();		// M.M
	}

	//-- Pred
	stateFile << "\n" << "Pred: " << Manip.vtos((int)wolves.size()) << endl;
	for (size_t i = 0; i < wolves.size(); i++) {

		//-- Pred number
		stateFile << "\n" << Manip.vtos(wolves[i].getID()) << endl;

		//-- Details
		buffer += "1: " + Manip.vtos(wolves[i].getPosition()->x) + " " + Manip.vtos(wolves[i].getPosition()->y) + "\n";
		buffer += "2: " + Manip.vtos(wolves[i].getEnergy()) + "\n";
		buffer += "3: " + Manip.vtos(wolves[i].getEnergyMax()) + "\n";
		buffer += "4: " + Manip.vtos(wolves[i].getAge()) + "\n";
		buffer += "5: " + Manip.vtos(wolves[i].getAgeMax(false)) + "\n";
		buffer += "6: " + Manip.vtos(wolves[i].getStateBirth()) + "\n";
		buffer += "7: " + Manip.vtos(wolves[i].getSpeed()) + "\n";
		buffer += "8: " + Manip.vtos(wolves[i].getDistanceEvol()) + "\n";
		buffer += "9: " + Manip.vtos(wolves[i].getLastAction()) + "\n";
		buffer += "10: " + Manip.vtos(wolves[i].get_action_offset()) + "\n";
		buffer += "11: " + Manip.vtos(wolves[i].getSpecies()) + "\n";
		buffer += "12: " + Manip.vtos(wolves[i].getSpeedMax()) + "\n";
		buffer += "13: " + Manip.vtos(wolves[i].getVision()) + "\n";
		buffer += "14: " + Manip.vtos(wolves[i].getSpeciesParent1ID()) + "\n";
		buffer += "15: " + Manip.vtos(wolves[i].getSpeciesParent2ID()) + "\n";
		buffer += "16: " + Manip.vtos(wolves[i].getParent1ID()) + "\n";
		buffer += "17: " + Manip.vtos(wolves[i].getParent2ID()) + "\n";
		buffer += "18: " + Manip.vtos(wolves[i].getGender()) + "\n";//Add Gender /MRE/
		buffer += "19: " + Manip.vtos((short)wolves[i].getPersuasionChar()) + "\n";
		stateFile << buffer;
		buffer.clear();

		//-- Concepts and concepts Old
		stateFile << "" << endl;
		fTemp = wolves[i].getFCM();
		buffer.clear();
		for (size_t j = 0; j < fTemp->get_rows(); j++) {
			buffer += Manip.vtos(fTemp->get_activation(j)) + " ";
		}

		buffer += "\n" + Manip.vtos(fTemp->ConceptIndex) + " " + Manip.vtos(fTemp->ConceptMultiplier) + "\n"; //-- Meisam
		stateFile << buffer;
		buffer.clear();

		//-- Chart
		stateFile << "" << endl;
		buffer.clear();
		for (size_t j = 0; j < fTemp->get_rows(); j++) {
			for (size_t k = 0; k < fTemp->get_cols(); k++) {
				buffer += Manip.vtos(fTemp->getchar_chart(j, k)) + " ";  // M.M modified
			}
			buffer += "\n";
		}
		//-- genome
		for (size_t j = 0; j < Predator::nbGenes; j++)			//M.M
			buffer += Manip.vtos(wolves[i].getPhGenome()->getGene(j)) + " ";
		buffer += "\n";
		stateFile << buffer;  // M.M
		buffer.clear();		// M.M
	}

	//-- World
	buffer.clear();
	stateFile << "" << endl;
	for (size_t i = 0; i < world.size(); i++) {
		for (size_t j = 0; j < world[i].size(); j++) {
			buffer += Manip.vtos(world[i][j][0]);
			for (size_t k = 1; k < world[i][j].size(); k++) {
				buffer += "," + Manip.vtos(world[i][j][k]);
			}
			buffer += " ";
		}
		stateFile << buffer << endl;
		buffer.clear();
	}

	//-- Species
	buffer.clear();
	stateFile << "\n" << "PreySpec: " << Manip.vtos((int)speciesPrey.size()) << endl;
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		stateFile << Manip.vtos((*S).getNum()) << " " << Manip.vtos((*S).getParent()) << endl;
	}
	buffer.clear();
	stateFile << "\n" << "PredSpec: " << (int)speciesPred.size() << endl;
	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		stateFile << Manip.vtos((*S).getNum()) << " " << Manip.vtos((*S).getParent()) << endl;
	}

	stateFile.close();

	//-- Save is complete -> erase the MaxSave backup
	if (gen > 1) {
		remove("MaxSave.txt.bak");
	}

	time(&end);
	printTime(start, end);

}


void Ecosystem::SpeciesStatistics() {
	//-- Meisam: This function for calculation Extinction Rati + Species Ratio

	int nbNewSpeciesPrey = 0, nbExtinctSpeciesPrey = 0;
	int nbNewSpeciesPred = 0, nbExtinctSpeciesPred = 0;

	//-- Extinct Species Prey
	bool extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			nbExtinctSpeciesPrey++;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Prey
	bool newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			nbNewSpeciesPrey++;
		}
		newSpeciesflag = true;
	}


	//-- Extinct Species Pred
	extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			nbExtinctSpeciesPred++;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Pred
	newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			nbNewSpeciesPred++;
		}
		newSpeciesflag = true;
	}

	if (generation == 1){
		statWorld.setSpeciesRatioPrey(0);
		statWorld.setSpeciesRatioPred(0);
		statWorld.setExtinctionRatioPrey(0);
		statWorld.setExtinctionRatioPred(0);
	}
	else{
		statWorld.setSpeciesRatioPrey((float)nbNewSpeciesPrey / (float)(speciesPrey.size() - nbNewSpeciesPrey + nbExtinctSpeciesPrey));
		statWorld.setSpeciesRatioPred((float)nbNewSpeciesPred / (float)(speciesPred.size() - nbNewSpeciesPred + nbExtinctSpeciesPred));
		statWorld.setExtinctionRatioPrey((float)nbExtinctSpeciesPrey / (float)(speciesPrey.size() - nbNewSpeciesPrey + nbExtinctSpeciesPrey));
		statWorld.setExtinctionRatioPred((float)nbExtinctSpeciesPred / (float)(speciesPred.size() - nbNewSpeciesPred + nbExtinctSpeciesPred));
	}
}

void Ecosystem::doTarLastSpecificFolder(int type)
{
	bool DEBUG_TAR = true;
	string tar_command;
	string tar_Dir;
	string rmdir_command;
	string tar_file_name;
	int gen = generation;
	ostringstream otar;
	switch (type)
	{
	case 1:
		otar << " Min" << (int)(((gen / 1000) - 1));
		break;
	case 2:
		otar << " Min_C" << (int)(((gen / 1000) - 1));
		break;
	case 3:
		otar << " World" << (int)(((gen / 1000) - 1));
		break;
	}
	tar_Dir = otar.str();
	if (DEBUG_TAR) cout << "Ready to tar Min directory " << tar_Dir << endl;
	tar_file_name = tar_Dir;
	tar_file_name.append(".tar ");

	if (DEBUG_TAR)
		tar_command = "tar  -cvf ";
	else
		tar_command = "tar  -cf ";

	tar_command.append(tar_file_name);
	tar_command.append(tar_Dir);

	char *cmd = (char*)tar_command.c_str();

	if (DEBUG_TAR) cout << "Executing tar_command = " << tar_command << endl;
	//system(cmd);
	int ret; // M.M
	if (!(ret = system(cmd))) {

		rmdir_command = "rm -rf ";
		rmdir_command.append(tar_Dir);
		cmd = (char*)rmdir_command.c_str();
		system(cmd);
	}
	else
		cout << "tar command failed!!! return code = " << ret;
}

//-- Morteza, Abbas => Do tar the last folder of MinSave and World files
void Ecosystem::doTarLastFolder()
{
	// Do a tar gz command of previous directory after starting new directory

	int gen = generation;

	//if (gen > minSaveFlag && gen  % fpd == minSaveFlag) {

	if (gen % 1000 == 1){

		if (minSaveFlag == 1)
			doTarLastSpecificFolder(1);  // MinSave
		else if (minSaveCompressedFlag == 1)
			doTarLastSpecificFolder(2);  // Compressed MinSave

		if (worldSaveFlag == 1)
			doTarLastSpecificFolder(3);  // World
	}
	// done with tar file
}

#ifdef HDF5_COMPILE
void Ecosystem::minSave_Compressed_HDF5() {

	long PrevRabbitsLastID, PrevWolvesLastID;						//-- Index used for MinSave

	int gen = generation;

	cout << " Saving min state" << flush;

	char fname[20];
	sprintf(fname, "MinWorld_C%i.h5", (gen - 1) / maxSaveFlag);

	int newPreyCount = 0;
	int newPredCount = 0;

	for (size_t i = 0; i < rabbits.size(); ++i) {
		if (rabbits[i].getID() > lastGenPreyID)
			newPreyCount++;

	}

	for (size_t i = 0; i < wolves.size(); ++i) {
		if (wolves[i].getID() > lastGenPredID)
			newPredCount++;
	}

	time(&start);

	sr = new SaveResults(fname);  // an object to write MinSave based on HDF5

	sr->CreateCompressedMinSaveGroups(gen); // form the structure of the MinSave
	delete sr;
	sr = new SaveResults(fname);

	ostringstream ss;
	ss << gen;
	string root = "/G_Gen" + ss.str();


	//-- In the first generation the rabbits and the wolves of the previous generation does not exit, so it is set to 0
	PrevRabbitsLastID = (gen == 1) ? 0 : lastGenPreyID;
	PrevWolvesLastID = (gen == 1) ? 0 : lastGenPredID;

	//-- For next available species IDs
	SpecExt maxSpec[1] = { { Species::numSpeciesPreyMax, "numSpeciesPreyMax", Species::numSpeciesPredMax, "numSpeciesPredMax" } }; // M.M
	string tmpstr = root + "/G0_MaxSpecNum";
	sr->WrSpecExt(maxSpec, 1, tmpstr.c_str()); //M.M


	//-- Extinct Species Prey
	bool extinctSpeciesflag = true;
	SpecExt extSpec[max(prevSpeciesPrey.size(), prevSpeciesPred.size())];
	int cc = 0;
	for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			extSpec[cc].v1 = (*prevSp).getNum();      extSpec[cc].st1 = "Extinct Species Prey ";
			extSpec[cc].v2 = (prevSp)->getParent();   extSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		extinctSpeciesflag = true;
	}
	tmpstr = root + "/G1_ExtPreySpec";
	if (cc > 0)
		sr->WrSpecExt(extSpec, cc, tmpstr.c_str()); //M.M


	//-- New Species Prey
	bool newSpeciesflag = true;
	SpecExt newSpec[max(speciesPrey.size(), speciesPred.size())];
	cc = 0;
	for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			newSpec[cc].v1 = (*species).getNum();      newSpec[cc].st1 = "New Species Prey ";
			newSpec[cc].v2 = (species)->getParent();   newSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		newSpeciesflag = true;
	}
	tmpstr = root + "/G2_NewPreySpec";
	if (cc > 0)
		sr->WrSpecExt(newSpec, cc, tmpstr.c_str()); //M.M


	//-- Extinct Species Pred
	extinctSpeciesflag = true;
	cc = 0;
	for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			extSpec[cc].v1 = (*prevSp).getNum();      extSpec[cc].st1 = "Extinct Species Pred ";
			extSpec[cc].v2 = (prevSp)->getParent();   extSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		extinctSpeciesflag = true;
	}

	tmpstr = root + "/G3_ExtPredSpec";
	if (cc > 0)
		sr->WrSpecExt(extSpec, cc, tmpstr.c_str()); //M.M

	//-- New Species Pred
	newSpeciesflag = true;
	cc = 0;
	for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			newSpec[cc].v1 = (*species).getNum();      newSpec[cc].st1 = "New Species Pred` ";
			newSpec[cc].v2 = (species)->getParent();   newSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		newSpeciesflag = true;
	}
	tmpstr = root + "/G4_NewPredSpec";
	if (cc > 0)
		sr->WrSpecExt(newSpec, cc, tmpstr.c_str()); //M.M


	//-- already created preys
	tmpstr = root + "/G5_CurrPrey";
	int preycount = (gen == 1) ? 0 : rabbits.size() - newPreyCount;
	sr->WrCompressedCurrIndv(rabbits, preyChangedSp, tmpstr.c_str(), preycount, PrevRabbitsLastID);

	//-- new preys
	tmpstr = root + "/G6_NewPrey";
	preycount = (gen == 1) ? rabbits.size() : newPreyCount;
	sr->WrCompressedNewIndv(rabbits, tmpstr.c_str(), preycount, PrevRabbitsLastID);


	//-- already created predators
	tmpstr = root + "/G7_CurrPred";
	int predcount = (gen == 1) ? 0 : wolves.size() - newPredCount;
	sr->WrCompressedCurrIndv(wolves, predChangedSp, tmpstr.c_str(), predcount, PrevWolvesLastID);

	//-- new predators

	tmpstr = root + "/G8_NewPred";
	predcount = (gen == 1) ? wolves.size() : newPredCount;
	sr->WrCompressedNewIndv(wolves, tmpstr.c_str(), predcount, PrevWolvesLastID);

	delete sr;
	time(&end);
	printTime(start, end);
}
#endif // HDF5

void Ecosystem::minSave_Compressed() {

	int PrevRabbitsLastID, PrevWolvesLastID;						//-- Index used for MinSave

	int gen = generation;

	cout << " Saving min state" << flush;

	time(&start);

	//-- Open file for writing
	ostringstream o;
#ifdef LinuxSystem
	o << "./" << "//Min_C" << (int)(gen / 1000) << "//";
	mkdir(o.str().c_str(), 0777);
#endif
	o << "MinSave_C" << gen << ".bin";
	string s = o.str();

	ofstream stateFile;

	stateFile.open(s.c_str(), ios::out | ios::trunc | ios::binary);


	//-- For fewer write operations
	string buffer;

	//-- In the first generation the rabbits and the wolves of the previous generation does not exit, so it is set to 0
	PrevRabbitsLastID = (gen == 1) ? 0 : lastGenPreyID;
	PrevWolvesLastID = (gen == 1) ? 0 : lastGenPredID;

	//-- For next available species IDs
	stateFile << Manip.vtos(Species::numSpeciesPreyMax) << " " << Manip.vtos(Species::numSpeciesPredMax) << endl;

	//-- Extinct Species Prey
	bool extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			stateFile << "Extinct Species Prey " << Manip.vtos((*prevSp).getNum()) << " Parent " << Manip.vtos((prevSp)->getParent()) << endl;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Prey
	bool newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			stateFile << "New Species Prey " << Manip.vtos((*species).getNum()) << " Parent " << Manip.vtos((species)->getParent()) << endl;
		}
		newSpeciesflag = true;
	}


	//-- Extinct Species Pred
	extinctSpeciesflag = true;
	for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			stateFile << "Extinct Species Pred " << Manip.vtos((*prevSp).getNum()) << " Parent " << Manip.vtos((prevSp)->getParent()) << endl;
		}
		extinctSpeciesflag = true;
	}

	//-- New Species Pred
	newSpeciesflag = true;
	for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			stateFile << "New Species Pred " << Manip.vtos((*species).getNum()) << " Parent " << Manip.vtos((species)->getParent()) << endl;
		}
		newSpeciesflag = true;
	}

	buffer.clear();


	//-- Prey
	stateFile << "Prey: " << Manip.vtos((int)rabbits.size()) << endl;

	//-- Details of already created preys
	size_t i;									//-- index of previous individuals
	for (i = 0; i < rabbits.size(); i++) {
		if (rabbits[i].getID() <= PrevRabbitsLastID){

			stateFile << Manip.vtos(rabbits[i].getID()) << endl; //-- Prey number

			buffer += Manip.vtos(rabbits[i].getPosition()->x) + " " + Manip.vtos(rabbits[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(rabbits[i].getEnergy()) + "\n";
			buffer += Manip.vtos(rabbits[i].getAge()) + "\n";

			buffer += Manip.vtos(rabbits[i].getSpecies()) + " ";
			for (list<long>::iterator it = preyChangedSp.begin(); it != preyChangedSp.end(); ++it) {
				if ((*it) == rabbits[i].getID()) {
					buffer += "T";
				}
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}


	//-- NEWLY ADDED
	stateFile << "NW PY:" << endl;

	//-- Details for newly created preys
	for (i = 0; i < rabbits.size(); i++)  {
		if (rabbits[i].getID() > PrevRabbitsLastID){

			stateFile << Manip.vtos(rabbits[i].getID()) << endl; //-- Prey number
			//-- Details
			buffer += Manip.vtos(rabbits[i].getPosition()->x) + " " + Manip.vtos(rabbits[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(rabbits[i].getEnergy()) + "\n";
			buffer += Manip.vtos(rabbits[i].getAge()) + "\n";

			buffer += Manip.vtos(rabbits[i].getSpecies()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeciesParent1ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getSpeciesParent2ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getParent1ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getParent2ID()) + "\n";
			buffer += Manip.vtos(rabbits[i].getGender()) + "\n"; //Add Gender /MRE/
			stateFile << buffer;
			buffer.clear();
		}
	}

	//preyChangedSp.clear();  //-- Meisam: Transfered to UpdateEco()


	//-- Pred
	stateFile << "\n" << "Pred: " << Manip.vtos((int)wolves.size()) << endl;

	//-- Details for already created predators
	for (i = 0; i < wolves.size(); i++) {
		if (wolves[i].getID() <= PrevWolvesLastID){
			stateFile << Manip.vtos(wolves[i].getID()) << endl; //-- Pred number

			buffer += Manip.vtos(wolves[i].getPosition()->x) + " " + Manip.vtos(wolves[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(wolves[i].getEnergy()) + "\n";
			buffer += Manip.vtos(wolves[i].getAge()) + "\n";

			buffer += Manip.vtos(wolves[i].getSpecies()) + " ";
			for (list<long>::iterator it = predChangedSp.begin(); it != predChangedSp.end(); ++it) {
				if ((*it) == wolves[i].getID()) {
					buffer += "T";
				}
			}
			buffer += "\n";
			stateFile << buffer;
			buffer.clear();
		}
	}


	//-- Newly Added Pred
	stateFile << "NW PD:" << endl;

	//--  Details for newly created predators
	for (i = 0; i < wolves.size(); i++) {
		if (wolves[i].getID() > PrevWolvesLastID){

			stateFile << Manip.vtos(wolves[i].getID()) << endl; //-- Pred number
			//-- Details
			buffer += Manip.vtos(wolves[i].getPosition()->x) + " " + Manip.vtos(wolves[i].getPosition()->y) + "\n";
			buffer += Manip.vtos(wolves[i].getEnergy()) + "\n";
			buffer += Manip.vtos(wolves[i].getAge()) + "\n";

			buffer += Manip.vtos(wolves[i].getSpecies()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeciesParent1ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getSpeciesParent2ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getParent1ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getParent2ID()) + "\n";
			buffer += Manip.vtos(wolves[i].getGender()) + "\n"; //Add Gender /MRE/
			stateFile << buffer;
			buffer.clear();
		}
	}

	//predChangedSp.clear();  //-- Meisam: Transfered to UpdateEco()

	//prevSpeciesPred = speciesPred; //-- Meisam: removed
	//prevSpeciesPrey = speciesPrey; //-- Meisam: removed

	stateFile.close();

	//-- Zip the file
	ostringstream syscom;            //-- add syscom variable to store gzip system command
	syscom << "gzip -f " << o.str();    //-- build gzip command, o.str contains file name
	string stsyscom = syscom.str();  //-- generate string
	system(stsyscom.c_str());        //-- carry out the gzip operation, right after closing the file

	time(&end);
	printTime(start, end);
}

#ifdef HDF5_COMPILE
void Ecosystem::minWorld_HDF5() {

	long PrevRabbitsLastID, PrevWolvesLastID;						//-- Index used for MinSave
	int gen = generation;
	char fname[20];
	sprintf(fname, "MinWorld%i.h5", (gen - 1) / maxSaveFlag);
	int newPreyCount = 0;
	int newPredCount = 0;


	for (size_t i = 0; i < rabbits.size(); ++i) {
		if (rabbits[i].getID() > lastGenPreyID)
			newPreyCount++;
	}

	for (size_t i = 0; i < wolves.size(); ++i) {
		if (wolves[i].getID() > lastGenPredID)
			newPredCount++;
	}

	sr = new SaveResults(fname);  // an object to write MinSave based on HDF5

	sr->CreateMinSaveGroups(gen); // form the structure of the MinSave
	delete sr;
	sr = new SaveResults(fname);

	ostringstream ss;
	ss << gen;
	string root = "/G_Gen" + ss.str();

	time(&start);


	//-- In the first generation the rabbits and the wolves of the previous generation does not exit, so it is set to 0
	PrevRabbitsLastID = (gen == 1) ? 0 : lastGenPreyID;
	PrevWolvesLastID = (gen == 1) ? 0 : lastGenPredID;

	//-- For next available species IDs
	SpecExt maxSpec[1] = { { Species::numSpeciesPreyMax, "numSpeciesPreyMax", Species::numSpeciesPredMax, "numSpeciesPredMax" } }; // M.M
	string tmpstr = root + "/G0_MaxSpecNum";
	sr->WrSpecExt(maxSpec, 1, tmpstr.c_str()); //M.M


	//-- Extinct Species Prey
	bool extinctSpeciesflag = true;
	SpecExt extSpec[max(prevSpeciesPrey.size(), prevSpeciesPred.size())];
	int cc = 0;
	for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			extSpec[cc].v1 = (*prevSp).getNum();      extSpec[cc].st1 = "Extinct Species Prey ";
			extSpec[cc].v2 = (prevSp)->getParent();   extSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		extinctSpeciesflag = true;
	}
	tmpstr = root + "/G1_ExtPreySpec";
	if (cc > 0)
		sr->WrSpecExt(extSpec, cc, tmpstr.c_str()); //M.M

	//-- New Species Prey
	bool newSpeciesflag = true;

	SpecExt newSpec[max(speciesPrey.size(), speciesPred.size())];
	cc = 0;
	for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPrey.begin(); prevSp != prevSpeciesPrey.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			newSpec[cc].v1 = (*species).getNum();      newSpec[cc].st1 = "New Species Prey ";
			newSpec[cc].v2 = (species)->getParent();   newSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		newSpeciesflag = true;
	}
	tmpstr = root + "/G2_NewPreySpec";
	if (cc > 0)
		sr->WrSpecExt(newSpec, cc, tmpstr.c_str()); //M.M

	//-- Extinct Species Pred
	extinctSpeciesflag = true;
	cc = 0;
	for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*prevSp).getNum() == (*species).getNum()){
				extinctSpeciesflag = false;
				break;
			}
		}
		if (extinctSpeciesflag){
			extSpec[cc].v1 = (*prevSp).getNum();      extSpec[cc].st1 = "Extinct Species Pred ";
			extSpec[cc].v2 = (prevSp)->getParent();   extSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		extinctSpeciesflag = true;
	}
	tmpstr = root + "/G3_ExtPredSpec";
	if (cc > 0)
		sr->WrSpecExt(extSpec, cc, tmpstr.c_str()); //M.M

	//-- New Species Pred
	newSpeciesflag = true;
	cc = 0;
	for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
		for (list<Species>::iterator prevSp = prevSpeciesPred.begin(); prevSp != prevSpeciesPred.end(); ++prevSp) {
			if ((*species).getNum() == (*prevSp).getNum()){
				newSpeciesflag = false;
				break;
			}
		}
		if (newSpeciesflag){
			newSpec[cc].v1 = (*species).getNum();      newSpec[cc].st1 = "New Species Pred` ";
			newSpec[cc].v2 = (species)->getParent();   newSpec[cc].st2 = "Parent "; //M.M
			cc++;
		}
		newSpeciesflag = true;
	}
	tmpstr = root + "/G4_NewPredSpec";
	if (cc > 0)
		sr->WrSpecExt(newSpec, cc, tmpstr.c_str()); //M.M

	int preycount = (gen == 1) ? 0 : rabbits.size() - newPreyCount;
	//-- already created preys
	//cout << "\n gen:"<<gen <<"AllPrey:" << rabbits.size() << "  new prey:" << newPreyCount << "  PrevRabbitsLastID=" << PrevRabbitsLastID  << "preycount="<< preycount<<endl;
	tmpstr = root + "/G5_CurrPrey";
	sr->WrCurrIndv(rabbits, preyChangedSp, tmpstr.c_str(), preycount, PrevRabbitsLastID);

	//-- new prey
	preycount = (gen == 1) ? rabbits.size() : newPreyCount;
	tmpstr = root + "/G6_NewPrey";
	sr->WrNewIndv(rabbits, tmpstr.c_str(), preycount, PrevRabbitsLastID);

	//-- dead prey
	size_t i;
	DeadIndv deadIndv[DeadPrey.size()];
	for (i = 0; i < DeadPrey.size(); i++) {
		deadIndv[i].specId = DeadPrey[i].species;
		deadIndv[i].id = DeadPrey[i].id;
		deadIndv[i].deathType = DeadPrey[i].deathType;
		deadIndv[i].lastAction = DeadPrey[i].lastAction;
		deadIndv[i].age = DeadPrey[i].age;
		deadIndv[i].distEvol = DeadPrey[i].evolDist;
		deadIndv[i].energy = DeadPrey[i].energy;
		deadIndv[i].speed = DeadPrey[i].speed;
	}
	if (i>0){
		tmpstr = root + "/G7_DeadPrey";
		sr->WrDeadIndv(deadIndv, DeadPrey.size(), tmpstr.c_str());
	}

	//-- Pred

	//-- already created predators
	int predcount = (gen == 1) ? 0 : wolves.size() - newPredCount;
	tmpstr = root + "/G8_CurrPred";
	sr->WrCurrIndv(wolves, predChangedSp, tmpstr.c_str(), predcount, PrevWolvesLastID);

	//-- Newly Added Pred
	predcount = (gen == 1) ? wolves.size() : newPredCount;
	tmpstr = root + "/G9_NewPred";
	sr->WrNewIndv(wolves, tmpstr.c_str(), predcount, PrevWolvesLastID);

	//-- Dead Pred

	DeadIndv deadIndvPred[DeadPred.size()];
	for (i = 0; i < DeadPred.size(); i++) {
		deadIndvPred[i].specId = DeadPred[i].species;
		deadIndvPred[i].id = DeadPred[i].id;
		deadIndvPred[i].deathType = DeadPred[i].deathType;
		deadIndvPred[i].lastAction = DeadPred[i].lastAction;
		deadIndvPred[i].age = DeadPred[i].age;
		deadIndvPred[i].distEvol = DeadPred[i].evolDist;
		deadIndvPred[i].energy = DeadPred[i].energy;
		deadIndvPred[i].speed = DeadPred[i].speed;
	}
	if (i > 0){
		tmpstr = root + "/G10_DeadPred";
		sr->WrDeadIndv(deadIndvPred, DeadPred.size(), tmpstr.c_str());
	}

	//-- world
	tmpstr = root + "/G11_World";
	sr->WrWorld(world, tmpstr.c_str());

	delete sr;

	time(&end);
	printTime(start, end);
}
#endif // HDF5

void Ecosystem::worldSave() {

	int gen = generation;

	cout << " Saving world vector" << flush;

	time(&start);

	//-- Open file for writing
	ostringstream o;
#ifdef LinuxSystem
	o << "./" << "//World" << (int)(gen / 1000) << "//";
	mkdir(o.str().c_str(), 0777);
#endif
	o << "World" << gen << ".bin";
	string s = o.str();

	ofstream stateFile;

	stateFile.open(s.c_str(), ios::out | ios::trunc | ios::binary);

	//-- For fewer write operations
	string buffer;

	buffer = "";
	for (size_t i = 0; i < world.size(); i++) {
		for (size_t j = 0; j < world[i].size(); j++) {
			buffer += Manip.vtos(world[i][j][0]);
			for (size_t k = 1; k < world[i][j].size(); k++) {
				buffer += "," + Manip.vtos(world[i][j][k]);
			}
			buffer += " ";
		}
		stateFile << buffer << endl;
		buffer = "";
	}

	stateFile.close();

	//-- Zip the file
	ostringstream syscom;            //-- add syscom variable to store gzip system command
	syscom << "gzip -f " << o.str();    //-- build gzip command, o.str contains file name
	string stsyscom = syscom.str();  //-- generate string
	system(stsyscom.c_str());        //-- carry out the gzip operation, right after closing the file

	time(&end);
	printTime(start, end);

}

void Ecosystem::restoreState() {

	string entreInfo;
	vector<string> words;

	//-- Backup previous MaxSave in exist means, new MaxSave has problem
	ifstream ifile("MaxSave.txt.bak");
	if (bool(ifile)) {
		//-- new MaxSave has problem then used previous one
		ifile.close();
		remove("MaxSave.txt");
		char oldname[] = "MaxSave.txt.bak";
		char newname[] = "MaxSave.txt";
		rename(oldname, newname);
	}

	string s;
	s = "MaxSave.txt";

	ifstream paramMonde(s.c_str());

	//-- Details of the individuals

	short a;		//-- Individual's age
	short aM;		//-- Max possible age of individual
	float e;		//-- Individual's energy
	float eM;		//-- Individuals' max allowed energy
	float dE;		//-- Distance of evolution
	float spd;		//-- Speed of movement
	short sM;		//-- Max speed of movement
	short v;		//-- How far the individual can see in the world
	short rA;		// -- Reproduction Age / M.M
	short lA;		//-- Last action performed
	short nA;		//-- Current action performed
	short t;		//-- Type of individual, 0 - Prey, 1 - Predator
	int spec;		//-- Spcecies to which it belongs
	float sB;		//-- State birth
	int p1sid;		//-- Parent 1 species ID
	int p2sid;		//-- Parent 2 species ID
	long p1id;		//-- Parent 1 id
	long p2id;		//-- Parent 2 id
	long pid;		//-- Individual id
	float d; 		//-- Individual defence level  /M.M
	float cd;		//-- Individual cooperative defence  /M.M
	short ConceptIndex;      //-- Meisam: effects of act on Internal Concept (index)
	float ConceptMultiplier; //-- Meisam: effects of act on Internal Concept (value)

	short genderVal;	//Add Gender /MRE/ Female=1, Male=0
	short persuasionVal;

	FCMPrey fcm_prey;
	FCMPredator fcm_pred;

	int numberOfSpecies = 0;	//-- Number of species found

	Position p;					//-- (x, y) coordinates of position in world

	bool foundSpecies = false;	//-- True if species exists

	//-- Open state file for reading

	if (!paramMonde.is_open()) {
		cout << "Error! State file not found" << endl;
		exit(-1);
	}

	//-- Get the generation number
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	generation = atoi(words[1].c_str()); words.clear();
	cout << "Restoring from GEN " << generation << endl;
	generation += 1;

	//-- Get next available IDs
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	maxPreyID = atol(words[0].c_str());
	maxPredID = atol(words[1].c_str());
	words.clear();

	//-- Get next available species IDs
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	Species::numSpeciesPreyMax = atoi(words[0].c_str());
	Species::numSpeciesPredMax = atoi(words[1].c_str());
	words.clear();

	//-- Blank line
	getline(paramMonde, entreInfo);

	//-- Get the number of prey
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	int numPrey = atoi(words[1].c_str()); words.clear();

	//-- Blank line
	getline(paramMonde, entreInfo);

	/*
	* RESTORING THE PREY
	*/

	//-- Restore the prey
	cout << " - Restoring prey." << endl;
	for (int i = 0; i < numPrey; i++) {

		//-- Prey number
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		pid = atol(words[0].c_str()); words.clear();

		//-- Details
		t = 0; //-- Type is prey
		words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Position
		p.x = atoi(words[1].c_str());
		p.y = atoi(words[2].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Energy
		e = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Energy max
		eM = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Age
		a = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Age max
		aM = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- State birth
		sB = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Speed
		spd = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Distance evol
		dE = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Last action
		lA = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Num action
		nA = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Species
		spec = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Speed max
		sM = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- Vision
		v = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- p1 spec id
		p1sid = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- p2 spec id
		p2sid = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- p1 id
		p1id = atol(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //-- p2 id
		p2id = atol(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);	//-- Gender //Add Gender /MRE/
		genderVal = atoi(words[1].c_str()); words.clear();//Add Gender /MRE/
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		persuasionVal = (short)atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); words.clear(); //-- Blank

		/*
		* Only need the first two columns
		* of concept and conceptOld
		*/

		fcm_prey = FCMPrey(NULL);

		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		for (size_t j = 0; j < fcm_prey.get_rows(); ++j) {
			fcm_prey.set_activation(j, (float)atof(words[j].c_str()));
		}

		//-- Meisam
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words); //-- Effect of Action on Activation Level
		ConceptIndex = atoi(words[0].c_str());
		ConceptMultiplier = (float)atof(words[1].c_str());
		//-- Meisam	End

		getline(paramMonde, entreInfo); //-- Blank


		for (size_t j = 0; j < fcm_prey.get_rows(); ++j) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
			for (size_t k = 0; k < fcm_prey.get_cols(); ++k) {
				//fcm_prey.set_chart(j, k, (float) atof(words[k].c_str()));
				fcm_prey.setchar_chart(j, k, (FCM_TYPE)atoi(words[k].c_str())); // M.M modified
			}
		}

		//-- restore initial genome for prey /M.M
		float prey_genome[Prey::nbGenes];  // M.M
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);  // M.M
		for (size_t j = 0; j < Prey::nbGenes; j++)   	  // M.M
			prey_genome[j] = atof(words[j].c_str());   // M.M
		short aMO = (short)(0); // ageMaxThreshold
		eM = prey_genome[0];	// M.M
		aM = (short)prey_genome[1];	// M.M
		v = (short)prey_genome[2]; 	// M.M
		sM = (short)prey_genome[3]; 	// M.M
		rA = (short)prey_genome[4]; 	// M.M
		sB = prey_genome[5];				// M.M
		d = prey_genome[6];				// M.M
		cd = prey_genome[7];			// M.M
		getline(paramMonde, entreInfo); //-- Blank

		fcm_prey.ConceptIndex = ConceptIndex;           //-- Meisam
		fcm_prey.ConceptMultiplier = ConceptMultiplier; //-- Meisam

		Prey * newPrey = new Prey(t, pid, spec, p1id, p2id, p1sid, p2sid, p, e, eM, spd, sM, a, aM, v, rA, dE, sB, &fcm_prey, genderVal, d, cd, persuasionVal);	//Add Gender /MRE/


		newPrey->setLastAction(lA);

		rabbits.push_back(*newPrey);
		rabbits.back().getFCM()->prey = &(rabbits.back());
		nbPrey++;

		delete newPrey;
	}

	/*
	* Recreate prey species
	*/

	foundSpecies = false;
	//int cntr = 0;
	for (size_t i = 0; i < rabbits.size(); i++) {
		addListCreature(*(rabbits[i].getPosition()), 0, i);
		if (rabbits[i].getGender() == 0){ //MALE
			addListMaleCreature(*(rabbits[i].getPosition()), 0, i);
			//cntr++;
		}
		rabbits[i].getFCM()->prey = &rabbits[i];
		int s = rabbits[i].getSpecies();
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*species).getNum() == s) {
				foundSpecies = true;
				(*species).preyMembers.push_back(&rabbits[i]);
				rabbits[i].speciesPtr = &(*species);
				(*species).incNbMembers();
				break;
			}
		}
		if (!foundSpecies) {
			numberOfSpecies++;
			FCMPrey s_fcm_prey;
			s_fcm_prey = FCMPrey(rabbits[i].getFCM()->get_chart(), NULL, &rabbits[i]);
			PhysicalGenome* genome_temp = rabbits[i].getPhGenome();	// M.M
			//PhysicalGenome s_genome_prey = PhysicalGenome (Prey::nbGenes, genome_temp->getGenome(), probaMutation); // M.M
			PhysicalGenome s_genome_prey = PhysicalGenome(Prey::nbGenes, genome_temp->getRealGenome()); // M.M
			Species * newPreySpecies = new Species(s, 1, &s_fcm_prey, &s_genome_prey); // M.M modified
			speciesPrey.push_back(*newPreySpecies);
			speciesPrey.back().preyMembers.clear();
			speciesPrey.back().preyMembers.push_back(&rabbits[i]);
			rabbits[i].speciesPtr = &speciesPrey.back();
			delete newPreySpecies;
		}
		foundSpecies = false;
	}

	cout << "   " << (int)rabbits.size() << " prey, " << (int)speciesPrey.size() << " species restored." << endl;

	numberOfSpecies = 0;

	/*
	* RESTORE PREDATOR
	*/

	//-- Get the number of pred
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	int numPred = atoi(words[1].c_str()); words.clear();

	//-- Blank line
	getline(paramMonde, entreInfo);

	//-- Restore the pred
	cout << " - Restoring pred." << endl;
	for (int i = 0; i < numPred; i++) {

		//-- Pred number
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		pid = atol(words[0].c_str()); words.clear();

		//-- Details
		t = 1; //-- Type is predator
		words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		p.x = atoi(words[1].c_str());
		p.y = atoi(words[2].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		e = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		eM = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		a = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		aM = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		sB = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		spd = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		dE = (float)atof(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		lA = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		nA = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		spec = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		sM = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		v = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		p1sid = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		p2sid = atoi(words[1].c_str());  words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		p1id = atol(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		p2id = atol(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words); //Add Gender /MRE/
		genderVal = atoi(words[1].c_str()); words.clear();
		getline(paramMonde, entreInfo); Manip.Tokenize(entreInfo, words);
		persuasionVal = (short)atoi(words[1].c_str()); words.clear();

		getline(paramMonde, entreInfo); words.clear(); //-- Blank

		fcm_pred = FCMPredator(NULL);
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		for (size_t j = 0; j < fcm_pred.get_rows(); ++j) {
			fcm_pred.set_activation(j, (float)atof(words[j].c_str()));
		}

		//-- Meisam
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words); //-- Effect of Action on Activation Level
		ConceptIndex = atoi(words[0].c_str());
		ConceptMultiplier = (float)atof(words[1].c_str());
		//-- Meisam	End

		getline(paramMonde, entreInfo); //-- Blank

		for (size_t j = 0; j < fcm_pred.get_rows(); ++j) {
			getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
			for (size_t k = 0; k < fcm_pred.get_cols(); ++k) {
				//fcm_pred.set_chart(j, k, (float) atof(words[k].c_str()));
				fcm_pred.setchar_chart(j, k, (FCM_TYPE)atoi(words[k].c_str()));
			}
		}

		//-- restore initial genome for pred /M.M
		float pred_genome[Predator::nbGenes];  // M.M
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);  // M.M
		for (size_t j = 0; j < Predator::nbGenes; j++)   // M.M
			pred_genome[j] = atof(words[j].c_str());   // M.M
		short aMO = (short)(0);
		eM = pred_genome[0];	// M.M
		aM = (short)pred_genome[1];	// M.M
		v = (short)pred_genome[2]; 	// M.M
		sM = (short)pred_genome[3];		// M.M
		rA = (short)pred_genome[4]; 	// M.M
		sB = pred_genome[5];		// M.M

		getline(paramMonde, entreInfo); //-- Blank

		fcm_pred.ConceptIndex = ConceptIndex;           //-- Meisam
		fcm_pred.ConceptMultiplier = ConceptMultiplier; //-- Meisam

		Predator * newPred = new Predator(t, pid, spec, p1id, p2id, p1sid, p2sid, p, e, eM, spd, sM, a, aM, v, rA, dE, sB, &fcm_pred, genderVal, persuasionVal); //Add Gender /MRE/


		newPred->setLastAction(lA);

		wolves.push_back(*newPred);
		delete newPred;
		wolves.back().getFCM()->predator = &(wolves.back());
		nbPredator++;
	}

	/*
	* Recreate pred species
	*/

	foundSpecies = false;
	//cntr = 0;
	for (size_t i = 0; i < wolves.size(); i++) {
		addListCreature(*(wolves[i].getPosition()), 1, i);
		if (wolves[i].getGender() == 0){ //MALE
			addListMaleCreature(*(wolves[i].getPosition()), 1, i);
			//cntr++;
		}
		wolves[i].getFCM()->predator = &wolves[i];
		int s = wolves[i].getSpecies();
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*species).getNum() == s) {
				foundSpecies = true;
				(*species).predMembers.push_back(&wolves[i]);
				wolves[i].speciesPtr = &(*species);
				(*species).incNbMembers();
				break;
			}
		}
		if (!foundSpecies) {
			numberOfSpecies++;
			FCMPredator s_fcm_pred = FCMPredator(wolves[i].getFCM()->get_chart(), NULL, &wolves[i]);
			PhysicalGenome* genome_temp = wolves[i].getPhGenome();	// M.M
			//PhysicalGenome s_genome_pred = PhysicalGenome (Predator::nbGenes, genome_temp->getGenome(), probaMutation); // M.M
			PhysicalGenome s_genome_pred = PhysicalGenome(Predator::nbGenes, genome_temp->getRealGenome()); // M.M
			Species * newPredSpecies = new Species(s, 1, &fcm_pred, &s_genome_pred); // M.M
			speciesPred.push_back(*newPredSpecies);
			speciesPred.back().predMembers.clear();
			speciesPred.back().predMembers.push_back(&wolves[i]);
			wolves[i].speciesPtr = &speciesPred.back();
			delete newPredSpecies;
		}
		foundSpecies = false;
	}

	cout << "   " << (int)wolves.size() << " pred, " << (int)speciesPred.size() << " species restored." << endl;

	/*
	* Restore world vector
	*/

	vector<string> values;
	for (size_t i = 0; i < world.size(); i++) {
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		for (size_t j = 0; j < world[i].size(); j++) {
			values.clear(); Manip.Tokenize(words[j], values, ",");
			for (size_t k = 0; k < values.size(); k++) {
				world[i][j][k] = (float)atof(values[k].c_str());
			}
		}
	}

	/*
	* Restore prey species' parents
	*/

	getline(paramMonde, entreInfo);
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	int numberOfPreySpecies = atoi(words[1].c_str());
	for (int nP = 1; nP <= numberOfPreySpecies; nP++) {
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		int currPreySpecID = atoi(words[0].c_str());
		for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
			if ((*S).getNum() == currPreySpecID) {
				(*S).setParent(atoi(words[1].c_str()));
				break;
			}
		}
	}

	/*
	* Restore pred species' parents
	*/

	getline(paramMonde, entreInfo);
	getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
	int numberOfPredSpecies = atoi(words[1].c_str());
	for (int nP = 1; nP <= numberOfPredSpecies; nP++) {
		getline(paramMonde, entreInfo); words.clear(); Manip.Tokenize(entreInfo, words);
		int currPredSpecID = atoi(words[0].c_str());
		for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
			if ((*S).getNum() == currPredSpecID) {
				(*S).setParent(atoi(words[1].c_str()));
				break;
			}
		}
	}

	/*
	* Recalculate maps for species
	*/
	vector< vector<float> > fcm_temp_value = vector<vector<float> >(this->fcm_prey.get_rows(), vector<float>(this->fcm_prey.get_cols()));
	//for (size_t j=0; j<fcm_temp_value.size(); j++){                      // M.M  This is a wrong place for initialization
	//    for (size_t k=0; k<fcm_temp_value[j].size(); k++)				 // M.M  It should be inside the loop and reset per species
	//        fcm_temp_value[j][k] = 0;									 // M.M
	//}

	for (list<Species>::iterator it = speciesPrey.begin(); it != speciesPrey.end(); ++it) {

		FCM * fAvg = (*it).getFCMAvg();		//-- Get current FCM
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  //Get current Genome M.M

		vector<double> genome_temp_value = vector<double>(7, 0);  // M.M
		fAvg->reset_chart();				                //-- Reset current FCM

		for (size_t j = 0; j < fcm_temp_value.size(); j++){          // M.M
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)	 // M.M
				fcm_temp_value[j][k] = 0;						 // M.M
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).preyMembers.size(); ++p) {	//-- For each member of this species
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).preyMembers[p]->getFCM()->get_chart(j, k);
				}
			}

			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).preyMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);  // sum up the real value of the genes
			}
		}
		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / (*it).getNbMembers());
			}
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			// (*genomeAvg)[j] = int((float)genome_temp_value[j]/(*it).getNbMembers() +0.5); // for rounding
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / (*it).getNbMembers()); // for rounding
		}
	}

	//-- For Predator M.M
	fcm_temp_value = vector<vector<float> >(this->fcm_pred.get_rows(), vector<float>(this->fcm_pred.get_cols()));
	vector<double> genome_temp_value = vector<double>(Predator::nbGenes, 0);  // M.M

	for (list<Species>::iterator it = speciesPred.begin(); it != speciesPred.end(); ++it) {
		FCM * fAvg = (*it).getFCMAvg();		//-- Get current FCM
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  // M.M
		fAvg->reset_chart();				//-- Reset current FCM

		for (size_t j = 0; j < fcm_temp_value.size(); j++){
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
				fcm_temp_value[j][k] = 0;
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).predMembers.size(); ++p) {	//-- For each member of this species
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).predMembers[p]->getFCM()->get_chart(j, k);
				}
			}

			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).predMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);
			}
		}
		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / (*it).getNbMembers());
			}
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			// (*genomeAvg)[j] = int((float)genome_temp_value[j]/(*it).getNbMembers() +0.5); // for rounding
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / (*it).getNbMembers()); // for rounding
		}
	}


	//-- Clearing Extra Generation from Result files

	getStat()->RemoveEnd(this);
	statSpeciesPrey->RemoveEnd(this, 0);//male
	statSpeciesPred->RemoveEnd(this, 0);//male

	statSpeciesPrey->RemoveEnd(this, 1);//female
	statSpeciesPred->RemoveEnd(this, 1);//female

	statSpeciesPrey->RemoveEnd(this, 2);//all
	statSpeciesPred->RemoveEnd(this, 2);//all
	
	RemovePhylogenyEnds();

}

void Ecosystem::RemovePhylogenyEnds(){
	int gen = generation;

	cout.clear();
	cout << "\nRemoving ends of Phylogeny files" << flush;
	time(&start);

	string Info;
	int Result = 1;


	ifstream phylogenyIn;
	ofstream phylogenyOut;

	phylogenyIn.open("PreyPhylogeny.txt");   //-- Try to open Results file
	if (!phylogenyIn.is_open()) {
		cout << "\nPhylogenyPrey file not found! No speciations yet." << endl;
	}
	else {

		phylogenyOut.open("PreyPhylogeny.txt.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
		if (!phylogenyOut.is_open()){
			phylogenyIn.close();
			cout << "\nError! New PhylogenyPrey cannot create" << endl;
			return;
		}

		while (!phylogenyIn.eof()){
			getline(phylogenyIn, Info);

			if (int(Info.find(","))>-1){
				Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
			}


			if ((gen - 1) < Result) //-- Remove after this generation
				break;
			else if (Info.size() > 0)
				phylogenyOut << Info << endl;
		}


		phylogenyIn.close();
		phylogenyOut.close();

		remove("PreyPhylogeny.txt");
		rename("PreyPhylogeny.txt.bak", "PreyPhylogeny.txt");
	}
	//preds
	
	
	phylogenyIn.open("PredPhylogeny.txt");   //-- Try to open Results file
	if (!phylogenyIn.is_open()) {
		cout << "\nPhylogenyPred file not found! No speciations yet." << endl;
	}
	else{
		phylogenyOut.open("PredPhylogeny.txt.bak", ios::out | ios::trunc | ios::binary);   //-- Try to open New Results file
		if (!phylogenyOut.is_open()){
			phylogenyIn.close();
			cout << "\nError! New PhylogenyPred cannot create" << endl;
			return;
		}

		while (!phylogenyIn.eof()){
			getline(phylogenyIn, Info);

			if (int(Info.find(","))>-1){
				Result = (int)atof(Info.substr(0, Info.find(",", 0)).c_str());
			}


			if ((gen - 1) < Result) //-- Remove after this generation
				break;
			else if (Info.size() > 0)
				phylogenyOut << Info << endl;
		}


		phylogenyIn.close();
		phylogenyOut.close();

		remove("PredPhylogeny.txt");
		rename("PredPhylogeny.txt.bak", "PredPhylogeny.txt");
		
	}

	time(&end);
	printTime(start, end);
}

#ifdef HDF5_COMPILE
void Ecosystem::restoreState_HDF5() {

	LoadResults *lr = new LoadResults("MaxSave.h5");

	//-- Details of the individuals

	short a;		//-- Individual's age
	short aM;		//-- Max possible age of individual
	float e;		//-- Individual's energy
	float eM;		//-- Individuals' max allowed energy
	float dE;		//-- Distance of evolution
	float spd;		//-- Speed of movement
	short sM;		//-- Max speed of movement
	short v;		//-- How far the individual can see in the world
	short rA;		// -- Reproduction Age / M.M
	short lA;		//-- Last action performed
	short nA;		//-- Current action performed
	short t;		//-- Type of individual, 0 - Prey, 1 - Predator
	int spec;		//-- Spcecies to which it belongs
	float sB;		//-- State birth
	int p1sid;		//-- Parent 1 species ID
	int p2sid;		//-- Parent 2 species ID
	long p1id;		//-- Parent 1 id
	long p2id;		//-- Parent 2 id
	long pid;		//-- Individual id
	float d; 		//-- Individual defence level  /M.M
	float cd;		//-- Individual cooperative defence  /M.M
	short ConceptIndex;      //-- Meisam: effects of act on Internal Concept (index)
	float ConceptMultiplier; //-- Meisam: effects of act on Internal Concept (value)

	short genderVal; // individual gender.
	float persuasionVal;

	FCMPrey fcm_prey;
	FCMPredator fcm_pred;

	int numberOfSpecies = 0;	//-- Number of species found

	Position p;					//-- (x, y) coordinates of position in world

	bool foundSpecies = false;	//-- True if species exists

	//-- Open state file for reading

	//-- Get the generation number
	Gen gn;
	lr->RdGen(gn);
	generation = gn.v1;

	cout << "Restoring from GEN " << generation << endl;
	generation += 1;

	//-- Get next available IDs
	SpecExt maxSpec[1];
	int cc;
	lr->RdSpecExt(maxSpec, "/G0_MaxIndvId", cc);
	maxPreyID = maxSpec[0].v1;
	maxPredID = maxSpec[0].v2;

	//-- Get next available species IDs
	lr->RdSpecExt(maxSpec, "/G1_MaxSpecNum", cc);
	Species::numSpeciesPreyMax = maxSpec[0].v1;
	Species::numSpeciesPredMax = maxSpec[0].v2;

	/*
	* RESTORING THE PREY
	*/
	short FCM_row;
	short FCM_col;
	short genomeSize;
	int popSize = lr->getPopInfo("/G2_Prey", FCM_row, FCM_col, genomeSize);
	Indv popArr[popSize];
	short t_FCM[popSize][FCM_row][FCM_col];
	short t_phGenome[popSize][genomeSize];
	float t_actvs[popSize][FCM_row + 2];
	cout << popSize << " " << FCM_row << " " << FCM_col; cout.flush();
	lr->RdIndv1("/G2_Prey", popArr, t_FCM, t_phGenome, t_actvs);


	//-- Restore the prey
	cout << " - Restoring prey." << endl;
	int numPrey = popSize;
	for (int i = 0; i < numPrey; i++) {

		pid = popArr[i].id;
		p.x = popArr[i].x;
		p.y = popArr[i].y;
		e = popArr[i].energy;
		a = popArr[i].age;
		aM = popArr[i].maxage;
		spd = popArr[i].speed;
		dE = popArr[i].distEvol;
		lA = popArr[i].lastAction;
		nA = popArr[i].actionOffset;
		spec = popArr[i].specId;
		p1sid = popArr[i].specP1Id;
		p2sid = popArr[i].specP2Id;
		p1id = popArr[i].parent1Id;
		p2id = popArr[i].parent2Id;
		genderVal = popArr[i].gender;
		persuasionVal = popArr[i].persuasion;

		/*
		* Only need the first two columns
		* of concept and conceptOld
		*/

		fcm_prey = FCMPrey(NULL);

		for (size_t j = 0; j < fcm_prey.get_rows(); ++j) {
			fcm_prey.set_activation(j, t_actvs[i][j]);
		}
		ConceptIndex = t_actvs[i][FCM_row];
		ConceptMultiplier = t_actvs[i][FCM_row + 1];

		for (size_t j = 0; j < fcm_prey.get_rows(); ++j) {
			for (size_t k = 0; k < fcm_prey.get_cols(); ++k) {
				fcm_prey.setchar_chart(j, k, t_FCM[i][j][k]); // M.M modified
			}
		}

		//-- restore initial genome for prey /M.M

		short aMO = (short)(aM - t_phGenome[i][1]); // ageMaxThreshold

		eM = t_phGenome[i][0];
		aM = t_phGenome[i][1];
		v = t_phGenome[i][2];
		sM = t_phGenome[i][3];
		rA = t_phGenome[i][4];
		sB = t_phGenome[i][5];
		d = t_phGenome[i][6];
		cd = t_phGenome[i][7];

		fcm_prey.ConceptIndex = ConceptIndex;           //-- Meisam
		fcm_prey.ConceptMultiplier = ConceptMultiplier; //-- Meisam

		Prey * newPrey = new Prey(t, pid, spec, p1id, p2id, p1sid, p2sid, p, e, eM, spd, sM, a, aM, aMO, v, rA, dE, sB, &fcm_prey, genderVal, d, cd, persuasionVal);

		newPrey->setLastAction(lA);

		rabbits.push_back(*newPrey);

		rabbits.back().getFCM()->prey = &(rabbits.back());

		nbPrey++;

		delete newPrey;
	}


	/*
	* Recreate prey species
	*/

	foundSpecies = false;
	//int cntr = 0;
	for (size_t i = 0; i < rabbits.size(); i++) {
		addListCreature(*(rabbits[i].getPosition()), 0, i);
		if (rabbits[i].getGender() == 0){ //MALE
			addListMaleCreature(*(rabbits[i].getPosition()), 0, i);
			//cntr++;
		}
		rabbits[i].getFCM()->prey = &rabbits[i];
		int s = rabbits[i].getSpecies();
		for (list<Species>::iterator species = speciesPrey.begin(); species != speciesPrey.end(); ++species) {
			if ((*species).getNum() == s) {
				foundSpecies = true;
				(*species).preyMembers.push_back(&rabbits[i]);
				rabbits[i].speciesPtr = &(*species);
				(*species).incNbMembers();
				break;
			}
		}
		if (!foundSpecies) {
			numberOfSpecies++;
			FCMPrey s_fcm_prey;
			s_fcm_prey = FCMPrey(rabbits[i].getFCM()->get_chart(), NULL, &rabbits[i]);
			PhysicalGenome* genome_temp = rabbits[i].getPhGenome();	// M.M
			PhysicalGenome s_genome_prey = PhysicalGenome(Prey::nbGenes, genome_temp->getRealGenome()); // M.M
			Species * newPreySpecies = new Species(s, 1, &s_fcm_prey, &s_genome_prey); // M.M modified
			speciesPrey.push_back(*newPreySpecies);
			speciesPrey.back().preyMembers.clear();
			speciesPrey.back().preyMembers.push_back(&rabbits[i]);
			rabbits[i].speciesPtr = &speciesPrey.back();
			delete newPreySpecies;
		}
		foundSpecies = false;
	}

	cout << "   " << (int)rabbits.size() << " prey, " << (int)speciesPrey.size() << " species restored." << endl;

	numberOfSpecies = 0;

	/*
	* RESTORE PREDATOR
	*/

	popSize = lr->getPopInfo("/G3_Pred", FCM_row, FCM_col, genomeSize);
	Indv popArr_pred[popSize];
	short t_FCM_pred[popSize][FCM_row][FCM_col];
	short t_phGenome_pred[popSize][genomeSize];
	float t_actvs_pred[popSize][FCM_row + 2];

	lr->RdIndv1("/G3_Pred", popArr_pred, t_FCM_pred, t_phGenome_pred, t_actvs_pred);

	//-- Restore the pred
	int numPred = popSize;
	cout << " - Restoring pred." << endl;
	for (int i = 0; i < numPred; i++) {

		pid = popArr_pred[i].id;
		t = 1; //-- Type is predator
		p.x = popArr_pred[i].x;
		p.y = popArr_pred[i].y;
		e = popArr_pred[i].energy;
		a = popArr_pred[i].age;
		aM = popArr_pred[i].maxage;
		spd = popArr_pred[i].speed;
		dE = popArr_pred[i].distEvol;
		lA = popArr_pred[i].lastAction;
		nA = popArr_pred[i].actionOffset;
		spec = popArr_pred[i].specId;
		p1sid = popArr_pred[i].specP1Id;
		p2sid = popArr_pred[i].specP2Id;
		p1id = popArr_pred[i].parent1Id;
		p2id = popArr_pred[i].parent2Id;
		genderVal = popArr_pred[i].gender;
		persuasionVal = popArr_pred[i].persuasion;
		
		fcm_pred = FCMPredator(NULL);

		for (size_t j = 0; j < fcm_pred.get_rows(); ++j) {
			fcm_pred.set_activation(j, t_actvs_pred[i][j]);
		}

		ConceptIndex = t_actvs_pred[i][FCM_row];
		ConceptMultiplier = t_actvs_pred[i][FCM_row + 1];

		for (size_t j = 0; j < fcm_pred.get_rows(); ++j) {
			for (size_t k = 0; k < fcm_pred.get_cols(); ++k) {
				fcm_pred.setchar_chart(j, k, t_FCM_pred[i][j][k]);
			}
		}

		//-- restore initial genome for pred /M.M

		short aMO = (short)(aM - t_phGenome_pred[i][1]);

		eM = t_phGenome_pred[i][0];
		aM = t_phGenome_pred[i][1];
		v = t_phGenome_pred[i][2];
		sM = t_phGenome_pred[i][3];
		rA = t_phGenome_pred[i][4];
		sB = t_phGenome_pred[i][5];

		fcm_pred.ConceptIndex = ConceptIndex;           //-- Meisam
		fcm_pred.ConceptMultiplier = ConceptMultiplier; //-- Meisam

		Predator * newPred = new Predator(t, pid, spec, p1id, p2id, p1sid, p2sid, p, e, eM, spd, sM, a, aM, v, rA, dE, sB, &fcm_pred, genderVal, persuasionVal);


		newPred->setLastAction(lA);

		wolves.push_back(*newPred);
		delete newPred;
		wolves.back().getFCM()->predator = &(wolves.back());
		nbPredator++;
	}

	/*
	* Recreate pred species
	*/

	foundSpecies = false;
	//cntr = 0;
	for (size_t i = 0; i < wolves.size(); i++) {
		addListCreature(*(wolves[i].getPosition()), 1, i);
		if (wolves[i].getGender() == 0){ //MALE
			addListMaleCreature(*(wolves[i].getPosition()), 1, i);
			//cntr++;
		}
		wolves[i].getFCM()->predator = &wolves[i];
		int s = wolves[i].getSpecies();
		for (list<Species>::iterator species = speciesPred.begin(); species != speciesPred.end(); ++species) {
			if ((*species).getNum() == s) {
				foundSpecies = true;
				(*species).predMembers.push_back(&wolves[i]);
				wolves[i].speciesPtr = &(*species);
				(*species).incNbMembers();
				break;
			}
		}
		if (!foundSpecies) {
			numberOfSpecies++;
			FCMPredator s_fcm_pred = FCMPredator(wolves[i].getFCM()->get_chart(), NULL, &wolves[i]);
			PhysicalGenome* genome_temp = wolves[i].getPhGenome();	// M.M
			PhysicalGenome s_genome_pred = PhysicalGenome(Predator::nbGenes, genome_temp->getRealGenome()); // M.M
			Species * newPredSpecies = new Species(s, 1, &fcm_pred, &s_genome_pred); // M.M
			speciesPred.push_back(*newPredSpecies);
			speciesPred.back().predMembers.clear();
			speciesPred.back().predMembers.push_back(&wolves[i]);
			wolves[i].speciesPtr = &speciesPred.back();
			delete newPredSpecies;
		}
		foundSpecies = false;
	}

	cout << "   " << (int)wolves.size() << " pred, " << (int)speciesPred.size() << " species restored." << endl;

	/*
	* Restore world vector
	*/

	lr->RdWorld(world, "G4_World");
	/*
	* Restore prey species' parents
	*/

	int numberOfPreySpecies;
	numberOfPreySpecies = lr->getSpecInfo("G5_PreySpec");
	SpecExt se1[numberOfPreySpecies];

	lr->RdSpecExt(se1, "G5_PreySpec", numberOfPreySpecies);

	for (int nP = 1; nP <= numberOfPreySpecies; nP++) {
		int currPreySpecID = se1[nP - 1].v1;
		for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
			if ((*S).getNum() == currPreySpecID) {
				(*S).setParent(se1[nP - 1].v2);
				break;
			}
		}
	}

	/*
	* Restore pred species' parents
	*/

	int numberOfPredSpecies = lr->getSpecInfo("G6_PredSpec");

	SpecExt se2[numberOfPredSpecies];
	lr->RdSpecExt(se2, "G6_PredSpec", numberOfPredSpecies);
	for (int nP = 1; nP <= numberOfPredSpecies; nP++) {
		int currPredSpecID = se2[nP - 1].v1;
		for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
			if ((*S).getNum() == currPredSpecID) {
				(*S).setParent(se2[nP - 1].v2);
				break;
			}
		}
	}

	/*
	* Recalculate maps for species
	*/
	vector< vector<float> > fcm_temp_value = vector<vector<float> >(this->fcm_prey.get_rows(), vector<float>(this->fcm_prey.get_cols()));

	for (list<Species>::iterator it = speciesPrey.begin(); it != speciesPrey.end(); ++it) {

		FCM * fAvg = (*it).getFCMAvg();		//-- Get current FCM
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  //Get current Genome M.M

		vector<double> genome_temp_value = vector<double>(7, 0);  // M.M
		fAvg->reset_chart();				                //-- Reset current FCM

		for (size_t j = 0; j < fcm_temp_value.size(); j++){          // M.M
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)	 // M.M
				fcm_temp_value[j][k] = 0;						 // M.M
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).preyMembers.size(); ++p) {	//-- For each member of this species
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).preyMembers[p]->getFCM()->get_chart(j, k);
				}
			}

			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).preyMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);  // sum up the real value of the genes
			}
		}
		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / (*it).getNbMembers());
			}
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			// (*genomeAvg)[j] = int((float)genome_temp_value[j]/(*it).getNbMembers() +0.5); // for rounding
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / (*it).getNbMembers()); // for rounding
		}
	}

	//-- For Predator M.M
	fcm_temp_value = vector<vector<float> >(this->fcm_pred.get_rows(), vector<float>(this->fcm_pred.get_cols()));
	vector<double> genome_temp_value = vector<double>(Predator::nbGenes, 0);  // M.M

	for (list<Species>::iterator it = speciesPred.begin(); it != speciesPred.end(); ++it) {
		FCM * fAvg = (*it).getFCMAvg();		//-- Get current FCM
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  // M.M
		fAvg->reset_chart();				//-- Reset current FCM

		for (size_t j = 0; j < fcm_temp_value.size(); j++){
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
				fcm_temp_value[j][k] = 0;
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).predMembers.size(); ++p) {	//-- For each member of this species
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).predMembers[p]->getFCM()->get_chart(j, k);
				}
			}

			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).predMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);
			}
		}
		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / (*it).getNbMembers());
			}
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			// (*genomeAvg)[j] = int((float)genome_temp_value[j]/(*it).getNbMembers() +0.5); // for rounding
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / (*it).getNbMembers()); // for rounding
		}
	}


	//-- Clearing Extra Generation from Result files

	getStat()->RemoveEnd(this);
	statSpeciesPrey->RemoveEnd(this, 0);//male
	statSpeciesPred->RemoveEnd(this, 0);//male

	statSpeciesPrey->RemoveEnd(this, 1);//female
	statSpeciesPred->RemoveEnd(this, 1);//female

	delete (lr);
}
#endif // HDF5

void Ecosystem::TwoMeanPreySpeciation() {

	/*
	* Calculate new center for each species
	*/

	int nbPreyMembers = 0;
	vector< vector<float> > fcm_temp_value = vector<vector<float> >(this->fcm_prey.get_rows(), vector<float>(this->fcm_prey.get_cols()));
	vector<double> genome_temp_value = vector<double>(Prey::nbGenes, 0);  // M.M
	for (list<Species>::iterator it = speciesPrey.begin(); it != speciesPrey.end(); ++it) {

		FCM * fAvg = (*it).getFCMAvg();
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  // M.M
		nbPreyMembers = 0;

		for (size_t j = 0; j < fcm_temp_value.size(); j++){
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
				fcm_temp_value[j][k] = 0;
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).preyMembers.size(); ++p) {

			nbPreyMembers++;
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).preyMembers[p]->getFCM()->get_chart(j, k);
				}
			}

			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).preyMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);


			}
		}

		(*it).setNbMembers(nbPreyMembers);

		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / nbPreyMembers);
			}
		}
		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPreyMembers +0.5); // for rounding
			//			float test = (float)genome_temp_value[j]/nbPreyMembers;
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPreyMembers); // for rounding
		}
		//cout << endl;
		//fAvg->print_chart();      // M.M
		//genomeAvg->printGenome(); // M.M
		//(*genomeAvg)[0] = 0;  for testing the reference
	}

	// for test M.M => for testing the reference
	//	list<Species>::iterator it = speciesPrey.begin();
	//	FCM * fAvg = (*it).getFCMAvg();
	//	PhysicalGenome *genomeAvg= (*it).getGenomeAvg();  // M.M
	//	float tt= genomeAvg->getGene(0);
	///////////////////  M.M
	/*
	* 2-MEAN CLUSTERING
	*/

	list<Species> newSpeciesPrey;
	bool newSpecies = false;

	float fDistance = 0;
	float cDistance = 0;

	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {

		fDistance = 0;
		cDistance = 0;

		//-- Find individual, I, with greatest distance from S.center
		for (size_t p = 0; p < (*S).preyMembers.size(); ++p) {
			cDistance = (*S).preyMembers[p]->getFCM()->calculDistanceEvol(this, (*S).getFCMAvg());
			PhysicalGenome* genome_temp = (*S).preyMembers[p]->getPhGenome(); // M.M
			//cDistance += genome_temp->calcDistance((*S).getGenomeAvg()->getGenome());  // M.M
			cDistance += genome_temp->calcDistance((*S).getGenomeAvg());  // M.M

			if (cDistance > fDistance) {
				fDistance = cDistance;
			}
		}

		//-- If fDistance > threshold, start clustering
		if (fDistance > 1.6 * distanceSpeciesPrey) {

			newSpecies = true;

			//-- Select random individual and set first center C1 = R1.FCM
			FCM * C1;
			PhysicalGenome* C1_Genome; // M.M
			Prey * R1;
			unsigned int R1ID;
			R1ID = rand.next(0, (int)(*S).preyMembers.size() - 1);
			R1 = (*S).preyMembers[R1ID];
			C1 = R1->getFCM();
			C1_Genome = R1->getPhGenome(); // M.M
			//-- Select furthest individual from R1, named R2, and set second center C2 = R2.FCM
			Prey * R2 = NULL;
			unsigned int R2ID = 0;
			FCM * C2 = NULL;
			PhysicalGenome* C2_Genome = NULL; // M.M
			float fDistance2 = 0;
			float cDistance2 = 0;
			for (size_t p = 0; p < (*S).preyMembers.size(); ++p) {
				cDistance2 = (*S).preyMembers[p]->getFCM()->calculDistanceEvol(this, C1);
				PhysicalGenome* genome_temp = (*S).preyMembers[p]->getPhGenome(); // M.M
				//cDistance2 += genome_temp->calcDistance(C1_Genome->getGenome());  // M.M
				cDistance2 += genome_temp->calcDistance(C1_Genome);  // M.M
				if (cDistance2 > fDistance2) {
					fDistance2 = cDistance2;
					R2 = (*S).preyMembers[p];
					C2 = (*S).preyMembers[p]->getFCM();
					C2_Genome = (*S).preyMembers[p]->getPhGenome(); // M.M
					R2ID = p;
				}
			}

			//-- Create two new species centered around C1 and C2;
			FCMPrey s_fcm_prey;
			s_fcm_prey = FCMPrey(C1->get_chart(), NULL, R1);
			//PhysicalGenome s_genome_prey= PhysicalGenome(Prey::nbGenes, C1_Genome->getGenome() ,probaMutation); // M.M
			PhysicalGenome s_genome_prey = PhysicalGenome(Prey::nbGenes, C1_Genome->getRealGenome()); // M.M
			Species * S1 = new Species(-1, 1, &s_fcm_prey, &s_genome_prey);

			s_fcm_prey = FCMPrey(C2->get_chart(), NULL, R2);
			//s_genome_prey= PhysicalGenome(Prey::nbGenes, C2_Genome->getGenome() ,probaMutation); // M.M
			s_genome_prey = PhysicalGenome(Prey::nbGenes, C2_Genome->getRealGenome()); // M.M
			Species * S2 = new Species(-1, 1, &s_fcm_prey, &s_genome_prey);

			S1->preyMembers.push_back(R1);
			S2->preyMembers.push_back(R2);

			//-- Try for convergence
			int repeat = 0;
			int T = 8;
			while (repeat < T) {

				//-- Group remaining individuals into one of S1 or S2
				for (size_t p = 0; p < (*S).preyMembers.size(); ++p) {
					if (p != R1ID && p != R2ID) {
						float dfs1 = (*S).preyMembers[p]->getFCM()->calculDistanceEvol(this, C1);
						PhysicalGenome* genome_temp = (*S).preyMembers[p]->getPhGenome(); // M.M
						//dfs1 += genome_temp->calcDistance(C1_Genome->getGenome());  // M.M
						dfs1 += genome_temp->calcDistance(C1_Genome);  // M.M

						float dfs2 = (*S).preyMembers[p]->getFCM()->calculDistanceEvol(this, C2);
						//dfs2 += genome_temp->calcDistance(C2_Genome->getGenome());  // M.M
						dfs2 += genome_temp->calcDistance(C2_Genome);  // M.M
						if (dfs1 < dfs2) {
							S1->preyMembers.push_back((*S).preyMembers[p]);
						}
						else {
							S2->preyMembers.push_back((*S).preyMembers[p]);
						}
					}
				}

				//-- Recalculate center of S1
				FCM * sFCM = S1->getFCMAvg();
				PhysicalGenome *genomeAvg = S1->getGenomeAvg();  // M.M
				nbPreyMembers = 0;
				for (size_t j = 0; j < fcm_temp_value.size(); j++){
					for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
						fcm_temp_value[j][k] = 0;
				}

				for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
					genome_temp_value[j] = 0;

				for (size_t p = 0; p < S1->preyMembers.size(); ++p) {
					nbPreyMembers++;
					for (size_t j = 0; j < sFCM->get_rows(); ++j) {
						for (size_t k = 0; k < sFCM->get_cols(); ++k) {
							fcm_temp_value[j][k] += S1->preyMembers[p]->getFCM()->get_chart(j, k);
						}
					}
					for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
						PhysicalGenome* genome_temp = S1->preyMembers[p]->getPhGenome();
						//genome_temp_value[j]+= (*genome_temp)[j];
						genome_temp_value[j] += genome_temp->getGene(j);
					}
				}
				S1->setNbMembers(nbPreyMembers);
				for (size_t j = 0; j < sFCM->get_rows(); ++j) {
					for (size_t k = 0; k < sFCM->get_cols(); ++k) {
						sFCM->set_chart(j, k, fcm_temp_value[j][k] / nbPreyMembers);
					}
				}
				for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
					//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPreyMembers +0.5); // for rounding
					genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPreyMembers); // for rounding
				}

				//-- Recalculate the center of S2
				sFCM = S2->getFCMAvg();
				genomeAvg = S2->getGenomeAvg();  // M.M

				nbPreyMembers = 0;
				for (size_t j = 0; j < fcm_temp_value.size(); j++){
					for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
						fcm_temp_value[j][k] = 0;
				}

				for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
					genome_temp_value[j] = 0;

				for (size_t p = 0; p < S2->preyMembers.size(); ++p) {
					nbPreyMembers++;
					for (size_t j = 0; j < sFCM->get_rows(); ++j) {
						for (size_t k = 0; k < sFCM->get_cols(); ++k) {
							fcm_temp_value[j][k] += S2->preyMembers[p]->getFCM()->get_chart(j, k);
						}
					}
					for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
						PhysicalGenome* genome_temp = S2->preyMembers[p]->getPhGenome();
						//genome_temp_value[j]+= (*genome_temp)[j];
						genome_temp_value[j] += genome_temp->getGene(j);
					}

				}
				S2->setNbMembers(nbPreyMembers);
				for (size_t j = 0; j < sFCM->get_rows(); ++j) {
					for (size_t k = 0; k < sFCM->get_cols(); ++k) {
						sFCM->set_chart(j, k, fcm_temp_value[j][k] / nbPreyMembers);
					}
				}
				for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
					//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPreyMembers +0.5); // for rounding
					genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPreyMembers); // for rounding
				}

				repeat++;
				if (repeat < T) {
					C1 = S1->getFCMAvg();
					C1_Genome = S1->getGenomeAvg();  //M.M
					C2 = S2->getFCMAvg();
					C2_Genome = S2->getGenomeAvg();   // M.M
					S1->preyMembers.clear();
					S2->preyMembers.clear();
					S1->preyMembers.push_back(R1);
					S2->preyMembers.push_back(R2);
				}

			}

			//-- Assign species IDs to new species
			float dS1 = S1->getFCMAvg()->calculDistanceEvol(this, S->getFCMAvg());
			PhysicalGenome* genome_temp = S1->getGenomeAvg(); // M.M
			//dS1 += genome_temp->calcDistance(S->getGenomeAvg()->getGenome());  // M.M
			dS1 += genome_temp->calcDistance(S->getGenomeAvg());  // M.M
			float dS2 = S2->getFCMAvg()->calculDistanceEvol(this, S->getFCMAvg());
			genome_temp = S2->getGenomeAvg(); // M.M
			//dS2 += genome_temp->calcDistance(S->getGenomeAvg()->getGenome());  // M.M
			dS2 += genome_temp->calcDistance(S->getGenomeAvg());  // M.M

			Species::numSpeciesPreyMax++;

			if (dS1 < dS2) {
				S1->setNum(S->getNum());
				S2->setNum(Species::numSpeciesPreyMax);
				S1->setParent(S->getParent());
				S2->setParent(S->getNum());
				newSpeciesPrey.push_back(*S1);
				newSpeciesPrey.push_back(*S2);

			}
			else {
				S1->setNum(Species::numSpeciesPreyMax);
				S2->setNum(S->getNum());
				S1->setParent(S->getNum());
				S2->setParent(S->getParent());
				newSpeciesPrey.push_back(*S2);
				newSpeciesPrey.push_back(*S1);

			}
			
			ofstream preyPhylogeny;
			preyPhylogeny.open ("PreyPhylogeny.txt", ios::app);
			if (S1->getNum() < S2->getNum()) preyPhylogeny << generation << "," << S1->getNum() << ",Speciation-" << S2->getNum() << "\n";
			else preyPhylogeny << generation << "," << S2->getNum() << ",Speciation-" << S1->getNum() << "\n";
			preyPhylogeny.close();
			
			cout << "\n" << "     " << (*S).getNum() << "(" << (int)(*S).preyMembers.size() << ")" << " --> " << S1->getNum() << "(" << (int)S1->preyMembers.size() << ") + " << S2->getNum() << "(" << (int)S2->preyMembers.size() << ")" << endl;
			delete S1;
			delete S2;

		}
		else {
			newSpeciesPrey.push_back(*S);
		}

	}

	speciesPrey.clear();
	speciesPrey = newSpeciesPrey;
	preyChangedSp.clear();

	//-- Update pointers and stats
	for (list<Species>::iterator S = speciesPrey.begin(); S != speciesPrey.end(); ++S) {
		for (size_t p = 0; p < (*S).preyMembers.size(); ++p) {
			if ((*S).preyMembers[p]->getSpecies() != (*S).getNum()){
				preyChangedSp.push_back((*S).preyMembers[p]->getID());
			}
			(*S).preyMembers[p]->setSpecies((*S).getNum());
			(*S).preyMembers[p]->speciesPtr = &(*S);
			/*
			(*S).preyMembers[p]->setUpdated(0);
			(*S).preyMembers[p]->incAge();
			//statWorld.incDistribAge(0, (*S).preyMembers[p]->getAge());
			statWorld.incNbArcAvgPrey((*S).preyMembers[p]->getFCM()->getNbArc());
			statWorld.incAgePrey((*S).preyMembers[p]->getAge());
			statWorld.incEnergyAvgPrey((*S).preyMembers[p]->getEnergy());
			statWorld.incStateAvgPrey((*S).preyMembers[p]->getStateBirth());
			statWorld.incAvgDistEvPrey((*S).preyMembers[p]->getDistanceEvol());
			FCM * temp = (*S).preyMembers[p]->getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
			statWorld.inc_activation_prey(j, (*temp->get_activations())[j]);
			}
			statWorld.incSpeedAvgPrey((*S).preyMembers[p]->getSpeed());
			*/
		}
	}


}

void Ecosystem::TwoMeanPredSpeciation() {

	/*
	* Calculate new center for each species
	*/

	int nbPredMembers = 0;
	vector< vector<float> > fcm_temp_value = vector<vector<float> >(this->fcm_pred.get_rows(), vector<float>(this->fcm_pred.get_cols()));
	vector<double> genome_temp_value = vector<double>(Predator::nbGenes, 0);  // M.M
	for (list<Species>::iterator it = speciesPred.begin(); it != speciesPred.end(); ++it) {

		FCM * fAvg = (*it).getFCMAvg();
		PhysicalGenome *genomeAvg = (*it).getGenomeAvg();  // M.M
		nbPredMembers = 0;

		for (size_t j = 0; j < fcm_temp_value.size(); j++){
			for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
				fcm_temp_value[j][k] = 0;
		}

		for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
			genome_temp_value[j] = 0;

		for (size_t p = 0; p < (*it).predMembers.size(); ++p) {

			nbPredMembers++;
			for (size_t j = 0; j < fAvg->get_rows(); ++j) {
				for (size_t k = 0; k < fAvg->get_cols(); ++k) {
					fcm_temp_value[j][k] += (*it).predMembers[p]->getFCM()->get_chart(j, k);
				}
			}
			for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
				PhysicalGenome* genome_temp = (*it).predMembers[p]->getPhGenome();
				//genome_temp_value[j]+= (*genome_temp)[j];
				genome_temp_value[j] += genome_temp->getGene(j);
			}

		}
		(*it).setNbMembers(nbPredMembers);

		for (size_t j = 0; j < fAvg->get_rows(); ++j) {
			for (size_t k = 0; k < fAvg->get_cols(); ++k) {
				fAvg->set_chart(j, k, fcm_temp_value[j][k] / nbPredMembers);
			}
		}
		for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
			//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPredMembers +0.5); // for rounding
			genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPredMembers); // for rounding
		}
	}


	/*
	* 2-MEAN CLUSTERING
	*/

	list<Species> newSpeciesPred;
	bool newSpecies = false;

	float fDistance = 0;
	float cDistance = 0;

	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {

		fDistance = 0;
		cDistance = 0;

		//-- Find individual, I, with greatest distance from S.center
		for (size_t p = 0; p < (*S).predMembers.size(); ++p) {
			cDistance = (*S).predMembers[p]->getFCM()->calculDistanceEvol(this, (*S).getFCMAvg());
			PhysicalGenome* genome_temp = (*S).predMembers[p]->getPhGenome(); // M.M
			//cDistance += genome_temp->calcDistance((*S).getGenomeAvg()->getGenome());  // M.M
			cDistance += genome_temp->calcDistance((*S).getGenomeAvg());  // M.M
			if (cDistance > fDistance) {
				fDistance = cDistance;
			}
		}

		//-- If fDistance > threshold, start clustering
		if (fDistance > 1.6 * distanceSpeciesPred) {

			newSpecies = true;

			//-- Select random individual and set first center C1 = R1.FCM
			FCM * C1;
			PhysicalGenome* C1_Genome; // M.M
			Predator * R1;
			unsigned int R1ID;
			R1ID = rand.next(0, (int)(*S).predMembers.size() - 1);
			R1 = (*S).predMembers[R1ID];
			C1 = R1->getFCM();
			C1_Genome = R1->getPhGenome(); // M.M
			//-- Select furthest individual from R1, named R2, and set second center C2 = R2.FCM
			Predator * R2 = NULL;
			unsigned int R2ID = 0;
			FCM * C2 = NULL;
			PhysicalGenome* C2_Genome = NULL; // M.M
			float fDistance2 = 0;
			float cDistance2 = 0;
			for (size_t p = 0; p < (*S).predMembers.size(); ++p) {
				cDistance2 = (*S).predMembers[p]->getFCM()->calculDistanceEvol(this, C1);
				PhysicalGenome* genome_temp = (*S).predMembers[p]->getPhGenome(); // M.M
				//cDistance2 += genome_temp->calcDistance(C1_Genome->getGenome());  // M.M
				cDistance2 += genome_temp->calcDistance(C1_Genome);  // M.M
				if (cDistance2 > fDistance2) {
					fDistance2 = cDistance2;
					R2 = (*S).predMembers[p];
					C2 = (*S).predMembers[p]->getFCM();
					C2_Genome = (*S).predMembers[p]->getPhGenome(); // M.M
					R2ID = p;
				}
			}

			//-- Create two new species centered around C1 and C2;
			//-- new: Can I pass null to activations?

			FCMPredator s_fcm_pred;
			s_fcm_pred = FCMPredator(C1->get_chart(), NULL, R1);
			//PhysicalGenome s_genome_pred= PhysicalGenome(Predator::nbGenes, C1_Genome->getGenome() ,probaMutation); // M.M
			PhysicalGenome s_genome_pred = PhysicalGenome(Predator::nbGenes, C1_Genome->getRealGenome()); // M.M
			Species * S1 = new Species(-1, 1, &s_fcm_pred, &s_genome_pred);

			s_fcm_pred = FCMPredator(C2->get_chart(), NULL, R2);
			//s_genome_pred= PhysicalGenome(Predator::nbGenes, C2_Genome->getGenome() ,probaMutation); // M.M
			s_genome_pred = PhysicalGenome(Predator::nbGenes, C2_Genome->getRealGenome()); // M.M
			Species * S2 = new Species(-1, 1, &s_fcm_pred, &s_genome_pred);

			S1->predMembers.push_back(R1);
			S2->predMembers.push_back(R2);

			//-- Try for convergence
			int repeat = 0;
			int T = 8;
			while (repeat < T) {

				//-- Group remaining individuals into one of S1 or S2
				for (size_t p = 0; p < (*S).predMembers.size(); ++p) {
					if (p != R1ID && p != R2ID) {
						float dfs1 = (*S).predMembers[p]->getFCM()->calculDistanceEvol(this, C1);
						PhysicalGenome* genome_temp = (*S).predMembers[p]->getPhGenome(); // M.M
						//dfs1 += genome_temp->calcDistance(C1_Genome->getGenome());  // M.M
						dfs1 += genome_temp->calcDistance(C1_Genome);  // M.M
						float dfs2 = (*S).predMembers[p]->getFCM()->calculDistanceEvol(this, C2);
						//dfs2 += genome_temp->calcDistance(C2_Genome->getGenome());  // M.M
						dfs2 += genome_temp->calcDistance(C2_Genome);  // M.M
						if (dfs1 < dfs2) {
							S1->predMembers.push_back((*S).predMembers[p]);
						}
						else {
							S2->predMembers.push_back((*S).predMembers[p]);
						}
					}
				}

				//-- Recalculate center of S1
				nbPredMembers = 0;
				FCM * sFCM = S1->getFCMAvg();
				PhysicalGenome *genomeAvg = S1->getGenomeAvg();  // M.M
				for (size_t j = 0; j < fcm_temp_value.size(); j++){
					for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
						fcm_temp_value[j][k] = 0;
				}

				for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
					genome_temp_value[j] = 0;

				for (size_t p = 0; p < S1->predMembers.size(); ++p) {
					nbPredMembers++;
					for (size_t j = 0; j < sFCM->get_rows(); ++j) {
						for (size_t k = 0; k < sFCM->get_cols(); ++k) {
							fcm_temp_value[j][k] += S1->predMembers[p]->getFCM()->get_chart(j, k);
						}
					}
					for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
						PhysicalGenome* genome_temp = S1->predMembers[p]->getPhGenome();
						//genome_temp_value[j]+= (*genome_temp)[j];
						genome_temp_value[j] += genome_temp->getGene(j);
					}

				}
				S1->setNbMembers(nbPredMembers);
				for (size_t j = 0; j < sFCM->get_rows(); ++j) {
					for (size_t k = 0; k < sFCM->get_cols(); ++k) {
						sFCM->set_chart(j, k, fcm_temp_value[j][k] / nbPredMembers);
					}
				}
				for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
					//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPredMembers +0.5); // for rounding
					genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPredMembers); // for rounding
				}
				//-- Recalculate the center of S2
				nbPredMembers = 0;
				sFCM = S2->getFCMAvg();
				genomeAvg = S2->getGenomeAvg();  // M.M

				for (size_t j = 0; j < fcm_temp_value.size(); j++){
					for (size_t k = 0; k < fcm_temp_value[j].size(); k++)
						fcm_temp_value[j][k] = 0;
				}
				for (size_t j = 0; j < genome_temp_value.size(); j++) // M.M
					genome_temp_value[j] = 0;

				for (size_t p = 0; p < S2->predMembers.size(); ++p) {
					nbPredMembers++;
					for (size_t j = 0; j < sFCM->get_rows(); ++j) {
						for (size_t k = 0; k < sFCM->get_cols(); ++k) {
							fcm_temp_value[j][k] += S2->predMembers[p]->getFCM()->get_chart(j, k);
						}
					}
					for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
						PhysicalGenome* genome_temp = S2->predMembers[p]->getPhGenome();
						//genome_temp_value[j]+= (*genome_temp)[j];
						genome_temp_value[j] += genome_temp->getGene(j);
					}
				}
				S2->setNbMembers(nbPredMembers);
				for (size_t j = 0; j < sFCM->get_rows(); ++j) {
					for (size_t k = 0; k < sFCM->get_cols(); ++k) {
						sFCM->set_chart(j, k, fcm_temp_value[j][k] / nbPredMembers);
					}
				}
				for (size_t j = 0; j < genome_temp_value.size(); j++){ // M.M
					//(*genomeAvg)[j] = int((float)genome_temp_value[j]/nbPredMembers +0.5); // for rounding
					genomeAvg->setPhRealGene(j, (float)genome_temp_value[j] / nbPredMembers); // for rounding
				}

				repeat++;
				if (repeat < T) {
					C1 = S1->getFCMAvg();
					C1_Genome = S1->getGenomeAvg();  //M.M
					C2 = S2->getFCMAvg();
					C2_Genome = S2->getGenomeAvg();   // M.M
					S1->predMembers.clear();
					S2->predMembers.clear();
					S1->predMembers.push_back(R1);
					S2->predMembers.push_back(R2);
				}

			}

			//-- Assign species IDs to new species
			float dS1 = S1->getFCMAvg()->calculDistanceEvol(this, S->getFCMAvg());
			PhysicalGenome* genome_temp = S1->getGenomeAvg(); // M.M
			//dS1 += genome_temp->calcDistance(S->getGenomeAvg()->getGenome());  // M.M
			dS1 += genome_temp->calcDistance(S->getGenomeAvg());  // M.M
			float dS2 = S2->getFCMAvg()->calculDistanceEvol(this, S->getFCMAvg());
			genome_temp = S2->getGenomeAvg(); // M.M
			//dS2 += genome_temp->calcDistance(S->getGenomeAvg()->getGenome());  // M.M
			dS2 += genome_temp->calcDistance(S->getGenomeAvg());  // M.M

			Species::numSpeciesPredMax++;

			if (dS1 < dS2) {
				S1->setNum(S->getNum());
				S2->setNum(Species::numSpeciesPredMax);
				S1->setParent(S->getParent());
				S2->setParent(S->getNum());
				newSpeciesPred.push_back(*S1);
				newSpeciesPred.push_back(*S2);
			}
			else {
				S1->setNum(Species::numSpeciesPredMax);
				S2->setNum(S->getNum());
				S1->setParent(S->getNum());
				S2->setParent(S->getParent());
				newSpeciesPred.push_back(*S2);
				newSpeciesPred.push_back(*S1);
			}

			ofstream predPhylogeny;
			predPhylogeny.open ("PredPhylogeny.txt", ios::app);
			if (S1->getNum() < S2->getNum()) predPhylogeny << generation << "," << S1->getNum() << ",Speciation-" << S2->getNum() << "\n";
			else predPhylogeny << generation << "," << S2->getNum() << ",Speciation-" << S1->getNum() << "\n";
			predPhylogeny.close();
			
			cout << "\n" << "     " << (*S).getNum() << "(" << (int)(*S).predMembers.size() << ")" << " --> " << S1->getNum() << "(" << (int)S1->predMembers.size() << ") + " << S2->getNum() << "(" << (int)S2->predMembers.size() << ")" << endl;
			delete S1;
			delete S2;


		}
		else {
			newSpeciesPred.push_back(*S);
		}

	}

	speciesPred.clear();
	speciesPred = newSpeciesPred;
	predChangedSp.clear();

	//-- Update pointers and stats
	for (list<Species>::iterator S = speciesPred.begin(); S != speciesPred.end(); ++S) {
		for (size_t p = 0; p < (*S).predMembers.size(); ++p) {
			if ((*S).predMembers[p]->getSpecies() != (*S).getNum()){
				predChangedSp.push_back((*S).predMembers[p]->getID());
			}
			(*S).predMembers[p]->setSpecies((*S).getNum());
			(*S).predMembers[p]->speciesPtr = &(*S);
			/*
			(*S).predMembers[p]->setUpdated(0);
			(*S).predMembers[p]->incAge();
			//statWorld.incDistribAge(1, (*S).predMembers[p]->getAge());
			statWorld.incNbArcAvgPred((*S).predMembers[p]->getFCM()->getNbArc());
			statWorld.incAgePred((*S).predMembers[p]->getAge());
			statWorld.incEnergyAvgPred((*S).predMembers[p]->getEnergy());
			statWorld.incStateAvgPred((*S).predMembers[p]->getStateBirth());
			statWorld.incAvgDistEvPred((*S).predMembers[p]->getDistanceEvol());
			FCM * temp = (*S).predMembers[p]->getFCM();
			for (size_t j = 0; j < temp->get_rows(); ++j) {
			statWorld.inc_activation_pred(j, (*temp->get_activations())[j]);
			}
			statWorld.incSpeedAvgPred((*S).predMembers[p]->getSpeed());
			*/
		}
	}

}


void Ecosystem::printTime(time_t s, time_t e) {
	double difference = difftime(e, s);
	cout << " (" << difference << "s.)" << endl;
}

void Ecosystem::initializeMaxGrass(){
	if (circularFoodGrowth == 1){
		float pi = 3.14159265;
		int a = 0;
		int b = 0;
		for (a = 0; a < width; a++){
			for (b = 0; b < height; b++){
				worldMaxGrassArray[a][b] = (float)fmin(fmax((sin(pi*a/foodCircleDiameter) * sin(pi*b/foodCircleDiameter)) * (foodCircleMaxRatio) + foodCircleMinimumRatio, -(sin(pi*a/foodCircleDiameter) * sin(pi*b/foodCircleDiameter)) * (foodCircleMaxRatio) + foodCircleMinimumRatio), 1.0);
			}
		}
	}
	else{
		int a = 0;
		int b = 0;
		for (a = 0; a < width; a++){
			for (b = 0; b < height; b++){
				worldMaxGrassArray[a][b] = 1.0;
			}
		}
	}	
}

void Ecosystem::snap(){
	if (Visualizations == 0) return;
	else if (generation % Visualizations != 0) return;
	//-- Open file for writing
	ostringstream o;
	o << "./" << "//Image";
	mkdir(o.str().c_str(), 0777);
	
	Position p;
	int cnt=0,cnt2, flag=0;
	string a = "./Image/Tmt" + Manip.vtos(generation)+".bmp";
	string a2 = "./Image/GrassAvailability" + Manip.vtos(generation)+".bmp";
	string a3 = "./Image/MeatAvailability" + Manip.vtos(generation)+".bmp";
	const char *b=a.c_str();
	const char *b2 = a2.c_str();
	const char *b3 = a3.c_str();
	
	//Setting Background
	for(int i=0;i<1000;i++)
	{
		for(int j=0;j<1000;j++)
		{
			for(int k=i*5 ;k<((i*5)+5);k++)
			{
				for(int l=j*5 ;l<((j*5)+5);l++)
				{
					snapShot.SetPixel(k,l,bkground);
					snapShot2.SetPixel(k,l,bkground);
					snapShot3.SetPixel(k,l,bkground);
				}
			}
		}
	}

	// Plotting top left section of EcoSim World from 0,0 to 250,250
	for(int i=0;i<1000;i++)
	{
		for(int j=0;j<1000;j++)
		{
			p.x=i;
			p.y=j;
			vector<int> ListOfPrey(worldPreyArray[p.x][p.y]);
			cnt=(int)ListOfPrey.size();
			vector<int> ListOfPred(worldPredArray[p.x][p.y]);
			cnt2=(int)ListOfPred.size();
			if(cnt>15)
				cnt=15;
			if(cnt2>10)
				cnt2=10;
			
			RGBApixel temp;temp.Green=99; temp.Red=99;
			
			// Plotting Prey
			while (cnt!=0)
			{
				Position pos;
				do
				{

					pos.x= rand.next(i*5, ((i*5)+4));
					pos.y= rand.next(j*5, ((j*5)+4));
					temp= snapShot.GetPixel(pos.y,pos.x);
				}
				while(temp.Red!=0 && temp.Green!=0);
				snapShot.SetPixel(pos.y,pos.x,preyPix);
				cnt--;

			}
			
			// Plotting Pred

			while (cnt2!=0)
			{
				Position pos;
				do
				{

					pos.x= rand.next(i*5, ((i*5)+4));
					pos.y= rand.next(j*5, ((j*5)+4));
					temp= snapShot.GetPixel(pos.y,pos.x);
				}
				while(temp.Red!=0 && temp.Green!=0);
				snapShot.SetPixel(pos.y,pos.x,predPix);
				cnt2--;

			}
			float meatRatio = (float) world[i][j][1] / (float) maxMeat;
			float grassRatio = (float) world[i][j][0] / ((float) maxGrass * (fluctuatingResources == 1 ? fluctuationMinimumRatio : 1) + (float) maxGrass * (fluctuatingResources == 1 ? fluctuationAmplitudeRatio * 2 : 0));
			int meatPixel = int(meatRatio * 255 + 0.5);
			int grassPixel = int(grassRatio * 255 + 0.5);
			grassPix.Blue = 0; grassPix.Red = 0; grassPix.Green = grassPixel; grassPix.Alpha = 0;
			meatPix.Blue = 0; meatPix.Red=meatPixel; meatPix.Green=0; meatPix.Alpha=0;
			// plotting food resources
			for (int g = 0; g < 5; g++){
				for (int h = 0; h < 5; h++){
					snapShot2.SetPixel(j * 5 + h, i * 5 + g, grassPix);
					snapShot3.SetPixel(j * 5 + h, i * 5 + g, meatPix);
				}
			}
		} //End of Inner For
	} //End of Outer For

	snapShot.WriteToFile(b);
	snapShot2.WriteToFile(b2);
	snapShot3.WriteToFile(b3);
}



