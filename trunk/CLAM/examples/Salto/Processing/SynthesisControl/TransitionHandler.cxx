#include "TransitionHandler.hxx"

namespace SALTO
{
	void TransitionHandlerConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	TransitionHandler::TransitionHandler():
		mpTransitionFrameBase( NULL ), mpTransitionFrameTarget( NULL ),
		mUseRandomLoop( false ), mUseRandomDeviations( false ) , mRandomRange( 8 ),
		mInUseRandomLoop( "Use Random Loop In Control", this, &TransitionHandler::UseRandomLoopCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &TransitionHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this, &TransitionHandler::RandomRangeCB ),
		mInTransitionInterval( "Transition Inerval In Control", this, &TransitionHandler::TransitionIntervalCB ),
		mInTransitionUpwards( "Transition Upwards In Control", this, &TransitionHandler::TransitionUpwardsCB ),
		mOut_InLoopSynthesis( " In Loop Synthesis Control", this ),
	    mOutUseRandomLoop( " Use Random Loop Control", this),
	    mOutUseRandomDeviations( " Use Random Deviations Control", this),
	    mOutPitchFactor( "Pitch Factor Control", this),
	    mOutRandomRange( "Random Range Control", this),
	    mOutTargetFreq( "Target Frequency Control", this),
	    mOutMagInterpolFactor( "Magnitude Interpolation Factor" , this),
	    mOutMagGain( "Magnitude Gain", this),
	    mOutFreqInterpolFactor( "Frequency Interpolation Factor Control", this),
	    mOutMagInterpolFactor2( "Magnitude Interpolation Factor 2 Control", this),
		mOutTransitionInit( "Transtition Init Out Control", this ),
		mOutTransitionFrequency( "Transition Frequency Out Control", this ),
		mOutDisplayedValuesChanged( "Displayed Values Changed Out Control", this ),
		mOutState( "State Out Control", this )
	{
		TransitionHandlerConfig cfg;

		Configure( cfg );
	}

	TransitionHandler::TransitionHandler( TransitionHandlerConfig &cfg ):
		mpTransitionFrameBase( NULL ), mpTransitionFrameTarget( NULL ),
		mUseRandomLoop( false ), mUseRandomDeviations( false ) , mRandomRange( 8 ),
		mInUseRandomLoop( "Use Random Loop In Control", this, &TransitionHandler::UseRandomLoopCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &TransitionHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this, &TransitionHandler::RandomRangeCB ),
		mInTransitionInterval( "Transition Inerval In Control", this, &TransitionHandler::TransitionIntervalCB ),
		mInTransitionUpwards( "Transition Upwards In Control", this, &TransitionHandler::TransitionUpwardsCB ),
		mOut_InLoopSynthesis( " In Loop Synthesis Control", this ),
	    mOutUseRandomLoop( " Use Random Loop Control", this),
	    mOutUseRandomDeviations( " Use Random Deviations Control", this),
	    mOutPitchFactor( "Pitch Factor Control", this),
	    mOutRandomRange( "Random Range Control", this),
	    mOutTargetFreq( "Target Frequency Control", this),
	    mOutMagInterpolFactor( "Magnitude Interpolation Factor" , this),
	    mOutMagGain( "Magnitude Gain", this),
	    mOutFreqInterpolFactor( "Frequency Interpolation Factor Control", this),
	    mOutMagInterpolFactor2( "Magnitude Interpolation Factor 2 Control", this),
		mOutTransitionInit( "Transtition Init Out Control", this ),
		mOutTransitionFrequency( "Transition Frequency Out Control", this ),
		mOutDisplayedValuesChanged( "Displayed Values Changed Out Control", this ),
		mOutState( "State Out Control", this )
	{
		Configure( cfg );
	}

	TransitionHandler::~TransitionHandler()
	{}

	bool TransitionHandler::ConcreteConfigure( const ProcessingConfig &c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	bool TransitionHandler::ConcreteStart()
	{
		return true;
	}

	bool TransitionHandler::ConcreteStop()
	{
		return true;
	}

	bool TransitionHandler::Do()
	{
		return true;
	}

	int TransitionHandler::TransitionIntervalCB( TControlData value )
	{
		mTransitionInterval = value;

		return 0;
	}

	int TransitionHandler::TransitionUpwardsCB( TControlData value )
	{
		if( value > 0 )		mTransitionUpwards = true;
		else				mTransitionUpwards = false;

		return 0;
	}

	int TransitionHandler::UseRandomDeviationsCB( TControlData value )
	{
		if( value > 0 )		mUseRandomDeviations = true;
		else				mUseRandomDeviations = false;

		return 0;
	} 

	int TransitionHandler::UseRandomLoopCB( TControlData value )
	{
		if( value > 0 )		mUseRandomLoop = true;
		else				mUseRandomLoop = false;

		return 0;
	}

	int TransitionHandler::RandomRangeCB( TControlData value )
	{
		mRandomRange = value;

		return 0;
	}

	void TransitionHandler::InitTransitionSynthesis( SynthesisState &synthState,
													 Frame			*pCurrSpectralFrameBase)
	{
		CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
		
		mpTransitionFrameBase = pCurrSpectralFrameBase;
		mSegPositionTransition = synthState.GetAttackTimbre().GetPosition(); // its already updated in CSaltoMIDI !
		TIndex loopFrameTarget = ( synthState.GetSegDataByPos( mSegPositionTransition ) ).GetLoopStart();
		mpTransitionFrameTarget = handleDB->GetSpectralFrame( mSegPositionTransition,loopFrameTarget );
		
		// check for level differences in transition mode
		// mLevelAtBeginOfTransition = mTimbreVektorBase.GetLevel();
		// mLevelAtEndOfTransition = mpParams->GetAttackTimbre().GetLevel();
		mNumTransitionFrames = synthState.GetAttackTimbre().GetTransitionFrames();
		synthState.SetFrameCounterTransition( 1 );// init to one to discard the first transition frame which would be equal to the last baseframe
		
		// pitch correction
		synthState.SetPitchReference( LOWEST_PITCH*pow(2.0,synthState.GetAttackTimbre().GetPitch()/12.0) ); // c3 == pitch0
		
		TFloat64 dB = ( synthState.GetSegDataByPos(mSegPositionTransition)).GetIndividualVolume();
		mLastIndividualGain = synthState.GetIndividualGain();

		double gainValue = pow(10,dB/20.);

		synthState.SetIndividualGain( gainValue );

		mOutTransitionInit.SendControlAsBoolean(false);
	}

	bool TransitionHandler::Do( CSaltoSynthFrame   &pSynthFrame,
								SynthesisState     &synthState,
								Frame			   *pTransitionFrameTarget,
								Frame              *pCurrSpectralFrameBase)
	{
		DoTransitionSynthesis( pSynthFrame, synthState, pTransitionFrameTarget );							
		
		//DoStationarySynthesisProcess( mpSynthFrame );
		mSynthProcessor->Do( synthState );
																			
		synthState.SetFrameCounterTransition( synthState.GetFrameCounterTransition() + 1 );

		if ( synthState.GetFrameCounterTransition() >= mNumTransitionFrames )
		{
			EndTransitionSynthesis( synthState, pCurrSpectralFrameBase );
			//mState = Sustain;
			mOutState.SendControl( 2 );
		}

		return true;
	}

	void TransitionHandler::EndTransitionSynthesis( SynthesisState &synthState,
													Frame          *pCurrSpectralFrameBase)
	{
		CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();

		synthState.SetFrameCounterTransition( 0 );
		synthState.SetTransitionMode(false);
		mOutDisplayedValuesChanged.SendControlAsBoolean(true); // display needs update
		synthState.SetLoopDirectionFW( true );// start the loop always in forwards direction
		synthState.SetStatResLoopDirectionFW( true ); // start stat res loop in forwards direction

		// set new params for interpolating synthesis
		// just like init interpolating synthesis, but respecting the new stationary
		// base frame
		synthState.SetSegPositionBase( mSegPositionTransition );
		pCurrSpectralFrameBase = mpTransitionFrameTarget;

		synthState.SetNumFramesBase( handleDB->GetFramesInSeg( synthState.GetSegPositionBase() ) );

		// framecounter already set to enter loop
		synthState.SetFrameCounterBase( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetLoopStart() );

		double dB = ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetIndividualVolume();

		synthState.SetIndividualGain( pow(10,dB/20.) );

		synthState.SetLimitLoopStart(( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetLoopStart() );
		synthState.SetLimitLoopEnd( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetLoopEnd() );
		if ( synthState.GetLimitLoopEnd() < synthState.GetLimitLoopStart() )
			synthState.SetLimitLoopEnd( synthState.GetLimitLoopStart() ); // security

		synthState.SetLoopStart( synthState.GetLimitLoopStart() ); // variable borders for Random Mirror Looping
		synthState.SetLoopEnd( synthState.GetLimitLoopEnd() );// dto.

		// set these anyway, though not needed
		synthState.SetInterpolStart( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetInterpolStart() );
		synthState.SetInterpolEnd( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetInterpolEnd() );
		synthState.SetInterpolDuration( synthState.GetInterpolEnd() - synthState.GetInterpolStart() );

		synthState.SetResFadeStart( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetResFadeStart() );
		synthState.SetResFadeEnd( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() )).GetResFadeEnd() );

		synthState.SetStatResFadeInFrom( synthState.GetResFadeStart() );
		synthState.SetStatResFadeInTo( synthState.GetResFadeEnd() );

		synthState.SetAttackResVolume( 0 ); // there is no need for another attack residual after transition

	}


	void TransitionHandler::DoTransitionSynthesis( CSaltoSynthFrame &pSynthFrame,
												   SynthesisState   &synthState,
												   Frame			*pTransitionFrameTarget)
	{
		pSynthFrame.ClearSpectrum();
		
		double iPFactor = (double) synthState.GetFrameCounterTransition()/mNumTransitionFrames;
    
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
			- ( 20 + mTransitionInterval + synthState.GetAttackTimbre().GetPitch());
		
		gainReductionFactor=CLIP(gainReductionFactor,-80,-20);
		
		mOutTransitionFrequency.SendControl(gainReductionFactor);
		mOutDisplayedValuesChanged.SendControlAsBoolean(true);
		
		if ( mTransitionUpwards )
			{ 
				int freqIPUpEnd = mNumTransitionFrames/2+1;
				int freqIPUpStart = mNumTransitionFrames/2-1;
				freqIPUpStart = CLIP(freqIPUpStart,0,mNumTransitionFrames);
				freqIPUpEnd = CLIP(freqIPUpEnd,0,mNumTransitionFrames);
				
				/* frequency transition function */
				if ( synthState.GetFrameCounterTransition()<freqIPUpStart)
					{
						freqIPFactor = 0;
					}
				else if ( synthState.GetFrameCounterTransition()>=freqIPUpStart&&synthState.GetFrameCounterTransition()<freqIPUpEnd)  
					{
						baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
						targetFreq = pTransitionFrameTarget->GetFundamental().GetFreq();
						b = (freqIPUpEnd - freqIPUpStart) / (log10 (targetFreq/baseFreq)); // optimize and calc this factors in trans init !
						a = baseFreq/pow(10.0,freqIPUpStart/b);
						resultFreq = a * pow (10,synthState.GetFrameCounterTransition()/b);
						freqIPFactor = resultFreq/targetFreq;
					}
				else if (synthState.GetFrameCounterTransition()>freqIPUpEnd)
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
				
				if (synthState.GetFrameCounterTransition()<freqIPDownStart)
					{
						freqIPFactor = 0;
					}
				else if (synthState.GetFrameCounterTransition()==freqIPDownStart)   //we are in the fourth quarter of the transition...
					{
						baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
						targetFreq = pTransitionFrameTarget->GetFundamental().GetFreq();
						// optimize and calc this factors in trans init !
						b = (freqIPDownEnd - freqIPDownStart) / (log10 (targetFreq/baseFreq));
						a = baseFreq/pow(10.0,freqIPDownStart/b);
						resultFreq = a * pow (10,synthState.GetFrameCounterTransition()/b);
						freqIPFactor = resultFreq/targetFreq;
					}
				else if (synthState.GetFrameCounterTransition()>freqIPDownEnd) 
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
		
		mOut_InLoopSynthesis.SendControlAsBoolean( synthState.GetInLoopSynthesis() );
		mOutUseRandomDeviations.SendControlAsBoolean( mUseRandomDeviations );
		mOutUseRandomLoop.SendControlAsBoolean( mUseRandomLoop );
		mOutRandomRange.SendControl( mRandomRange );
		mOutTargetFreq.SendControl( pTransitionFrameTarget->GetFundamental().GetFreq() );
		mOutMagInterpolFactor.SendControl( iPFactor );
		mOutMagGain.SendControl( gain * (mLastIndividualGain+iPFactor*( synthState.GetIndividualGain()-mLastIndividualGain)) );
		mOutFreqInterpolFactor.SendControl( freqIPFactor );
		mOutMagInterpolFactor2.SendControl( synthState.GetIPFactor() );

/*		mpInterpolPO.DoInterpolation(* mpTransitionFrameBase,
									 mpTransitionFrameTarget->GetSpectralPeakArray(),
	//								 mpTransitionFrameTarget->GetFundamental().GetFreq(),
	//								 iPFactor,
	//								 freqIPFactor,
	//								 gain * (mLastIndividualGain+iPFactor*(mIndividualGain-mLastIndividualGain)), // interpolate individual gain factors	
									 *pSynthFrame,
									 *mpCurrPeakArrayTarget);
	//								 mIPFactor);
*/
		// use last pith correction factor here too
		mOutPitchFactor.SendControl( synthState.GetPitchModFactor()*synthState.GetLastPitchCorrectionFactor() );
//		mpInterpolPO.DoPitchMod(*pSynthFrame);	

	}


}
