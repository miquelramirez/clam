#include "CSaltoSineSynthesis.hxx"

namespace CLAM
{

CSaltoSineSynthesis::CSaltoSineSynthesis(TSize spectralFrameSize,TSize maxSines,double frameTime,double sampleRate,Parameters *pParams)
 :mpSpecSynthPO				  (NULL),
	mpPhaseManagerPO		  (NULL),
	mSpectralFrameSize	  (spectralFrameSize),
	mMaxSines						  (maxSines),
	mFrameTime					  (frameTime),
	mSampleRate					  (sampleRate),
	mpParameter					  (pParams),
	mSwitchToRandomPhases (false)	
{
	// Init the internal Processing Objects
	SynthSineSpectrumConfig sscfg;
	sscfg.SetSpectrumSize(mSpectralFrameSize);
	sscfg.SetSamplingRate(mSampleRate);
	mpSpecSynthPO			= new SynthSineSpectrum(sscfg);

	SynPhaseManagementConfig pmcfg;
	pmcfg.SetMaxSines(mMaxSines);
	pmcfg.SetSamplingRate(mSampleRate);
	//TODO XA: DYN_ATTRIBUTE(3,public,EPhaseGeneration,Type);
	mpPhaseManagerPO	= new SynPhaseManagement(pmcfg);
	
	if(mpSpecSynthPO==NULL||mpPhaseManagerPO==NULL)
			throw Err("OOM in CSaltoSineSynthesis::CSaltoSineSynthesis");			
}

CSaltoSineSynthesis::~CSaltoSineSynthesis()
{	
	if (mpSpecSynthPO!=NULL){
		delete mpSpecSynthPO;
	}
	
	if (mpPhaseManagerPO!=NULL){
		delete mpPhaseManagerPO;
	}	
}

//----------------------------------------------------------------------------//
void CSaltoSineSynthesis::DoSineSynthesis(CSaltoSynthFrame &synthFrame,double gain)
{
	if (mpParameter->GetUsePhaseAlignment() && !mSwitchToRandomPhases)
	{
		mpPhaseManagerPO->DoPhaseAlignment( *synthFrame.GetPeakArrayPtr(),
                                  		  synthFrame.GetSynthTime(),
                                  		  synthFrame.GetFundFreq());
    
		if(mpParameter->GetLastAlignedFrame())
		{	
			// if this is the last aligned frame, fill the 
			// internal phase and freq buffer of the phasemanagment class
			// and prepare for phase continuation
			// with the next frame we want to go into Random Phases...
			
			mpPhaseManagerPO->SetLastPhasesAndFreqs(*synthFrame.GetPeakArrayPtr());
			mpParameter->SetLastAlignedFrame(false);
			mSwitchToRandomPhases = true;
		}
	}
	else
	{
		// .. the phases of synthFrame.PeakArray get changed here ..
		mpPhaseManagerPO->DoRandomPhases(*synthFrame.GetPeakArrayPtr());		
	}																									
		
	mpSpecSynthPO->Do(*(synthFrame.GetPeakArrayPtr()),*(synthFrame.GetSpectrumPtr()),gain);
	
	synthFrame.SetSynthTime(synthFrame.GetSynthTime()+mFrameTime);

}
//----------------------------------------------------------------------------//
void CSaltoSineSynthesis::ResetSineSynthesis()
{
  // here all inits will happen before we start a new synthesis 'thread'..
  mpPhaseManagerPO->ResetPhaseAlignment();
  mSwitchToRandomPhases = false;  // control flag switches PhaseAlignment to Random 
}
//----------------------------------------------------------------------------//

} // end of namespace CLAM