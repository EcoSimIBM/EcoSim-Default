#include "Random.h"
#include <vector>
#include <math.h>
#ifndef PHYSICALGENOME_H
#define PHYSICALGENOME_H

class Ecosystem;

typedef unsigned char GENETYPE;

// for mapping from char to corresponding float values
const GENETYPE MAXENERGY_START=100;    // Min= 100; Max= 6475; Step= 25
const int MAXENERGY_MAX = 6475;
const GENETYPE MAXENERGY_STEP=25;

//MRE change energy formula
// for mapping from char to corresponding float values
const float MAXSTRENGTH_START = 100;    // Min= 100; Max= 6475; Step= 25
const int MAXSTRENGTH_MAX = 6475;
const float MAXSTRENGTH_STEP = 25;
 
const GENETYPE MAXAGE_START=10;        // Min= 10;  Max= 265;  Step= 1
const int MAXAGE_MAX = 265;
const GENETYPE MAXAGE_STEP=1;

const GENETYPE VSR_START = 1;
const int VSR_MAX =25; // vision, MAx Speed , Rep age

const GENETYPE VISION_START = 6; //MRE RandomGoodGene

const GENETYPE PERSUASIONSTART = 0; //RS, persuasion increases chance a male will choose to reproduce rather than its alternatively decided upon action
const GENETYPE PERSUASIONMAX = 1;
const float PERSUASIONSTEP = 1.0/128.0;

//MRE RandomGoodGene
const float PREYSOB_START = 0;        // Min= 7;   Max= 32.5; Step= 0.1 //MRE default values are 7 and 32.5. if you increase max, should increase min as well.
const float PREYSOB_MAX = 25.5;//MRE RandomGoodGene
const int PREDSOB_MAX = 25.5;//MRE RandomGoodGene
const float SOB_STEP = 0.1;
const float PREDSOB_START = 0; //MRE RandomGoodGene       // Min= 14.5;Max= 40;   Step= 0.1

const GENETYPE DEFENCE_START=0;           // Min= 0;   Max= 1;    Step= 1/256
const GENETYPE DEFENCE_MAX = 1;
const float DEFENCE_STEP= 1/256.0;
const float COOPDEFENCE_MAX = 0.5;
const float COOPDEFENCE_STEP= 1/512.0; // Min= 0;   Max= 0.5;    Step= 1/512

/////////

class RandGen {
	public:
        static long long num_randoms;
		static int next(int max) {					//-- Random between 0 and max
			if (num_randoms % RESET_RANDOM ==0)
                srand(SEED);
			int r = rand() % max;
			num_randoms++;
			return r;
		}
};


class PhysicalGenome
{
    public:
		PhysicalGenome(){};
        PhysicalGenome(int size);//, float probMutation= 0.005 ); // for new born individuals
        PhysicalGenome(int size, vector<float>* genome);//, float probMutation= 0.005 ); // for first generation individuals
        PhysicalGenome(int size,  vector<GENETYPE>* genome);//  ,float probMutation= 0.005);//, bool changetype ); // used in 2-Mean Clustering
        ~PhysicalGenome();
        vector<GENETYPE>* getGenome()				{ return &phGenes; };
        vector<float>* getRealGenome()				{ return &realPhGenes; };
        int getGenomeSize()							{ return genomeSize; };
        float getGene(int id) 						{ return realPhGenes[id]; };
		GENETYPE getCharGene(int id)                { return phGenes[id]; };
        void setPhRealGene(int index, float value) 	{ realPhGenes[index] = value; };
		void crossover(vector<GENETYPE>* parent1, vector<GENETYPE>* parent2, short bebe_gender, char persuasion);
		void mutate(float probMutation, Ecosystem *);
        GENETYPE& operator[](int index);
        float changeToRealGene (int id);
        void printGenome();
        float calcDistance (PhysicalGenome* p2Genome);
		float getPersuasion()						{ return char2Float(persuasion, PERSUASIONSTART, PERSUASIONSTEP); };
		GENETYPE persuasion;
		void setPersuasion(unsigned char p)					{ persuasion = p; };
		void setPersuasionFloat(float p)					{ persuasion = float2Char(p, PERSUASIONSTART, PERSUASIONSTEP); };
		unsigned char getPersuasionChar()				{ return persuasion; };


    protected:
    private:
        vector<GENETYPE> phGenes;
        vector<float> realPhGenes;
        int genomeSize;
        char getMutateAmount(float p1, float p2);
        float char2Float(GENETYPE chargene, float start, float step) const { return start + chargene * step; };
        GENETYPE float2Char(float value, float start, float step );
        void checkbounds(float genome[], int size);

};



#endif // PHYSICALGENOME_H

