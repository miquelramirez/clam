#include "changes.h"
#include "stat.h"
#include <math.h>


void selectpeaks(vector<double> &changesvect, int total_time) 
{
	vector<double> tmpvector(total_time);

	for (int i=0; i<total_time; i++)
		tmpvector[i]=0;

	// Detect peaks	
	for (int i=1; i< total_time-1; i++) {
		if (changesvect[i] > changesvect[i-1] && changesvect[i] >= changesvect[i+1])
			tmpvector[i]=changesvect[i];
	}
	// Copy output
	changesvect=tmpvector;
}


void applyderivative(vector<double> &onebandvector, int context) 
{
	int ntime=onebandvector.size();

	// Make derivative (context is considered if CONTEXT!=0)

	vector<double> tmpvector(onebandvector);

	// Standard derivative if no context is considered

	if (!context) {
		for (int t=1; t<ntime; t++) onebandvector[t]=tmpvector[t]-tmpvector[t-1];
	}

	// Special derivative is context is considered

	else {
		for (int t=context; t<ntime-context; t++) 
		{
			double numerator=0.0;
			for (int i=1; i<=context; i++) 
				numerator+=i*(tmpvector[t+i]-tmpvector[t-i]);
			onebandvector[t]=numerator;
		}
		for (int t=0; t<context; t++) onebandvector[t]=0;
		for (int t=ntime-context; t<ntime; t++) onebandvector[t]=0;
	}
	
	// Keep only positive values
	for (int i=0; i<ntime; i++)
		if (onebandvector[i]<0) onebandvector[i]=0;

}

void processmatrix(Tmatrix bands, double maxbandsval, vector<double>&
output, int context) 
{
	int nbands=bands[0].size();
	int ntime=bands.size();

	// Create temporal matrix for storing results after relative difference function
	Tmatrix differential;
	differential.resize(ntime);
	for (int i=0; i<ntime; i++)
		differential[i].resize(nbands);

	// Compute relative difference function for each band in time

	vector<double> onebandvector(ntime);
	for (int i=0; i<nbands; i++) {

		for (int j=0; j<ntime; j++) 
			onebandvector[j]=bands[j][i];

		applyderivative(onebandvector,context);

		// Copy to temporal matrix 
		for (int j=0; j<ntime; j++)
			differential[j][i]=onebandvector[j];
	}

	// Compute o(t) for each time frame.

	output.resize(ntime);
	for (int t=context; t<ntime-context; t++) 
	{
		double sumabands=0, sumadiff=0;

		for (int j=0; j<nbands; j++)  {

			sumadiff+=differential[t][j];

			if (!context) {
				sumabands+=bands[t][j];
			}
			else {
				for (int k=1; k<=context; k++)
					sumabands+=k*bands[t+k][j];
			}
		}

		if (sumabands>kSILENCE) {
			output[t]=sumadiff/sumabands;
		}
		else output[t]=0;
	}

	// Print o(t) 

	//for (int t=0; t<ntime; t++)
	//	printf("%.4f\n", output[t]);

}


void applythreshold(vector<double> &vectchanges, double maxbandsval, int
context)
{
	double threshold;
	if (!context) 
		threshold= NEWPAR; 
	else threshold= NEWPARCONTEXT;

	for (int i=0; i<vectchanges.size(); i++)
		if (vectchanges[i] < threshold)
			vectchanges[i]=0;
}

/*-------------------------

 Main changes processing function

------------------------*/

void computechanges(int total_time, double *changesvect, Tmatrix &bands,
double maxbandsval, double timeres, char* filename, int context)
{
	/* Algorithm */

	for (int i=0; i<total_time; i++) 
		changesvect[i]=0;

	vector<double> vectchanges;
	processmatrix(bands,maxbandsval,vectchanges,context);

	selectpeaks(vectchanges, total_time);

	applythreshold(vectchanges,maxbandsval,context);

	for (int i=0; i<total_time; i++) {
		if (vectchanges[i]!=0) 
			std::cout << i*timeres  + timeres/2.<< std::endl;
		changesvect[i]=vectchanges[i];
	}


	generateorc(filename);
	generatesco(changesvect, total_time, timeres);

}

