#ifndef CHANGES
#define CHANGES

#include "params.h"
#include "defines.h"
#include "ocsound.h"
#include <math.h>
#include <iostream>

using namespace std;

void selectpeaks(vector<double> &changesvect, int total_time); 
void applyderivative(vector<double> &onebandvector, int context);
void processmatrix(Tmatrix bands, double maxbandsval, vector<double>& output, int context); 
void applythreshold(vector<double> &vectchanges, double maxbandsval, int context);
void computechanges(int total_time, double *changesvect, Tmatrix &bands, double maxvalband, double timeres, char *filename, int context);

#endif
