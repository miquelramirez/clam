#ifndef _SynthSineSpectrumConfig_
#define _SynthSineSpectrumConfig_

namespace CLAM {

	#include "ProcessingConfig.hxx"

	/**
	 * Config class for the SynthSineSpectrum Processing
	 */
class SynthSineSpectrumConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (SynthSineSpectrumConfig,3,ProcessingConfig);
	/**
	 * Desired size of the resulting spectrum.
	 * If a zero-padding factor (see below) other than 
	 * 0 is used, the actual size of the output spectrum will 
	 * be SpectrumSize * 2^ZeroPadding.
	 */
	DYN_ATTRIBUTE(0,public,TSize,SpectrumSize);
	/** Sample rate used. */
	DYN_ATTRIBUTE(1,public,TData,SamplingRate);
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
	DYN_ATTRIBUTE(2,public,TData,ZeroPadding);
	
public:
	void DefaultInit();
	void DefaultValues();
};

} // namespace CLAM

#endif
