#ifndef __GENESIS_SETUP__
#define __GENESIS_SETUP__

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <fstream>
#include <cctype>

#include "StringProcessing.h"
#include "Lattice.h"

using namespace std;

class Lattice;

class Setup: public StringProcessing{
 public:
   Setup();
   virtual ~Setup();
   bool init(int, map<string,string> *, Lattice *,string,string,int );
   //   bool writeGlobal(hid_t, double, int, int,int,int,double, double, double, bool, bool);
   double getReferenceLength();
   void   setReferenceLength(double);
   double getReferenceEnergy();
   double getStepLength();
   void   setStepLength(double);
   bool   getOne4One();
   bool   getShotNoise();
   bool   getBeamGlobalStat();
   bool   getFieldGlobalStat();
   bool   outputFFT();
   bool   outputSpatial();
   bool   outputIntensity();
   bool   outputCurrent();
   bool   outputEnergy();
   bool   outputAux();
   bool   outputFieldDump();
   bool   getSemaEn();
   void   setSemaFN(string);
   bool   getSemaFN(string *);

   int    getNpart();
   int    getNbins();
   int    getSeed();
   //   bool   getInputFileNameBeam(string *);
   //   bool   getInputFileNameField(string *);
   bool   getRootName(string *);
   void   setRootName(string *);
   int getCount();
   void incrementCount();
   string getLattice();

 private:
   void usage();
   string rootname,lattice,beamline,partfile,fieldfile;
   double gamma0,lambda0,delz;
   bool one4one,shotnoise;
   bool beam_global_stat, field_global_stat;
   bool exclude_spatial_output, exclude_fft_output, exclude_intensity_output, exclude_energy_output, exclude_aux_output, exclude_current_output, exclude_field_dump;
   int seed, rank,npart,nbins,runcount;

   bool sema_file_enabled;
   string sema_file_name; // user-defined name of semaphore file, if empty: file name will be derived in function getSemaFN
};

inline string Setup::getLattice(){return lattice;}
inline double Setup::getReferenceLength(){ return lambda0; }
inline void   Setup::setReferenceLength(double lam){ lambda0=lam; return; }
inline double Setup::getReferenceEnergy(){ return gamma0; }
inline bool   Setup::getOne4One(){return one4one;}
inline bool   Setup::getShotNoise(){return shotnoise;}
inline bool   Setup::getBeamGlobalStat(){return beam_global_stat;}
inline bool   Setup::getFieldGlobalStat(){return field_global_stat;}
inline int    Setup::getNpart(){return npart;}
inline int    Setup::getNbins(){return nbins;}
inline int    Setup::getSeed(){return seed;}
inline double Setup::getStepLength(){return delz;}
inline void   Setup::setStepLength(double din){delz=din;return;}
inline int    Setup::getCount(){return(runcount);}
inline void   Setup::incrementCount(){runcount++; return;}
inline void   Setup::setRootName(string *newname){rootname=*newname; runcount=0; return;}
inline bool   Setup::outputFFT(){ return exclude_fft_output;}
inline bool   Setup::outputSpatial(){ return exclude_spatial_output;}
inline bool   Setup::outputIntensity(){ return exclude_intensity_output;}
inline bool   Setup::outputEnergy(){ return exclude_energy_output;}
inline bool   Setup::outputCurrent(){ return exclude_current_output;}
inline bool   Setup::outputAux(){ return exclude_aux_output;}
inline bool   Setup::outputFieldDump() { return exclude_field_dump;}
inline bool   Setup::getSemaEn() { return sema_file_enabled; }
#endif
