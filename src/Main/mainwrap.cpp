#include "genesis.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "VersionInfo.h"

using namespace std;

// very basic wrapper for genesis. Most of the genesis stuff is moved into genmain.


int main (int argc, char *argv[]) {


        //-------------------------------------------------------
        // init MPI and get rank etc.
        //

        MPI_Init(&argc, &argv); //MPI

	int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); // assign rank to node

        // information extracted from command line arguments
      	string filename (argv[argc-1]);  // input file is always last element
	string latname  ("");
	string outname  ("");
	int    seed = 123456789;
	
	// parse the command line arguments and launch simulation
	int sim_core_result=1; // non-zero return value signals 'error'
	bool ok=true;
	if (argc == 1){
	  if (rank == 0){
            VersionInfo vi;
            cout << "---------------------------------------------" << endl;
            cout << "GENESIS - Version " <<  vi.Major() <<"."<< vi.Minor() << "." << vi.Rev() ;
            if (vi.isBeta()) {cout << " (beta)";}
            cout << " has started..." << endl;
            cout << "compile info: " << vi.BuildInfo() << endl;

	    cout << "*** Error: No input file specified - Execution of Genesis will end" << endl;
	  }
	  MPI_Barrier(MPI_COMM_WORLD);
	  MPI_Finalize();
	  return(1);
	}
	// else {
	  for (int i = 1; i < argc-1; i++){
	    if (strstr(argv[i],"-o")){     // output file
		if (i < argc-2){
		  outname=argv[i+1];
		  i++;
		  continue;
		}
	    }
	    if (strstr(argv[i],"-l")){
	      if (i < argc-2){         // lattice file
		  latname=argv[i+1];
		  i++;
		  continue;
		}
	    }

	    if (strstr(argv[i],"-s")){
	      if (i < argc-2){        // seed
		seed=atoi(argv[i+1]);
		  i++;
		  continue;
		}
	    }

	    if (rank == 0){
		cout << "*** Error: Invalid command line argument - Execution of Genesis will end" << endl;
	    }
	    ok=false;
	    break;
	  }       
	  if (ok) {
	    sim_core_result=genmain(filename,latname,outname,seed,false);
	  }
	// }
	MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize(); // node turned off

        // FIXME: return result obtained from 'genmain' function (returns 0 if success)
        // return(sim_core_result);
        return(0);
}
