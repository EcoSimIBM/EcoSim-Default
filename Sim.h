#ifndef SIM_H_
#define SIM_H_

#include "Position.h"
#include "Species.h"
#include "Random.h"
#include "PhysicalGenome.h"

//enum Gender { Female = 1, Male = 0};//Add Gender /MRE/

class Species;


using namespace std;

class Sim {
private:

	long id;									//-- Unique id
	int species;							//-- Spcecies to which it belongs

	long parent1ID;							//-- Parent 1 ID
	long parent2ID;							//-- Parent 2 ID
	int speciesParent1ID;					//-- Parent 1 species ID
	int speciesParent2ID;					//-- Parent 2 species ID

	short age;								//-- Individual's age
	short ageMax;							//-- Max possible age of individual
	short repAge;							//-- Reproduction Age of individual. M.M
	float energy;							//-- Individual's energy
	float strength;							//-- The power of individual M.M
	float energyMax;						//-- Individuals' max allowed energy  => shows the size of individual
	float speed;							//-- Speed of movement
	short speedMax;							//-- Max speed of movement
	float distanceEvol;						//-- Distance of evolution
	float stateBirth;						//-- Base of calculation energy of new born
	short vision;							//-- How far the individual can see in the world
	short updated;							//-- Status of the individual's behaviour
	short lastAction;						//-- Last action performed
	short actionOffset;						//-- Current action to be performed, it's the node number and different from action number, that's why it has a different name
	short type;								//-- Type of individual, 0 - Prey, 1 - Predator
	short nbReprod;							//-- Number of reproduced individuals
	bool living;							//-- True if alive, False if dead
	float DistMating;						//-- Distance selected Parentes in mating
	float weight;

	short gender;							//-- Add Gender /MRE/ 0:male, 1:female

protected:
	PhysicalGenome phGenome;               // Phyisical genome of individual /M.M


public:
	int cntReprodFailedTries;
	short reproduceFailed1;				//-- 1: Energy of prey is not enoug, 10: partner has not found, 11: both of them
	float reproduceFailed2;			  	//-- "energy of partner is not sufficient"
	float reproduceFailed3;				//-- "Partner has acted befor"
	float reproduceFailed4;				//-- "Partner has chosen another action"
	float reproduceFailed5;				//-- "Distance is far"
	//float reproduceFailed6;			//-- Average of "Same gender" //MRE
	//float reproduceFailed7;				//-- "Extra Males after all females are done with reprod" 

	Species * speciesPtr;					//-- Pointer to species individual belongs to

	Position pos;							//-- (x, y) coordinates of position in world

	Sim();
	virtual ~Sim();
	// for first generation individuals
	Sim(short type, long ID, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, float energyMax, short speedMax, short ageMax, short vision, short repAge, float distanceEvol, float stateBirth, unsigned char nbGenes, short genderVal, float defence = 0, float coopDefence = 0, short persuasionVal = 0);//Add Gender /MRE/
	// for new born individuals
	Sim(short type, long ID, int species, long parent1ID, long parent2ID, int speciesParent1ID, int speciesParent2ID, float distanceEvol, unsigned char nbGenes, short genderVal);//Add Gender /MRE/

	PhysicalGenome* getPhGenome()	{ return &phGenome; };
	long getID()                  	{ return id; };
	long getParent1ID()           	{ return parent1ID; };
	long getParent2ID()           	{ return parent2ID; };

	int getSpecies()             	{ return species; };
	int getSpeciesParent1ID()    	{ return speciesParent1ID; };
	int getSpeciesParent2ID()    	{ return speciesParent2ID; };

	short getAge() const              	{ return age; };
	short getRepAge()            	{ return (short)phGenome.getGene(4); };  // M.M
	short getAgeMax(bool);
	float getSpeed()             	{ return speed; };
	short getSpeedMax()          	{ return (short)phGenome.getGene(3); }; // M.M
	float getEnergy()            	{ return energy; };
	float getStrength() const  	  	{ return strength; };	// M.M
	float getDefence()           	{ return phGenome.getGene(6); }; // M.M
	float getCoopDefence()       	{ return phGenome.getGene(7); }; // M.M
	float getEnergyMax()         	{ return phGenome.getGene(0); }; // M.M
	char getPersuasionChar()		{ return phGenome.getPersuasionChar(); };
	float getPersuasion()			{ return phGenome.getPersuasion(); };
	short getLastAction()        	{ return lastAction; };

	short getNbReprod()          	{ return nbReprod; };
	float getDistanceEvol()      	{ return distanceEvol; };
	float getStateBirth()        	{ return phGenome.getGene(5); }; // M.M
	short getType()              	{ return type; };
	bool getLiving()             	{ return living; };
	short getUpdated()           	{ return updated; };
	short getVision()            	{ return (short)phGenome.getGene(2); }   // M.M;
	short get_action_offset()    	{ return actionOffset; };
	float get_DistMating(void)   	{ return DistMating; };
	Position * getPosition()     	{ return &pos; };

	short getGender()               { return gender; }; //Add Gender /MRE////

	void setSpecies(int s)       	{ species = s; };

	void setLiving(bool b)       	{ living = b; };
	void setLastAction(short a)  	{ lastAction = a; };
	void set_action_offset(short a) { actionOffset = a; };
	void setUpdated(short i)     	{ updated = i; };
	void setEnergy(float);
	void setStrength(float s)    	{ strength = s; };
	void setAge(short a) 			{ age = a; };
	void setRepAge(short rA) 		{ repAge = rA; };	// M.M
	void setSpeed(float s) 			{ speed = s; };
	void setPosition(Position p) 	{ pos.x = p.x; pos.y = p.y; };
	void set_DistMating(float dm) 	{ DistMating = dm; };
	void set_DistEvolution(float d) { distanceEvol = d; }; // M.M

	float getWeight() const			{ return weight; };
	void calcWeight()				{ if (getAge() < getRepAge()) weight = 1.2 * getStrength(); else weight = getStrength(); };

	void setGender(short g)         { gender = g; };//Add Gender /MRE/

	void incAge() 					{ age++; };
	void incNbReprod() 				{ nbReprod++; };

	Position Movement(int, int, int, int);
	float fixRange(float, float, float); //MRE change energy formula
	float calculationEnergyAction(float, int, float, float, float, Ecosystem *);
	float calculationStateOfBirth(float, float, float, float, Random *);
	float calculationEnergyNewBorn(float, float, float, float);
	float calculationEnergyNewParent(float, float, float, float);
	void calcStrength(short type); // M.M
	// for sorting rabbits & wovels vectors based on Strength M.M
	bool operator < (const Sim& p) const { return getStrength() > p.getStrength(); };  // M.M
	//bool operator < ( const Sim& p) const { return getWeight() > p.getWeight(); };  // M.M
	//bool operator < ( const Sim& p) const { return getAge() < p.getAge(); };  // M.M

};



#endif /* SIM_H_ */

