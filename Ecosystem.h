#ifndef ECOSYSTEM_H_
#define ECOSYSTEM_H_

#include <vector>
#include <string>
#include <map>

#include "Position.h"
#include "Random.h"

#include "Stat.h"
#include "StatSpeciesPrey.h"
#include "StatSpeciesPred.h"
#include "Manipulation.h"

#include "EasyBMP.h"
#include "Species.h"
#include "Prey.h"
#include "Predator.h"

#undef HDF5_COMPILE //change this to "#define HDF5_COMPILE" if you want to have HDF5 in place - and to "#undef HDF5_COMPILE" for without HDF5
// Use makefile_g++ if you want to use g++ compiler (LoadResults and SaveResults are excluded from that file)
#ifdef HDF5_COMPILE
#include "SaveResults.h"
#include "LoadResults.h"
#endif // HDF5


//-- If uncomment this definition, MinSave+World+Result_species will be created in the folder with Linux standard
#define LinuxSystem

using namespace std;

class Ecosystem {

private:
#ifdef HDF5_COMPILE
	SaveResults *sr;							//-- HDF5
#endif // HDF5
	int width, height;							//-- Height and width of environment

	int nbPrey;									//-- Number of rabbits (prey)
	int nbPredator;								//-- Number of wolves (predators)
	short ageMaxPrey, ageMaxPredator;			//-- Max ages of the individuals
	short ageReprodPrey, ageReprodPredator;		//-- Age needed to be able to reproduce

	float probaInitialGrass, probaGrowGrass;
	int maxGrass;								//-- Max amount of grass in a cell
	float speedGrowGrass;						//-- Speed at which grass grows	(previously vitessePousseHerbe)
	int maxMeat;								//-- Max amount of meat in a cell
	int wastedMeatPerGen;						//-- Wasted meat in every time step /M.M
	int nbResources;							//-- Number of resources available

	float probaMutation, probaMutation_HighAge;			//-- Probability of mutation
	float probaMutationLow, probaMutationLow_HighAge;	//-- Probability of mutation [floor]
	float percentMutation;						//-- Amount of mutation
	float percentMutationHigh;					//-- Amount of mutation [ceiling]

	float distanceSpeciesPrey;					//-- Distance of evolution of prey
	float distanceSpeciesPred;					//-- Distance of evolution of predator
	float distanceMin;							//-- Minimum distance needed to be considered 'evolving'
	float valueGrass, valuePrey;				//-- The amount of energy grass and prey are worth
	#ifdef TWO_RESOURCES						//-- Probabilities of new grass and growing existing grass
		float valueGrass2, probaInitialGrass2, probaGrowGrass2;
		int maxGrass2;							//-- Armin
		float speedGrowGrass2;					//-- Armin
	#endif

	int restore;								//-- The state to restor
	short MatingMode;							//-- // 0-Random mate selection method, 1-Good gene mate selection method, 2-Intermediate // MRE

	long lastGenPreyID, lastGenPredID;			//-- last ID of prey and predator at the end of last generation

	vector<vector<vector<float> > > world;		//-- Maintains resource information (meat and grass)
	vector<int> worldPreyArray[1000][1000];		//-- Worlds list of prey and their locations
	vector<int> worldPredArray[1000][1000];		//-- Worlds list of predators and their locations
	
	float worldMaxGrassArray[1000][1000]; //MaxGrass allocations for the world

	vector<int> worldMalePreyArray[1000][1000];		//-- Worlds list of Male prey and their locations
	vector<int> worldMalePredArray[1000][1000];		//-- Words list of Male predators and their locations
	

	vector<string> nameConceptsPrey;			//-- The name of the concepts for prey	  //?????????????????????????? do we need it?
	vector<string> nameConceptsPred;			//-- The name of the concepts for predator  //?????????????????????????? do we need it?

	list<long> preyChangedSp;					//-- List of the prey IDs that their species has changed
	list<long> predChangedSp;					//-- List of the pred IDs that their species has changed
	list<Species> prevSpeciesPrey;				//-- List of the previous species of prey ***1,2,5,9
	list<Species> prevSpeciesPred;				//-- List of the previous species of predators

	//-- Meisam
	struct Death{								//-- Information for MinSave + Results file
	    int species;
		long id;
		short deathType;						//-- 1:= Death of Old age, 2:= Death of Energy, 3:= Killed
		short lastAction;
		short age;
		float evolDist;
		float energy;
		float speed;
		short gender;
	};

	struct Parent{								//-- Information for Results file
		short age;
		float energy;
		bool PregnancyDeath;
	};
	//-- Meisam End

	void SpeciesStatistics(void);				//-- Meisam
	void doTarLastSpecificFolder(int type);             // Do tar the last folder based on the type => type=1(Min) ; type=2(Min_C); type=3 (world) /M.M
	static bool greaterID(Sim i, Sim j) { return (i.getID() < j.getID()); };


public:
	int IsWithoutPredator;						//-- Without predator
	float FertilizerDivideTo;					//-- Divide sum of fertilizer to decrease its effect
	int Visualizations;
	bool HDF5;									//-- HDF5 
	int generation;								//-- Current generation number
	int maxSaveFlag;							//-- Flag: 0-no, #-save every # generations
	int minSaveFlag;							//-- Flag: 0-no, #-save stat every # generations (all information of all individuals)
	int minSaveCompressedFlag;					//-- Flag: 0-no, #-save stat every # generations (abstract info of all individuals)
	int tarMinSaveWorldFlag;                    //-- Flag: 0-no tar, 1- tar 1000 MinSave & WorldSave in each folder to one file /M.M
	int worldSaveFlag;							//-- Flag: 0-no, 1-yes
	int perspeciesPreyFlag;						//-- Flag: 0-no, 1-yes
	int perspeciesPredFlag;						//-- Flag: 0-no, 1-yes
	int fluctuatingResources;					//-- Flag: 0-no, 1-yes
	int circularFoodGrowth;						//-- Flag: 0-no, 1-yes
	float fluctuationAmplitudeRatio;			//-- Amplitude of fluctuation as ratio of maximum grass 
	float fluctuationMinimumRatio;				//-- Minimum of grass as ratio of maximum grass 
	int fluctuationCycle;						//-- Grass fluctuation cycle length 
	float foodCircleDiameter;					//-- Diameter of grass circles
	float foodCircleMaxRatio;					//-- Peak of grass as ratio of maximum grass... used to increase the area of circles that have maximum grass
	float foodCircleMinimumRatio;				//-- Minimum of grass as a ratio of maxgrass 
	int persuasionHappens;						//-- Does persuasion occur? 1 = yes, 0 = no (which is original EcoSim)
	
	float maxModifiedGrass;

	RGBApixel preyPix, predPix, grassPix, meatPix, bkground; 
	BMP snapShot;
	BMP snapShot2;
	BMP snapShot3;
	long maxPreyID;								//-- Current available Prey ID for new Prey
	long maxPredID;								//-- Current avialable Pred ID for new Predators

	vector<Prey> rabbits;						//-- The world's prey
	vector<Predator> wolves;					//-- The world's predators

	vector<int> femaleRabbitsIndexes;			//-- The world's Female prey Indexes//Reproduction based on gender /MRE/
	vector<int> femaleWolvesIndexes;			//-- The world's Female predators Indexes//Reproduction based on gender /MRE/

	list<Species> speciesPrey;					//-- List of the species of prey
	list<Species> speciesPred;					//-- List of the species of predators

	time_t start, end;							//-- For measuring time elapsed for various eventts
	time_t gStart, gEnd;						//-- For measuring time elapsed for entire generation


	//-- Meisam: Information for MinSave + Results file
	Death DeadInd;
	vector<Death> DeadPrey;
	vector<Death> DeadPred;

	map < long, Parent > ParentPrey;  			//-- <Individual ID, {age,energy}>
	map < long, Parent > ParentPred;
	//-- Meisam End

	//-- Class Instances
	FCMPrey fcm_prey;
	FCMPredator fcm_pred;
	//MRE RandomGoodGene
	PhysicalGenome ini_MalePreyGenome;  			// to save the initial genome for prey => is used to calculate evolution distance.
	PhysicalGenome ini_MalePredGenome;  			// to save the initial genome for pred

	PhysicalGenome ini_FemalePreyGenome;  			// to save the initial genome for prey => is used to calculate evolution distance.
	PhysicalGenome ini_FemalePredGenome;  			// to save the initial genome for pred

	Stat statWorld;								//-- Statistics about the world (results file)
	StatSpeciesPrey *statSpeciesPrey;			//-- Statistics about the world (results file per species)
	StatSpeciesPred *statSpeciesPred;			//-- Statistics about the world (results file per species)
	Manipulation Manip;							//-- Manipulating the input
	Random rand;								//-- For random number generating

	//----
	Ecosystem();
	virtual ~Ecosystem();

	/*
	 * GET methods
	 */

	vector<int> * getListCreature(Position, int);
	vector<int> * getListMaleCreature(Position, int);
	float getSumofStrength(Position, int);		// A.G
	float getMaxStrength(short, Position, int);		// A.G
	vector<string> * getNameConceptsPrey() { return &nameConceptsPrey; };
	vector<string> * getNameConceptsPred() { return &nameConceptsPred; };
	vector<Predator> * getVectorPredator() { return &wolves; };
	vector<Prey> * getVectorPrey()         { return &rabbits; };
	list<Species> * getSpeciesPrey()       { return &speciesPrey; };
	list<Species> * getSpeciesPred()       { return &speciesPred; };
	int getWidth()                         { return width; };
	int getHeight()                        { return height; };
	float getDistanceMin()                 { return distanceMin; };
	float getDistanceSpeciesPrey()         { return distanceSpeciesPrey; };
	float getDistanceSpeciesPred()         { return distanceSpeciesPred; };
	float getValueGrass()                  { return valueGrass; };
	#ifdef TWO_RESOURCES
		float getValueGrass2()             { return valueGrass2; };
	#endif
	float getValuePrey()                   { return valuePrey; };
	float getProbaMut()                    { return probaMutation; };
	float getProbaMutLow()                 { return probaMutationLow; };
	float getProbaMut_HighAge()            { return probaMutation_HighAge; };	// Abbas
	float getProbaMutLow_HighAge()         { return probaMutationLow_HighAge; };	//Abbas
	float getPercentMut()                  { return percentMutation; };
	float getPercentMutHigh()              { return percentMutationHigh; };
	short getAgeMaxPrey()                  { return ageMaxPrey; };
	short getAgeMaxPred()                  { return ageMaxPredator; };
	short getAgeReprodPrey()               { return ageReprodPrey; };
	short getAgeReprodPred()               { return ageReprodPredator; };
	int getMaxGrass()                      { return maxGrass; };
	#ifdef TWO_RESOURCES
		int getMaxGrass2()                 { return maxGrass2; };
		float getLevelGrass2(Position p)   { return world[p.x][p.y][2]; };
	#endif
	int getMaxMeat()                       { return maxMeat; };
	int getNbPrey()                        { return nbPrey; };
	int getNbPred()                        { return nbPredator; };
	float getLevelGrass(Position p)        { return world[p.x][p.y][0]; };
	float getLevelMeat(Position p)         { return world[p.x][p.y][1]; };

	Stat * getStat()                       { return &statWorld; };	    //-- results file
	Random * getRandom()                   { return &rand; };

	float getSumOfStrengthAndDefence(Position);				// RS
	float getMaxStrengthAndDefencePrey(Position);			// RS
	float getDistanceMaxStrengthAndDefencePrey(Position); 	//RS

	/*
	 * Other methods
     */
	void incNbPrey() 						{ nbPrey++; };
	void decNbPrey() 						{ nbPrey--; };
	void incNbPred() 						{ nbPredator++; };
	void decNbPred() 						{ nbPredator--; };
	void incLevelGrass(Position);
	void addFertilizer(Position);
	float addFertilizerFunc(float, float, float);
	float findSpeedGrass(float, float, float);
	//void incLevelMeat1(Position); => instead call incLevelMeatByStrength M.M
	//void incLevelMeat2(Position); => instead call incLevelMeatByStrength M.M
	void incLevelMeatByStrength(Position, float preyStrength); // M.M  New Meat Policy
	//void decLevelGrass(Position);   M.M
	void decLevelGrass(Position, float);  //M.M
	#ifdef TWO_RESOURCES
		void incLevelGrass2(Position);
		void decLevelGrass2(Position);
	#endif
	//void decLevelMeat(Position); => instead call decLevelMeat with value = available meat. M.M
	//void decLevelMeat2(Position); => instead call decLevelMeat with value = 300  M.M
	void decLevelMeat(Position, float value); // M.M New Meat Policy

	void removeDeadPrey();
	void removeDeadPred();
	void removeDeadSpeciesPrey();
	void removeDeadSpeciesPred();
	void addListCreature(Position, int, int);
	void addListMaleCreature(Position, int, int);

	void printTime(time_t, time_t);
	void RemovePhylogenyEnds();
	void initializeMaxGrass();
	void updateEco();
	void maxSave();					//-- saves the complete world for restoration
	void minSave();					//-- saves  data of all current individuals
	void minSave_Compressed();		//-- saves  minimum data of all current individuals
	void worldSave();
	void restoreState();			//-- restores the world from the state saved by saveState
	void TwoMeanPreySpeciation();
	void TwoMeanPredSpeciation();
	void doTarLastFolder();      //-- Morteza, Abbas => Do tar the last folder of MinSave and World files
	void snap();
#ifdef HDF5_COMPILE
	void maxSave_HDF5();
	void minWorld_HDF5();
	void minSave_Compressed_HDF5();
	void restoreState_HDF5();
#endif // HDF5
};

#endif /* ECOSYSTEM_H_ */

