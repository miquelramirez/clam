#ifndef PEAKSATT
#define PEAKSATT

#include "defines.h"
#include <iostream>

class peaksatt {
	public: 

	  peaksatt();
	  peaksatt(int t_ini, int t_end, mapa2 spectrum_peaks); 
	  peaksatt(const peaksatt& patt);
	
	  mapa2 specpeaks;	// CREO QUE DEBERIA SER UN PUNTERO!!
	  int tini;
	  int tend;
};

ostream &operator<<(ostream &output, peaksatt patt);

#endif
