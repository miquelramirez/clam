#ifndef BANDS
#define BANDS

#include <iostream>
#include <vector>

#define vectorbands vector<band>

using namespace std;

class band {
  public:
	band();

	band(const band& bandcopy);

	// Creates a triangular window, from low to high bin. Window values rate from 0 to 1

	band(int low, int center, int high, double center_freq);
	band(double low, double center, double high, double center_freq);

	// Returns value of that band for a given FFT values
	double applyband(double *fft, int fftlen);
	double applybanddB(double *fft, int fftlen);
	double applybandRMS(double *fft, int fftlen);
	double applybanddBNorm(double *fft, int fftlen);

  	int lowsample;
  	int highsample;
  	int centersample;
  	int size;
	int effectivesize;
	double centerfreq;
	double sumvalues;
  	vector<double> values;

};

ostream &operator<<(ostream &output, const band b);

void generatebands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);
void generatehalfbands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);
void generateexacthalfbands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);

#endif
