#include "SineSynthesis.hxx"

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
  	  mInBrOnlyCtl( "BreathOnlySound Detected", this, &SineSynthesis::BreathOnlySoundCB ),
	  mAtkTimLvlCtl( "Attack Timbre Level Control", this, &SineSynthesis::AttackTimbreLevelCB ),
	  mEnPhAlignCtl( "Enable Phase Alignment", this, &SineSynthesis::UsePhaseAlignmentCB ),
	  mInLAFrameCtl( "Last Aligned Frame Detected", this, &SineSynthesis::LastAlignedFrameCB ),
	  mOutLAFrameCtl( "Last Aligned Frame Control", this),
	  mOutBrOnlyCtl( "BreathOnlySound Control", this),
	  mGain( 0 ), mPhaseAlignmentEnabled( false ), mLastAlignedFrame( false ),
	  mBreathOnlySound( false )
{
	SineSynthesisConfig cfg;

	Configure( cfg );		
}

SineSynthesis::SineSynthesis( const SineSynthesisConfig& cfg )
	: mCurrentTime( 0 ), mSwitchToRandomPhases( false ),
  	  mInBrOnlyCtl( "BreathOnlySound Detected", this, &SineSynthesis::BreathOnlySoundCB ),
	  mAtkTimLvlCtl( "Attack Timbre Level Control", this, &SineSynthesis::AttackTimbreLevelCB ),
	  mEnPhAlignCtl( "Enable Phase Alignment", this, &SineSynthesis::UsePhaseAlignmentCB ),
	  mInLAFrameCtl( "Last Aligned Frame Detected", this, &SineSynthesis::LastAlignedFrameCB ),
	  mOutLAFrameCtl( "Last Aligned Frame Control", this),
	  mOutBrOnlyCtl( "BreathOnlySound Control", this),
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

bool SineSynthesis::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);

	CLAM_ASSERT( mConfig.HasFrameTime(), "Configuration Object hasn't FrameTime Attribute instantiated" );

	mFrameTime = mConfig.GetFrameTime();

	SynthSineSpectrumConfig sscfg;
	sscfg.SetSpectrumSize( mConfig.GetSpectralFrameSize());
	sscfg.SetSamplingRate( mConfig.GetSampleRate());
	mSpecSynthPO.Configure(sscfg);

	PhaseManagementConfig pmcfg;
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
			if( mBreathOnlySound )
			{
				mOutBrOnlyCtl.SendControl( -1 ); // sending false
				mBreathOnlySound = false;
			}
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

	return 0;
}

int SineSynthesis::UsePhaseAlignmentCB( TControlData value )
{
	if( value > 0 ) mPhaseAlignmentEnabled = true ;
	else			mPhaseAlignmentEnabled = false;

	return 0;
}

int SineSynthesis::LastAlignedFrameCB( TControlData value )
{
	if( value > 0 ) mLastAlignedFrame = true;
	else			mLastAlignedFrame = false;

	return 0;
}

int SineSynthesis::BreathOnlySoundCB( TControlData value )
{
	if( value > 0 )	mBreathOnlySound = true;
	else			mBreathOnlySound = false;

	return 0;
}

bool SineSynthesis::Do( CSaltoSynthFrame& synthFrame )
{
	if (mPhaseAlignmentEnabled && !mSwitchToRandomPhases)
	{
		mPhaseManagerPO.mCurrentPitch.DoControl(synthFrame.GetFundFreq());
		mPhaseManagerPO.mCurrentTime.DoControl(synthFrame.GetSynthTime());
		mPhaseManagerPO.DoPhaseAlignment( *synthFrame.GetPeakArrayPtr());
    
		if( mLastAlignedFrame )
		{	
			// if this is the last aligned frame, fill the 
			// internal phase and freq buffer of the phasemanagment class
			// and prepare for phase continuation
			// with the next frame we want to go into Random Phases...
			
			mPhaseManagerPO.SetLastPhasesAndFreqs(*synthFrame.GetPeakArrayPtr());
			mLastAlignedFrame = false;
			mOutLAFrameCtl.SendControlAsBoolean( false );
			// XA oopsla demo klutch mSwitchToRandomPhases = true;
		}
	}
/*	XA oopsla demo klutch else
	{
		// .. the phases of synthFrame.PeakArray get changed here ..
		mPhaseManagerPO.DoRandomPhases(*synthFrame.GetPeakArrayPtr());		
	}																									
*/		
	mSpecSynthPO.Do(*(synthFrame.GetPeakArrayPtr()),*(synthFrame.GetSpectrumPtr()),mGain);

	mCurrentTime += mFrameTime;
	synthFrame.SetSynthTime(synthFrame.GetSynthTime()+mFrameTime);

	return true;
}

//----------------------------------------------------------------------------//
void SineSynthesis::DoSineSynthesis( CSaltoSynthFrame &synthFrame, Parameters* mpParameter )
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
		mPhaseManagerPO.mCurrentPitch.DoControl(synthFrame.GetFundFreq());
		mPhaseManagerPO.mCurrentTime.DoControl(synthFrame.GetSynthTime());
		mPhaseManagerPO.DoPhaseAlignment( *synthFrame.GetPeakArrayPtr());
        
		if(mpParameter->GetLastAlignedFrame())
		{	
			// if this is the last aligned frame, fill the 
			// internal phase and freq buffer of the phasemanagment class
			// and prepare for phase continuation
			// with the next frame we want to go into Random Phases...
			
			mPhaseManagerPO.SetLastPhasesAndFreqs(*synthFrame.GetPeakArrayPtr());
			mpParameter->SetLastAlignedFrame(false);
			//XA oopsla demo klutch mSwitchToRandomPhases = true;
		}
	}
/*	XA oopsla demo klutch else
	{
		// .. the phases of synthFrame.PeakArray get changed here ..
		mPhaseManagerPO.DoRandomPhases(*synthFrame.GetPeakArrayPtr());		
	}																									
*/		
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
