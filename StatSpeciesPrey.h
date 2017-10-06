#ifndef STATSPECIESPREY_H_
#define STATSPECIESPREY_H_

#include "Species.h"
#include "FCMPrey.h"
#include "Manipulation.h"

#include <fstream>

#include <list>
class Ecosystem;

using namespace std;

class StatSpeciesPrey {

private:
	//-- Information of each species
	int gen;
	int pop;							//-- Current population of all individuals in this generation
	int speciesParentID;
	int speciesID;

	//-- Average of all individuls in one species
	int nbSpecies;						//-- Number of species in the current generation
	int nbIndividuals;					//-- Current population of species in the current generation
	float individualRatio;				//-- Ratio number of individuals one species to number of all individuals
	int nbBirth;						//-- Number of newborn
	float DeathRatio;					//-- Ratio of death
	int nbDeadA;						//-- Number of death because of age
	int nbDeadE;						//-- Number of death because of energy <= 0
	int nbDeadK;						//-- Number of death because they were eaten by competitor
	float DeadA;						//-- Average age of death
	float DeadE;						//-- Average energy of death
	float DeadAratio;					//-- Ratio of death because of age
	float DeadEratio;					//-- Ratio of death because of energy
	float DeadKratio;					//-- Ratio of death because of killed

	float nbSameGenderInds;				//-- Current population of species in the current generation (same gender)

	float entropy;						//-- Marwa: Entropy of species
	float diversityS;					//-- Spacial Diversity
	float diversitySratio;				//-- ratio of sqrt (Spacial Diversity)
	float distEvol;
	float stateOfBirth;
	float age;							//-- Average age
	float energy;						//-- Average energy
	float strength;						//-- Average strength
	float speed;						//-- Average speed

	int SIZE_activ;						//-- size of activations
	vector<float> activations;			//-- Contain of activation vectors
	vector<int> actions;				//-- Number of actions per species

	int reprodFailedA;					//-- Age of parents in the failure of Reproduction
	float reprodFailedE;				//-- Energy of parents in the failure of Reproduction
  	int nbInterbreeding;				//-- Number of Interbreeding

	int parent1_reproduceA;				//-- Age of chooser parents inside a species in reproduction action
	float parent1_reproduceE;			//-- Energy of chooser parents inside a species in reproduction action
	int nbParent1;
	int parent2_reproduceA;
	float parent2_reproduceE;
	int nbParent2;
    float DistMating;					//-- Average of Distance Parents when reproduce
	int PregnantyDeath;

	float nbCase;							//-- Number of cells contain individuals
	int nbArc;								//-- Average number of arcs

	int reproductionAttempts;
	int reasonReproduceFailed_Energy;			//-- Energy of partner is not enough to reproduce
	int reasonReproduceFailed_NoPartner;		//-- partner not found to reproduce
	float reasonReproduceFailed_PartnerEnergy;	//-- Average of "energy of partner is not sufficient" to reproduce
	float reasonReproduceFailed_PartnerActed;	//-- Average of "Partner has acted befor" to reproduce
	float reasonReproduceFailed_PartnerAction;	//-- Average of "Partner has chosen another action" to reproduce
	float reasonReproduceFailed_PartnerDist;	//-- Average of "Distance is far" to reproduce
	float reasonReproduceFailed_SameGender;	    //-- Average of "Same gender" //Since Females are initiating reproduction, it happens when mate is male//Reproduction based on gender /MRE/



	//-- structure for calculating Spatial Diversity
	struct SpatialDiversityParameters {
		double m_center_x, m_center_y;
		double m_variance_x, m_variance_y;
	};

	int fcm_row, fcm_col;
	vector<vector<float> > fcm_avg;				//-- Average of fcm
	vector<vector<float> > fcm_var;				//-- Variance of fcm
	PhysicalGenome genetic_avg;					//-- keep the average of genetome /M.M

	void reset();
	void Death(Ecosystem *, list<Species>::iterator, int);

	void write(ofstream *);
	void write_FCM(ofstream *);
	void write_Header(string, string, int);
	void fileName(string, string *, string *, int);

	double ArcTAN(double delX, double delY);
	SpatialDiversityParameters CalcSpatialDiversityParams(list<Species>::iterator, int);
	float DiversitySpatial(list<Species>::iterator, int);

	float Entropy(list<Species>::iterator, int gender);
    void AverageFCM(list<Species>::iterator , vector<vector<float> > * , vector<vector<float> > *, int);

	Manipulation Manip;							//-- Class Instances: Manipulating the input

public:
	StatSpeciesPrey();
	StatSpeciesPrey(int, int);
	virtual ~StatSpeciesPrey();

	void writeSpeciesStat(Ecosystem *, int);
	void RemoveEnd(Ecosystem *, int);  //-- Remove additional generations after Restoring
};

#endif /* STATSPECIESPREY_H_ */
