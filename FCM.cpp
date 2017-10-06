/*
 * FCM.cpp
 *  - The prey and predators fuzzy cognitive map
 *  - Maintains data structures to represent concepts and cognitive charts
 */

#include <iostream>
#include <cmath>
#include "FCM.h"
#include "Ecosystem.h"
#include "Predator.h"
#include "Prey.h"
#include "Sim.h"  //-- Armin

using namespace std;




FCM::FCM() {

}

FCM::~FCM() {

}

FCM::FCM (int rows, int cols) {
	reproductionSumBeforeFuzzification = 0;
	largestActivation = 0;
    this->rows = rows;
    this->cols = cols;

    this->chart = vector<vector<FCM_TYPE> >(rows, vector<FCM_TYPE>(cols,ZERO));

    this->activations = vector<float>(rows);
    this->nbArc = 0;
	ConceptIndex = 0;
	ConceptMultiplier = 1;

}

FCM::FCM (vector<vector<FCM_TYPE> > * chart, vector<float> *activations) {


	reproductionSumBeforeFuzzification = 0;
	largestActivation = 0;
    this->chart = *chart;
    rows = chart->size();
    cols = (rows!=0) ? (*chart)[0].size() : 0;
    if (activations!=NULL)
        this->activations = *activations;
    else
        this->activations = vector<float>(rows);
    calculate_nb_arc();
	ConceptIndex = 0;		//-- Initialization
	ConceptMultiplier = 1;	//-- Initialization
}

void FCM::calculate_nb_arc(){
    nbArc = 0;
    for (size_t i=0; i<chart.size(); i++)
        for (size_t j=0; j<chart[i].size(); j++){
            if (getchar_chart(i,j)!=ZERO)       // M.M
                nbArc++;
        }
}

vector<float> * FCM::get_activations() {
	return &activations;
}

vector<vector<FCM_TYPE> > * FCM::get_chart() {
	return &chart;
}

int FCM::getNbArc() {
	return nbArc;
}

float FCM::calcul_node_activation_pure(vector<float> *activations_old, int i) {
    int nbSens = rows-cols;
	float current = (*activations_old)[i];
	for (size_t j = 0; j < rows; j++) {
		current += get_chart(j,i-nbSens) * (*activations_old)[j];

	}
	if (nbSens == FCMPredator::nbSens && i == FCMPredator::Reproduce)//RS!! it's a predator
	{
		reproductionSumBeforeFuzzification = current;
	}
	else if (nbSens == FCMPrey::nbSens && i == FCMPrey::Reproduce) //it's a prey
	{
		reproductionSumBeforeFuzzification = current;
	}
	
	return current;

}

void FCM::reset_chart() {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			chart[i][j]=0; // M.M
		}
	}
}

void FCM::set_activation(int i, float v) {
	activations[i] = v;
}

float FCM::get_activation(int i) {
	return activations[i];
}

void FCM::set_chart(int i, int j , float v) {
    FCM_TYPE hold = chart[i][j] ;
    if ((v>12.8)||(v<-12.7)){
        cout <<"\nUnable to store FCM vale\n";
        exit(-1);
    }
	chart[i][j] = float2char(v);

    if ((hold==ZERO) && chart[i][j]!=ZERO)
        nbArc++;
    else if ((hold!=ZERO) && chart[i][j]==ZERO)
        nbArc--;
}

void FCM::setchar_chart(int i, int j, FCM_TYPE v)
{
    float hold = chart[i][j];

	chart[i][j] =v;

    if ((hold==ZERO) && chart[i][j]!=ZERO)
        nbArc++;
    else if ((hold!=ZERO) && chart[i][j]==ZERO)
        nbArc--;
}


float FCM::GetModifiedReproductionActivation (float m) {
	int nbSens = rows-cols;
	
	if (nbSens == FCMPredator::nbSens){
		int index = FCMPredator::Reproduce;
		return standardizeContinuous(reproductionSumBeforeFuzzification + m, FCMPredator::fuzzyfication_params2[index][0], FCMPredator::fuzzyfication_params2[index][1], FCMPredator::fuzzyfication_params2[index][2]);
	}
	else if (nbSens == FCMPrey::nbSens){
		int index = FCMPrey::Reproduce;
		return standardizeContinuous(reproductionSumBeforeFuzzification + m, FCMPrey::fuzzyfication_params2[index][0], FCMPrey::fuzzyfication_params2[index][1], FCMPrey::fuzzyfication_params2[index][2]);
	}
	return 0;
}

bool FCM::ReviseReproductionDecision(float persuasion, Ecosystem * eco){ //True means decide to reproduce instead, false means keep original decision
	int nbSens = rows-cols;
	
	if (nbSens == FCMPredator::nbSens){
		if (GetLargestActivationIndex() == FCMPredator::Reproduce){
			return true;
		}
		int index = FCMPredator::Reproduce;
	}
	else if (nbSens == FCMPrey::nbSens){
		if (GetLargestActivationIndex() == FCMPrey::Reproduce){
			return true;
		}
		int index = FCMPrey::Reproduce;
	}
	
	if (eco->persuasionHappens == 0) return false;
	
	if (GetModifiedReproductionActivation(persuasion) > GetLargestActivation()){
		return true;
	}
	else if (GetModifiedReproductionActivation(persuasion) == GetLargestActivation()){
			Random *r = eco->getRandom();
			float test = (float) (r->next(10000) / 10000.0);
			if (test<0.5){
				return true;
			}
			else{
				return false;
			}
	}
	else{
		return false;
	}
}

float FCM::get_chart(int i, int j) { // return the float value
	return char2float(chart[i][j]);
}

FCM_TYPE FCM::getchar_chart(int i, int j) { // return the char value
	return chart[i][j];
}

//vector<vector<float> > FCM::get_chart_float() {
//    vector<vector<float> > float_chart = vector<vector<float> >(rows, vector<float>(cols));
//    for (size_t i=0; i<rows; i++)
//        for(size_t j=0; j<cols; j++)
//            float_chart[i][j] = char2float(chart[i][j]);
//	return float_chart;
//}

void FCM::print_chart() {
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			cout << (int)(chart[i][j]) << "  ";
		}
		cout << "" << endl;
	}
}

int FCM::calculDistanceEvol(Ecosystem * eco, FCM *fcm) {
	int distanceEvol = 0;
	for (size_t i = 0; i < rows; ++i) {
		//find out if predator or prey (check #rows and #columns)
		//check if i >= pred/prey nbSens
		if (rows == FCMPrey::nbSens + FCMPrey::nbConcepts + FCMPrey::nbMoteursDep + FCMPrey::nbMoteursFix && cols == FCMPrey::nbConcepts + FCMPrey::nbMoteursDep + FCMPrey::nbMoteursFix){
			if (i >= FCMPrey::nbSens){
				for (size_t j = 0; j < cols; ++j) {
					FCM_TYPE thischar=chart[i][j];
					FCM_TYPE fcmchar= fcm->getchar_chart(i,j);
					bool flag= ((chart[i][j]>=128 && fcmchar<128) || (chart[i][j]<128 && fcmchar>=128)) ? true: false; // because of new coding schema 0 <=> 0
					if (thischar==0) thischar=128;
					if (fcmchar==0) fcmchar=128;
					FCM_TYPE dist= abs(thischar - fcmchar);
					if (flag) dist++;
					distanceEvol += dist;
				}
			}
		} else if (rows == FCMPredator::nbSens + FCMPredator::nbConcepts + FCMPredator::nbMoteursDep + FCMPredator::nbMoteursFix && cols == FCMPredator::nbConcepts + FCMPredator::nbMoteursDep + FCMPredator::nbMoteursFix){
			if (i >= FCMPredator::nbSens){
				for (size_t j = 0; j < cols; ++j) {
					FCM_TYPE thischar=chart[i][j];
					FCM_TYPE fcmchar= fcm->getchar_chart(i,j);
					bool flag= ((chart[i][j]>=128 && fcmchar<128) || (chart[i][j]<128 && fcmchar>=128)) ? true: false; // because of new coding schema 0 <=> 0
					if (thischar==0) thischar=128;
					if (fcmchar==0) fcmchar=128;
					FCM_TYPE dist= abs(thischar - fcmchar);
					if (flag) dist++;
					distanceEvol += dist;
				}
			}
		}
	}
	return distanceEvol;
}
// M.M
int FCM::calculDistanceEvol(FCM fcm) {
	int distanceEvol = 0;
	for (size_t i = 0; i < rows; ++i) {
		if (rows == FCMPrey::nbSens + FCMPrey::nbConcepts + FCMPrey::nbMoteursDep + FCMPrey::nbMoteursFix && cols == FCMPrey::nbConcepts + FCMPrey::nbMoteursDep + FCMPrey::nbMoteursFix){
			if (i >= FCMPrey::nbSens){
				for (size_t j = 0; j < cols; ++j) {
					FCM_TYPE thischar=chart[i][j];
					FCM_TYPE fcmchar= fcm.getchar_chart(i,j);
					bool flag= ((chart[i][j]>=128 && fcmchar<128) || (chart[i][j]<128 && fcmchar>=128)) ? true: false; // because of new coding schema 0 <=> 0
					if (thischar==0) thischar=128;
					if (fcmchar==0) fcmchar=128;
					FCM_TYPE dist= abs(thischar - fcmchar);
					if (flag) dist++;
					distanceEvol += dist;
				}
			}	
		} else if (rows == FCMPredator::nbSens + FCMPredator::nbConcepts + FCMPredator::nbMoteursDep + FCMPredator::nbMoteursFix && cols == FCMPredator::nbConcepts + FCMPredator::nbMoteursDep + FCMPredator::nbMoteursFix){
			if (i >= FCMPredator::nbSens){
				for (size_t j = 0; j < cols; ++j) {
					FCM_TYPE thischar=chart[i][j];
					FCM_TYPE fcmchar= fcm.getchar_chart(i,j);
					bool flag= ((chart[i][j]>=128 && fcmchar<128) || (chart[i][j]<128 && fcmchar>=128)) ? true: false; // because of new coding schema 0 <=> 0
					if (thischar==0) thischar=128;
					if (fcmchar==0) fcmchar=128;
					FCM_TYPE dist= abs(thischar - fcmchar);
					if (flag) dist++;
					distanceEvol += dist;
				}
			}
		}
	}
	return distanceEvol;
}

size_t FCM::get_rows(){
    return rows;
}
size_t FCM::get_cols(){
    return cols;
}
// M.M
FCM_TYPE FCM::float2char(float f)
{
    double fracpart, intpart;
    int sign = f<0 ? -1:1;
    f= (f<0) ? f*10-0.5 : f*10 ; // solving rounding problem.
    fracpart = modf (f , &intpart);
    intpart= fracpart <0.5 ? intpart : intpart + sign;
    if (intpart==0) return 0;
    if (intpart>0) return int(intpart)+127;
    return int(intpart)+128;
}
//M.M
float FCM::char2float(unsigned char c){
    if (c==0) return 0;
    if (c<128) return (c-128)/10.0;
    return (c-127)/10.0;
}

float FCM::fuzzy1(float p , float begin, float end) {
	if (p > end) {
		return 0;
	} else {
		if (p < begin) {
			return 1;
		} else {
			return (end - p) / (end - begin);
		}
	}
}

float FCM::fuzzy2(float p , float begin, float end) {
	if (p < begin) {
		return 0;
	} else {
		if (p > end) {
			return 1;
		} else {
			return (p - begin) / (end - begin);
		}
	}
}

float FCM::fuzzy0(float p) {
	return p;
}

float FCM::standardizeContinuous(float a, float a0, float delta, float ro) {
	float inter1 = 0;
	float inter2 = 0;
	inter1 = exp(-ro * (a - a0));
	inter1 += 1;
	inter2 = (1 + delta) / inter1;
	inter2 -= delta;
	return inter2;
}
