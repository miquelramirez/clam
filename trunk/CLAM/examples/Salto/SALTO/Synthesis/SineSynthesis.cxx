#include "CSaltoSineSynthesis.hxx"
#include "Float2Bool.hxx"

namespace SALTO
{

void SineSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();

	SetName( "Sine Processor" );
}

SineSynthesis::SineSynthesis()
	: mCurrentTime( 0 ), mSwitchToRandomPhases( false ),
	  mAtkTimLvl( "Attack Timbre Level Control", &SineSynthesis::AttackTimbreLevelCB ),
	  mEnPhAlignCtl( "Enable Phase Alignment", &SineSynthesis::UsePhaseAlignmentCB ),
	  mInLAFrameCtl( "Last Aligned Frame Detected", &SineSynthesis::LastAlignedFrameCB ),
	  mInBrOnlyCtl( "BreathOnlySound Detected", &SineSynthesis::BreathOnlySoundCB ),
	  mGain( 0 ), mPhaseAlignmentEnabled( false ), mLastAlignedFrame( false ),
	  mBreathOnlySound( false )
{

	Configure( CSaltoSineSynthesisConfig() );
		
}

SineSynthesis::SineSynthesis( CSaltoSineSynthesis& cfg )
	: mCurrentTime( 0 ), mSwitchToRandomPhases( false ),
	  mAtkTimLvl( "Attack Timbre Level Control", &SineSynthesis::AttackTimbreLevelCB ),
	  mEnPhAlignCtl( "Enable Phase Alignment", &SineSynthesis::UsePhaseAlignmentCB ),
	  mInLAFrameCtl( "Last Aligned Frame Detected", &SineSynthesis::LastAlignedFrameCB ),
	  mInBrOnlyCtl( "Breath Only Sound Detected", &SineSynthesis::BreathOnlySoundCB ),
	  mGain( 0 ), mPhaseAlignmentEnabled( false ), mLastAlignedFrame( false ),
	  mBreathOnlySound( false )
{

	Configure( cfg );
	// Init the internal Processing Objects
	
}

SineSynthesis::~SineSynthesis()
{	
	
}

bool SineSynthesis::ConcreteStart()
{

	mSpecSynthPO.Start();
	mPhaseManagerPO.Start();
	mCurrentTime = 0;
	return true;
}

bool SineSynthesis::ConcreteStop()
{

	mSpecSynthPO.Stop();
	mPhaseManagerPO.Stop();

	return true;
}

bool SineSynthesis::ConcreteConfigure( ProcessingConfig& cfg ) throw ( std::bad_cast )
{
	mConfig = dynamic_cast< const CSaltoSineSynthesisConfig& > ( cfg );

	CLAM_ASSERT( mConfig.HasFrameTime(), "Configuration Object hasn't FrameTime Attribute instantiated" );

	mFrameTime = mConfig.GetFrameTime();

	SynthSineSpectrumConfig sscfg;
	sscfg.SetSpectrumSize( mConfig.GetSpectralFrameSize());
	sscfg.SetSamplingRate( mConfig.GetSampleRate());
	mSpecSynthPO.Configure(sscfg);

	SynPhaseManagementConfig pmcfg;
	pmcfg.SetMaxSines(mConfig.GetMaxSines());
	pmcfg.SetSamplingRate(mConfig.GetSampleRate());
	//TODO XA: DYN_ATTRIBUTE(3,public,EPhaseGeneration,Type);
	mPhaseManagerPO.Configure(pmcfg);

	return true;

}

bool SineSynthesis::Do()
{
	return true;
}

int SineSynthesis::AttackTimbreLevelCB( TControlData value )
{
	if( value > 0) 
		{
			mOutBrOnlyCtl.SendControl( -1 ); // sending true
			mBreathOnlySound = false;
			mGain = value/127.0;
		}
	else
		{
			if ( !mBreathOnlySound )
				{
					mOutBrOnlyCtl.SendControl( 1 );
					mBreathOnlySound = true;
				}
			mGain = 0.0;
		}	

}

int SineSynthesis::UsePhaseAlignmentCB( TControlData value )
{
	mPhaseAlignmentEnabled = EvalAsBool( value );
}

int SineSynthesis::LastAlignedFrameCB( TControlData value )
{
	mLastAlignedFrame = EvalAsBool( value );
}

int SineSynthesis::BreathOnlySoundCB( TControlData value )
{
	mBreathOnlySound = EvalAsBool( value );
}

bool SineSynthesis::Do( CSaltoSynthFrame& synthFrame )
{
	if (mPhaseAlignmentEnabled && !mSwitchToRandomPhases)
	{
		mPhaseManagerPO.DoPhaseAlignment( *synthFrame.GetPeakArrayPtr(),
                                  		  synthFrame.GetSynthTime(),
                                  		  synthFrame.GetFundFreq());
    
		if( mLastAlignedFrame )
		{	
			// if this is the last aligned frame, fill the 
			// internal phase and freq buffer of the phasemanagment class
			// and prepare for phase continuation
			// with the next frame we want to go into Random Phases...
			
			mPhaseManagerPO.SetLastPhasesAndFreqs(*synthFrame.GetPeakArrayPtr());
			mLastAlignedFrame = false;
			mSwitchToRandomPhases = true;
		}
	}
	else
	{
		// .. the phases of synthFrame.PeakArray get changed here ..
		mPhaseManagerPO.DoRandomPhases(*synthFrame.GetPeakArrayPtr());		
	}																									
		
	mSpecSynthPO.Do(*(synthFrame.GetPeakArrayPtr()),*(synthFrame.GetSpectrumPtr()),mGain);

	mCurrentTime += mFrameTime;
	synthFrame.SetSynthTime(synthFrame.GetSynthTime()+mFrameTime);
}

//----------------------------------------------------------------------------//
void SineSynthesis::DoSineSynthesis( CSaltoSynthFrame &synthFrame )
{
	double gain = 0.0;

	//at which level we go from breath-only sound to note sound
	if(mpParameter->GetAttackTimbre().GetLevel() > 0) 
		{
			if ( mpParameter->GetBreathOnlySound() )
				mpParameter->SetBreathOnlySound(false);
			gain = mpParameter->GetAttackTimbre().GetLevel()/127.0;
		}
	else
		{
			if ( !mpParameter->GetBreathOnlySound() )
				mpParameter->SetBreathOnlySound(true);
			gain = 0.0;
		}	

	if (mpParameter->GetUsePhaseAlignment() && !mSwitchToRandomPhases)
	{
		mPhaseManagerPO.DoPhaseAlignment( *synthFrame.GetPeakArrayPtr(),
                                  		  synthFrame.GetSynthTime(),
                                  		  synthFrame.GetFundFreq());
    
		if(mpParameter->GetLastAlignedFrame())
		{	
			// if this is the last aligned frame, fill the 
			// internal phase and freq buffer of the phasemanagment class
			// and prepare for phase continuation
			// with the next frame we want to go into Random Phases...
			
			mPhaseManagerPO.SetLastPhasesAndFreqs(*synthFrame.GetPeakArrayPtr());
			mpParameter->SetLastAlignedFrame(false);
			mSwitchToRandomPhases = true;
		}
	}
	else
	{
		// .. the phases of synthFrame.PeakArray get changed here ..
		mPhaseManagerPO.DoRandomPhases(*synthFrame.GetPeakArrayPtr());		
	}																									
		
	mSpecSynthPO.Do(*(synthFrame.GetPeakArrayPtr()),*(synthFrame.GetSpectrumPtr()),gain);
	
	synthFrame.SetSynthTime(synthFrame.GetSynthTime()+mFrameTime);

}
//----------------------------------------------------------------------------//
void SineSynthesis::ResetSineSynthesis()
{
  // here all inits will happen before we start a new synthesis 'thread'..
  mPhaseManagerPO.ResetPhaseAlignment();
  mSwitchToRandomPhases = false;  // control flag switches PhaseAlignment to Random 
}
//----------------------------------------------------------------------------//

} // end of namespace CLAM
