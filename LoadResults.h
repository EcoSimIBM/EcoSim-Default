 #ifndef LOADRESULTS_H
 #define LOADRESULTS_H

 #include "hdf5.h"
  #include <list>
 #include <vector>
 #include <string>
 #include "structs.h"


 using namespace std;



 class LoadResults
 {
     private:
         hid_t      file, group, space, dset, strtype, memtype, filetype, plist;         /* Handles */
         herr_t      status;
	 public:
         LoadResults(const char*);
         ~LoadResults();


         void RdGen(Gen&); // maxsave
         void RdSpecExt(SpecExt[], const char*, int&);

         int getPopInfo(const char*, short&, short& , short& );
         int getSpecInfo(const char*);
         void RdIndv1(const char*, Indv wdata2[], short FCM[][42][18], short phGenome[][8], float actvs[][42+2]);
		 void RdWorld(vector<vector<vector<float> > >&, const char*);
         void RdIntArr (int arr[], int arr_size, const char*, const char*);
         void RdFloatArr (float arr[], int arr_size, const char*, const char*);
         void RdDeadIndv(DeadIndv[], int, const char*);


 };

 #endif // LOADRESULTS_H
