#ifndef __GENESIS_COLLECTIVE__
#define __GENESIS_COLLECTIVE__

#include <vector>
#include <iostream>
#include <string>
#include <complex>
#include <cmath>

#include <mpi.h>

class Beam;
class Undulator;

extern bool MPISingle; 
extern const double ce;   

using namespace std;


class Collective{
public:
   Collective();
   virtual ~Collective();
   //   void initWake(unsigned int, double, double *, double *, double *, double, double, bool);
   void initWake(unsigned int, unsigned int, double, double *, double *, double *, double *, double, double, bool);
   void clearWake();
   void apply(Beam *,Undulator *, double );
   void update(Beam *, double);
   void forceUpdate();
   [[nodiscard]] bool hasWakeDefined() const;

private:
   bool transient,hasWake,needsUpdate;
   double ztrans,radius;
   double ds,dscur;
   unsigned int ns;
   int size,rank,ncur;
   double *wakeext, *wakeint, *wakeres, *wakegeo, *wakerou, *wake, *current, *dcurrent;
   // double *cur;
   std::vector<double> cur;
   int *count;

   // statistics for debugging
   unsigned long long loc_count_usage, loc_count_workaround;
};

inline bool Collective::hasWakeDefined() const{
    return hasWake;
}
inline void Collective::forceUpdate()
{
  needsUpdate=true;
}
#endif
