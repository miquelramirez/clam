#ifndef PROCESSWAV
#define PROCESSWAV

#include "defines.h"
#include "myfft.h"
#include "info.h"
#include "params.h"
#include "bands.h"
#include <sndfile.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;

void open_wavfile(char* name);
int get_samples(int len, int dr);
int read_data(Float *input);
void initialize_spectrum_parameters();
void rectangfilter(Float *cumsumvector, int len);
void computesinusoidal(Float *input, double Hnorm, Float *spectrum, Float *peaks);
double initializeSinusoidal();
int input_data_wav(char *name , peaksvector& allpeaks, vectorbands& spectralbands, Tmatrix& outbandvalues, pinfo& tmpspectruminfo); 

#endif
