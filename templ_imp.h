#ifndef TEMPL_IMP_H
#define TEMPL_IMP_H
template <class T>
void SaveResults::WrNewIndv(vector <T>& p, const char* g, int newIndvCount, long PrevIndvIndex)
{
	if (newIndvCount > 0){
		group = H5Gopen(file, g);

		NewIndv    wdata2[newIndvCount];

		int rows = p[0].getFCM()->get_rows();
		int cols = p[0].getFCM()->get_cols();

		short FCM[newIndvCount][rows][cols]; // FCM of all individuals.
		short phGenome[newIndvCount][T::nbGenes]; // physical Genome of all individuals.

        int i = 0;
		for (int loop=0; loop< p.size(); loop++){
            if ( p[loop].getID() > PrevIndvIndex ) {   // new indv
                wdata2[i].id = p[loop].getID();
                wdata2[i].x = p[loop].getPosition()->x;
                wdata2[i].y = p[loop].getPosition()->y;
                wdata2[i].energy = p[loop].getEnergy();
                wdata2[i].age = p[loop].getAge();
                wdata2[i].maxage = p[loop].getAgeMax(true);
                wdata2[i].speed = p[loop].getSpeed();
                wdata2[i].distEvol = p[loop].getDistanceEvol();
                wdata2[i].specId = p[loop].getSpecies();
                wdata2[i].specP1Id = p[loop].getSpeciesParent1ID();
                wdata2[i].specP2Id = p[loop].getSpeciesParent2ID();
                wdata2[i].parent1Id = p[loop].getParent1ID();
                wdata2[i].parent2Id = p[loop].getParent2ID();
                wdata2[i].gender = p[loop].getGender();
                for (size_t j = 0; j < rows; j++)
                    for (size_t k = 0; k < cols; k++)
                        FCM[i][j][k] = p[loop].getFCM()->getchar_chart(j, k);
                for (size_t j = 0; j < T::nbGenes ; j++)
                    phGenome[i][j] = (short)(p[loop].getPhGenome())->getCharGene(j);
                i++;
            }
		}


        memtype = H5Tcreate (H5T_COMPOUND, sizeof (NewIndv));
        status = H5Tinsert (memtype, "Id", HOFFSET (NewIndv, id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "X", HOFFSET (NewIndv, x), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Y", HOFFSET (NewIndv, y), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Energy", HOFFSET (NewIndv, energy), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "Age", HOFFSET (NewIndv, age), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "MaxAge", HOFFSET (NewIndv, maxage), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Speed", HOFFSET (NewIndv, speed), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "DistEvolution", HOFFSET (NewIndv, distEvol), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "SpecId", HOFFSET (NewIndv, specId), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "SpecP1Id", HOFFSET (NewIndv, specP1Id), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "SpecP2Id", HOFFSET (NewIndv, specP2Id), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "Parent1Id", HOFFSET (NewIndv, parent1Id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "Parent2Id", HOFFSET (NewIndv, parent2Id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "Gender", HOFFSET (NewIndv, gender), H5T_NATIVE_SHORT);

        filetype = H5Tcreate (H5T_COMPOUND, 3*8 + 6*4 + 5*2);
        status = H5Tinsert (filetype, "Id", 0, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "X", 8, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Y", 8+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Energy", 8+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "Age", 8+2+2+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "MaxAge", 8+2+2+4+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Speed", 8+2+2+4+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "DistEvolution", 8+2+2+4+2+2+4, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "SpecId", 8+2+2+4+2+2+4+4, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "SpecP1Id", 8+2+2+4+2+2+4+4+4, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "SpecP2Id", 8+2+2+4+2+2+4+4+4+4, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "Parent1Id", 8+2+2+4+2+2+4+4+4+4+4, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "Parent2Id", 8+2+2+4+2+2+4+4+4+4+4+8, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "Gender", 8+2+2+4+2+2+4+4+4+4+4+8+8, H5T_STD_I16BE);

        hsize_t dims[1] = {newIndvCount};
        space = H5Screate_simple (1, dims, NULL);
        // for compression
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 1, dims);
        //
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_NewIndv", filetype, space, plist);//, H5P_DEFAULT, H5P_DEFAULT);

        status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

        status = H5Pclose(plist);
        status = H5Tclose (memtype);
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Tclose (filetype);



        // Write FCM of all individuals.

        hsize_t     dims2[3] = {newIndvCount, rows, cols};
        space = H5Screate_simple (3, dims2, NULL);
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 3, dims2);
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_FCM", H5T_STD_I16BE, space, plist);//,H5P_DEFAULT,H5P_DEFAULT);
        status = H5Dwrite (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, FCM);
        status = H5Pclose(plist);
        status = H5Dclose (dset);
        status = H5Sclose (space);

        // Write Physical Genome of all individuals.

        hsize_t     dims3[2] = {newIndvCount, T::nbGenes};
        space = H5Screate_simple (2, dims3, NULL);
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 2, dims3);
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_phGenome", H5T_STD_I16BE, space, plist);//,H5P_DEFAULT,H5P_DEFAULT);
        status = H5Dwrite (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, phGenome);


        status = H5Pclose(plist);
        status = H5Dclose (dset);
        status = H5Sclose (space);

        status = H5Gclose (group);
    }

}

template <class T>
void SaveResults::WrCompressedNewIndv(vector <T>& p, const char* g, int newIndvCount, long PrevIndvIndex)
{
    if ( newIndvCount > 0 ){
		group = H5Gopen(file, g);
        int i =0;
		NewIndv_C    wdata2[newIndvCount];
		for (int loop=0; loop< p.size(); loop++){
            if ( p[loop].getID() > PrevIndvIndex ) {   // new indv
                wdata2[i].id = p[loop].getID();
                wdata2[i].x = p[loop].getPosition()->x;
                wdata2[i].y = p[loop].getPosition()->y;
                wdata2[i].energy = p[loop].getEnergy();
                wdata2[i].age = p[loop].getAge();
                wdata2[i].specId = p[loop].getSpecies();
                wdata2[i].specP1Id = p[loop].getSpeciesParent1ID();
                wdata2[i].specP2Id = p[loop].getSpeciesParent2ID();
                wdata2[i].parent1Id = p[loop].getParent1ID();
                wdata2[i].parent2Id = p[loop].getParent2ID();
                wdata2[i].gender = p[loop].getGender();
                i++;
            }
		}

        memtype = H5Tcreate (H5T_COMPOUND, sizeof (NewIndv_C));
        status = H5Tinsert (memtype, "Id", HOFFSET (NewIndv_C, id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "X", HOFFSET (NewIndv_C, x), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Y", HOFFSET (NewIndv_C, y), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Energy", HOFFSET (NewIndv_C, energy), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "Age", HOFFSET (NewIndv_C, age), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "SpecId", HOFFSET (NewIndv_C, specId), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "SpecP1Id", HOFFSET (NewIndv_C, specP1Id), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "SpecP2Id", HOFFSET (NewIndv_C, specP2Id), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "Parent1Id", HOFFSET (NewIndv_C, parent1Id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "Parent2Id", HOFFSET (NewIndv_C, parent2Id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "Gender", HOFFSET (NewIndv_C, gender), H5T_NATIVE_SHORT);

        filetype = H5Tcreate (H5T_COMPOUND, 3*8 + 4*4 + 4*2);
        status = H5Tinsert (filetype, "Id", 0, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "X", 8, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Y", 8+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Energy", 8+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "Age", 8+2+2+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "SpecId", 8+2+2+4+2, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "SpecP1Id", 8+2+2+4+2+4, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "SpecP2Id", 8+2+2+4+2+4+4, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "Parent1Id", 8+2+2+4+2+4+4+4 , H5T_STD_I64BE);
        status = H5Tinsert (filetype, "Parent2Id", 8+2+2+4+2+4+4+4+8, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "Gender", 8+2+2+4+2+4+4+4+8+8, H5T_STD_I16BE);


        hsize_t dims[1] = {newIndvCount};
        space = H5Screate_simple (1, dims, NULL);
        // for compression
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 1, dims);
        //
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_NewIndv", filetype, space, plist);//, H5P_DEFAULT, H5P_DEFAULT);

        status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

        status = H5Pclose(plist);
        status = H5Tclose (memtype);
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Tclose (filetype);

        status = H5Gclose (group);
    }


}

template <class T>
void SaveResults::WrCurrIndv(vector <T>& p, list<long>& preyChangedSp, const char* g, int currIndvCount, long PrevIndvIndex)
{
	if (currIndvCount > 0) {
        CurrIndv cIndvs[currIndvCount];
        group = H5Gopen(file, g);


        int rows = p[0].getFCM()->get_rows();
        float actvs [currIndvCount][rows];
       int i =0;
        for (int loop=0; loop< p.size(); loop++){
            if (p[loop].getID() <= PrevIndvIndex){
                cIndvs[i].id = p[loop].getID();
                cIndvs[i].x = p[loop].getPosition()->x;
                cIndvs[i].y = p[loop].getPosition()->y;
                cIndvs[i].energy = p[loop].getEnergy();
                cIndvs[i].age = p[loop].getAge();
                cIndvs[i].maxage = p[loop].getAgeMax(true);
                cIndvs[i].speed = p[loop].getSpeed();
                cIndvs[i].distEvol = p[loop].getDistanceEvol();
                cIndvs[i].actionOffset = p[loop].getLastAction();
                cIndvs[i].specId = p[loop].getSpecies();
                cIndvs[i].reprodFail1 = p[loop].reproduceFailed1;
                cIndvs[i].reprodFail2 = p[loop].reproduceFailed2;
                cIndvs[i].reprodFail3 = p[loop].reproduceFailed3;
                cIndvs[i].reprodFail4 = p[loop].reproduceFailed4;
                cIndvs[i].reprodFail5 = p[loop].reproduceFailed5;
                cIndvs[i].trans = 0;

                for (list<long>::iterator it = preyChangedSp.begin(); it != preyChangedSp.end(); ++it) {
                    if ((*it) == p[loop].getID())
                        cIndvs[i].trans= 1;
                }
                for (int j = 0; j < rows; j++)
                    actvs[i][j] = p[loop].getFCM()->get_activation(j);
                i++;
            }
        }


        memtype = H5Tcreate (H5T_COMPOUND, sizeof (CurrIndv));
        status = H5Tinsert (memtype, "Id", HOFFSET (CurrIndv, id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "X", HOFFSET (CurrIndv, x), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Y", HOFFSET (CurrIndv, y), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Energy", HOFFSET (CurrIndv, energy), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "Age", HOFFSET (CurrIndv, age), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "MaxAge", HOFFSET (CurrIndv, maxage), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Speed", HOFFSET (CurrIndv, speed), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "DistEvolution", HOFFSET (CurrIndv, distEvol), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "ActionOffset", HOFFSET (CurrIndv, actionOffset), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "SpecId", HOFFSET (CurrIndv, specId), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "ReprodFail1", HOFFSET (CurrIndv, reprodFail1), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "ReprodFail2", HOFFSET (CurrIndv, reprodFail2), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "ReprodFail3", HOFFSET (CurrIndv, reprodFail3), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "ReprodFail4", HOFFSET (CurrIndv, reprodFail4), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "ReprodFail5", HOFFSET (CurrIndv, reprodFail5), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "Trans", HOFFSET (CurrIndv, trans), H5T_NATIVE_SHORT);

        filetype = H5Tcreate (H5T_COMPOUND, 1*8 + 8*4 + 7*2);
        status = H5Tinsert (filetype, "Id", 0, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "X", 8, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Y", 8+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Energy", 8+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "Age", 8+2+2+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "MaxAge", 8+2+2+4+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Speed", 8+2+2+4+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "DistEvolution", 8+2+2+4+2+2+4, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "ActionOffset", 8+2+2+4+2+2+4+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "SpecId", 8+2+2+4+2+2+4+4+2, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "ReprodFail1", 8+2+2+4+2+2+4+4+2+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "ReprodFail2", 8+2+2+4+2+2+4+4+2+4+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "ReprodFail3", 8+2+2+4+2+2+4+4+2+4+2+4, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "ReprodFail4", 8+2+2+4+2+2+4+4+2+4+2+4+4, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "ReprodFail5", 8+2+2+4+2+2+4+4+2+4+2+4+4+4, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "Trans", 8+2+2+4+2+2+4+4+2+4+2+4+4+4+4,  H5T_STD_I16BE);

        hsize_t dims[1] = {currIndvCount};
        space = H5Screate_simple (1, dims, NULL);
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 1, dims);
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_CurrIndv", filetype, space, plist);//, H5P_DEFAULT, H5P_DEFAULT);
        status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, cIndvs);
        status = H5Pclose (plist);
        status = H5Tclose (memtype);
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Tclose (filetype);

        // Write Activation level of concepts for all individuals.

        hsize_t     dims3[2] = {currIndvCount, rows};
        space = H5Screate_simple (2, dims3, NULL);
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 2, dims3);
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_activations", H5T_IEEE_F32BE, space, plist);//,H5P_DEFAULT,H5P_DEFAULT);
        status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, actvs);
        status = H5Pclose (plist);
        status = H5Dclose (dset);
        status = H5Sclose (space);

        status = H5Gclose (group);
    }

}

template <class T>
void SaveResults::WrCompressedCurrIndv(vector <T>& p, list<long>& preyChangedSp, const char* g, int currIndvCount, long PrevIndvIndex)
{
    if (currIndvCount > 0) {
        CurrIndv_C cIndvs[currIndvCount];
        group = H5Gopen(file, g);

        int i=0;
        for (int loop=0; loop< p.size(); loop++){
            if ( p[loop].getID() <= PrevIndvIndex ){
                cIndvs[i].id = p[loop].getID();
                cIndvs[i].x = p[loop].getPosition()->x;
                cIndvs[i].y = p[loop].getPosition()->y;
                cIndvs[i].energy = p[loop].getEnergy();
                cIndvs[i].age = p[loop].getAge();
                cIndvs[i].specId = p[loop].getSpecies();
                cIndvs[i].trans = 0;

                for (list<long>::iterator it = preyChangedSp.begin(); it != preyChangedSp.end(); ++it) {
                    if ((*it) == p[loop].getID())
                        cIndvs[i].trans= 1;
                }
                i++;
            }
        }

        memtype = H5Tcreate (H5T_COMPOUND, sizeof (CurrIndv_C));
        status = H5Tinsert (memtype, "Id", HOFFSET (CurrIndv_C, id), H5T_NATIVE_LONG);
        status = H5Tinsert (memtype, "X", HOFFSET (CurrIndv_C, x), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Y", HOFFSET (CurrIndv_C, y), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "Energy", HOFFSET (CurrIndv_C, energy), H5T_NATIVE_FLOAT);
        status = H5Tinsert (memtype, "Age", HOFFSET (CurrIndv_C, age), H5T_NATIVE_SHORT);
        status = H5Tinsert (memtype, "SpecId", HOFFSET (CurrIndv_C, specId), H5T_NATIVE_INT);
        status = H5Tinsert (memtype, "Trans", HOFFSET (CurrIndv_C, trans), H5T_NATIVE_SHORT);

        filetype = H5Tcreate (H5T_COMPOUND, 1*8 + 2*4 + 4*2);
        status = H5Tinsert (filetype, "Id", 0, H5T_STD_I64BE);
        status = H5Tinsert (filetype, "X", 8, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Y", 8+2, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "Energy", 8+2+2, H5T_IEEE_F32BE );
        status = H5Tinsert (filetype, "Age", 8+2+2+4, H5T_STD_I16BE);
        status = H5Tinsert (filetype, "SpecId", 8+2+2+4+2, H5T_STD_I32BE);
        status = H5Tinsert (filetype, "Trans", 8+2+2+4+2+4,  H5T_STD_I16BE);

        hsize_t dims[1] = {currIndvCount};
        space = H5Screate_simple (1, dims, NULL);
        plist = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(plist, 1, dims);
        H5Pset_deflate( plist, COMPRESS_LEVEL);
        dset = H5Dcreate (group, "DS_CurrIndv", filetype, space, plist);
        status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, cIndvs);
        status = H5Pclose (plist);
        status = H5Tclose (memtype);
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Tclose (filetype);

        status = H5Gclose (group);
    }


}


template <class T>
void SaveResults::WrIndv(vector <T>& p, const char* g)
{
	group = H5Gopen(file, g);
	int size = p.size();
	hsize_t dims[1] = {size};
	Indv    wdata2[size];

	int rows = p[0].getFCM()->get_rows();
	int cols = p[0].getFCM()->get_cols();

	short FCM[size][rows][cols]; // FCM of all individuals.
	float actvs [size][rows + 2]; // +2 i
	short phGenome[size][T::nbGenes]; // physical Genome of all individuals.

	for (int i=0; i< p.size(); i++){
		wdata2[i].id = p[i].getID();
		wdata2[i].x = p[i].getPosition()->x;
		wdata2[i].y = p[i].getPosition()->y;
		wdata2[i].energy = p[i].getEnergy();
		wdata2[i].age = p[i].getAge();
		wdata2[i].maxage = p[i].getAgeMax(true);
		wdata2[i].speed = p[i].getSpeed();
		wdata2[i].distEvol = p[i].getDistanceEvol();
		wdata2[i].lastAction = p[i].getLastAction();
		wdata2[i].actionOffset = p[i].get_action_offset();
		wdata2[i].specId = p[i].getSpecies();
		wdata2[i].specP1Id = p[i].getSpeciesParent1ID();
		wdata2[i].specP2Id = p[i].getSpeciesParent2ID();
		wdata2[i].parent1Id = p[i].getParent1ID();
		wdata2[i].parent2Id = p[i].getParent2ID();
		wdata2[i].conceptIndex = p[i].getFCM()->ConceptIndex;
		wdata2[i].conceptMultiplier = p[i].getFCM()->ConceptMultiplier;
		wdata2[i].gender = p[i].getGender();
		wdata2[i].persuasion = (short)(p[i].getPersuasionChar());

		for (size_t j = 0; j < rows; j++)
			for (size_t k = 0; k < cols; k++)
				FCM[i][j][k] = p[i].getFCM()->getchar_chart(j, k);
		for (size_t j = 0; j < T::nbGenes ; j++)
			phGenome[i][j] = (short)(p[i].getPhGenome())->getGene(j);

		for (size_t j = 0; j < (p[i].getFCM())->get_rows(); j++) {
			actvs[i][j]= (p[i].getFCM())->get_activation(j);
		}
		actvs[i][rows] = (p[i].getFCM())->ConceptIndex;
		actvs[i][rows+1] = (p[i].getFCM())->ConceptMultiplier;
	}

	memtype = H5Tcreate (H5T_COMPOUND, sizeof (Indv));
	status = H5Tinsert (memtype, "Id", HOFFSET (Indv, id), H5T_NATIVE_LONG);
	status = H5Tinsert (memtype, "X", HOFFSET (Indv, x), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Y", HOFFSET (Indv, y), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Energy", HOFFSET (Indv, energy), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "Age", HOFFSET (Indv, age), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "MaxAge", HOFFSET (Indv, maxage), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Speed", HOFFSET (Indv, speed), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "DistEvolution", HOFFSET (Indv, distEvol), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "LastAction", HOFFSET (Indv, lastAction), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "ActionOffset", HOFFSET (Indv, actionOffset), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "SpecId", HOFFSET (Indv, specId), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "SpecP1Id", HOFFSET (Indv, specP1Id), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "SpecP2Id", HOFFSET (Indv, specP2Id), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Parent1Id", HOFFSET (Indv, parent1Id), H5T_NATIVE_LONG);
	status = H5Tinsert (memtype, "Parent2Id", HOFFSET (Indv, parent2Id), H5T_NATIVE_LONG);
	status = H5Tinsert (memtype, "ConceptIndex", HOFFSET (Indv, conceptIndex), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "ConceptMultiplier", HOFFSET (Indv, conceptMultiplier), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "Gender", HOFFSET (Indv, gender), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Persuasion", HOFFSET (Indv, persuasion), H5T_NATIVE_SHORT);


	filetype = H5Tcreate (H5T_COMPOUND, 3*8 + 7*4 + 8*2);
	status = H5Tinsert (filetype, "Id", 0, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "X", 8, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "Y", 8+2, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "Energy", 8+2+2, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "Age", 8+2+2+4, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "MaxAge", 8+2+2+4+2, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "Speed", 8+2+2+4+2+2, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "DistEvolution", 8+2+2+4+2+2+4, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "LastAction", 8+2+2+4+2+2+4+4, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "ActionOffset", 8+2+2+4+2+2+4+4+2, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "SpecId", 8+2+2+4+2+2+4+4+2+2, H5T_STD_I32BE);
	status = H5Tinsert (filetype, "SpecP1Id", 8+2+2+4+2+2+4+4+2+2+4, H5T_STD_I32BE);
	status = H5Tinsert (filetype, "SpecP2Id", 8+2+2+4+2+2+4+4+2+2+4+4, H5T_STD_I32BE);
	status = H5Tinsert (filetype, "Parent1Id", 8+2+2+4+2+2+4+4+2+2+4+4+4, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "Parent2Id", 8+2+2+4+2+2+4+4+2+2+4+4+4+8, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "ConceptIndex", 8+2+2+4+2+2+4+4+2+2+4+4+4+8+8, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "ConceptMultiplier", 8+2+2+4+2+2+4+4+2+2+4+4+4+8+8+2, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "Gender", 	       8+2+2+4+2+2+4+4+2+2+4+4+4+8+8+2+4, H5T_STD_I16BE );
	status = H5Tinsert (filetype, "Persuasion", 8+2+2+4+2+2+4+4+2+2+4+4+4+8+8+2+4+2, H5T_STD_I16BE );


	space = H5Screate_simple (1, dims, NULL);
	// for compression
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 1, dims);
	//
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, "DS_Indv", memtype, space, H5P_DEFAULT );// plist);


	status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

	status = H5Pclose(plist);
	status = H5Tclose (memtype);
	status = H5Dclose (dset);
	status = H5Sclose (space);
	status = H5Tclose (filetype);



	// Write FCM of all individuals.

	hsize_t     dims2[3] = {size, rows, cols};
	space = H5Screate_simple (3, dims2, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 3, dims2);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, "DS_FCM", H5T_STD_I16BE, space, H5P_DEFAULT);//plist);
	status = H5Dwrite (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, FCM);
	status = H5Pclose(plist);
	status = H5Dclose (dset);
	status = H5Sclose (space);

	// Write Physical Genome of all individuals.

	hsize_t     dims3[2] = {size, T::nbGenes};
	space = H5Screate_simple (2, dims3, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 2, dims3);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, "DS_phGenome", H5T_STD_I16BE, space,H5P_DEFAULT);// plist);
	status = H5Dwrite (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, phGenome);
	status = H5Pclose(plist);
	status = H5Dclose (dset);
	status = H5Sclose (space);

	hsize_t     dims4[2] = {size, rows + 2};
	space = H5Screate_simple (2, dims4, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 2, dims4);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, "DS_ActivationLevel", H5T_IEEE_F32BE, space,H5P_DEFAULT);// plist);
	status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, actvs);
    status = H5Pclose(plist);
	status = H5Dclose (dset);
	status = H5Sclose (space);

	status = H5Gclose (group);

}
#endif

