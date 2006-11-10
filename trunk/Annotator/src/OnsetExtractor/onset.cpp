/**************************************************************************************
*   									      	      *
*  Antonio Pertusa Ibáñez						              *
*  Onset detection system							      *
***************************************************************************************/

#include "processwav.h"
#include "changes.h"
#include "onset.h"

peaksvector allpeaks;


/*--------------------------------------------------------------------------

 Initialize output matrix (the matrix with the sounding notes)

----------------------------------------------------------------------------*/

void initialize(int n_time, int numbands) {

	/* Outmatrix initialization */

	double currtime=info.time_resolution/2.0;

	outmatrix.resize(n_time);
	for (int i=0; i<n_time; i++)
		outmatrix[i].resize(numbands+1);

	for (int i=0; i< n_time; i++) {
		outmatrix[i][0]=currtime;
		for (int j=1; j< numbands+1; j++)
			outmatrix[i][j]=0.0;
		currtime+=info.time_resolution;
	}
}



/*------------------------------------------------------------------------------

	MAIN

-------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

	// Process parameters

	if (argc!=2 && argc!=3) 
		error(0,argv[0]);

	int context=0;
	if (argc==3) context=atoi(argv[2]);

	cerr << "Scanning input data..." << endl;

	// Output from input_data_wav: the peaks vector, information
	// about spectral bands, the spectrum grouped into bands and 
	// information about the spectrum parameters.
 
	Tmatrix outbandmatrix(1);
	int n_time=input_data_wav(argv[1], allpeaks, spectralbands, outbandmatrix, info);

	initialize(n_time,info.numbands);

	// Change detection. Changes are stored in changesvector 

	cerr << "Computing onsets..." << endl;
	changesvector=(double*)malloc(sizeof(double)*(n_time));
	computechanges(n_time,changesvector,outbandmatrix,info.maxbandsval,info.time_resolution,argv[1],context);
	delete changesvector;
}
