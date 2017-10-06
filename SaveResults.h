#ifndef SAVERESULTS_H
#define SAVERESULTS_H

#include "hdf5.h"
#include "Prey.h"
#include <vector>
#include <string>

#include "structs.h"


using namespace std;



class SaveResults
{
    private:
        hid_t      file, group, space, dset, strtype, memtype, filetype, plist;         /* Handles */
        herr_t      status;
	public:
	    SaveResults(){};
        SaveResults(const char*);
        ~SaveResults();
        void CreateFile(const char*, short);
        void CreateMaxSave(const char*);
        void CreateMinSaveGroups();
        void CreateMinSaveGroups(int);
        void CreateCompressedMinSaveGroups(int);
		void CreateMaxSaveGroups();
		void OpenMaxSave(const char*);
        void WrGen(Gen); // maxsave
        void WrSpecExt(SpecExt[], int, const char*);
		template <class T>
        void WrCurrIndv(vector <T>&, list<long>&, const char*, int, long);
        template <class T>
        void WrCompressedCurrIndv(vector <T>&, list<long>&, const char*, int, long);
		template <class T>
        void WrNewIndv(vector <T>&, const char*, int, long);
        template <class T>
        void WrCompressedNewIndv(vector <T>&, const char*, int, long);
		template <class T>
        void WrIndv(vector <T>&, const char*); // use for maxsave
		void WrWorld(vector<vector<vector<float> > >&, const char*);
        void WrIntArr (int arr[], int arr_size, const char*, const char*);
        void WrFloatArr (float arr[], int arr_size, const char*, const char*);
        void WrDeadIndv(DeadIndv[], int, const char*);


};

#include "templ_imp.h"
#endif // SAVERESULTS_H
