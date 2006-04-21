#ifndef ConstantQTransform_hxx
#define ConstantQTransform_hxx

#include <vector>

namespace Simac
{

/**
 * ConstantQTransform extract a ConstantQ spectrum using Blankertz's paper algorithm.
 * This transformation is similar to the Fourier transform but it generates
 * bins which bins are proportional to their center frequency.
 * 
 * This implementation saves computational time by moving most computation to
 * configuration time and using a fft as starting point for the transform.
 *
 * \b Parameters:
 * - samplinRate: The sampling rate for the input audio
 * - minFrequency: The minimum frequency to be considered
 * - maxFrequency: The maximum frequency to be considered
 * - binsPerOctave: The number of bins required for each octave
 *
 * \b Configuration Outputs:
 * - Q: Quality factor for the bins
 * - K: Number of Constant Q spectrum bins
 * - spectrumSize: Number of required fft spectrum bins
 *
 * \b Inputs:
 * - spectrum: An interlaced complex spectrum (complex, so, size 2*fftLength)
 *
 * \b Outputs:
 * - constantQSpectrum: An interlaced complex spectrum (complex, so, size 2*K)
 */

class ConstantQTransform
{
public:
	typedef std::vector<double> ConstantQSpectrum;
	typedef std::vector<double> Spectrum;
private:
	ConstantQSpectrum cqdata;
	unsigned FS;
	double fmin;
	double fmax;
	double Q;
	unsigned _binsPerOctave;
	unsigned mSpectrumSize;
	unsigned K;

	// Sparse complex numbers matrix represented by the i and j indexes
	// for non null cells and the real and imaginary components of the cell
	std::vector<unsigned> mSparseKernelIs;
	std::vector<unsigned> mSparseKernelJs;
	std::vector<double> mSparseKernelImagValues;
	std::vector<double> mSparseKernelRealValues;
public:
	void doIt(const std::vector<double> & fftData);

//public functions incl. sparsekernel so can keep out of loop in main
public:
	ConstantQTransform(unsigned FS, double fmin, double fmax, unsigned binsPerOctave);
	void sparsekernel(double);
	~ConstantQTransform();
	// Results
	const ConstantQSpectrum & constantQSpectrum() const {return cqdata;}
	double getQ() const {return Q;}
	int getK() const {return K;}
	int getfftlength() const {return mSpectrumSize;}
private:
	double Hamming(int len, int n) {
		double out = 0.54 - 0.46*cos(2*M_PI*n/len);
		return(out);
	}
};

}

#endif//ConstantQTransform_hxx

