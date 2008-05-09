#include "bands.h"
#include <math.h>
#include <cstdlib>

band::band() {
	lowsample=0;
	highsample=0;
	centersample=0;
	size=0;
	effectivesize=0;
	centerfreq=0;
	sumvalues=0;
	if (!values.empty()) values.clear();
}

band::band(const band& bandcopy)
{
	lowsample=bandcopy.lowsample;
	highsample=bandcopy.highsample;
	centersample=bandcopy.centersample;
	size=bandcopy.size;
	effectivesize=bandcopy.effectivesize;
	centerfreq=bandcopy.centerfreq;
	values=bandcopy.values;
	sumvalues=bandcopy.sumvalues;
}


// Creates a triangular window, from low to high bin. Window values rate from 0 to 1
	
band::band(int low, int center, int high, double center_freq) 
{
	if (low>high) {
		cerr << "Error: high sample must be higher than low sample";
		exit(-1);
	}
	//....
		
	lowsample=low;
	highsample=high;
	centersample=center;
	size=high-low+1;
	centerfreq=center_freq;

	/* Generate triangular window */

	int lenfirst= center-low;
	int lensecond= high-center; 

	if (!values.empty()) values.clear();

	for (int i=0; i<lenfirst; i++) {
		values.push_back((double)i/lenfirst);
	}
	for (int i=0; i<=lensecond; i++) {
		values.push_back((double)(lensecond-i)/lensecond);
	}
	
	if (size>=3) effectivesize=size-2;
	else effectivesize=size;

	sumvalues=0;
	for (int i=0; i<size; i++)
		sumvalues+=values[i];
}

// Creates a triangular window, from low to high bin. Window values rate from 0 to 1
// BETA: STILL DOESNT WORK, BUT NOT USED
band::band(double low, double center, double high, double center_freq) 
{
	if (low>high) {
		cerr << "Error: high sample must be higher than low sample";
		exit(-1);
	}
	//....
		
//	lowsample=(int)(ceil(low));
//	highsample=(int)(floor(high));

	centersample=(int)round(center);
	highsample=(int)round(high);
	lowsample=(int)round(low);
	size=highsample-lowsample+1;
	centerfreq=center_freq;

	/* Generate triangular window */

	int lenfirst= centersample-lowsample;
	int lensecond= highsample-centersample; 
	double doublelenfirst=center-low;
	double doublelensecond=high-center;

	if (!values.empty()) values.clear();

	int j;

	cout << "BAND.\n";
	cout << "doublelenfirst= " << doublelenfirst << "\ndoublelensecond= " << doublelensecond << endl;
	cout << "lowsample= " << lowsample << "\ncentersample=" <<
centersample << "\nhighsample=" << highsample << endl;

	for (j=lowsample; j<centersample; j++) {
		values.push_back((j-low)/doublelenfirst);
	}
	for (j=centersample; j<=highsample; j++) {
		values.push_back((high-j)/doublelensecond);
	}

	if (size>=3) effectivesize=size-2;
	else effectivesize=size;

	sumvalues=0;
	for (int i=0; i<size; i++)  
		sumvalues+=values[i];

cout << *this;

}



// Output similar to that of Spectralab(TM)

double band::applyband(double* fft, int fftlen)
{
	double val=0;
	int j;
	for (int i=0, j=lowsample; i<fftlen && j<=highsample ; i++, j++)
		val+= fft[j] * values[i];
	val/=60.0;

	return val;
}

// Output in dBs similar to that in Spectralab (TM)

double band::applybanddB(double* fft, int fftlen) 
{
	double val=applyband(fft,fftlen);
	if (val<=0) return -100;
	return 20*log10(val)-40;
}


double band::applybandRMS(double *fft, int fftlen) 
{
	double val=0;
	int j;
	for (int i=0, j=lowsample; i<fftlen && j<=highsample ; i++, j++)
		val+= pow(fft[j] * values[i]/60.0 , 2.0);

	//val/=sqrt((double)effectivesize);
	//val/=log10((double)effectivesize);

	val=sqrt(val);

	return val;
}


double band::applybanddBNorm(double* fft, int fftlen) 
{
	double val=applyband(fft,fftlen);
	double output= (val); //(pow(val,2.0));
	if (output<0) return 0;
	else return output;
}


ostream &operator<<(ostream &output, const band b) {
	output << "Centerfreq: " << b.centerfreq << endl;
	output << "limits: " << b.lowsample << "..." << b.centersample << "..." << b.highsample << endl;
	output << "Size: " << b.size << endl;
	output << "Effective size: " << b.effectivesize << endl;
	for (int i=0; i<(int)b.values.size(); i++)
		output << "values[" << i << "]= " << b.values[i] << endl;

	return output;
}


/*-----------------------------

  Generates center frequencies and triangular windowing. Bands are of size
  band bandwidth * 2

------------------------------*/

void generatebands(double min_freq, double max_freq, vectorbands
&spectralbands, double freq_resolution) 
{
	const double stepbands=pow(2.0,1.0/12.0);
	if (!spectralbands.empty())
		spectralbands.clear();

	for (double curr_freq=min_freq; curr_freq < max_freq; curr_freq*=stepbands)
	{
		int firstsample= (int)(floor(curr_freq/stepbands/freq_resolution));
		int lastsample= (int)(ceil(curr_freq*stepbands/freq_resolution));
		int centersample= (int)(round(curr_freq/freq_resolution));

		band myband(firstsample, centersample, lastsample, curr_freq);
		spectralbands.push_back(myband);
	}
}

void generatehalfbands(double min_freq, double max_freq, vectorbands
&spectralbands, double freq_resolution) 
{
	const double stepbands=pow(2.0,1.0/12.0);
	const double stepinterbands=pow(2.0,1.0/24.0);
//	const double stepinterbands=pow(2.0,1.0/18.0);


	if (!spectralbands.empty())
		spectralbands.clear();

	for (double curr_freq=min_freq; curr_freq < max_freq; curr_freq*=stepbands)
	{
		int firstsample= (int)(floor(curr_freq/stepinterbands/freq_resolution));
		int lastsample= (int)(ceil(curr_freq*stepinterbands/freq_resolution));
		int centersample= (int)(round(curr_freq/freq_resolution));

		band myband(firstsample, centersample, lastsample, curr_freq);
		spectralbands.push_back(myband);
	}

//	for (int i=0; i<spectralbands.size(); i++) {
//		cout << "BAND " << i << "=" << endl;
//		cout << spectralbands[i];
//		cout << endl;
//	}
}



// NOT USED!!

void generateexacthalfbands(double min_freq, double max_freq, vectorbands
&spectralbands, double freq_resolution) 
{
	const double stepbands=pow(2.0,1.0/12.0);
	const double stepinterbands=pow(2.0,1.0/24.0);

	if (!spectralbands.empty())
		spectralbands.clear();

	for (double curr_freq=min_freq; curr_freq < max_freq; curr_freq*=stepbands)
	{
		double firstsample= curr_freq/stepinterbands/freq_resolution;
		double lastsample= curr_freq*stepinterbands/freq_resolution;
		double centersample= curr_freq/freq_resolution;

		band myband(firstsample, centersample, lastsample, curr_freq);
		spectralbands.push_back(myband);
	}
}
