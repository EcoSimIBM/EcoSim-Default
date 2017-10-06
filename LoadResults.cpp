//#ifdef HDF5_COMPILE
#include "LoadResults.h"
#include "iostream"
#include <stdlib.h>

LoadResults::LoadResults(const char* fname)
{
	//ctor
	file = H5Fopen (fname, H5F_ACC_RDONLY, H5P_DEFAULT);
}

void LoadResults::RdGen(Gen &gn)
{
	hsize_t dims[1] = {1};
	Gen	wdata2[1];

	strtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (strtype, H5T_VARIABLE);

	memtype = H5Tcreate (H5T_COMPOUND, sizeof (Gen));
	status = H5Tinsert (memtype, "Generation Number", HOFFSET (Gen, v1), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String1", HOFFSET (Gen, st1), strtype);

	dset = H5Dopen (file, "DS_Gen");
	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

	gn = wdata2[0];

	status = H5Tclose (memtype);
	status = H5Dclose (dset);
}
void LoadResults::RdSpecExt(SpecExt se[], const char* g, int& count)
{

	group = H5Gopen(file, g );


	strtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (strtype, H5T_VARIABLE);

	memtype = H5Tcreate (H5T_COMPOUND, sizeof (SpecExt));
	status = H5Tinsert (memtype, "Species Id", HOFFSET (SpecExt, v1), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String1", HOFFSET (SpecExt, st1), strtype);
	status = H5Tinsert (memtype, "Parent Id", HOFFSET (SpecExt, v2), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String2", HOFFSET (SpecExt, st2), strtype);


	dset = H5Dopen (group, "DS_ExtPreySpec");
	space = H5Dget_space(dset);
	int ndims = H5Sget_simple_extent_ndims(space);

	hsize_t dims[ndims];
	hsize_t maxdims[ndims];
	H5Sget_simple_extent_dims (space, dims, maxdims);

	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, se);
	count = dims[0];

	status = H5Tclose (memtype);
	status = H5Dclose (dset);
	status = H5Gclose (group);
}


void LoadResults::RdDeadIndv(DeadIndv di[], int size, const char* g)
{
	group = H5Gopen(file, g );

	hsize_t dims[1] = {size};

	memtype = H5Tcreate (H5T_COMPOUND, sizeof (DeadIndv));
	status = H5Tinsert (memtype, "SpecId", HOFFSET (DeadIndv, specId), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Id", HOFFSET (DeadIndv, id), H5T_NATIVE_LONG);
	status = H5Tinsert (memtype, "DeathType", HOFFSET (DeadIndv, deathType), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "LastAction", HOFFSET (DeadIndv, lastAction), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Age", HOFFSET (DeadIndv, age), H5T_NATIVE_SHORT);
	status = H5Tinsert (memtype, "Energy", HOFFSET (DeadIndv, energy), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "DistEvolution", HOFFSET (DeadIndv, distEvol), H5T_NATIVE_FLOAT);
	status = H5Tinsert (memtype, "Speed", HOFFSET (DeadIndv, speed), H5T_NATIVE_FLOAT);

	dset = H5Dopen (group, "DS_DeadIndv");
	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, di);

	status = H5Tclose (memtype);
	status = H5Dclose (dset);

	status = H5Gclose (group);
}


void LoadResults::RdIntArr (int arr[], int arr_size, const char* g, const char* ds_name)
{
	group = H5Gopen(file, g );
	hsize_t     dims[1] = {arr_size};

	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 1, dims);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dopen2(group, ds_name, plist);

	status = H5Dread (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, arr);

	status = H5Pclose(plist);

	status = H5Dclose(dset);
	status = H5Gclose (group);
}

void LoadResults::RdFloatArr (float arr[], int arr_size, const char* g, const char* ds_name)
{
	group = H5Gopen(file, g );
	hsize_t     dims[1] = {arr_size};

	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 1, dims);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dopen2 (group, ds_name , plist);
	status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, arr);
	status = H5Pclose(plist);

	status = H5Dclose(dset);
	status = H5Gclose (group);
}

void LoadResults::RdWorld(vector<vector<vector<float> > >& world, const char* g){
	short rows = world.size();
	short cols = world[0].size();
	short heights = world[0][0].size();

	float wdata[rows][cols][heights] ;

	group = H5Gopen(file, g );

	hsize_t     dims2[3] = {rows, cols, heights};

	dset = H5Dopen (group, "DS_WORLD");
	status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata);

	status = H5Dclose (dset);

	status = H5Gclose(group);

	for ( short i = 0; i < rows; i++ )
	for ( short j  = 0; j < cols; j++ )
	for ( short k = 0; k < heights; k++ )
		world[i][j][k] = wdata[i][j][k];
}

int LoadResults::getSpecInfo(const char* g)
{
	group = H5Gopen(file, g);
	dset = H5Dopen (group, "DS_ExtPreySpec");
	space = H5Dget_space(dset);
	int ndims = H5Sget_simple_extent_ndims(space);
	hsize_t dims[ndims];
	hsize_t maxdims[ndims];
	H5Sget_simple_extent_dims (space, dims, maxdims);
	int pop = dims[0];
	H5Dclose(dset);
	H5Gclose(group);
	return pop;

}
int LoadResults::getPopInfo(const char* g, short& FCM_row, short& FCM_col, short& genomeSize)
{
	group = H5Gopen(file, g);

	dset = H5Dopen (group, "DS_Indv");
	space = H5Dget_space(dset);
	int ndims = H5Sget_simple_extent_ndims(space);
	hsize_t dims[ndims];
	hsize_t maxdims[ndims];
	H5Sget_simple_extent_dims (space, dims, maxdims);
	int pop = dims[0];
	H5Dclose(dset);

	dset = H5Dopen (group, "DS_FCM");
	space = H5Dget_space(dset);
	ndims = H5Sget_simple_extent_ndims(space);
	hsize_t dims1[ndims];
	hsize_t maxdims1[ndims];
	H5Sget_simple_extent_dims (space, dims1, maxdims1);
	FCM_row=dims1[1];
	FCM_col=dims1[2];
	H5Dclose(dset);

	dset = H5Dopen (group, "DS_phGenome");
	space = H5Dget_space(dset);
	ndims = H5Sget_simple_extent_ndims(space);
	hsize_t dims2[ndims];
	hsize_t maxdims2[ndims];
	H5Sget_simple_extent_dims (space, dims2, maxdims2);
	genomeSize = dims2[1];
	H5Dclose(dset);

	H5Gclose(group);

	return pop;

}

void LoadResults::RdIndv1(const char* g, Indv wdata2[], short FCM[][42][18], short phGenome[][8], float actvs[][42+2])
{
	group = H5Gopen(file, g);
	dset = H5Dopen (group, "DS_Indv");


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
	status = H5Tinsert(memtype, "Gender", HOFFSET(Indv, gender), H5T_NATIVE_SHORT);


	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

	status = H5Tclose (memtype);

	status = H5Dclose (dset);

	dset = H5Dopen (group, "DS_FCM");
	status = H5Dread (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, FCM);

	status = H5Dclose (dset);



	// Read Physical Genome of all individuals.
	dset = H5Dopen (group, "DS_phGenome");
	status = H5Dread (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, phGenome);

	status = H5Dclose (dset);

	dset = H5Dopen (group, "DS_ActivationLevel");
	status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, actvs);

	status = H5Dclose (dset);
	status = H5Gclose (group);

}

LoadResults::~LoadResults()
{
	//dtor
	status = H5Fclose (file);
}
//#endif
