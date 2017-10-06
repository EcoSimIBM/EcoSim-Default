/*
 * Entry point for the simulation
 *  - Creates the ecosystem
 *  - Updates the ecosystem every time step
 */

#include <iostream>
#include "Ecosystem.h"

using namespace std;

int main() {

	Ecosystem * eco = new Ecosystem();

	while (1) {

		//-- for synchronization of diffrent runs
		//if (eco->generation > 25000)
			//break;

		if (eco->IsWithoutPredator == 0){
			//cout << endl << "With Predator..." << endl;
			//-- Termination if one of prey or predatore dead out
			if (((int)eco->wolves.size() == 0) || ((int)eco->rabbits.size() == 0)){
				cout << endl << "Terminated because of deth of all individuals..." << endl;
				break;
			}
		}
		else{
			//MRE without predator run
			//cout << endl << "Without Predator..." << endl;

			if ((int)eco->rabbits.size() == 0){
				cout << endl << "Terminated because of deth of all individuals..." << endl;
				break;
			}
		}

		//-- Updeate ecosystem
		eco->updateEco();

		//-- Finish recording time elapsed for generation
		time(&eco->gEnd);
		cout << " * Time to complete GEN:" << flush;
		eco->printTime(eco->gStart, eco->gEnd);

		//-- Write the results stats file
		eco->getStat()->writeStat(eco);
		eco->getStat()->reset(); //-- Reset the stats

		//-- Write the results stats per species file
		if (eco->perspeciesPreyFlag == 1) { eco->statSpeciesPrey->writeSpeciesStat(eco, 0); } //male
		if (eco->perspeciesPredFlag == 1) { eco->statSpeciesPred->writeSpeciesStat(eco, 0); } //male

		//-- Write the results stats per species file
		if (eco->perspeciesPreyFlag == 1) { eco->statSpeciesPrey->writeSpeciesStat(eco, 1); } //female
		if (eco->perspeciesPredFlag == 1) { eco->statSpeciesPred->writeSpeciesStat(eco, 1); } //female

		//-- Write the results stats per species file
		if (eco->perspeciesPreyFlag == 1) { eco->statSpeciesPrey->writeSpeciesStat(eco, 2); } //all
		if (eco->perspeciesPredFlag == 1) { eco->statSpeciesPred->writeSpeciesStat(eco, 2); } //all


		//-- Full save to restore the simulation for restoration
#ifdef HDF5_COMPILE
		if (eco->maxSaveFlag != 0 && eco->generation % eco->maxSaveFlag == 0) { eco->maxSave_HDF5(); }
#else
		if (eco->maxSaveFlag != 0 && eco->generation % eco->maxSaveFlag == 0) { eco->maxSave(); }
#endif // HDF5

		//-- Min save or compresed Min save for stats
#ifdef HDF5_COMPILE
		if (eco->minSaveFlag != 0 && eco->generation % eco->minSaveFlag == 0) { eco->minWorld_HDF5(); }
#else
		if (eco->minSaveFlag != 0 && eco->generation % eco->minSaveFlag == 0) { eco->minSave(); }
#endif // HDF5

#ifdef HDF5_COMPILE
		if (eco->minSaveCompressedFlag != 0 && eco->generation % eco->minSaveCompressedFlag == 0) { eco->minSave_Compressed_HDF5(); }
#else
		if (eco->minSaveCompressedFlag != 0 && eco->generation % eco->minSaveCompressedFlag == 0) { eco->minSave_Compressed(); }
#endif


#ifndef HDF5_COMPILE
		if (eco->worldSaveFlag) { eco->worldSave(); }
#endif // HDF5

		//-- Increment generation number
		eco->generation = eco->generation + 1;
		//-- Tar the previous folder of MinSave and World
#ifndef HDF5_COMPILE
		if (eco->tarMinSaveWorldFlag)   { eco->doTarLastFolder(); }
#endif // HDF5

	}

	return 0;

}