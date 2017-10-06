#ifndef RANDOM_CPP_
#define RANDOM_CPP_

#include <iostream>

#include <cstdlib>
#include <ctime>

#define RESET_RANDOM 100000
//#define SEED 5 //-- For test
#define SEED ((unsigned int) time(NULL))

using namespace std;

struct Random {
	public:
        long long num_randoms;

		Random() {
			srand(SEED);
            num_randoms=0;
		}

		int next(int max) {					//-- Random between 0 and max
			int r = rand() % max;
			num_randoms++;
			if (num_randoms % RESET_RANDOM ==0)
                    srand(SEED);
			return r;
		}

		int next(int min, int max) {		//-- Random between min and max
			int range = max - min + 1;
			int r = rand() % range + min;
			num_randoms++;
			if (num_randoms % RESET_RANDOM ==0)
                    srand(SEED);
			return r;
		}

		unsigned int getSEED(){
			return SEED;
		}
};

#endif

