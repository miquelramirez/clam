#include "SaltoSynth.hxx"
#include "CSaltoEditor.hxx"
#include "CSaltoDataManagment.hxx"
#include "Assert.hxx"
#include "HeapDbg.hxx"
#include <pthread.h>
#include <iostream>

#define TRACESTATE if (1) ; else std::cout

using SALTO::CSaltoEditor;

namespace CLAM
{

pthread_mutex_t DrawingMutex = PTHREAD_MUTEX_INITIALIZER;


void SaltoSynthConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();

	SetAnalysisFrameSize( 256 );
	SetSampleRate( 44100 );
	SetAnalysisWindowType( EWindowType::eBlackmanHarris92 );
	SetPitchModRange( float(0.05946309436) ); // 1 semitone
}

SaltoSynth::SaltoSynth()
	: 
	  mpSynthFrame(NULL), mpCurrPeakArrayTarget(NULL), mpTransitionFrameBase(NULL),
	  mpTransitionFrameTarget(NULL), mpCurrSpectralFrameBase(NULL),	mpCurrSpectralFrameResidual(NULL),
	  mEventSample(0), mLoopStart(0), mLoopEnd(0),
	  mLimitLoopStart(0), mLimitLoopEnd(0), mInterpolEnd(0), mInterpolStart(0),	mResFadeStart(0), mResFadeEnd(0),
	  mAttackResVolume(1.0), mStatResVolume(1.0), mIPFactor(0.0), mStatResFadeInFrom(20), mStatResFadeInTo(40),
	  mLoopDirectionFW(true), mStatResLoopDirectionFW(true), 
	  mFrameCounterStatRes(0),
	  mFrameCounterAttackResidual(0), mNumFramesStatRes(0), 
	  mIndividualGain(50.0), mLastPitchCorrectionFactor(0),
	  mState( Idle ), mSampleStepping( 0 ),
	  mStateIn( "StateIn", this, &SaltoSynth::UpdateState ),
	  mInLastAlignedFrame( "Last Aligned Frame InControl", this, &SaltoSynth::UpdateLastAlignedFrame ),
	  mInBreathOnlySound( "Breath Only Sound InControl", this, &SaltoSynth::UpdateBreathOnlySound ),
	  mOut_InLoopSynthesis( "In Loop Synthesis", this ),
	  mOutUseRandomLoop( "Use Random Loop", this ),
	  mOutUseRandomDeviations("Use Random Deviations", this),
	  mOutPitchFactor( "Pitch Factor", this),
	  mOutRandomRange( "RandomRange", this ),
	  mOutTargetFreq( "Target Frequency", this ),
	  mOutMagInterpolFactor( "Magnitude Interpolation Factor", this ),
	  mOutMagGain( "Magnitude Gain", this ),
	  mOutFreqInterpolFactor( "Frequency Interpolation Factor", this ),
	  mOutMagInterpolFactor2( "Magnitude Interpolation Factor", this ),
	  mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
	  mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
	  mOutUsePhaseAlignment( "Use Phase Alignment Out Control", this ),
	  mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
	  mOutResGain( "Resonance Gain Out Control", this ),
	  mOutResonanceFreq( "Resonance Frequency Out Control", this ),
	  mpGUI(NULL)
{
		SaltoSynthConfig cfg;

		Configure( cfg );

		AdoptChildren();

		ControlSetup();
}

SaltoSynth::SaltoSynth( const SaltoSynthConfig& cfg )
	: mpSynthFrame(NULL), mpCurrPeakArrayTarget(NULL), mpTransitionFrameBase(NULL),
	  mpTransitionFrameTarget(NULL), mpCurrSpectralFrameBase(NULL),	mpCurrSpectralFrameResidual(NULL),
	  mEventSample(0), mLoopStart(0), mLoopEnd(0),
	  mLimitLoopStart(0), mLimitLoopEnd(0), mInterpolEnd(0), mInterpolStart(0),	mResFadeStart(0), mResFadeEnd(0),
	  mAttackResVolume(1.0), mStatResVolume(1.0), mIPFactor(0.0), mStatResFadeInFrom(20), mStatResFadeInTo(40),
	  mLoopDirectionFW(true), mStatResLoopDirectionFW(true), 
	  mFrameCounterStatRes(0),
	  mFrameCounterAttackResidual(0), mNumFramesStatRes(0),
	  mIndividualGain(50.0), mLastPitchCorrectionFactor(0),
	  mState( Idle ), mSampleStepping( 0 ),
	  mStateIn( "StateIn", this, &SaltoSynth::UpdateState ),
  	  mInLastAlignedFrame( "Last Aligned Frame InControl", this, &SaltoSynth::UpdateLastAlignedFrame ),
	  mInBreathOnlySound( "Breath Only Sound InControl", this, &SaltoSynth::UpdateBreathOnlySound ),
	  mOut_InLoopSynthesis( "In Loop Synthesis", this ),
	  mOutUseRandomLoop( "Use Random Loop", this ),
	  mOutUseRandomDeviations("Use Random Deviations", this),
	  mOutPitchFactor( "Pitch Factor", this),
	  mOutRandomRange( "RandomRange", this ),
	  mOutTargetFreq( "Target Frequency", this ),
	  mOutMagInterpolFactor( "Magnitude Interpolation Factor", this ),
	  mOutMagGain( "Magnitude Gain", this ),
	  mOutFreqInterpolFactor( "Frequency Interpolation Factor", this ),
	  mOutMagInterpolFactor2( "Magnitude Interpolation Factor", this ),
  	  mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
	  mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
	  mOutUsePhaseAlignment( "Use Phase Alignment Out Control", this ),
	  mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
	  mOutResGain( "Resonance Gain Out Control", this ),
	  mOutResonanceFreq( "Resonance Frequency Out Control", this ),
	  mpGUI(NULL)
{
		Configure( cfg );

		AdoptChildren();

		ControlSetup();
}

bool SaltoSynth::ConcreteConfigure( const ProcessingConfig& c)
{
	bool res = true;
	
	CopyAsConcreteConfig(mConfig, c);
	try{
		mpParams = mConfig.GetParams();

		mpSynthFrame= new CSaltoSynthFrame(	MAX_SINES, 
											mConfig.GetSpectralFrameSize(), 
											mConfig.GetSampleRate() );
		if( !ConfigureChildren() )
		{
			std::string aux(mStatus);
			mStatus = "\nFailed to configure children: \n";
			mStatus += aux;
			res = false;
		}
		if( !ConfigureData() )
		{
			std::string aux(mStatus);
			mStatus = "Failed to configure data:\n";
			mStatus += aux;
			res = false;
		}
	}
	catch(std::exception &e)
	{
		mStatus += "Failed to configure children:\n";
		mStatus += e.what();
		return false;
	}

	return res;
}

bool SaltoSynth::ConcreteStart()
{
	ProcessingComposite::ConcreteStart();

	InitializeData();

	return true;
}

void SaltoSynth::InitializeData()
{
	mState = Idle;
}

bool SaltoSynth::ConcreteStop()
{
	ProcessingComposite::ConcreteStop();

	return true;
}

SaltoSynth::~SaltoSynth()
{
	//storage	
	if (mpSynthFrame!= NULL)
	{
		delete mpSynthFrame;
		mpSynthFrame=NULL;
	}

	CSaltoDataManagment::ReleaseSaltoDB();
}

void SaltoSynth::AdoptChildren(void)
{
	mSpectralSynthesis.SetParent(this);
	mInterpol.SetParent(this);
	mSineSynth.SetParent(this);
	mAttackResSynth.SetParent(this);
	mStatResSyhthesis.SetParent(this);
}

bool SaltoSynth::ConfigureChildren(void)
{
	SineSynthesisConfig sineCfg;
 
	sineCfg.SetFrameTime( mConfig.GetFrameTime() );
	sineCfg.SetSampleRate( mConfig.GetSampleRate() );
	sineCfg.SetSpectralFrameSize( mConfig.GetSpectralFrameSize() );

	if( ! mSineSynth.Configure( sineCfg ) )
	{
		mStatus += "mSineSynth: ";
		mStatus += mSineSynth.GetStatus();
		return false;
	}

	OverlapAddConfig olaCfg;
	
	olaCfg.SetHopSize(mConfig.GetHopSize());
	olaCfg.SetFrameSize(mConfig.GetHopSize()*3);
	olaCfg.SetBufferSize(mConfig.GetHopSize()*3);

	InterpolatingSynthesisConfig interpCfg;
	interpCfg.SetSpectralRange( mConfig.GetSpectralRange() );

	if( ! mInterpol.Configure( interpCfg ) )
	{
		mStatus += "mInterpol: ";
		mStatus += mInterpol.GetStatus();
		return false;
	}

	return true;
}

bool SaltoSynth::ConfigureData(void)
{
	TSize internalBufferSize = mConfig.GetHopSize();

	mOLABuffer.SetSize(internalBufferSize*2);
	AudioOutBuffer.SetSize(internalBufferSize);
	EmptyAudioBuffer.SetSize(internalBufferSize);

	mDrawingBuffer.Resize(internalBufferSize);
	mDrawingBuffer.SetSize(internalBufferSize);
	
	mSampleStepping = mConfig.GetAnalysisFrameSize();
	
	return true;
}


void SaltoSynth::ControlSetup(void)
{
	LinkOutWithInControl( 0, &mInterpol, 0 );
	LinkOutWithInControl( 1, &mInterpol, 1 );
	LinkOutWithInControl( 2, &mInterpol, 2 );
	LinkOutWithInControl( 3, &mInterpol, 3 );
	LinkOutWithInControl( 4, &mInterpol, 4 );
	LinkOutWithInControl( 5, &mInterpol, 5 );
	LinkOutWithInControl( 6, &mInterpol, 6 );
	LinkOutWithInControl( 7, &mInterpol, 7 );
	LinkOutWithInControl( 8, &mInterpol, 8 );
	LinkOutWithInControl( 9, &mInterpol, 9 );

	LinkOutWithInControl( 10, &mSineSynth, 0 );
	LinkOutWithInControl( 11, &mSineSynth, 1 );
	LinkOutWithInControl( 12, &mSineSynth, 2 );
	LinkOutWithInControl( 13, &mSineSynth, 3 );

	LinkOutWithInControl( 14, &mAttackResSynth,   0 );
	LinkOutWithInControl( 14, &mStatResSyhthesis, 0 );
	LinkOutWithInControl( 15, &mStatResSyhthesis, 1 );



	mSineSynth.LinkOutWithInControl( 0, this, 1 );
	mSineSynth.LinkOutWithInControl( 1, this, 2 );

}

void SaltoSynth::InitializeControlValues(void)
{}

int SaltoSynth::UpdateBreathOnlySound( TControlData value )
{
	if( value > 0 ) mpParams->SetBreathOnlySound( true );
	else			mpParams->SetBreathOnlySound( false );

	return 0;
}

int SaltoSynth::UpdateLastAlignedFrame( TControlData value )
{
	if( value > 0 )	mpParams->SetLastAlignedFrame( true );
	else			mpParams->SetLastAlignedFrame( false );

	return 0;
}

int SaltoSynth::UpdateState( TControlData state )
{
	if( mState == Termination ) // Ignore any update data
		return 0;
	if( state == 1 )
	{
		mState = Attack;
		return 0;
	}
	if( state == 2 )
	{
		mState = Transition;
		return 0;
	}
	if( ( state == 4 ) && ( mState == Release ) )
	{
		mState = Idle;
		return 0;
	}
	if(state == 4)
	{
		mState = Release;
		return 0;
	}

	return 0;
}

void SaltoSynth::HandleIdle(Audio*& synthBuffer)
{

	synthBuffer = &EmptyAudioBuffer;
}

void SaltoSynth::HandleAttack(Audio*& synthBuffer)
{
	mpSynthFrame->SetSynthTime( 0.0 );
	mSineSynth.ResetSineSynthesis();

	InitInterpolatingSynthesis();

	HandleSustain( synthBuffer );
	mState = Sustain;
}

void SaltoSynth::InitReleaseSynthesis()
{
	mNumFramesRelease = 6;
	mFrameCounterRelease = 1;
	mpParams->SetTransitionMode( false );
}

void SaltoSynth::HandleRelease(Audio*& synthBuffer)
{
	DoInterpolatingReleaseSynthesis( mpSynthFrame, 0.9, mFrameCounterRelease  );

	DoReleaseSynthesisProcess( mpSynthFrame );

	mFrameCounterRelease++;

	if ( mFrameCounterRelease >= mNumFramesRelease )
		mState = Idle;

	CLAM_CHECK_HEAP("Release Synthesis corrupted the heap");

	synthBuffer = &AudioOutBuffer;
}

void SaltoSynth::HandleTransition(Audio*& synthBuffer)
{
	DoTransitionSynthesis( mpSynthFrame );
								
	DoStationarySynthesisProcess( mpSynthFrame );
																		
	mFrameCounterTransition++;

	if ( mFrameCounterTransition >= mNumTransitionFrames )
	{
		EndTransitionSynthesis( mpSynthFrame );
		mState = Sustain;
	}

	synthBuffer = &AudioOutBuffer;
}

void SaltoSynth::HandleSustain(Audio*& synthBuffer)
{
	DoInterpolatingSynthesis( mpSynthFrame );
						
	DoStationarySynthesisProcess( mpSynthFrame );

	synthBuffer = &AudioOutBuffer;

}

bool SaltoSynth::Do( Audio*& synthBuffer )
{
	bool terminationReceived = false;

	switch( mState )
		{		
		case Idle:
			HandleIdle( synthBuffer);
			TRACESTATE << "State: Idle"<< std::endl;

			if ( mpParams->GetUseMelody())
				mEventSample += mSampleStepping;
			break;
		case Attack:
			HandleAttack( synthBuffer );
			TRACESTATE << "State: Attack"<< std::endl;
			mEventSample += mSampleStepping;	// Count processed samples
			break;			
		case Sustain:
			TRACESTATE << "State: Sustain"<< std::endl;
			HandleSustain( synthBuffer );
			mEventSample += mSampleStepping;	// Count processed samples
			break;
		case Transition:
			TRACESTATE << "State: Transition"<< std::endl;
			InitTransitionSynthesis( mpSynthFrame );
			mState = TransitionLoop; 
		case TransitionLoop:
			HandleTransition( synthBuffer);
			mEventSample += mSampleStepping;	// Count processed samples
			break;
		case Release:
			InitReleaseSynthesis();
			mState = ReleaseLoop;
		case ReleaseLoop:
			TRACESTATE << "State: Release"<< std::endl;
			HandleRelease( synthBuffer);
			mEventSample += mSampleStepping;	// Count processed samples
			break;
		case Termination:
			terminationReceived = true;
		}
	UpdateGUI();

	return !terminationReceived;
}

void SaltoSynth::InitInterpolatingSynthesis()
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
	
	// remember : after a transition we dont step into here again
	// this means we have do the necessary inits in CSaltoDSP::EndTransitionSynthesis too !!!
	mpParams->SetInLoopSynthesis(false);// we're not yet in the loop
	mOut_InLoopSynthesis.SendControlAsBoolean( false );
	mpParams->SetTransitionMode(false); // we're not yet in transition mode
	mLoopDirectionFW = true;// start the loop always in forwards direction
	mStatResLoopDirectionFW = true; // start stat res loop in forwards direction
	mFrameCounterTransition = 0;
	CSaltoTimbreVektor timbreVektorBase = mpParams->GetAttackTimbre();
	
	// inital values for interpolation
	mSegPositionBase = timbreVektorBase.GetPosition();
	
	// for security reasons only, can go out later
	if (mSegPositionBase>=SPECTRAL_SEGMENTS_IN_USE)
		mSegPositionBase=0;
	
	// num frames in the current sdif segment
	mNumFramesBase = handleDB->GetFramesInSeg(mSegPositionBase);
	
	mFrameCounterBase = (mpParams->GetSegDataByPos(mSegPositionBase)).GetPlayBackStart();
	
	double dB =(mpParams->GetSegDataByPos(mSegPositionBase)).GetIndividualVolume(); // CHANGE THIS NAME !!!!
	
	mIndividualGain = pow(10,dB/20.);
	
	mLimitLoopStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();
	mLimitLoopEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	if (mLimitLoopEnd<mLimitLoopStart)
		mLimitLoopEnd=mLimitLoopStart; // security
	
	mLoopStart = mLimitLoopStart; // variable borders for Random Mirror Looping
	mLoopEnd = mLimitLoopEnd; // dto.
	
	mInterpolStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolStart();
	mInterpolEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolEnd();
	mInterpolDuration = mInterpolEnd-mInterpolStart;
	
	mResFadeStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeStart();
	mResFadeEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeEnd();
	mAttackResVolume = (double)(mpParams->GetSegDataByPos(mSegPositionBase)).GetAttackResVolume();
	CLAM_ASSERT (mAttackResVolume >= 0, "mAttackResVolume < 0!!");
		
	mFrameCounterStatRes= 100;// manually set for now... should be adjustable
	mNumFramesStatRes = handleDB->GetFramesInStatResidualSegment();
	mStatResFadeInFrom= mResFadeStart;
	mStatResFadeInTo= mResFadeEnd;
	
	mStatResVolume = (double)(mpParams->GetSegDataByPos(mSegPositionBase)).GetStatResVolume();
	CLAM_ASSERT(mStatResVolume >= 0, "mStatResVolume <0!");
	
	mFrameCounterAttackResidual= 0;//(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	
	// pitch correction (fund.freq for pitch 0) use for tuning the instrument
	// and correcting individual deviation of samples
	
	mPitchReference = LOWEST_PITCH *pow(2.0,timbreVektorBase.GetPitch()/12.0);

}

void SaltoSynth::DoInterpolatingReleaseSynthesis( CSaltoSynthFrame* pSynthFrame, double gain, int currRelFrame )
{
	mpSynthFrame->ClearSpectrum();

	mOut_InLoopSynthesis.SendControlAsBoolean( mpParams->GetInLoopSynthesis() );
	mOutUseRandomDeviations.SendControlAsBoolean( mpParams->GetUseRandomDeviations() );
	mOutUseRandomLoop.SendControlAsBoolean( mpParams->GetUseRandomLoop() );
	mOutRandomRange.SendControl( mpParams->GetRandomRange() );
	mOutTargetFreq.SendControl( mpParams->GetCurrentStatTemplateFundFreq() );
	mOutMagInterpolFactor.SendControl( mIPFactor );
	mOutMagGain.SendControl( mIndividualGain / ( currRelFrame * currRelFrame ) );

	mInterpol.DoMagInterpolation( *mpCurrSpectralFrameBase,
									  *mpCurrPeakArrayTarget,
									  *mpSynthFrame );

	double pitchCorrectionFactor=1;
	
	if (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
		pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();

	mOutPitchFactor.SendControl( mpParams->GetPitchModFactor()*pitchCorrectionFactor );
	mInterpol.DoPitchMod( *pSynthFrame);
	
}

void SaltoSynth::DoReleaseSynthesisProcess( CSaltoSynthFrame *pSynthFrame )
{

	 // from here on, the content of the synthframe shouldn't be changed anymore
	
	// -- Sinusoidal Synthesis --
	if	(mpParams->GetUseSines())
	{
		mOutAttackTimbreLevel.SendControl( mpParams->GetAttackTimbre().GetLevel() );
		mOutUsePhaseAlignment.SendControlAsBoolean( mpParams->GetUsePhaseAlignment() );
		mOutLastAlignedFrame.SendControlAsBoolean( mpParams->GetLastAlignedFrame() );
		mOutBreathOnlySound.SendControlAsBoolean( mpParams->GetBreathOnlySound() );

		mSineSynth.Do( *pSynthFrame );
	}

	// -- Spectral Synthesis
	mSpectralSynthesis.Do(*(pSynthFrame->GetSpectrumPtr()),mOLABuffer);

	mOverlapAdd.Do(mOLABuffer,AudioOutBuffer);
	
}


void SaltoSynth::DoTransitionSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	pSynthFrame->ClearSpectrum();
	
	double iPFactor = (double) mFrameCounterTransition/mNumTransitionFrames;
    
	double baseFreq,targetFreq,resultFreq,a,b;
	double freqIPFactor=0;
	double gain;
	double gainReductionFactor; // -20=-10dB -40=-20dB -60=-30dB
	
	//  gainReductionFactor= -(mpParams->GetTransitionInterval() + 20); // gain reduction in transition depends on interval..
	/* the gain reduction in a transition seems to be dependent on
	   a.) the absolut pitch ( the higher the more we have to reduce the gain inbetween)
	   b.) the interval ( for small intervall we dont need that much reduction)
	*/
	gainReductionFactor= 
		- ( 20 + mpParams->GetTransitionInterval() + mpParams->GetAttackTimbre().GetPitch());
	gainReductionFactor=CLIP(gainReductionFactor,-80,-20);
	
	mpParams->SetTransitionFrequency(gainReductionFactor);
	mpParams->SetDisplayedValuesChanged(true);
	
	if (mpParams->GetTransitionUpwards())
		{ 
			int freqIPUpEnd = mNumTransitionFrames/2+1;
			int freqIPUpStart = mNumTransitionFrames/2-1;
			freqIPUpStart = CLIP(freqIPUpStart,0,mNumTransitionFrames);
			freqIPUpEnd = CLIP(freqIPUpEnd,0,mNumTransitionFrames);

			/* frequency transition function */
			if (mFrameCounterTransition<freqIPUpStart)
				{
					freqIPFactor = 0;
				}
			else if (mFrameCounterTransition>=freqIPUpStart&&mFrameCounterTransition<freqIPUpEnd)  
				{
					baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
					targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
					b = (freqIPUpEnd - freqIPUpStart) / (log10 (targetFreq/baseFreq)); // optimize and calc this factors in trans init !
					a = baseFreq/pow(10.0,freqIPUpStart/b);
					resultFreq = a * pow (10,mFrameCounterTransition/b);
					freqIPFactor = resultFreq/targetFreq;
				}
			else if (mFrameCounterTransition>freqIPUpEnd)
				{
					freqIPFactor = 1;
				}
			
			/* gain transition function */
			if (iPFactor<0.5)
				{
					gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down in the first half
				}
			else
				{
					gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
				}
		}
	else // transition downwards
		{
			int freqIPDownStart = mNumTransitionFrames/2-1;
			int freqIPDownEnd = mNumTransitionFrames/2+1;
			freqIPDownStart = CLIP(freqIPDownStart,0,mNumTransitionFrames);
			freqIPDownEnd = CLIP(freqIPDownEnd,0,mNumTransitionFrames);
			
			if (mFrameCounterTransition<freqIPDownStart)
				{
					freqIPFactor = 0;
				}
			else if (mFrameCounterTransition==freqIPDownStart)   //we are in the fourth quarter of the transition...
				{
					baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
					targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
					// optimize and calc this factors in trans init !
					b = (freqIPDownEnd - freqIPDownStart) / (log10 (targetFreq/baseFreq));
					a = baseFreq/pow(10.0,freqIPDownStart/b);
					resultFreq = a * pow (10,mFrameCounterTransition/b);
					freqIPFactor = resultFreq/targetFreq;
				}
			else if (mFrameCounterTransition>freqIPDownEnd) 
				{
					freqIPFactor = 1;
				}
			
			if (iPFactor<0.5)
				{
					gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down about 10 dB in the first half
				}
			else
				{
					gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
				}
		}
	


	mOut_InLoopSynthesis.SendControlAsBoolean( mpParams->GetInLoopSynthesis() );
	mOutUseRandomDeviations.SendControlAsBoolean( mpParams->GetUseRandomDeviations() );
	mOutUseRandomLoop.SendControlAsBoolean( mpParams->GetUseRandomLoop() );
	mOutRandomRange.SendControl( mpParams->GetRandomRange() );
	mOutTargetFreq.SendControl( mpTransitionFrameTarget->GetFundamental().GetFreq() );
	mOutMagInterpolFactor.SendControl( iPFactor );
	mOutMagGain.SendControl( gain * (mLastIndividualGain+iPFactor*(mIndividualGain-mLastIndividualGain)) );
	mOutFreqInterpolFactor.SendControl( freqIPFactor );
	mOutMagInterpolFactor2.SendControl( mIPFactor );

	mInterpol.DoInterpolation(* mpTransitionFrameBase,
								 mpTransitionFrameTarget->GetSpectralPeakArray(),
								 *pSynthFrame,
								 *mpCurrPeakArrayTarget);

	// use last pith correction factor here too
	mOutPitchFactor.SendControl( mpParams->GetPitchModFactor()*mLastPitchCorrectionFactor );
	mInterpol.DoPitchMod(*pSynthFrame);

}
void SaltoSynth::DoStationarySynthesisProcess( CSaltoSynthFrame* pSynthFrame )
{

	if ( mpParams->GetUseSines() ) 
		{	
			mOutAttackTimbreLevel.SendControl( mpParams->GetAttackTimbre().GetLevel() );
			mOutUsePhaseAlignment.SendControlAsBoolean( mpParams->GetUsePhaseAlignment() );
			mOutLastAlignedFrame.SendControlAsBoolean( mpParams->GetLastAlignedFrame() );
			mOutBreathOnlySound.SendControlAsBoolean( mpParams->GetBreathOnlySound() );

			mSineSynth.Do( *pSynthFrame );
		}

	// Attack Residual Synthesis
	if	( mpParams->GetUseAttackResidual() &!mpParams->GetPlayFrameOnly())
		{
			AttackResidualSynthesis( pSynthFrame );
		}

	// Stationary Residual Synthesis
	if ( mpParams->GetUseStatResidual() )
		{
			if ( mFrameCounterBase >= mStatResFadeInFrom )
				StationaryResidualSynthesis( pSynthFrame );
		}
	// -- Spectral Synthesis
		
	mSpectralSynthesis.Do(*(pSynthFrame->GetSpectrumPtr()),mOLABuffer);

	mOverlapAdd.Do(mOLABuffer,AudioOutBuffer);
	
}

void SaltoSynth::DoInterpolatingSynthesis(CSaltoSynthFrame *pSynthFrame)
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
	
	mpSynthFrame->ClearSpectrum();  // reset synth frame OPTIMIZE
	
	// -- data managment BaseFrame --
	if (mpParams->GetUseRandomLoop())
		{
			if (mFrameCounterBase==mLoopStart)
				{
					mLoopDirectionFW = true;
					mpParams->SetInLoopSynthesis(true);       // indicates that we are in loop mode
					mOut_InLoopSynthesis.SendControlAsBoolean( true );
					int randRange = mLimitLoopEnd-mLoopStart;
					mLoopEnd = ((TFloat32)rand()/RAND_MAX)*randRange+mLoopStart;	
				}
			else if (mFrameCounterBase> mLoopEnd)
				{
					mLoopDirectionFW = false;
					mpParams->SetInLoopSynthesis(true);       // indicates that we are in loop mode
					mOut_InLoopSynthesis.SendControlAsBoolean( true );
					int randRange = mLoopEnd-mLimitLoopStart;
					mLoopStart = ((TFloat32)rand()/RAND_MAX)*randRange+mLimitLoopStart;
					CLAM_ASSERT(mLoopStart >= 0, "mLoopStart < 0!!!");
				}
		}
	else  // repeat always one frame...
		{
			if (mFrameCounterBase>=mLoopStart)    
				{
					mFrameCounterBase=mLoopStart;
					mpParams->SetInLoopSynthesis(true);
					mOut_InLoopSynthesis.SendControlAsBoolean( true );
				}
		}	
	
	mpCurrSpectralFrameBase = handleDB->GetSpectralFrame(mSegPositionBase,mFrameCounterBase);
	
	// increase or decrease position of readout frame depending on loop direction
	if (mLoopDirectionFW)
		mFrameCounterBase++;
	else
		mFrameCounterBase--;
	
	if(mFrameCounterBase>=mNumFramesBase)        // security-stop at end of the segment
		{
			mFrameCounterBase=0;
			mpParams->SetPlay(false);
			return;
		}
	
	// -- data managment TargetFrame --
	mpCurrPeakArrayTarget = mpParams->GetCurrentStatTemplatePtr();
	
	// -- data managment attack residual --
	TIndex nResFrames= handleDB->GetFramesInSeg(mpParams->GetAttackTimbre().GetPosition());
	
	if (mFrameCounterAttackResidual>=nResFrames)
		mFrameCounterAttackResidual=nResFrames-1;
	
	mpCurrSpectralFrameResidual = handleDB->GetSpectralFrame(mpParams->GetAttackTimbre().GetPosition(),mFrameCounterAttackResidual);

	pSynthFrame->SetResidualPtr(&mpCurrSpectralFrameResidual->GetResidualSpec()); //BUGFRAME? //SEGMENTBUG
	
	
	mFrameCounterAttackResidual++;
	
	// spectral interpolation process starts after 'mInterpolationstart' Frames
	if(mFrameCounterBase>=mInterpolStart)
		{
			mIPFactor =  mpParams->GetInterPolFactor() * (mFrameCounterBase-mInterpolStart)/mInterpolDuration;
			if (mFrameCounterBase>=mInterpolEnd)
				mIPFactor = mpParams->GetInterPolFactor();
		}
	else
		{
			mIPFactor=0;
		}

	mOut_InLoopSynthesis.SendControlAsBoolean( mpParams->GetInLoopSynthesis() );
	mOutUseRandomDeviations.SendControlAsBoolean( mpParams->GetUseRandomDeviations() );
	mOutUseRandomLoop.SendControlAsBoolean( mpParams->GetUseRandomLoop() );
	mOutRandomRange.SendControl( mpParams->GetRandomRange() );
	mOutTargetFreq.SendControl( mpParams->GetCurrentStatTemplateFundFreq() );
	mOutMagInterpolFactor.SendControl( mIPFactor );
	mOutMagGain.SendControl( mIndividualGain );

	mInterpol.DoMagInterpolation( *mpCurrSpectralFrameBase,
									  *mpCurrPeakArrayTarget,
									  *pSynthFrame );

	double pitchCorrectionFactor=1.0;
	if  (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
		pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	mLastPitchCorrectionFactor=pitchCorrectionFactor; // used in transition

	mOutPitchFactor.SendControl( mpParams->GetPitchModFactor()*pitchCorrectionFactor );
	mInterpol.DoPitchMod(*pSynthFrame);
}

void SaltoSynth::AttackResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	double ipFactor = 1;
	
	if (mFrameCounterBase >= mResFadeStart)
		{
			// precalculate the reciprocal to avoid the floating point division
			// DEBUG_ASSERT added to check against divisions by zero
			CLAM_DEBUG_ASSERT( (mResFadeEnd-mResFadeStart), "Division by zero! check mResFadeEnd and mResFadeStart");


			// calc fade-out factor for decaying attack residual
			ipFactor = 1-(mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);

			// if the previous difference results in a negative value we clamp it to
			// [ 0, +inf ]
			if ( ipFactor < 0 )
				ipFactor = 0;
		}

	// The conditional below exists due to an efficiency issue: since the ipFactor might be zero or one it pays
	// to check wether we have to do complicated multiplications or not

	if ( !ipFactor )
	{
		mOutResGain.SendControl( 0.0 );
		mAttackResSynth.Do( *pSynthFrame );
	}
	else
		{
			// precalculate the reciprocal of 127.0
			static const double inv_127 = 1.0/127.0;

			// ipFactor has not been changed
			if ( ipFactor == 1 ) 
			{
				mOutResGain.SendControl( mAttackResVolume*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
				mAttackResSynth.Do( *pSynthFrame );
			}
			// otherwise we are in the worst case
			else
			{
				mOutResGain.SendControl( mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
				mAttackResSynth.Do( *pSynthFrame );
			}

		}
}

void SaltoSynth::StationaryResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	static const double inv_127 = 1.0/127.0;
	static const double inv_150 = 1.0/150.0;
	Frame* pStatResidual = NULL;

	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
	pStatResidual = handleDB->GetStatResidualSpectralFrame(mFrameCounterStatRes);
	
	if ( mStatResLoopDirectionFW )
		{
			mFrameCounterStatRes++;
			if (mFrameCounterStatRes >= mNumFramesStatRes-100)
				mStatResLoopDirectionFW=!mStatResLoopDirectionFW;
		}
	else
		{
			mFrameCounterStatRes--;
			if (mFrameCounterStatRes <= 100)
				mStatResLoopDirectionFW=!mStatResLoopDirectionFW;
		}
	
	double ipFactor = (mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
	double level = (double) mpParams->GetAttackTimbre().GetLevel() * inv_127;
	double correctionFactor = log(8.5*level+1.5)*inv_150 * mpParams->GetStatResGain();
	
	// -- TEST increase level of stationary residual for smaller volumes...
	// We clamp down the ipFactor value, and since ipFactor is the identiy it pays not
	// to multiply it

	if ( ipFactor > 1 )
		{
			mOutResGain.SendControl( mStatResVolume*correctionFactor );
			mOutResonanceFreq.SendControl( mpParams->GetStatResFreq() );
			mStatResSyhthesis.Do( *pSynthFrame, *pStatResidual );
		}
	else
		{
			mOutResGain.SendControl( mStatResVolume*ipFactor*correctionFactor );
			mOutResonanceFreq.SendControl( mpParams->GetStatResFreq() );
			mStatResSyhthesis.Do( *pSynthFrame, *pStatResidual );
		}
}

void SaltoSynth::InitTransitionSynthesis( CSaltoSynthFrame *pSynthFrame)
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
	
	mpTransitionFrameBase = mpCurrSpectralFrameBase;
	mSegPositionTransition = mpParams->GetAttackTimbre().GetPosition(); // its already updated in CSaltoMIDI !
	TIndex loopFrameTarget = (mpParams->GetSegDataByPos(mSegPositionTransition)).GetLoopStart();
	mpTransitionFrameTarget = handleDB->GetSpectralFrame(mSegPositionTransition,loopFrameTarget);
	
	// check for level differences in transition mode
	mNumTransitionFrames = mpParams->GetAttackTimbre().GetTransitionFrames();
	mFrameCounterTransition = 1;// init to one to discard the first transition frame which would be equal to the last baseframe
	
	// pitch correction
	mPitchReference = LOWEST_PITCH*pow(2.0,mpParams->GetAttackTimbre().GetPitch()/12.0); // c3 == pitch0
	
	TFloat64 dB = (mpParams->GetSegDataByPos(mSegPositionTransition)).GetIndividualVolume();
	mLastIndividualGain = mIndividualGain;
	mIndividualGain = pow(10,dB/20.);

	mpParams->SetTransitionInit(false);
}

void SaltoSynth::EndTransitionSynthesis(CSaltoSynthFrame *pSynthFrame)
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();

	mFrameCounterTransition = 0;
	mpParams->SetTransitionMode(false);
	mpParams->SetDisplayedValuesChanged(true); // display needs update
	mLoopDirectionFW = true;// start the loop always in forwards direction
	mStatResLoopDirectionFW = true; // start stat res loop in forwards direction

	// set new params for interpolating synthesis
	// just like init interpolating synthesis, but respecting the new stationary
	// base frame
	mSegPositionBase = mSegPositionTransition;
	mpCurrSpectralFrameBase = mpTransitionFrameTarget;

	mNumFramesBase = handleDB->GetFramesInSeg(mSegPositionBase);

	// framecounter already set to enter loop
	mFrameCounterBase =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();

	double dB = (mpParams->GetSegDataByPos(mSegPositionBase)).GetIndividualVolume();

	mIndividualGain = pow(10,dB/20.);

	mLimitLoopStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();
	mLimitLoopEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	if (mLimitLoopEnd<mLimitLoopStart)
		mLimitLoopEnd=mLimitLoopStart; // security

	mLoopStart = mLimitLoopStart; // variable borders for Random Mirror Looping
	mLoopEnd = mLimitLoopEnd; // dto.

	// set these anyway, though not needed
	mInterpolStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolStart();
	mInterpolEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolEnd();
	mInterpolDuration = mInterpolEnd-mInterpolStart;

	mResFadeStart=(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeStart();
	mResFadeEnd=(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeEnd();

	mStatResFadeInFrom= mResFadeStart;
	mStatResFadeInTo= mResFadeEnd;

	mAttackResVolume = 0; // there is no need for another attack residual after transition

}

void SaltoSynth::UpdateGUI( )
{
	if (!mpGUI) return;
	if (mState == Idle ) return;

	pthread_mutex_lock(&DrawingMutex);
	mpGUI->FillDisplayPeakArray(*GetCurrentPeakArrayPtr());
	mpGUI->FillDisplayAttackResidual(*GetCurrentResidualPtr());
	mpGUI->FillDisplaySynthesizedSpectrum(*GetSynthFramePtr()->GetSpectrumPtr());
	pthread_mutex_unlock(&DrawingMutex);
}

const SpectralPeakArray* SaltoSynth::GetCurrentPeakArrayPtr()
{
  return mpSynthFrame->GetPeakArrayPtr();
}

const Spectrum* SaltoSynth::GetCurrentResidualPtr()
{
  return mpSynthFrame->GetResidualPtr();
}

void SaltoSynth::BindWithGUI( CSaltoEditor* pGUI )
{
	mpGUI = pGUI;
	mpGUI->SetDisplayParameters( mConfig.GetSampleRate(), mConfig.GetSpectralFrameSize() );
}

/* METHODS REQUIRED BY CSALTOEDITOR... */

Frame* SaltoSynth::GetStatTmplFrame(TIndex pos)
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();

  if (pos >= handleDB->GetFramesInEditSegment())
    pos = handleDB->GetFramesInEditSegment()-1;

  Frame* pCurrSpecFrame = handleDB->GetEditSpectralFrame(pos);

  return pCurrSpecFrame;	  	
}

const DataArray& SaltoSynth::GetAudioBufferReference()
{
  //TODO XA: is this ok?
	int constant;
	if(mpParams->GetWriteToFile())
	{
		constant=32768*10;
	}
	else
		constant=32768;

	DataArray& tmpArray=AudioOutBuffer.GetBuffer();
	for(int i=0;i<mDrawingBuffer.Size();i++)
		mDrawingBuffer[i]=tmpArray[i]*constant;
	return mDrawingBuffer;
}


}
