#include "StationaryHandler.hxx"

namespace SALTO
{
	void StationaryHandlerConfig::DefaultInit( void )
	{
		AddAll();
		UpdateData();
	}

	StationaryHandler::StationaryHandler():
		mUseRandomLoop( false ), mInterpolFactor( 0.0 ),mUseRandomDeviations( false ),
		mRandomRange( 8.0 ), mUsePitchCorrection( true ), mUseSines( true ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ), mPlayFrameOnly( false ),
		mUseAttackResidual( true ), mUseStatResidual( false ), mStatResGain( 1.0 ),
		mStatResFreq( 200.0 ),
		mInUseRandomLoop( "Random Loop In Control", this, &StationaryHandler::UseRandomLoopCB ),
		mInInterpolFactor( "Interpolation Factor In Control", this, &StationaryHandler::InterpolFactorCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &StationaryHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this, &StationaryHandler::RandomRangeCB ),
		mInUsePitchCorrection( "Use Pitch Correction In Control", this, &StationaryHandler::UsePitchCorrectionCB ),
		mInUseSines( "Use Sines In Control", this , &StationaryHandler::UseSinesCB ),
		mInUsePhaseAlignment( "UsePhaseAlignment In Control", this, &StationaryHandler::UsePhaseAlignmentCB ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &StationaryHandler::BreathOnlySoundCB ),
		mInPlayFrameOnly( "Play Frame Only In Control", this, &StationaryHandler::PlayFrameOnlyCB ),
		mInUseAttackResidual( "Use Attack Residual In Control", this ,&StationaryHandler::UseAttackResidualCB ),
		mInUseStatResidual( "Use Stationary Residual In Control", this, &StationaryHandler::UseStatResidualCB  ),
		mInStatResGain( "Stationary Residual Gain In Control", this, &StationaryHandler::StatResGainCB ),
		mInStatResFreq( "Stationary Residual Frequency In Control", this, &StationaryHandler::StatResFreqCB ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
		mOutResGain( "Residual Gain Out Control", this ),
		mOutResFreq( "Residual Frequency Out Control", this ),
		mOut_InLoopSynthesis( "In Loop Synthesis Out Control", this ),
		mOutUseRandomLoop( "Use Random Loop Out Control", this ),
		mOutUseRandomDeviations( "Use Random Deviations Out Control", this ),
		mOutPitchFactor( "Pitch Factor Out Control", this ),
		mOutRandomRange( "Random Range Out Control", this ),
		mOutTargetFreq( "Target Frequency Out Control", this ),
		mOutMagInterpolFactor( "Magnitude Interpolation Factor Out Control", this ),
		mOutMagGain( "Magintude Gain Out Control", this ),
		mOutFreqInterpolFactor( "Frequency Interpolation Factor Out Control", this ),
		mOutMagInterpolFactor2( "Magnitude  Interpolation Factor 2 Out Control", this ),
		mOutAttackResGain( "Attack Residual Gain Out Control", this )
	{
		StationaryHandlerConfig cfg;

		Configure( cfg );
	}

	StationaryHandler::StationaryHandler( StationaryHandlerConfig& cfg ):
		mUseRandomLoop( false ), mInterpolFactor( 0.0 ),mUseRandomDeviations( false ),
		mRandomRange( 8.0 ), mUsePitchCorrection( true ), mUseSines( true ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ), mPlayFrameOnly( false ),
		mUseAttackResidual( true ), mUseStatResidual( false ), mStatResGain( 1.0 ),
		mStatResFreq( 200.0 ),
		mInUseRandomLoop( "Random Loop In Control", this, &StationaryHandler::UseRandomLoopCB ),
		mInInterpolFactor( "Interpolation Factor In Control", this, &StationaryHandler::InterpolFactorCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &StationaryHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this, &StationaryHandler::RandomRangeCB ),
		mInUsePitchCorrection( "Use Pitch Correction In Control", this, &StationaryHandler::UsePitchCorrectionCB ),
		mInUseSines( "Use Sines In Control", this , &StationaryHandler::UseSinesCB ),
		mInUsePhaseAlignment( "UsePhaseAlignment In Control", this, &StationaryHandler::UsePhaseAlignmentCB ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &StationaryHandler::BreathOnlySoundCB ),
		mInPlayFrameOnly( "Play Frame Only In Control", this, &StationaryHandler::PlayFrameOnlyCB ),
		mInUseAttackResidual( "Use Attack Residual In Control", this ,&StationaryHandler::UseAttackResidualCB ),
		mInUseStatResidual( "Use Stationary Residual In Control", this, &StationaryHandler::UseStatResidualCB  ),
		mInStatResGain( "Stationary Residual Gain In Control", this, &StationaryHandler::StatResGainCB ),
		mInStatResFreq( "Stationary Residual Frequency In Control", this, &StationaryHandler::StatResFreqCB ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
		mOutResGain( "Residual Gain Out Control", this ),
		mOutResFreq( "Residual Frequency Out Control", this ),
		mOut_InLoopSynthesis( "In Loop Synthesis Out Control", this ),
		mOutUseRandomLoop( "Use Random Loop Out Control", this ),
		mOutUseRandomDeviations( "Use Random Deviations Out Control", this ),
		mOutPitchFactor( "Pitch Factor Out Control", this ),
		mOutRandomRange( "Random Range Out Control", this ),
		mOutTargetFreq( "Target Frequency Out Control", this ),
		mOutMagInterpolFactor( "Magnitude Interpolation Factor Out Control", this ),
		mOutMagGain( "Magintude Gain Out Control", this ),
		mOutFreqInterpolFactor( "Frequency Interpolation Factor Out Control", this ),
		mOutMagInterpolFactor2( "Magnitude  Interpolation Factor 2 Out Control", this ),
		mOutAttackResGain( "Attack Residual Gain Out Control", this )
	{
		Configure( cfg );
	}

	StationaryHandler::~StationaryHandler()
	{
	}

	bool StationaryHandler::ConcretStart()
	{
		return true;
	}

	bool StationaryHandler::ConcretStop()
	{
		return true;
	}

	bool StationaryHandler::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	int StationaryHandler::UseRandomLoopCB( TControlData value )
	{
		if( value > 0 )		mUseRandomLoop = true;
		else				mUseRandomLoop = false;

		return 0;
	}

	int StationaryHandler::UseRandomDeviationsCB( TControlData value )
	{
		if( value > 0 )		mUseRandomDeviations = true;
		else				mUseRandomDeviations = false;

		return 0;
	}

	int StationaryHandler::UsePitchCorrectionCB( TControlData value )
	{
		if( value > 0 )		mUsePitchCorrection = true;
		else				mUsePitchCorrection = false;

		return 0;
	}

	int StationaryHandler::UseSinesCB( TControlData value )
	{
		if( value > 0 )		mUseSines = true;
		else				mUseSines = false;

		return 0;
	}

	int StationaryHandler::UsePhaseAlignmentCB( TControlData value )
	{
		if( value > 0 )		mUsePhaseAlignment = true;
		else				mUsePhaseAlignment = false;

		return 0;
	}

	int StationaryHandler::BreathOnlySoundCB( TControlData value )
	{
		if( value > 0 )		mBreathOnlySound = true;
		else				mBreathOnlySound = false;

		return 0;
	}

	int StationaryHandler::PlayFrameOnlyCB( TControlData value )
	{
		if( value > 0 )		mPlayFrameOnly = true;
		else				mPlayFrameOnly = false;

		return 0;
	}

	int StationaryHandler::UseAttackResidualCB( TControlData value )
	{
		if( value > 0 )		mUseAttackResidual = true;
		else				mUseAttackResidual = false;

		return 0;
	}

	int StationaryHandler::UseStatResidualCB( TControlData value )
	{
		if( value > 0 )		mUseStatResidual = true;
		else				mUseStatResidual = false;

		return 0;
	}

	int StationaryHandler::InterpolFactorCB( TControlData value )
	{
		mInterpolFactor = value;

		return 0;
	}

	int StationaryHandler::RandomRangeCB( TControlData value )
	{
		mRandomRange = value;

		return 0;
	}

	int StationaryHandler::StatResGainCB( TControlData value )
	{
		mStatResGain = value;

		return 0;
	}

	int StationaryHandler::StatResFreqCB( TControlData value )
	{
		mStatResFreq = value;

		return 0;
	}

	bool StationaryHandler::Do()
	{
		return true;
	}

	bool StationaryHandler::Do( CSaltoSynthFrame  &pSynthFrame,
						   Frame			 *pCurrSpectralFrameBase,										
						   SynthesisState    &synthState,
						   Frame			 *pCurrSpectralFrameResidual,
						   SpectralPeakArray *pCurrPeakArrayTarget)
	{

		DoInterpolatingSynthesis( pSynthFrame, pCurrSpectralFrameBase, 
								  synthState, pCurrSpectralFrameResidual, pCurrPeakArrayTarget );

//		DoStationarySynthesisProcess( synthState );

		return true;
	}

	void StationaryHandler::DoInterpolatingSynthesis( CSaltoSynthFrame  &pSynthFrame,
													  Frame			    *pCurrSpectralFrameBase,										
													  SynthesisState    &synthState,
													  Frame			    *pCurrSpectralFrameResidual,
													  SpectralPeakArray *pCurrPeakArrayTarget)
	{
		CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
		
		pSynthFrame.ClearSpectrum();  // reset synth frame OPTIMIZE
		
		// -- data managment BaseFrame --
		if ( mUseRandomLoop )
			{
				if ( synthState.GetFrameCounterBase() == synthState.GetLoopStart() )
					{
						synthState.SetLoopDirectionFW(true);
						synthState.SetInLoopSynthesis(true);       // indicates that we are in loop mode
						int randRange = synthState.GetLimitLoopEnd() - synthState.GetLoopStart() ;
						//mLoopEnd = (Random()+32767)/65535.0*randRange+mLoopStart;	
						synthState.SetLoopEnd( ((TFloat32)rand()/RAND_MAX)*
												randRange+synthState.GetLoopStart() );	
					}
				else if (synthState.GetFrameCounterBase() > synthState.GetLoopEnd() )
					{
						synthState.SetLoopDirectionFW(false);
						synthState.SetInLoopSynthesis(true);       // indicates that we are in loop mode
						int randRange = synthState.GetLoopEnd() - synthState.GetLimitLoopStart();
						synthState.SetLoopStart( ((TFloat32)rand()/RAND_MAX)*
												randRange+synthState.GetLimitLoopStart() );
					}
			}
		else  // repeat always one frame...
			{
				if (synthState.GetFrameCounterBase() >= synthState.GetLoopStart() )    
					{
						synthState.SetFrameCounterBase( synthState.GetLoopStart() );
						synthState.SetInLoopSynthesis(true);
					}
			}	
		
		pCurrSpectralFrameBase = handleDB->GetSpectralFrame( synthState.GetSegPositionBase() ,
															 synthState.GetFrameCounterBase() );
		
		// increase or decrease position of readout frame depending on loop direction
		if ( synthState.GetLoopDirectionFW() )
			synthState.SetFrameCounterBase( synthState.GetFrameCounterBase() + 1 );
		else
			synthState.SetFrameCounterBase( synthState.GetFrameCounterBase() - 1 );
		
		if(synthState.GetFrameCounterBase() >= synthState.GetNumFramesBase() )// security-stop at end of the segment
			{
				synthState.SetFrameCounterBase( 0 );
				//mpParams->SetPlay(false);
				return;
			}
		
		// -- data managment TargetFrame --
		pCurrPeakArrayTarget = synthState.GetCurrentStatTemplatePtr();
		
		// -- data managment attack residual --
		TIndex nResFrames= handleDB->GetFramesInSeg(synthState.GetAttackTimbre().GetPosition());
		
		if ( synthState.GetFrameCounterAttackResidual() >= nResFrames)
			synthState.SetFrameCounterAttackResidual( nResFrames-1 );
		
		pCurrSpectralFrameResidual = handleDB->GetSpectralFrame(synthState.GetAttackTimbre().GetPosition(),
																synthState.GetFrameCounterAttackResidual() );
		
		pSynthFrame.SetResidualPtr(&pCurrSpectralFrameResidual->GetResidualSpec()); //BUGFRAME? //SEGMENTBUG
		
		
		synthState.SetFrameCounterAttackResidual( synthState.GetFrameCounterAttackResidual() + 1 );
		
		// spectral interpolation process starts after 'mInterpolationstart' Frames
		if( synthState.GetFrameCounterBase() >= synthState.GetInterpolStart() )
			{
				synthState.SetIPFactor( mInterpolFactor * 
										( synthState.GetFrameCounterBase() - synthState.GetInterpolStart() )
										/ synthState.GetInterpolDuration() );
				if ( synthState.GetFrameCounterBase() >= synthState.GetInterpolEnd() )
					synthState.SetIPFactor( mInterpolFactor );
			}
		else
			{
				synthState.SetIPFactor( 0 );
			}

		mOut_InLoopSynthesis.SendControlAsBoolean( synthState.GetInLoopSynthesis() );
		mOutUseRandomDeviations.SendControlAsBoolean( mUseRandomDeviations );
		mOutUseRandomLoop.SendControlAsBoolean( mUseRandomLoop );
		mOutRandomRange.SendControl( mRandomRange );
		mOutTargetFreq.SendControl( synthState.GetCurrentStatTemplateFundFreq() );
		mOutMagInterpolFactor.SendControl( synthState.GetIPFactor() );
		mOutMagGain.SendControl( synthState.GetIndividualGain() );

	/*	mpInterpolPO.DoMagInterpolation( *mpCurrSpectralFrameBase,
										  *mpCurrPeakArrayTarget,
	//									  mpParams->GetCurrentStatTemplateFundFreq(),
	//									  mIPFactor,
	//									  mIndividualGain,
										  *pSynthFrame );
	*/
		double pitchCorrectionFactor=1.0;
		if  ( mUsePitchCorrection && pSynthFrame.GetFundFreq()!=0)
			pitchCorrectionFactor= synthState.GetPitchReference() /pSynthFrame.GetFundFreq();
		synthState.SetLastPitchCorrectionFactor( pitchCorrectionFactor ); // used in transition

		mOutPitchFactor.SendControl( synthState.GetPitchModFactor()*pitchCorrectionFactor );
	//	mpInterpolPO.DoPitchMod(*pSynthFrame);
	}

	void StationaryHandler::DoStationarySynthesisProcess( SynthesisState &synthState)
	{
		if ( mUseSines ) 
			{
	//			mpSineSynthPO->DoSineSynthesis( *pSynthFrame );
		
				mOutAttackTimbreLevel.SendControl( synthState.GetAttackTimbre().GetLevel() );
				mOutUsePhaseAlignment.SendControlAsBoolean( mUsePhaseAlignment );
				mOutLastAlignedFrame.SendControlAsBoolean( synthState.GetLastAlignedFrame() );
				mOutBreathOnlySound.SendControlAsBoolean( mBreathOnlySound );

//				mpSineSynthPO.Do( *pSynthFrame );
			}

		// Attack Residual Synthesis
		if	(mUseAttackResidual &!mPlayFrameOnly )
			{
				AttackResidualSynthesis( synthState );
			}

		// Stationary Residual Synthesis
		if ( mUseStatResidual )
			{
				if ( synthState.GetFrameCounterBase() >= synthState.GetStatResFadeInFrom() )
					StationaryResidualSynthesis( synthState );
			}
		// -- Spectral Synthesis
//		mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
		
	}

	void StationaryHandler::AttackResidualSynthesis( SynthesisState   &synthState )
	{
		double ipFactor = 1;
		
		if ( synthState.GetFrameCounterBase() >= synthState.GetResFadeStart() )
			{
				// precalculate the reciprocal to avoid the floating point division
				// ipfactor = 1 - ( (mFrameCounterBase - mResFadeStart) / ( mResFadeEnd - mResFadeStart ) )
				// DEBUG_ASSERT added to check against divisions by zero
				CLAM_DEBUG_ASSERT( (synthState.GetResFadeEnd()-synthState.GetResFadeStart()), "Division by zero! check mResFadeEnd and mResFadeStart");


				// calc fade-out factor for decaying attack residual
				//ipFactor = 1 - ( (mFrameCounterBase-mResFadeStart) * difference_reciprocal );
				ipFactor = 1-( synthState.GetFrameCounterBase() - synthState.GetResFadeStart() )
					       /( synthState.GetResFadeEnd() - synthState.GetResFadeStart() );

				// if the previous difference results in a negative value we clamp it to
				// [ 0, +inf ]
				if ( ipFactor < 0 )
					ipFactor = 0;
			}

	//	mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()/127.0);

		// The conditional below exists due to an efficiency issue: since the ipFactor might be zero or one it pays
		// to check wether we have to do complicated multiplications or not

		if ( !ipFactor )
		{
		//	mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,0);
			mOutAttackResGain.SendControl( 0.0 );
//			mpAttackResSynthPO.Do( *pSynthFrame );
		}
		else
			{
				// precalculate the reciprocal of 127.0
				static const double inv_127 = 1.0/127.0;

				// ipFactor has not been changed
				if ( ipFactor == 1 ) 
				{
				//	mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
				//									mAttackResVolume*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
					mOutAttackResGain.SendControl( synthState.GetAttackResVolume() * 
						                     synthState.GetAttackTimbre().GetLevel() * inv_127 );
//					mpAttackResSynthPO.Do( *pSynthFrame );
				}
				// otherwise we are in the worst case
				else
				{
				//	mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
				//									mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
					mOutAttackResGain.SendControl( synthState.GetAttackResVolume()* ipFactor * 
						                     synthState.GetAttackTimbre().GetLevel() * inv_127 );
//					mpAttackResSynthPO.Do( *pSynthFrame );
				}

			}
	}

	void StationaryHandler::StationaryResidualSynthesis( SynthesisState &synthState )
	{	
		static const double inv_127 = 1.0/127.0;
		static const double inv_150 = 1.0/150.0;
		Frame* pStatResidual = NULL;

		CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
		pStatResidual = handleDB->GetStatResidualSpectralFrame( synthState.GetFrameCounterStatRes() );
		
		if ( synthState.GetStatResLoopDirectionFW() )
			{
				synthState.SetFrameCounterStatRes( synthState.GetFrameCounterStatRes() + 1 ) ;
				if ( synthState.GetFrameCounterStatRes() >= synthState.GetNumFramesStatRes() - 100 )
					synthState.SetStatResLoopDirectionFW( !synthState.GetStatResLoopDirectionFW() );
			}
		else
			{
				synthState.SetFrameCounterStatRes( synthState.GetFrameCounterStatRes() - 1 ) ;
				if ( synthState.GetFrameCounterStatRes()  <= 100)
					synthState.SetStatResLoopDirectionFW( ! synthState.GetStatResLoopDirectionFW() ) ;
			}
		
		double ipFactor = ( synthState.GetFrameCounterBase() - synthState.GetResFadeStart() ) 
			              / ( synthState.GetResFadeEnd() - synthState.GetResFadeStart() );
		double level = (double) synthState.GetAttackTimbre().GetLevel() * inv_127;
		double correctionFactor = log(8.5*level+1.5)*inv_150 * mStatResGain ;
		
		// -- TEST increase level of stationary residual for smaller volumes...
		// We clamp down the ipFactor value, and since ipFactor is the identiy it pays not
		// to multiply it

		if ( ipFactor > 1 )
			{
			//	mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
			//										   mStatResVolume*correctionFactor,mpParams->GetStatResFreq());
				mOutResGain.SendControl( synthState.GetStatResVolume() * correctionFactor );
				mOutResFreq.SendControl( mStatResFreq );
//				mpStatResSyhthesisPO.Do( *pSynthFrame, *pStatResidual );
			}
		else
			{
			//	mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
			//										   mStatResVolume*ipFactor*correctionFactor,mpParams->GetStatResFreq());				
				mOutResGain.SendControl( synthState.GetStatResVolume() * ipFactor * correctionFactor );
				mOutResFreq.SendControl( mStatResFreq );
//				mpStatResSyhthesisPO.Do( *pSynthFrame, *pStatResidual );
			}
	}


}
