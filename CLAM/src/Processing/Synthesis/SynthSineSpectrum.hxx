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

namespace CLAM{

/**
 * Configuration class for the SynthSineSpectrum Processing.
 */
class SynthSineSpectrumConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (SynthSineSpectrumConfig,4,ProcessingConfig);
	/** Name. */
	DYN_ATTRIBUTE(0,public,std::string,Name);
	/**
	 * Desired size of the resulting spectrum.
	 * If a zero-padding factor (see below) other than 
	 * 0 is used, the actual size of the output spectrum will 
	 * be SpectrumSize * 2^ZeroPadding.
	 */
	DYN_ATTRIBUTE(1,public,TSize,SpectrumSize);
	/** Sample rate used. */
	DYN_ATTRIBUTE(2,public,TData,SamplingRate);
	/**
	 * Zero-padding factor. This determines the frequency 
	 * resolution of the resulting spectrum. For instance, 
	 * a SpectrumSize of 512 and a ZeroPadding factor of 1 
	 * will result in a spectrum of size 1024 but with a 
	 * better frequency resolution than using a SpectrumSize 
	 * of 1024 and a ZeroPadding factor of 0. The computational 
	 * cost will also increase when the zero-padding factor 
	 * is increased.
	 */
	DYN_ATTRIBUTE(3,public,TData,ZeroPadding);
	
public:
	
	~SynthSineSpectrumConfig(){};
	void DefaultInit();
	void DefaultValues();
};
	
							
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

