#ifndef _SpectralSynthesisConfig_
#define _SpectralSynthesisConfig_

#include "ProcessingConfig.hxx"
#include "WindowGeneratorConfig.hxx"
#include "CircularShiftConfig.hxx"
#include "IFFTConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the SpectralSynthesis Processing
	 */
class SpectralSynthesisConfig:public ProcessingConfig
{

friend class SpectralSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SpectralSynthesisConfig,8,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public,WindowGeneratorConfig,AnalWindowGenerator);
	DYN_ATTRIBUTE(2,public,WindowGeneratorConfig,SynthWindowGenerator);
	DYN_ATTRIBUTE(3,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(4,public,IFFTConfig,IFFT);
	DYN_ATTRIBUTE(5,protected,int, prZeroPadding);
	DYN_ATTRIBUTE(6,protected,int,prSamplingRate);
	DYN_ATTRIBUTE(7,public,bool,Residual);


//Config shortcuts

public:
	~SpectralSynthesisConfig(){};

/** Setter for Analysis Window size in number of samples. It should match the size used in the
 *	SpectralAnalysis Processing. An assertion fault is raised if size is not odd. This
 *	method changes size of AnalysisWindowGenerator, IFFT and CircularShift accordingly.
 */	
	void SetAnalWindowSize(TSize w);
/** Getter for Analysis Window Size
 *	@see SetAnalWindowSize
 */
	TSize GetAnalWindowSize() const;

/** Setter for Analysis Window type
 *	@see EWindowType
 */
	void SetAnalWindowType(const EWindowType& t);
/** Getter for Analysis Window type
 *	@see SetAnalWindowType
 */
	const EWindowType& GetAnalWindowType() const;

/** Sets Zero padding factor.
 *	A factor of zero means that no zero padding has been applied in the analysis.
 *	@see SpectralAnalysis::SetZeroPadding
 */
	void SetZeroPadding(int z);
/** Getter for Zero padding factor 
 *	@see SetZeroPadding*/
	int GetZeroPadding() const;
/** Setter for Synthesis window size in number of samples. Just affects the Synthesis
 *	Window Generator Size
 */
	void SetSynthWindowSize(TSize w);
/**	Getter for Synthesis Window Size
 *	@see SetSynthWindowSize
 */
	TSize GetSynthWindowSize()const;

/** Setter for Synthesis Hop size in number of samples.
 *	Changes size of the Synthesis WindowGenerator, and the Hop Size and Buffer Size in the
 *	OverlapAdd Processing
 */	
	void SetHopSize(TSize h);
/** Getter for Synthesis Hop size
 *	@see  SetHopSize
 */
	TSize GetHopSize() const;
/** Setter for global Sampling rate. It synchronizes all existing sizes. */
	void SetSamplingRate(TData sr);
/** Getter for global Sampling rate */
	TData GetSamplingRate() const;
/** Setter for Spectrum Size **/
	void SetSpectrumSize(TSize specSize);
	TSize GetSpectrumSize() const;

private:

	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

} // namespace CLAM

#endif
