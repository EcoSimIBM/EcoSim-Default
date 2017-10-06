#ifndef FCM_H_
#define FCM_H_

#include "math.h"
#include <vector>
class Ecosystem;
class Predator;
class Prey;
class Sim;


#define FCM_TYPE unsigned char
#define ZERO 0  // when float is mapped to char M.M


using namespace std;

class FCM {
private:
	vector<vector<FCM_TYPE> > chart;	//-- Map of arcs between concepts
	size_t rows;
	size_t cols;
	int nbArc;							//-- Number of arcs in individual's chart
	float reproductionSumBeforeFuzzification;
	float largestActivation;
	short largestActivationIndex;

public:
	vector<float> activations;			//-- Current concepts about the world
	short ConceptIndex;					//-- Meisam: shows Concept that should changes its' value after acting
	float ConceptMultiplier;			//-- Meisam: Based on the ConceptIndex, the value of this concpet should multipy to ConceptMultiplier

    FCM();
	virtual ~FCM();

    FCM (int rows, int cols);
    FCM (vector<vector<FCM_TYPE> > * cart, vector<float> *activations);

    void calculate_nb_arc();
	vector<float> * get_activations();
	vector<vector<FCM_TYPE> > * get_chart();

	//vector<vector<float> >  get_chart_float();
	vector<vector<FCM_TYPE> >  get_char_chart() { return chart; };

	int getNbArc();
	float calcul_node_activation_pure(vector<float> *activations_old, int i);
	float fuzzy1(float, float, float);
	float fuzzy2(float, float, float);
	float fuzzy0(float);
	float standardizeContinuous(float, float, float, float);
	void reset_chart();
	void set_activation(int i, float v);
	float get_activation(int i);
	void set_chart(int i, int j , float v);
	void setchar_chart(int i, int j , FCM_TYPE v);
	float get_chart(int i, int j);
	FCM_TYPE getchar_chart(int i, int j);
	size_t get_rows();
	size_t get_cols();
	void print_chart();
	void SetLargestActivation (float l)		{ largestActivation = l; };
	float GetLargestActivation ()			{ return largestActivation; };
	void SetLargestActivationIndex (short i)			{ largestActivationIndex = i; };
	short GetLargestActivationIndex ()			{ return largestActivationIndex; };
	float GetModifiedReproductionActivation (float m);
	bool ReviseReproductionDecision(float persuasion, Ecosystem * eco);

	int calculDistanceEvol(Ecosystem *, FCM *fcm);
	int calculDistanceEvol(FCM fcm); // M.M
	int reproduction_constraint(int, int);
    static float char2float(FCM_TYPE c);
    static FCM_TYPE  float2char(float f);

};


#endif /* FCM_H_ */
