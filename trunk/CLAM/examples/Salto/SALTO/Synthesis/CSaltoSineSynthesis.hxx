#ifndef _CSaltoSineSynthesis_
#define _CSaltoSineSynthesis_

#include "Spectrum.hxx"
#include "CSaltoSynthFrame.hxx"
#include "SynthSineSpectrum.hxx"
#include "SynPhaseManagement.hxx"
//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"

namespace CLAM
{

class	CSaltoSineSynthesis
{
public:
//	CSaltoSineSynthesis(TSize spectralFrameSize,TSize maxSines,double	frameTime,double sampleRate,CSaltoParameter	*pParams);
	CSaltoSineSynthesis(TSize spectralFrameSize,TSize maxSines,double	frameTime,double sampleRate,Parameters	*pParams);	
	~CSaltoSineSynthesis();

//public methods
	void 	DoSineSynthesis(CSaltoSynthFrame &synthFrame,double gain=1.0);
	void    ResetSineSynthesis();

private:
	// settings
	TSize                       mSpectralFrameSize;
	TSize                       mMaxSines;
														
	double                      mFrameTime;
	double                      mSampleRate;
	double                      mCurrentTime;
														
	bool                        mSwitchToRandomPhases;
	
	// storage
	double                      *mpRandomPhases;
	
	//processing
	SynthSineSpectrum			*mpSpecSynthPO;
	SynPhaseManagement          *mpPhaseManagerPO;
	
	// other classes
//	CSaltoParameter	*mpParameter;
	Parameters                  *mpParameter;
};

} // end of namespace CLAM
#endif
