#include "SaltoSynth.hxx"
#include "CSaltoEditor.hxx"
#include "CSaltoDataManagment.hxx"
#include "Assert.hxx"
#include "HeapDbg.hxx"
#include <pthread.h>
#include <iostream>

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
	SetPitchModRange( 0.05946309436 ); // 1 semitone
}

SaltoSynth::SaltoSynth()
	: mpSynthFrame(NULL), mpCurrPeakArrayTarget(NULL), mpTransitionFrameBase(NULL),
	  mpTransitionFrameTarget(NULL), mpCurrSpectralFrameBase(NULL),	mpCurrSpectralFrameResidual(NULL),
	  mEventSample(0), mLoopStart(0), mLoopEnd(0),
	  mLimitLoopStart(0), mLimitLoopEnd(0), mInterpolEnd(0), mInterpolStart(0),	mResFadeStart(0), mResFadeEnd(0),
	  mAttackResVolume(1.0), mStatResVolume(1.0), mIPFactor(0.0), mStatResFadeInFrom(20), mStatResFadeInTo(40),
	  mLoopDirectionFW(true), mStatResLoopDirectionFW(true), 
	  mFrameCounterStatRes(0),
	  mFrameCounterAttackResidual(0), mNumFramesStatRes(0), mSinAttackOffset(0), mSinAttackOffsetCounter(0),
	  mIndividualGain(50.0), mLastPitchCorrectionFactor(0),	mpSineSynthPO(NULL), mpResSynthPO(NULL), mpInterpolPO(NULL),
	  mStateIn( "StateIn", this, &SaltoSynth::UpdateState ),
	  mState( Idle ), mSampleStepping( 0 )
{
		SaltoSynthConfig cfg;

		Configure( cfg );
}

SaltoSynth::SaltoSynth( const SaltoSynthConfig& cfg )
	: mpSynthFrame(NULL), mpCurrPeakArrayTarget(NULL), mpTransitionFrameBase(NULL),
	  mpTransitionFrameTarget(NULL), mpCurrSpectralFrameBase(NULL),	mpCurrSpectralFrameResidual(NULL),
	  mEventSample(0), mLoopStart(0), mLoopEnd(0),
	  mLimitLoopStart(0), mLimitLoopEnd(0), mInterpolEnd(0), mInterpolStart(0),	mResFadeStart(0), mResFadeEnd(0),
	  mAttackResVolume(1.0), mStatResVolume(1.0), mIPFactor(0.0), mStatResFadeInFrom(20), mStatResFadeInTo(40),
	  mLoopDirectionFW(true), mStatResLoopDirectionFW(true), 
	  mFrameCounterStatRes(0),
	  mFrameCounterAttackResidual(0), mNumFramesStatRes(0), mSinAttackOffset(0), mSinAttackOffsetCounter(0),
	  mIndividualGain(50.0), mLastPitchCorrectionFactor(0),	mpSineSynthPO(NULL), mpResSynthPO(NULL), mpInterpolPO(NULL),
	  mStateIn( "StateIn", this, &SaltoSynth::UpdateState ),
	  mState( Idle ), mSampleStepping( 0 )
{
		Configure( cfg );
}

bool SaltoSynth::ConcreteConfigure( const ProcessingConfig& cfg)throw( std::bad_cast )
{
	mConfig = dynamic_cast< const SaltoSynthConfig& >( cfg );
	
	mpParams = mConfig.GetParams();

	mpSynthFrame= new CSaltoSynthFrame(	MAX_SINES, 
										mConfig.GetSpectralFrameSize(), 
										mConfig.GetSampleRate() );
	
	TSize internalBufferSize = mConfig.GetHopSize();

	AudioOutBuffer.SetSize(internalBufferSize);
	EmptyAudioBuffer.SetSize(internalBufferSize);

	mDrawingBuffer.Resize(internalBufferSize);
	mDrawingBuffer.SetSize(internalBufferSize);
		
	CSaltoDataManagment::InitSaltoDB( mpParams );

	// processing objects
	mpSineSynthPO = new CSaltoSineSynthesis(	mConfig.GetSpectralFrameSize(),
												MAX_SINES,
												mConfig.GetFrameTime(),
												mConfig.GetSampleRate(),
												mpParams);
																							
	mpResSynthPO = new CSaltoResidualSynthesis(	mConfig.GetSpectralFrameSize(),
												mConfig.GetFrameTime());
	SpectralSynthesisConfig sscfg;
	sscfg.SetFrameSize(mConfig.GetHopSize());
	sscfg.SetHopSize(mConfig.GetHopSize());
	sscfg.SetAnalWindowType(mConfig.GetAnalysisWindowType());
	sscfg.SetSamplingRate(mConfig.GetSampleRate());
	sscfg.SetSynthWindowSize(mConfig.GetAudioFrameSize()+1);
	mSpectralSynthesisPO.Configure(sscfg);
	mSpectralSynthesisPO.Start();
			
	CSaltoInterpolationConfig interpCfg;
	interpCfg.SetParams( *mpParams );
	interpCfg.SetSpectralRange( mConfig.GetSpectralRange() );

	mpInterpolPO = new CSaltoInterpolation( interpCfg );
	
	if( mpSineSynthPO==NULL || mpResSynthPO==NULL || mpInterpolPO==NULL )
		throw Err("OOM in CSaltoDSP::CSaltoDSP");

	mSampleStepping = mConfig.GetAnalysisFrameSize();

	return true;
}

bool SaltoSynth::ConcreteStart()
{
	mSpectralSynthesisPO.Start();
	mState = Idle;

	return true;
}

bool SaltoSynth::ConcreteStop()
{
	mSpectralSynthesisPO.Stop();

	return true;
}

SaltoSynth::~SaltoSynth()
{
	//storage	
	if (mpSynthFrame!= NULL)
		delete mpSynthFrame;

	CSaltoDataManagment::ReleaseSaltoDB();
	
	// processing objects
	if (mpSineSynthPO!=NULL)
		delete mpSineSynthPO;
	
	if (mpResSynthPO!=NULL)
		delete mpResSynthPO;
		
	if (mpInterpolPO!=NULL)
		delete mpInterpolPO;	
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
	if( ( state == 4 ) && ( ( mState == Sustain ) || ( mState == Transition ) ) )
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
	mpSineSynthPO->ResetSineSynthesis();

	InitInterpolatingSynthesis();

	HandleSustain(synthBuffer);

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

	DoTransitionSynthesis2( mpSynthFrame );
										
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
	//std::cout << "MIDI Handled" << std::endl;
	bool terminationReceived = false;

	switch( mState )
		{
		
		case Idle:
			HandleIdle( synthBuffer);
			if ( mpParams->GetUseMelody())
				mEventSample += mSampleStepping;
			break;

		case Attack:
			HandleAttack( synthBuffer );
			mEventSample += mSampleStepping;	// Count processed samples
			break;
			
		case Sustain:
			HandleSustain( synthBuffer );
			mEventSample += mSampleStepping;	// Count processed samples
			break;

		case Transition:
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
	mpParams->SetTransitionMode(false); // we're not yet in transition mode
	mLoopDirectionFW = true;// start the loop always in forwards direction
	mStatResLoopDirectionFW = true; // start stat res loop in forwards direction
	mFrameCounterTransition = 0;
	mSinAttackOffsetCounter = 0;
	
	mTimbreVektorBase = mpParams->GetAttackTimbre();
	
	// inital values for interpolation
	mSegPositionBase = mTimbreVektorBase.GetPosition();
	
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
	
	mSinAttackOffset =0;
	
	mFrameCounterStatRes= 100;// manually set for now... should be adjustable
	mNumFramesStatRes = handleDB->GetFramesInStatResidualSegment();
	mStatResFadeInFrom= mResFadeStart;
	mStatResFadeInTo= mResFadeEnd;
	
	mStatResVolume = (double)(mpParams->GetSegDataByPos(mSegPositionBase)).GetStatResVolume();
	CLAM_ASSERT(mStatResVolume >= 0, "mStatResVolume <0!");
	
	mFrameCounterAttackResidual= 0;//(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	
	// pitch correction (fund.freq for pitch 0) use for tuning the instrument
	// and correcting individual deviation of samples
	
	mPitchReference = LOWEST_PITCH *pow(2.0,mTimbreVektorBase.GetPitch()/12.0);

}

void SaltoSynth::DoInterpolatingReleaseSynthesis( CSaltoSynthFrame* pSynthFrame, double gain, int currRelFrame )
{
	mpSynthFrame->ClearSpectrum();

	mpInterpolPO->DoInterpolation2( mpCurrSpectralFrameBase,
									mpCurrPeakArrayTarget,
									mpParams->GetCurrentStatTemplateFundFreq(),
									mIPFactor,
									0,
									mIndividualGain/(currRelFrame*currRelFrame),
									mpSynthFrame );
	double pitchCorrectionFactor=1;
	
	if (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
		pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*pitchCorrectionFactor);
	
}

void SaltoSynth::DoReleaseSynthesisProcess( CSaltoSynthFrame *pSynthFrame )
{

	 // from here on, the content of the synthframe shouldn't be changed anymore
	
	// -- Sinusoidal Synthesis --
	if	(mpParams->GetUseSines())
	{
		mpSineSynthPO->DoSineSynthesis( *pSynthFrame );
	}

	// -- Spectral Synthesis
	mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
	
}

void SaltoSynth::DoTransitionSynthesis2( CSaltoSynthFrame* pSynthFrame )
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
	
	mpInterpolPO->DoInterpolation2
		(mpTransitionFrameBase,
		 &mpTransitionFrameTarget->GetSpectralPeakArray(),
		 mpTransitionFrameTarget->GetFundamental().GetFreq(),
		 iPFactor,
		 freqIPFactor,
		 gain * (mLastIndividualGain+iPFactor*(mIndividualGain-mLastIndividualGain)), // interpolate individual gain factors	
		 pSynthFrame,
		 true,
		 true,
		 mpCurrPeakArrayTarget,
		 mIPFactor);
	
	// use last pith correction factor here too
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*mLastPitchCorrectionFactor);	
}

void SaltoSynth::DoStationarySynthesisProcess( CSaltoSynthFrame* pSynthFrame )
{

	if ( mpParams->GetUseSines() )
		{
			mpSineSynthPO->DoSineSynthesis( *pSynthFrame );
		}

	// Attack Residual Synthesis
	if	( mpParams->GetUseAttackResidual() )//&!mpParams->GetPlayFrameOnly())
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
	mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
	
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
					int randRange = mLimitLoopEnd-mLoopStart;
					//mLoopEnd = (Random()+32767)/65535.0*randRange+mLoopStart;	
					mLoopEnd = ((TFloat32)rand()/RAND_MAX)*randRange+mLoopStart;	
				}
			else if (mFrameCounterBase> mLoopEnd)
				{
					mLoopDirectionFW = false;
					mpParams->SetInLoopSynthesis(true);       // indicates that we are in loop mode
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
	
	mpInterpolPO->DoInterpolation2(mpCurrSpectralFrameBase,
								   mpCurrPeakArrayTarget,
								   mpParams->GetCurrentStatTemplateFundFreq(),//0 // default no freq interpol
								   mIPFactor,      // mag IP Factor
								   0,              // freq IP Factor
								   mIndividualGain,// magnitude gain
								   pSynthFrame);
	
	double pitchCorrectionFactor=1.0;
	if  (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
		pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	mLastPitchCorrectionFactor=pitchCorrectionFactor; // used in transition
	
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*pitchCorrectionFactor);
}

void SaltoSynth::AttackResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	double ipFactor = 1;
	
	if (mFrameCounterBase >= mResFadeStart)
		{
			// precalculate the reciprocal to avoid the floating point division
			// ipfactor = 1 - ( (mFrameCounterBase - mResFadeStart) / ( mResFadeEnd - mResFadeStart ) )
			// DEBUG_ASSERT added to check against divisions by zero
			CLAM_DEBUG_ASSERT( (mResFadeEnd-mResFadeStart), "Division by zero! check mResFadeEnd and mResFadeStart");

			double difference_reciprocal = 1 / ( mResFadeEnd-mResFadeStart );

			// calc fade-out factor for decaying attack residual
			ipFactor = 1 - ( (mFrameCounterBase-mResFadeStart) * difference_reciprocal );

			// if the previous difference results in a negative value we clamp it to
			// [ 0, +inf ]
			if ( ipFactor < 0 )
				ipFactor = 0;
		}

	// The conditional below exists due to an efficiency issue: since the ipFactor might be zero or one it pays
	// to check wether we have to do complicated multiplications or not

	if ( !ipFactor )
		mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,0);
	else
		{
			// precalculate the reciprocal of 127.0
			static const double inv_127 = 1.0/127.0;

			// ipFactor has not been changed
			if ( ipFactor == 1 ) 
				mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
												mAttackResVolume*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
			// otherwise we are in the worst case
			else
				mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
												mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()*inv_127 );

		}
}

void SaltoSynth::StationaryResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	/*	
	double ipFactor = 1;

	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();

	Frame* pStatResidual=NULL;		
	if (mFrameCounterBase>=mStatResFadeInFrom)
		{
			// calc fade-in factor for increasing stationary residual
			ipFactor = (mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
			if(ipFactor>1) 
				ipFactor=1;
			
			// for now the stationary loop is readout forwards,backwards, forward....
			// a random mirror looping would be nice here too ??
			if (mFrameCounterStatRes<mNumFramesStatRes-100 && mStatResLoopDirectionFW)
				{
					pStatResidual = handleDB->GetStatResidualSpectralFrame(mFrameCounterStatRes);
					mFrameCounterStatRes++;
					if (mFrameCounterStatRes >= mNumFramesStatRes-100)
						mStatResLoopDirectionFW=false;
					
				}
			else if ((mFrameCounterStatRes<=mNumFramesStatRes-100) &&(! mStatResLoopDirectionFW))
				{
					pStatResidual = handleDB->GetStatResidualSpectralFrame(mFrameCounterStatRes);
					mFrameCounterStatRes--;
					if (mFrameCounterStatRes <= 100)
						mStatResLoopDirectionFW=true;
				}
		}
	// -- TEST increase level of stationary residual for smaller volumes...
	double level = (double) mpParams->GetAttackTimbre().GetLevel()/127.;
	double correctionFactor = 0;
	
	correctionFactor = log(8.5*level+1.5)/150 * mpParams->GetStatResGain();
	
	mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
										   mStatResVolume*ipFactor*correctionFactor,mpParams->GetStatResFreq());
	*/

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
			mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
												   mStatResVolume*correctionFactor,mpParams->GetStatResFreq());
		}
	else
		{
			mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
												   mStatResVolume*ipFactor*correctionFactor,mpParams->GetStatResFreq());				
		}
}

void SaltoSynth::InitTransitionSynthesis(CSaltoSynthFrame *pSynthFrame)
{
	CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
	
	mpTransitionFrameBase = mpCurrSpectralFrameBase;
	mSegPositionTransition = mpParams->GetAttackTimbre().GetPosition(); // its already updated in CSaltoMIDI !
	TIndex loopFrameTarget = (mpParams->GetSegDataByPos(mSegPositionTransition)).GetLoopStart();
	mpTransitionFrameTarget = handleDB->GetSpectralFrame(mSegPositionTransition,loopFrameTarget);
	
	// check for level differences in transition mode
	// mLevelAtBeginOfTransition = mTimbreVektorBase.GetLevel();
	// mLevelAtEndOfTransition = mpParams->GetAttackTimbre().GetLevel();
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
	if ( mState == Idle )
		return;

	pthread_mutex_lock(&DrawingMutex);
	mpGUI->FillDisplayPeakArray(*GetCurrentPeakArrayPtr());
	mpGUI->FillDisplayAttackResidual(*GetCurrentResidualPtr());
	//	this->mpGUI->FillDisplayStationaryResidual(this->mpCurrSpectralFrameResidual->GetResidual()); //should be stationary!!
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
