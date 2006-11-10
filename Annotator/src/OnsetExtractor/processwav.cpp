/***********************************************************************
*  
* Processwav computes peaks from a wavefile. This wav must be with a
* resolution of 16 bits?. Main call to this process is:
*
* int input_data_wav(char *name , peaksvector& peaks, pinfo& tmpspectruminfo) 
*
* From a wav filename, a peaks vector is obtained. Information about
* the spectral parameters is stored in tmpspectruminfo
*
*************************************************************************/

#include "processwav.h"

/* Global variables, for efficiency */ 

SNDFILE *wavfile;
SF_INFO wavinfo;
int first_window;
int *buf;
int *bufaux;
Float *data;
double maxval=-200.0;
Float *window;	   	// Window applied for FFT 
Float *windowpowered; 	// For the sinusoidal likeness
pinfo spectruminfo;


/*--------------------------------------------

  read wavefile

----------------------------------------------*/

void open_wavfile(char* name) 
{
	if (!(wavfile= sf_open(name, SFM_READ, &wavinfo))) 
	{
	 	printf("Error : File %s can not be processed.\n", name);
	 	sf_perror (NULL);
	 	exit(1);
	}
}
	


/*-------------------------------------------------

 Get input vector from wav file. Decimation ratio is applied

--------------------------------------------------*/

int get_samples(int len, int dr) 
{
	int i,j,k,n_samples,sum;

	if (dr==1) return sf_read_int(wavfile, buf, len);
	else {
		n_samples=sf_read_int(wavfile, bufaux,len*dr);

		for (i=0, j=0; i<len && j<n_samples; i++) {
			sum=0;
			for (k=0;k<dr;j++,k++) 
				sum+=bufaux[j];
			buf[i]=sum/dr;
		}
	        return n_samples/dr;
	}
}


/*----------------------------------------------

 Read wave data (only 16 bits?)

-----------------------------------------------*/

int read_data(Float *input) {

	long nread;
	int i,j;
	int overlapped=0;
	int len;
	
	if (spectruminfo.percentage!=0) 
		overlapped=(int)round(100.0/(100-spectruminfo.percentage));

	// First window data
	
	if (first_window || spectruminfo.percentage==0) 
	{
		nread = get_samples(spectruminfo.N,spectruminfo.dr);
	
		for (i=0; i<nread; i++) 
 		   data[i]=(buf[i]/DIVFACTOR);  // 2^21
		len=nread;
	}
	
	// Other windows data (for overlapping)
		
	else {
		for (j=spectruminfo.N/overlapped,i=0; j<spectruminfo.N; j++,i++)
			data[i]=data[j];
		
		nread = get_samples((spectruminfo.N)-i,spectruminfo.dr);
		len=i+nread;
		
	  	for (j=0; i<len; i++,j++)	
			data[i]=(buf[j]/DIVFACTOR);
	}

	// Copy output data

	for (i=0; i<len; i++) 
		input[i]=data[i];
	
	if (len<spectruminfo.N) return 0;
	return len;
}

/*-------------------------------------------
 
 Initialize FFT parameters

------------------------------------------------*/

void initialize_spectrum_parameters() 
{
	/* Create info data */

	spectruminfo.dr=kDR;		// Decimation ratio (fs=fs/dr)
	spectruminfo.res=kRES;		// Resolution. (winsize=winsize*res). Adds zeroes at the end of the window
	spectruminfo.percentage=kPERCENTAGE;	// Overlapping percentage
	spectruminfo.N=kN;		// Input window size
	spectruminfo.W=kW;		// Bandwidth for sinsoidal likeness
	spectruminfo.first_band_freq=kMINBANDFREQ;	// Minimum frequency for band processing

	spectruminfo.samplerate= (wavinfo.samplerate)/(spectruminfo.dr);
	spectruminfo.freq_resolution=(Float)(spectruminfo.samplerate)/((Float)(spectruminfo.N)*spectruminfo.res);
	spectruminfo.winsize= spectruminfo.N*spectruminfo.res;
	spectruminfo.firstsample=(int)round(spectruminfo.W/spectruminfo.freq_resolution); 	// Convert bandwidth (Hz->Samples)
	if (spectruminfo.percentage!=0) spectruminfo.time_resolution= (1.0-((Float)spectruminfo.percentage/100.0))* (spectruminfo.N) / spectruminfo.samplerate;
	else spectruminfo.time_resolution=(Float)(spectruminfo.N)/(Float)(spectruminfo.samplerate);

	spectruminfo.min_sample=(int)floor(spectruminfo.first_band_freq/spectruminfo.freq_resolution);

}

/*********************************************************************************************

  Functions for sinusoidal likeness

*********************************************************************************************/


void rectangfilter(Float *cumsumvector, int len) 
{
	int lenvector= len + spectruminfo.winsize/2 + spectruminfo.firstsample;

	Float tmp[lenvector];

	// Do the cumulative sum across the vector
	for (int i=1; i< lenvector; i++)
		cumsumvector[i]+=cumsumvector[i-1];

	// Compute correlation?
	for (int i=len; i< lenvector; i++)
		tmp[i]=cumsumvector[i]-cumsumvector[i-len];

	// Copy temporary vector into output
	for (int i=0; i< spectruminfo.winsize/2; i++)
		cumsumvector[i]=tmp[i+len];
}


void correctNaNValues(Float *val)
{
	for (int i=0; i<spectruminfo.winsize; i++)
		if (isNaN(val[i])) {
			cerr << "NaN Found: ERROR!!!!";
			val[i]=0.0;
		}
}


// For the sinusoidal likeness
// ATENCION: LOS spectruminfo.firstsample ULTIMOS SAMPLES y PRIMEROS SAMPLES NO DAN VALORES EXACTOS. EL RESTO ESTA COMPROBADO

void computesinusoidal(Float *input, double Hnorm, Float *X, Float *slikeness) 
{
	Float R[spectruminfo.winsize]; 
	Float Xnorm[spectruminfo.winsize];

	// abs(FFT(X))
	fourier_spectrum(input, X, spectruminfo.winsize, spectruminfo.winsize, window, spectruminfo.N);

	// abs(FFT(R))
	fourier_spectrum(input, R, spectruminfo.winsize, spectruminfo.winsize, windowpowered, spectruminfo.N);
	
	// Multiply R by window length
	for (int i=0; i<spectruminfo.winsize/2; i++)
		R[i]*=spectruminfo.winsize;

	// Create vector for cumulative sum (the power spectrum).
	// Cumsumvector contains: part of the negative frequencies (of size len),
	// the positive frequencies and again part of negative frequencies
	// (of size firstsample).

	int len=spectruminfo.firstsample*2+1;

	Float cumsumvector[len + spectruminfo.winsize/2 + spectruminfo.firstsample];
	int i,j;
	for (i=0, j=spectruminfo.winsize/2-1 ; i< len; i++, j--) {
		if (X[j]!=0) cumsumvector[i]=pow(X[j],2);
		else cumsumvector[i]=0;
		//cumsumvector[i]=0;
	}
	for (i=len; i<(spectruminfo.winsize/2)+len; i++) {
		if (X[i-len]!=0) cumsumvector[i]=pow(X[i-len],2);
		else cumsumvector[i]=0;
	}
	for (i=spectruminfo.winsize/2+len, j=spectruminfo.winsize/2-1; i < len+(spectruminfo.winsize/2)+spectruminfo.firstsample; i++, j--) {
		if (X[j]!=0) cumsumvector[i]=pow(X[j],2);
		else cumsumvector[i]=0;
	}

	// Norm of X is calculated using a rectangular FIR filter.
	// cumsumvector useful values after this function range [0...winsize/2]
	rectangfilter(cumsumvector,len);

	// Store values in Xnorm vector
	j= spectruminfo.firstsample;
	for (i=0; i<(spectruminfo.winsize/2); i++,j++)
		Xnorm[i]=cumsumvector[j];

	// Output is computed
	for (i=0; i<spectruminfo.winsize/2; i++) {
		if (Xnorm[i]!=0)
			slikeness[i]=R[i]/(Hnorm*sqrt(Xnorm[i]));
		else slikeness[i]=0.0;
	}

	correctNaNValues(slikeness);

	// Print results
	//for (i=0; i<spectruminfo.winsize/2; i++) {
	//	printf("\%.4f\t",(double)slikeness[i]);
	//}
	//cout << endl;

}

double initializeSinusoidal() 
{

	// Create windows; Hanning and powered Hanning

	Hanning(window,spectruminfo.N);
	for (int i=spectruminfo.N; i<spectruminfo.winsize; i++)
		window[i]=0;

	for (int i=0; i<spectruminfo.winsize; i++)
		windowpowered[i]=window[i]*window[i];


	// Hnorm computation
	double Hnorm, tmp=0;
	for (int i=0; i<spectruminfo.winsize; i++) 
		tmp+=windowpowered[i];
	Hnorm=sqrt(tmp*spectruminfo.winsize);

//printf("W= %f FIRSTSAMPL= %d Hnorm= %f\n",(double)spectruminfo.W,spectruminfo.firstsample,Hnorm);
	return Hnorm;
}

/**********************************************************/

/*-----------

 Compute peaks from spectrum at a given time

------------*/

void computepeaksfromspectrum(mapa2 &orderedspectrumpeaks, double *spectrumvalues, double *sinusoidalvalues)
{
	/* The first column is the current time. Wanted frequencies start at the
	2nd column, wich will be the first one of orderedspectrumbands */

	for (int i=1; i< (spectruminfo.winsize / 2) -1; i++) {


	 if (!kSINUSOIDAL) {
  	   double spcurr= spectrumvalues[i];
	   double spprev= spectrumvalues[i-1];
	   double spnext= spectrumvalues[i+1];  

	   if (spcurr > kMINPEAKVALUE &&  spcurr > spprev && spcurr > spnext) {
			orderedspectrumpeaks.insert(make_pair(i,spcurr));
			if (spcurr > spectruminfo.maxampval)
				spectruminfo.maxampval=spcurr;
			// cout << "PEAK AT: " << i << "(" << (double)spectrumvalues[i] << ")\n";
	    }
	 }
	 else {	
	    double spcurr=spectrumvalues[i];

	    if (sinusoidalvalues[i] > kMINSINLIKEVAL && 
		sinusoidalvalues[i] > sinusoidalvalues[i-1] &&
		sinusoidalvalues[i] > sinusoidalvalues[i+1] &&
		spcurr > kMINPEAKVALUE)
 		{
			orderedspectrumpeaks.insert(make_pair(i,spcurr));
			if (spcurr > spectruminfo.maxampval)
				spectruminfo.maxampval=spcurr;

			//cout << "PEAK AT: " << i << "(" << (double)sinusoidalvalues[i] << ")\n";
	    }
	 }	
	}
	//cout << "\n";
}


void memoryallocation() {
	buf = (int*)malloc(sizeof(int)*(spectruminfo.winsize));
	bufaux = (int*)malloc(sizeof(int)*(spectruminfo.winsize)*(spectruminfo.dr));
	data = (Float*)malloc(sizeof(Float)*(spectruminfo.winsize));
	window = (Float*)malloc(sizeof(Float)*(spectruminfo.winsize));
	windowpowered = (Float*)malloc(sizeof(Float)*(spectruminfo.winsize));
}


void computebands(double *fft, vectorbands& spectralbands, vector<double>& bandvalues, int
fftlen, double& maxbandval) 
{
	if (!bandvalues.empty()) bandvalues.clear();

	for (int i=0; i<(int)spectralbands.size(); i++) {
		double val=spectralbands[i].applybanddBNorm(fft,fftlen);
		if (val>maxbandval) maxbandval=val;
		bandvalues.push_back(val);
	}
}


int input_data_wav(char *name , peaksvector& peaks, vectorbands& spectralbands, Tmatrix& outbandvalues, pinfo& tmpspectruminfo) 
{
	// Open wavfile
	open_wavfile(name);

	// Initialize spectrum parameters (spectruminfo) 
	initialize_spectrum_parameters();

	// memory allocation for input data
	memoryallocation();
	
	Float input[spectruminfo.winsize];
	Float spectrum[spectruminfo.winsize+1];
	Float slikeness[spectruminfo.winsize+1];

//!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
// allow for allocation on declaration of 2.nd dimension of vec of vec
//!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  Tmatrix matrix(n+1);
//  for (int i = 0; i <= n; i++) {
//    matrix[i].resize(m+1);
//  }

	if (!peaks.empty()) peaks.clear();

	// Time axes 
	Float curr_time=(spectruminfo.time_resolution)/2.0;

	// Creation of the Hanning window 
	Hanning(window,spectruminfo.N);

	// Initialize sinusoidal likeness 
	double Hnorm=initializeSinusoidal();

	// Bands generation
	generatebands(spectruminfo.first_band_freq, spectruminfo.samplerate/2, spectralbands, spectruminfo.freq_resolution);
	spectruminfo.numbands=spectralbands.size();

	first_window=true;
	int len;
	int n_time=0;

	while (len=(read_data(input))) 
	{
		first_window=false;
		
		// Complete input window with zeroes 
		for (int i=len; i<spectruminfo.winsize; i++)
			input[i]=0.0;

		// Update current time 

		//printf("%.2f\t",(double)curr_time);
		curr_time+=spectruminfo.time_resolution;

// PRINT INPUT 
//for (int i=0;i<spectrum.winsize;i++)
//	printf("%.2f\t",input[i]);

		// Sinusoidal likeness computation. Spectrum values and sinusoidal likeness values are
		// returned. Only the first half of spectrum and slikeness is useful
		computesinusoidal(input,Hnorm,spectrum,slikeness);
		
		// Bands computation, using the spectrum obtained in the previous step

		outbandvalues.resize(n_time+1);
		outbandvalues[n_time].resize(spectruminfo.numbands);
		vector<double> bandval(spectruminfo.numbands);
		computebands(spectrum, spectralbands, bandval, spectruminfo.winsize / 2, spectruminfo.maxbandsval);
		outbandvalues.push_back(bandval);

		bandval.clear();
	
		// Calculate peaks 
		mapa2 orderedspectrumpeaks;
		computepeaksfromspectrum(orderedspectrumpeaks,(double*)spectrum,(double*)slikeness);

		// Add peaks to main peaks vector 
		peaks.resize(n_time+1);
		peaks.push_back(orderedspectrumpeaks);
		n_time++;
	}

	free(buf);
	free(bufaux);
	sf_close(wavfile);
	tmpspectruminfo=spectruminfo;

	return n_time-1;
}
