#ifndef INFO
#define INFO

class pinfo {
  public:
	pinfo() {
		dr=0;
		samplerate=0;
		percentage=0;
		N=0;
		freq_resolution=0;
		time_resolution=0;
		res=0;
		winsize=0;
		dBs=0;
		bands=0;
		power=0;
		W=0;
		firstsample=0;
		min_sample=0;
		maxampval=0;
		maxbandsval=0;
		first_band_freq=0;
		numbands=0;
	}

	pinfo(const pinfo &copyinfo) {
		dr=copyinfo.dr;
		samplerate=copyinfo.samplerate;
		percentage=copyinfo.percentage;
		N=copyinfo.N;
		freq_resolution=copyinfo.freq_resolution;
		time_resolution=copyinfo.time_resolution;
		res=copyinfo.res;
		winsize=copyinfo.winsize;
		dBs=copyinfo.dBs;
		bands=copyinfo.bands;
		power=copyinfo.power;
		W=copyinfo.W;
		firstsample=copyinfo.firstsample;
		min_sample=copyinfo.min_sample;
		maxampval=copyinfo.maxampval;
		maxbandsval=copyinfo.maxbandsval;
		first_band_freq=copyinfo.first_band_freq;
		numbands=copyinfo.numbands;

	}


	int dr; 	 // Decimation ratio
	int samplerate;  // Sampling rate
	int percentage;  // Ocerlapping percentage
	int N; 		 // input window size
	Float freq_resolution; // Spectral resolution
	Float time_resolution; // Time resolution
	int res;	 // Apply zeroes for getting more frequency resolution
	int winsize;	 // Winsize= N * res 
	int dBs; 	 // if dBs==0, amplitude is shown as linear values. Otherwise, as dBs.
	int bands; 	 // if bands==1, there is not band division. Otherwise, 1/value (1/12) of octave
	int power;	 // if power==1, power spectrum is calculated
	Float W;	 // Bandwidth for sinusoidal likeness
	int firstsample; // First sample corresponding to that bandwidth
	int min_sample;	 // First sample from minumum frequency
	double maxampval;   // Maximum amplitude found in the spectrum
	double maxbandsval; // Maximum amplitude found in the bands spectrum
	double first_band_freq; // Frequency for the first band
	int numbands;	// Number of bands

};

#endif
