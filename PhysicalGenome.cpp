#include "PhysicalGenome.h"
#include "Ecosystem.h"
 long long RandGen::num_randoms=0;
 
// for new born individuals
PhysicalGenome::PhysicalGenome(int size)//, float probMutation )
{
    phGenes=vector<GENETYPE>(size,0);
    realPhGenes = vector<float>(size,0);
    this->genomeSize = size;
    //this->probMutation=probMutation;
}

// for first generation individuals
PhysicalGenome::PhysicalGenome(int size, vector<float>* genome)//  ,float probMutation )
{
    phGenes=vector<GENETYPE>(size,0);
    realPhGenes = vector<float>(size,0);
    this->genomeSize = size;
	persuasion = 0;
    //this->probMutation=probMutation;
    //checkbounds(genome,size);   // to check the correct range foe every gene

    // -- IMPORTANT => if the input  genome is the average of the physical genome the phGenes
    // -- which is char version is not valid for some genes like MaxEnergy

    phGenes[0]= float2Char((*genome)[0],MAXENERGY_START,MAXENERGY_STEP);  //Max Energy
    phGenes[1]= (GENETYPE) (*genome)[1];        //Max Age
    phGenes[2]= (GENETYPE) (*genome)[2]; // Vision
    phGenes[3]= (GENETYPE) (*genome)[3]; // Max Speed
    phGenes[4]= (GENETYPE) (*genome)[4]; // Rep. Age
	phGenes[5]= (size > 6) ? float2Char((*genome)[5],PREYSOB_START,SOB_STEP) : float2Char((*genome)[5],PREDSOB_START,SOB_STEP); // SOB
    if (size > 6)
    {
        phGenes[6]= float2Char((*genome)[6],DEFENCE_START,DEFENCE_STEP);  // Prey Defence
        phGenes[7]= float2Char((*genome)[7],DEFENCE_START,COOPDEFENCE_STEP); // Prey Cooperative Defence
    }
    for (short i = 0; i< size; i++)
		realPhGenes[i] = (*genome)[i];
}

PhysicalGenome::PhysicalGenome(int size,  vector<GENETYPE>* genome)//  ,float probMutation)// , bool changetype)
{
	phGenes=vector<GENETYPE>(size,0);
	realPhGenes = vector<float>(size,0);
	this->genomeSize = size;
	//this->probMutation=probMutation;
	//checkbounds(genome,size);   // to check the correct range foe every gene
	phGenes[0]= (*genome)[0];  //Max Energy
	phGenes[1]= (*genome)[1];  //Max Age
	phGenes[2]= (*genome)[2]; // Vision
	phGenes[3]= (*genome)[3]; // Max Speed
	phGenes[4]= (*genome)[4]; // Rep. Age
	phGenes[5]= (*genome)[5]; // SOB
	if (size > 6)
	{
		phGenes[6]= (*genome)[6];  // Prey Defence
		phGenes[7]= (*genome)[7]; // Prey Cooperative Defence
	}

	for (short i = 0; i< size; i++)
		realPhGenes[i] = changeToRealGene(i);
}

PhysicalGenome::~PhysicalGenome()
{

}


GENETYPE& PhysicalGenome::operator[](int index)
{
    if (index>=0 && index <genomeSize)
        return phGenes[index];
    else
    {
        cout << "\nError Index in PhysicalGenome";
        exit(0);
    }
}



float PhysicalGenome::changeToRealGene(int index)
{
    GENETYPE ch= phGenes[index];
    switch (index)
    {
        case 0: return char2Float(ch, MAXENERGY_START, MAXENERGY_STEP);
                break;
        case 1: return ch;
                break;
        case 2: return ch; // [1,20]
                break;
        case 3: return ch; // [1,20]
                break;
		case 4: return ch; // [1,20]
                break;
        case 5: if(genomeSize > 6)
				{
					return char2Float(ch, PREYSOB_START, SOB_STEP);
				}
				else
				{
					return char2Float(ch, PREDSOB_START, SOB_STEP);
				}
				break;
        case 6: return char2Float(ch, DEFENCE_START, DEFENCE_STEP);
                break;
        case 7: return char2Float(ch, DEFENCE_START, COOPDEFENCE_STEP);
        break;

        default:
            cout << "\nError Index in PhysicalGenome";
            exit(0);
    }
}

char PhysicalGenome::getMutateAmount( float p1, float p2)
{
// for example for ( 90%, 9% , 1%) => p2=0.01; p1= 0.09; => Vision & Speed & Rep. Age
// for (60%, 30%, 10%) => p2=0.1; ; p1=0.3  => the rest.
	double prob=0.0;
	char test=0;

	prob = RandGen::next(32000) / 32000.0;
	if(prob <= p2)
	{
        prob = RandGen::next(32000) / 32000.0;
        if(prob <= 0.5)
            test =  3;
        else
            test = -3;
	}
	else if(prob <= p1)
	{   prob = RandGen::next(32000) / 32000.0;
		if(prob <= 0.5)
            test =  2;
		else
            test = -2;
	}
	else
	{
	 prob = RandGen::next(32000) / 32000.0;
	 if(prob <= 0.5)
		test =  1;
	 else
		test = -1;
	}
	return test*2; // M.M based on the changes in mutation probability (divided by 10)
}

void PhysicalGenome::crossover(vector<GENETYPE>* mother, vector<GENETYPE>* father, short bebe_gender, char p)
{
	//MRE RandomGoodGene
	
	for (int i = 0; i< genomeSize; i++)
	{
		if (i == 0){ //Max Energy, based on max energy of mother and father and gender of bebe
			if (bebe_gender == 1){//female
				phGenes[i] = round((5.0 * (*mother)[i] + (*father)[i]) / 6.0);
			}
			else{
				phGenes[i] = round((5.0 * (*father)[i] + (*mother)[i]) / 6.0);
			}
		}
		else if (i == 5)
		{
			if (bebe_gender == 1) //female
			{
				phGenes[i] = (*mother)[i];
			}
			else //male
			{
				phGenes[i] = (*father)[i];
			}
		}
		else{ //random
			if (RandGen::next(32000) / 32000.0 <= 0.5)
				phGenes[i] = (*mother)[i];
			else
				phGenes[i] = (*father)[i];
		}
	}
    // to update the real Physical Genome
    for (short i = 0; i< genomeSize; i++)
		realPhGenes[i] = changeToRealGene(i);
	
	persuasion = p;
}

void PhysicalGenome::mutate(float probMutation, Ecosystem * eco)
{
    float do_mutate;
    char mutate_amount;
    short t;
    for (int i=2; i< 5; i++) // Vision range & Max Speed and Rep. Age ;  All should be in [1,20] range
    {
        do_mutate = RandGen::next(32000) / 32000.0;
        mutate_amount = (do_mutate <= probMutation) ? getMutateAmount(0.09,0.01) : 0;
        t=mutate_amount;
		if (i == 4)
		{
			if (this->genomeSize > 6) // prey
			{
				if (phGenes[i]+t < eco->getAgeReprodPrey())
					phGenes [i] = eco->getAgeReprodPrey();
				else
					phGenes[i] += mutate_amount;
			}
			else // pred
			{
				if (phGenes[i]+t < eco->getAgeReprodPred())
					phGenes [i] = eco->getAgeReprodPred();
				else
					phGenes[i] += mutate_amount;
			}
		}
		else
		{
			if (phGenes[i]+t < 1)
				phGenes [i] = 1;
			else
				phGenes[i] += mutate_amount;
		}
        if (phGenes[i] > VSR_MAX)  phGenes[i] = VSR_MAX;
    }

    for (int i=0; i< getGenomeSize(); i++) // for the rest of genes
    {
        if (i!= 2 && i!=3 && i!=4)
        {
            do_mutate = RandGen::next(32000) / 32000.0;
            mutate_amount = (do_mutate <= probMutation) ? getMutateAmount(0.3, 0.1) : 0;
            t=mutate_amount;
            if (t+phGenes[i]> 255 )
                phGenes[i]=255;
            else if (t+phGenes[i] <0)
                phGenes[i]=0;
            else
                phGenes[i] += mutate_amount;
        }
    }
    // to update the real Physical Genome
    for (short i = 0; i< genomeSize; i++)
		realPhGenes[i] = changeToRealGene(i);
	
	if (eco->persuasionHappens == 1){
		do_mutate = RandGen::next(32000) / 32000.0;
		if (do_mutate <= probMutation){
			mutate_amount = getMutateAmount(0.3, 0.1);
		}else{
			mutate_amount = 0;
		}
		t = mutate_amount;
		if (t + persuasion > 255){
			persuasion = 255;
		}
		else if (t + persuasion < 0){
			persuasion = 0;
		}
		else{
			persuasion += mutate_amount;
		}
	}
}

GENETYPE PhysicalGenome::float2Char(float value, float start, float step )
{
     if (value >1)
        value+=0.05; // solve float precision problem
     return (GENETYPE) ((value-start)/step);
}

void PhysicalGenome::checkbounds(float genome[], int size)
{
    bool f=true;
    if (genome[0] < 100 || genome[0] > 6475 )
    {	//maxEnergy
        f=false; cout<< "Error! in gene0 value.=>" << genome[0];
    }
    if (genome[1] < 10 || genome[1] > 265 )
    {	//maxAge
        f=false; cout<< "Error! in gene1 value.=>"<<genome[1];
    }
    if (genome[2] < 1 || genome[2] > 20 )
    {	//vision
        f=false; cout<< "Error! in gene2 value.=>"<<genome[2];
    }
    if (genome[3] < 1 || genome[3] > 20 )
    {	//maxSpeed
        f=false; cout<< "Error! in gene3 value.=>"<<genome[3];
    }
    if (genome[4] < 1 || genome[4] > 20 )
    {	//RepAge
        f=false; cout<< "Error! in gene3 value.=>"<<genome[4];
    }
    if ((genome[5] < 4.5 || genome[5] > 30 ) && size > 6)
    {	//SOB prey
        f=false; cout<< "Error! in gene4 Prey value.=>"<<genome[5];
    }
    if ((genome[5] < 14.5 || genome[5] > 40 ) && size<=6)
    {	//SOB pred
        f=false; cout<< "Error! in gene4 Pred value.=>"<<genome[4];
    }
    if ((genome[6] < 0 || genome[6] > 1) && size >6  )
    {	
        f=false; cout<< "Error! in gene5 value.=>" <<genome[6];
    }
    if ((genome[7] < 0 || genome[7] > 0.5) && size >6 )
    {
        f=false; cout<< "Error! in gene6 value.=>" <<genome[7];
    }
    if (!f) exit(0);
}

void PhysicalGenome::printGenome()
{
    for (int i=0; i< genomeSize; i++)
        cout << int(phGenes[i])<< ",";
    cout << endl;
}

//int PhysicalGenome::calcDistance (vector<GENETYPE>* parent2)
//{
//	int distance=0;
//	for (short i=0; i<genomeSize; i++)
//	{
//		distance += abs(phGenes[i]-(*parent2)[i]);
//	}
//	return 5*distance;
//}

//int PhysicalGenome::calcDistance (vector<GENETYPE>* parent2)
//{
//	int wdistance=0;
//	PhysicalGenome phGenome2 = PhysicalGenome(genomeSize,  parent2 ,this->probMutation, false);
//	for (short i=0; i<genomeSize; i++)
//	{
//		//int distance = abs(phGenes[i]-(*parent2)[i]);
//		float distance = fabs(this->getGene(i) - phGenome2.getGene(i));
//		float weight = 0;
//		switch (i)
//		{
//			case 0:
//				weight = (distance - MAXENERGY_START) / (MAXENERGY_MAX - MAXENERGY_START) ;
//				break;
//			case 1:
//				weight = (distance - MAXAGE_START) / (MAXAGE_MAX - MAXAGE_START);
//				break;
//			case 2:  // Vision
//			case 3:  // MaxSpeed
//			case 4:  //Rep. Age
//				weight = (distance - VSR_START) / (VSR_MAX - VSR_START);
//				break;
//			case 5:
//				if (genomeSize > 6)
//					weight = (distance - PREYSOB_START) / (PREYSOB_MAX - PREYSOB_START);
//				else
//					weight = (distance - PREDSOB_START) / (PREDSOB_MAX - PREDSOB_START);
//				break;
//			case 6:
//				weight = (distance - DEFENCE_START) / (DEFENCE_MAX - DEFENCE_START);
//				break;
//			case 7:
//				weight = (distance - DEFENCE_START) / (COOPDEFENCE_MAX - DEFENCE_START);
//				break;
//		}
//		wdistance += distance * weight;
//	}
//	return 5*wdistance;
//}

float PhysicalGenome::calcDistance (PhysicalGenome* p2Genome)
{
	float wdistance=0;

	for (short i = 1; i < genomeSize; i++)
	{
		//int distance = abs(phGenes[i]-(*parent2)[i]);
		float distance = fabs(this->getGene(i) - p2Genome->getGene(i));
		float weight = 0;
		switch (i)
		{
			case 0:
				//weight = (distance - MAXENERGY_START) / (MAXENERGY_MAX - MAXENERGY_START) ;
				weight = 0; //distance / (MAXENERGY_MAX - MAXENERGY_START); SEX LINKED, NOT INCLUDED IN CALCULATION
				break;
			case 1:
				//weight = (distance - MAXAGE_START) / (MAXAGE_MAX - MAXAGE_START);
				weight = distance / (MAXAGE_MAX - MAXAGE_START);
				break;
			case 2:  // Vision
				weight = distance / (VSR_MAX - VISION_START);
				break;
			case 3:  // MaxSpeed
			case 4:  //Rep. Age
				//weight = (distance - VSR_START) / (VSR_MAX - VSR_START);
				weight = distance / (VSR_MAX - VSR_START);
				break;
			case 5:
				if (genomeSize > 6)
					//weight = (distance - PREYSOB_START) / (PREYSOB_MAX - PREYSOB_START);
					weight = 0; //distance / (PREYSOB_MAX - PREYSOB_START); SEX LINKED, NOT INCLUDED IN CALCULATION
				else
					//weight = (distance - PREDSOB_START) / (PREDSOB_MAX - PREDSOB_START);
					weight = 0; //distance / (PREDSOB_MAX - PREDSOB_START); SEX LINKED, NOT INCLUDED IN CALCULATION
				break;
			case 6:
				//weight = (distance - DEFENCE_START) / (DEFENCE_MAX - DEFENCE_START);
				weight = distance / (DEFENCE_MAX - DEFENCE_START);
				break;
			case 7:
				//weight = (distance - DEFENCE_START) / (COOPDEFENCE_MAX - DEFENCE_START);
				weight = distance / (COOPDEFENCE_MAX - DEFENCE_START);
				break;
		}
		wdistance += distance * weight;
	}
	return  (5 * wdistance);
}
