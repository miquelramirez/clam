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

class SynthSineSpectrumConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (SynthSineSpectrumConfig,4,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,TSize,SpectrumSize);
	DYN_ATTRIBUTE(2,public,TData,SamplingRate);
	DYN_ATTRIBUTE(3,public,TData,ZeroPadding);
	
public:
	
	~SynthSineSpectrumConfig(){};
	void DefaultInit();
	void DefaultValues();
};
	
							

class SynthSineSpectrum:public Processing
{
public:
	/* constructor/destructor */
	SynthSineSpectrum();
	SynthSineSpectrum(SynthSineSpectrumConfig& cfg);
	~SynthSineSpectrum();

	/* public member functions */
	bool Do(const SpectralPeakArray& peakArray,Spectrum& residualSpectrumOut,
	        double gain=1.0);
	bool Do(void){ return false;}

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

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
};// MTG namespace
#endif

