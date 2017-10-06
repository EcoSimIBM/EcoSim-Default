//#ifdef HDF5_COMPILE
#include "SaveResults.h"

SaveResults::SaveResults(const char* fname)
{
	//ctor
	file = H5Fopen (fname, H5F_ACC_RDWR, H5P_DEFAULT);
	if (file < 0)
		printf("Error in opening file %s", fname);
}

void SaveResults::CreateFile(const char* fn, short gen )
{
	char fname[20];
	sprintf(fname,"%s%i.h5", fn, gen );
	file = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

}

void SaveResults::CreateMaxSave(const char* fname)
{
	file = H5Fcreate (fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
}

void SaveResults::OpenMaxSave(const char* fname)
{
	file = H5Fopen (fname, H5F_ACC_RDWR, H5P_DEFAULT);
}
void SaveResults::CreateMinSaveGroups()
{

	group = H5Gcreate (file, "/G0_MaxSpecNum", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G1_ExtPreySpec", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G2_NewPreySpec", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G3_ExtPredSpec", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G4_NewPredSpec", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G5_CurrPrey", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G6_NewPrey", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G7_DeadPrey", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G8_CurrPred", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G9_NewPred", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G10_DeadPred", H5P_DEFAULT);
	status = H5Gclose (group);
}

void SaveResults::CreateCompressedMinSaveGroups(int gen)
{
	char g[20];
	char tmpg[20];
	sprintf(g,"/G_Gen%i",gen );

	group = H5Gcreate (file, g, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G0_MaxSpecNum", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G1_ExtPreySpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G2_NewPreySpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G3_ExtPredSpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G4_NewPredSpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G5_CurrPrey", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G6_NewPrey", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G7_CurrPred", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G8_NewPred", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

}

void SaveResults::CreateMinSaveGroups(int gen)
{
	char g[20];
	char tmpg[20];
	sprintf(g,"/G_Gen%i",gen );

	group = H5Gcreate (file, g, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G0_MaxSpecNum", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G1_ExtPreySpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G2_NewPreySpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G3_ExtPredSpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G4_NewPredSpec", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G5_CurrPrey", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G6_NewPrey", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G7_DeadPrey", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G8_CurrPred", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G9_NewPred", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G10_DeadPred", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);

	sprintf(tmpg, "%s/G11_World", g);
	group = H5Gcreate (file, tmpg, H5P_DEFAULT);
	status = H5Gclose (group);
}

void SaveResults::CreateMaxSaveGroups()
{
	group = H5Gcreate (file, "/G0_MaxIndvId", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G1_MaxSpecNum", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G2_Prey", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G3_Pred", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G4_World", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G5_PreySpec", H5P_DEFAULT);
	status = H5Gclose (group);

	group = H5Gcreate (file, "/G6_PredSpec", H5P_DEFAULT);
	status = H5Gclose (group);


}

void SaveResults::WrGen(Gen gn)
{
	hsize_t dims[1] = {1};
	Gen	wdata2[1];
	wdata2[0] = gn;

	/*
	 * Create variable length string data type.
	 */
	strtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (strtype, H5T_VARIABLE);

	/*
	 * Create the compound datatype for memory.
	 */
	memtype = H5Tcreate (H5T_COMPOUND, sizeof (Gen));
	status = H5Tinsert (memtype, "Generation Number", HOFFSET (Gen, v1), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String1", HOFFSET (Gen, st1), strtype);

	/*
	 * Create the compound datatype for the file.  Because the standard
	 * types we are using for the file may have different sizes than
	 * the corresponding native types, we must manually calculate the
	 * offset of each member.
	 */
	filetype = H5Tcreate (H5T_COMPOUND, 8 + sizeof (hvl_t) );
	status = H5Tinsert (filetype, "Generation Number", 0, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "Fixed String1", 8, strtype);

	/*
	 * Create dataspace.  Setting maximum size to NULL sets the maximum
	 * size to be the current size.
	 */
	space = H5Screate_simple (1, dims, NULL);


	/*
	 * Create the dataset and write the compound data to it.
	 */
	dset = H5Dcreate (file, "DS_Gen", filetype, space, H5P_DEFAULT);
	status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata2);

	status = H5Tclose (memtype);
	status = H5Dclose (dset);
	status = H5Sclose (space);
	status = H5Tclose (filetype);

}
void SaveResults::WrSpecExt(SpecExt se[], int size, const char* g)
{

	group = H5Gopen(file, g );

	hsize_t dims[1] = {size};


	/*
	 * Create variable length string data type.
	 */
	strtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (strtype, H5T_VARIABLE);

	/*
	 * Create the compound datatype for memory.
	 */
	memtype = H5Tcreate (H5T_COMPOUND, sizeof (SpecExt));
	status = H5Tinsert (memtype, "Species Id", HOFFSET (SpecExt, v1), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String1", HOFFSET (SpecExt, st1), strtype);
	status = H5Tinsert (memtype, "Parent Id", HOFFSET (SpecExt, v2), H5T_NATIVE_INT);
	status = H5Tinsert (memtype, "Fixed String2", HOFFSET (SpecExt, st2), strtype);

	/*
	 * Create the compound datatype for the file.  Because the standard
	 * types we are using for the file may have different sizes than
	 * the corresponding native types, we must manually calculate the
	 * offset of each member.
	 */
	filetype = H5Tcreate (H5T_COMPOUND, 8 + sizeof (hvl_t) + 8 + sizeof (hvl_t) );
	status = H5Tinsert (filetype, "Species Id", 0, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "Fixed String1", 8, strtype);
	status = H5Tinsert (filetype, "Parent Id", 8 + sizeof (hvl_t), H5T_STD_I64BE);
	status = H5Tinsert (filetype, "Fixed String2", 8 + sizeof (hvl_t) + 8, strtype);

	/*
	 * Create dataspace.  Setting maximum size to NULL sets the maximum
	 * size to be the current size.
	 */
	space = H5Screate_simple (1, dims, NULL);


	/*
	 * Create the dataset and write the compound data to it.
	 */
	dset = H5Dcreate (group, "DS_ExtPreySpec", filetype, space, H5P_DEFAULT);
	status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, se);

	status = H5Tclose (memtype);
	status = H5Dclose (dset);
	status = H5Sclose (space);
	status = H5Tclose (filetype);
	status = H5Gclose (group);
}


void SaveResults::WrDeadIndv(DeadIndv di[], int size, const char* g)
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

	filetype = H5Tcreate (H5T_COMPOUND, 1*8 + 4*4 + 3*2);
	status = H5Tinsert (filetype, "SpecId", 0, H5T_STD_I32BE);
	status = H5Tinsert (filetype, "Id", 4, H5T_STD_I64BE);
	status = H5Tinsert (filetype, "DeathType",4+8, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "LastAction", 4+8+2, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "Age", 4+8+2+2, H5T_STD_I16BE);
	status = H5Tinsert (filetype, "Energy", 4+8+2+2+2, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "DistEvolution", 4+8+2+2+2+4, H5T_IEEE_F32BE );
	status = H5Tinsert (filetype, "Speed", 4+8+2+2+2+4+4, H5T_IEEE_F32BE );

	space = H5Screate_simple (1, dims, NULL);

	dset = H5Dcreate (group, "DS_DeadIndv", filetype, space, H5P_DEFAULT);
	status = H5Dwrite (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, di);

	status = H5Tclose (memtype);
	status = H5Dclose (dset);
	status = H5Sclose (space);
	status = H5Tclose (filetype);
	status = H5Gclose (group);
}


void SaveResults::WrIntArr (int arr[], int arr_size, const char* g, const char* ds_name)
{
	group = H5Gopen(file, g );
	hsize_t     dims[1] = {arr_size};
	space = H5Screate_simple (1, dims, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 1, dims);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, ds_name , H5T_NATIVE_INT, space, plist);

	status = H5Dwrite (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, arr);

	status = H5Pclose(plist);
	status = H5Sclose(space);
	status = H5Dclose(dset);
	status = H5Gclose (group);
}

void SaveResults::WrFloatArr (float arr[], int arr_size, const char* g, const char* ds_name)
{
	group = H5Gopen(file, g );
	hsize_t     dims[1] = {arr_size};
	space = H5Screate_simple (1, dims, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 1, dims);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, ds_name , H5T_NATIVE_FLOAT, space, plist);
	status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, arr);
	status = H5Pclose(plist);
	status = H5Sclose(space);
	status = H5Dclose(dset);
	status = H5Gclose (group);
}

void SaveResults::WrWorld(vector<vector<vector<float> > >& world, const char* g){
	short rows = world.size();
	short cols = world[0].size();
	short heights = world[0][0].size();

	float wdata[rows][cols][heights] ;

	group = H5Gopen(file, g );

	for ( short i = 0; i < rows; i++ )
	for ( short j  = 0; j < cols; j++ )
	for ( short k = 0; k < heights; k++ )
		wdata[i][j][k] = world[i][j][k];

	hsize_t     dims2[3] = {rows, cols, heights};
	space = H5Screate_simple (3, dims2, NULL);
	plist = H5Pcreate(H5P_DATASET_CREATE);
	status = H5Pset_chunk(plist, 3, dims2);
	H5Pset_deflate( plist, COMPRESS_LEVEL);
	dset = H5Dcreate (group, "DS_WORLD", H5T_IEEE_F32BE, space, plist);
	status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, wdata);
	status = H5Pclose(plist);
	status = H5Dclose (dset);
	status = H5Sclose (space);
	status = H5Gclose(group);

}

SaveResults::~SaveResults()
{
	//dtor
	status = H5Fclose (file);
}
//#endif
