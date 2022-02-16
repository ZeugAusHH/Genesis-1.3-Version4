#include "genesis.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>

//#include "VersionInfo.h"
#include "version.h"
using namespace std;

// very basic wrapper for genesis. Most of the genesis stuff is moved into genmain.
#include "hdf5.h"

int main (int argc, char *argv[]) {


    //-------------------------------------------------------
    // init MPI and get size etc.
    //
    MPI_Init(&argc, &argv); //MPI

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // assign rank to node
    string filename (argv[argc-1]);  // input file is always last element
    map<string,string> arguments;
    bool ok=true;


    hid_t pid = H5Pcreate(H5P_FILE_ACCESS);
    string file = "test.h5";
//    if (!MPISingle){
//        H5Pset_fapl_mpio(pid,MPI_COMM_WORLD,MPI_INFO_NULL);
//    }
//    hid_t fid=H5Fcreate(file.c_str(),H5F_ACC_TRUNC, H5P_DEFAULT,pid);
    H5Pclose(pid);
//    H5Fclose(fid);
    MPI_Finalize(); // node turned off

    return 0;

	// parse the command line arguments

    if (rank == 0) {
        VersionInfo vi;
        cout << "---------------------------------------------" << endl;
        cout << "GENESIS - Version " << vi.Major() << "." << vi.Minor() << "." << vi.Rev();
        if (vi.isBeta()) { cout << " (beta)"; }
        cout << " has started..." << endl;
        cout << "Compile info: " << vi.Build() << endl;
    }

	if (argc == 1){
        if (rank==0) {
            cout << "*** Error: No input file specified - Execution of Genesis will end" << endl;
        }
	} else {
	  for (int i = 1; i < argc; i++){
          if ((strcmp(argv[i],"-h") ==0) || (strcmp(argv[argc-1],"-h")==0)) {
                ok = false;
                if (rank == 0) {
                    cout << endl << "-------------------------------------------------------------------------------"<< endl;
                    cout << "Usage: genesis4 [-o Output] [-l Lattice] [-b Beamline] [-s Seed] [-h] Input" << endl;
                    cout << "        -o Output:  equivalent to 'rootname' in the setup namelist " << endl;
                    cout << "        -l Lattice:  equivalent to 'lattice' in the setup namelist " << endl;
                    cout << "        -b Beamline: equivalent to 'beamline' in the setup namelist " << endl;
                    cout << "        -s Seed: equivalent to 'seed' in the setup namelist " << endl;
                    cout << "        -h: prints help-info " << endl;
                    cout << "  Any specified command line argument will overwrite" << endl;
                    cout << "  the corresponding entry in the setup namelist" << endl;
                    cout << "-------------------------------------------------------------------------------" << endl;
                }
                break;
          }
          // checking for the individual commandline argumen
          if (strcmp(argv[i],"-o")==0) {     // output file
              arguments["rootname"] = argv[i + 1];
              i++;
              continue;
          }
          if (strcmp(argv[i],"-l")==0) {     // output file
              arguments["lattice"] = argv[i + 1];
              i++;
              continue;
          }
          if (strcmp(argv[i],"-b")==0) {     // output file
              arguments["beamline"] = argv[i + 1];
              i++;
              continue;
          }
          if (strcmp(argv[i],"-s")==0) {     // output file
              arguments["seed"] = argv[i + 1];
              i++;
              continue;
          }
          // an unrecognized input is encountered
          if (i < (argc-1)) {
              if (rank == 0) {
                  cout << "*** Error: Invalid command line argument"<< endl;
                  cout << "*** Error: Argument " << argv[i] << " is not recognized" << endl;
                  cout << "*** Error: Execution of Genesis will end." << endl;
              }
              ok = false;
              break;
          }
	  }

      //   call the core routine for genesis
	  if (ok) {genmain(filename,arguments,false); }   //
	}

	MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize(); // node turned off

    return 0;

}
