#ifndef _SynthSineSpectrum_
#define _SynthSineSpectrum_

#define   MAINLOBE_TABLE_SIZE 4096		// Tablesize of the mainlobe of a BH92 Window 

#include "Err.hxx"
#include "WindowGenerator.hxx"
#include "AudioMultiplier.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Complex.hxx"
#include "Processing.hxx"
#include "SynthSineSpectrumConfig.hxx"

namespace CLAM{

/**
 * Configuration class for the SynthSineSpectrum Processing.
 */
	/* SynthSineSpectrumConfig moved to SynthSineSpectrumConfig.hxx */

	
							
/**
 * Processing which can synthesize a spectrum out of an 
 * array of spectral peaks.
 * It does this by convolving the spectral peaks with the 
 * significant part of the fourier transformed Blackman-Harris92 
 * window, it's main lobe.
 */
class SynthSineSpectrum:public Processing
{
public:
	/* constructor/destructor */
	SynthSineSpectrum();
	SynthSineSpectrum(SynthSineSpectrumConfig& cfg);
	~SynthSineSpectrum();
	const char * GetClassName() const {return "SynthSineSpectrum";}

	/* public member functions */
	bool Do(const SpectralPeakArray& peakArray,Spectrum& residualSpectrumOut,
	        double gain=1.0);
	bool Do(void){ return false;}

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const {return mConfig;}

private:
	/* private member functions */
	void InitSynthSpec(TSize size);

	void FillSynthSineSpectrum(const SpectralPeakArray& peakArray,double gain = 1.0);

	/* private configuration*/
	SynthSineSpectrumConfig mConfig;

	/* window table generation */
	WindowGenerator		mWndGen;
	//Spectrum			mBlackHarris92TransMainLobe;
	static double mBlackHarris92TransMainLobe[];

	/* internal buffer for synthesized sinusoidal spectrum */
	Array< Complex > mSynthSineSpectrum;
};
};// CLAM namespace
#endif

