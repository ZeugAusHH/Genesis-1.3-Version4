#include <sstream>
#include "Setup.h"

Setup::Setup()
{
  rootname="";
  lattice="";
  beamline="";
  partfile="";
  fieldfile="";
  one4one=false;
  shotnoise=true;
  nbins=4;
  npart=8192;
  gamma0=5800/0.511;
  lambda0=1e-10;
  delz=0.015; 
  seed=123456789;


  beam_global_stat=false;
  field_global_stat=false;

  exclude_spatial_output=false;
  exclude_fft_output=false;
  exclude_intensity_output=false;
  exclude_energy_output=false;
  exclude_aux_output=false;
  exclude_current_output=true;
  exclude_field_dump=false;

  // filtering of beam slices during dump process (information is forwarded into active instance of Beam class when actually needed there)
  BWF_set_enabled(false);
  BWF_load_defaults();

  // count of runs in conjunction of calls of altersetup
  runcount = 0;
}

Setup::~Setup(){}

void Setup::usage(){

  cout << "List of keywords for SETUP" << endl;
  cout << "&setup" << endl;
  cout << " string rootname = <taken from command line>" << endl;
  cout << " string lattice  = <taken from command line>" << endl;
  cout << " string beamline = <empty>" << endl;
  cout << " double gamma0 = 5800/0.511" << endl;
  cout << " double lambda0 = 1e-10" << endl;
  cout << " double delz = 0.015" << endl;
  cout << " int seed = 123456789" << endl;
  cout << " int npart = 8192" << endl;
  cout << " int nbins = 4" << endl;
  cout << " bool one4one = false" << endl;
  cout << " bool shotnoise = true" << endl;
  cout << " bool beam_global_stat = false" << endl;
  cout << " bool field_global_stat = false" << endl;
  cout << " bool exclude_spatial_output = false" << endl;
  cout << " bool exclude_fft_output = false" << endl;
  cout << " bool exclude_intensity_output = false" << endl;
  cout << " bool exclude_energy_output = false" << endl;
  cout << " bool exclude_aux_output = false" << endl;
  cout << " bool exclude_current_output = true" << endl;
  cout << " bool exclude_field_dump = false" << endl;
  cout << "&end" << endl << endl;
  return;
}

bool Setup::init(int inrank, map<string,string> *arg, Lattice *lat, FilterDiagnostics &filter){

  rank=inrank;
  map<string,string>::iterator end=arg->end();

  if (arg->find("rootname")!=end){rootname = arg->at("rootname"); arg->erase(arg->find("rootname"));}
  if (arg->find("lattice")!=end) {lattice  = arg->at("lattice");  arg->erase(arg->find("lattice"));}
  if (arg->find("beamline")!=end){beamline = arg->at("beamline"); arg->erase(arg->find("beamline"));}
  if (arg->find("lattice")!=end) {lattice  = arg->at("lattice");  arg->erase(arg->find("lattice"));}
  if (arg->find("gamma0")!=end)  {gamma0   = atof(arg->at("gamma0").c_str());  arg->erase(arg->find("gamma0"));}
  if (arg->find("lambda0")!=end) {lambda0  = atof(arg->at("lambda0").c_str()); arg->erase(arg->find("lambda0"));}
  if (arg->find("delz")!=end)    {delz     = atof(arg->at("delz").c_str());  arg->erase(arg->find("delz"));}
  if (arg->find("seed")!=end)    {seed     = atoi(arg->at("seed").c_str());  arg->erase(arg->find("seed"));}
  if (arg->find("one4one")!=end) {one4one  = atob(arg->at("one4one"));  arg->erase(arg->find("one4one"));}
  if (arg->find("npart")!=end)    {npart  = atoi(arg->at("npart").c_str());  arg->erase(arg->find("npart"));}
  if (arg->find("nbins")!=end)    {nbins  = atoi(arg->at("nbins").c_str());  arg->erase(arg->find("nbins"));}
  if (arg->find("shotnoise")!=end){shotnoise  = atob(arg->at("shotnoise"));  arg->erase(arg->find("shotnoise"));}
  if (arg->find("beam_global_stat")!=end)  {beam_global_stat  = atob(arg->at("beam_global_stat"));   arg->erase(arg->find("beam_global_stat"));}
  if (arg->find("field_global_stat")!=end) {field_global_stat = atob(arg->at("field_global_stat"));  arg->erase(arg->find("field_global_stat"));}
  if (arg->find("exclude_spatial_output")!=end)   {exclude_spatial_output  = atob(arg->at("exclude_spatial_output"));   arg->erase(arg->find("exclude_spatial_output"));}
  if (arg->find("exclude_fft_output")!=end)       {exclude_fft_output      = atob(arg->at("exclude_fft_output"));       arg->erase(arg->find("exclude_fft_output"));}
  if (arg->find("exclude_intensity_output")!=end) {exclude_intensity_output= atob(arg->at("exclude_intensity_output")); arg->erase(arg->find("exclude_intensity_output"));}
  if (arg->find("exclude_energy_output")!=end)    {exclude_energy_output   = atob(arg->at("exclude_energy_output"));    arg->erase(arg->find("exclude_energy_output"));}
  if (arg->find("exclude_aux_output")!=end)       {exclude_aux_output      = atob(arg->at("exclude_aux_output"));       arg->erase(arg->find("exclude_aux_output"));}
  if (arg->find("exclude_current_output")!=end)   {exclude_current_output  = atob(arg->at("exclude_current_output"));   arg->erase(arg->find("exclude_current_output"));}
  if (arg->find("exclude_field_dump")!=end)   {exclude_field_dump  = atob(arg->at("exclude_field_dump"));   arg->erase(arg->find("exclude_field_dump"));}

  // same code also in AlterSetup.cpp
  if (arg->find("beam_write_slices_from")!=end) {
    beam_write_slices_from = atoi(arg->at("beam_write_slices_from").c_str());
    arg->erase(arg->find("beam_write_slices_from"));
    beam_write_filter=true; // user can override this if needed
  }
  if (arg->find("beam_write_slices_to")!=end) {
    beam_write_slices_to = atoi(arg->at("beam_write_slices_to").c_str());
    arg->erase(arg->find("beam_write_slices_to"));
    beam_write_filter=true; // user can override this if needed
  }
  if (arg->find("beam_write_slices_inc")!=end) {
    BWF_set_inc(atoi(arg->at("beam_write_slices_inc").c_str())); // setter function does checks if valid value
    arg->erase(arg->find("beam_write_slices_inc"));
    beam_write_filter=true; // user can override this if needed
  }
  if (arg->find("beam_write_slices_filter")!=end) {
    beam_write_filter = atob(arg->at("beam_write_slices_filter"));
    arg->erase(arg->find("beam_write_slices_filter"));
  }

  if (arg->size()!=0){
    if (rank==0){ cout << "*** Error: Unknown elements in &setup" << endl; this->usage();}
    return false;
  }

  // sort the filter flags
  filter.beam.global = beam_global_stat;
  filter.beam.spatial = !exclude_spatial_output;
  filter.beam.energy = !exclude_energy_output;
  filter.beam.current = !exclude_energy_output;
  filter.beam.auxiliar = !exclude_aux_output;
  filter.field.global = field_global_stat;
  filter.field.spatial = !exclude_spatial_output;
  filter.field.fft = !exclude_fft_output;
  filter.field.intensity = !exclude_intensity_output;

  if (one4one)
  {
    nbins = 1;
  }

  return lat->parse(lattice,beamline,rank);
}



bool Setup::getRootName(string *filename)
{
  if (rootname.size()<1){
    return false;
  }
  *filename=rootname;
  if (runcount>0) {
    stringstream ss;
    ss << ".Run" << (runcount+1) ;
    *filename+=ss.str();
  }
  return true; 
}

void Setup::BWF_load_defaults()
{
  // note: only loading defaults, not diabling the filter (if desired, this has to be done in addition)
  beam_write_slices_from=-1;
  beam_write_slices_to=-1;
  beam_write_slices_inc=1;
}
