#include "SynthesisProcessor.hxx"

namespace SALTO
{
	void SynthesisProcessorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	SynthesisProcessor::SynthesisProcessor():
		mStatResFreq( 200.0 ), mStatResGain( 1.0 ), mUseSines( true ),
		mUseAttackResidual( true ), mUseStatResidual( false ), mPlayFrameOnly( false ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &SynthesisProcessor::BreathOnlySoundCB ),
		mInUsePhaseAlignment( "UsePhaseAlignment In Control", this, &SynthesisProcessor::UsePhaseAlignmentCB ),
		mInStatResGain( "Stationary Residual Gain In Control", this, &SynthesisProcessor::StatResGainCB ),
		mInStatResFreq( "Stationary Residual Frequency In Control", this, &SynthesisProcessor::StatResFreqCB ),
		mInUseAttackResidual( "Use Attack Residual In Control", this ,&SynthesisProcessor::UseAttackResidualCB ),
		mInUseStatResidual( "Use Stationary Residual In Control", this, &SynthesisProcessor::UseStatResidualCB  ),
		mInUseSines( "Use Sines In Control", this , &SynthesisProcessor::UseSinesCB ),
		mInPlayFrameOnly( "Play Frame Only In Control", this, &SynthesisProcessor::PlayFrameOnlyCB ),
		mOutResGain( "Residual Gain Out Control", this ),
		mOutResFreq( "Residual Frequency Out Control", this ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this )
	{
		SynthesisProcessorConfig cfg;

		Configure( cfg );
	}

	SynthesisProcessor::SynthesisProcessor( SynthesisProcessorConfig& cfg ):
		mStatResFreq( 200.0 ), mStatResGain( 1.0 ), mUseSines( true ),
		mUseAttackResidual( true ), mUseStatResidual( false ), mPlayFrameOnly( false ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &SynthesisProcessor::BreathOnlySoundCB ),
		mInUsePhaseAlignment( "UsePhaseAlignment In Control", this, &SynthesisProcessor::UsePhaseAlignmentCB ),
		mInStatResGain( "Stationary Residual Gain In Control", this, &SynthesisProcessor::StatResGainCB ),
		mInStatResFreq( "Stationary Residual Frequency In Control", this, &SynthesisProcessor::StatResFreqCB ),
		mInUseAttackResidual( "Use Attack Residual In Control", this ,&SynthesisProcessor::UseAttackResidualCB ),
		mInUseStatResidual( "Use Stationary Residual In Control", this, &SynthesisProcessor::UseStatResidualCB  ),
		mInUseSines( "Use Sines In Control", this , &SynthesisProcessor::UseSinesCB ),
		mInPlayFrameOnly( "Play Frame Only In Control", this, &SynthesisProcessor::PlayFrameOnlyCB ),
		mOutResGain( "Residual Gain Out Control", this ),
		mOutResFreq( "Residual Frequency Out Control", this ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this )
	{
		Configure( cfg );
	}

	SynthesisProcessor::~SynthesisProcessor()
	{}

	bool SynthesisProcessor::ConcreteStart()
	{
		return true;
	}

	bool SynthesisProcessor::ConcreteStop()
	{
		return true;
	}

	bool SynthesisProcessor::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	int SynthesisProcessor::StatResFreqCB( TControlData value )
	{
		mStatResFreq = value;

		return 0;
	}

	int SynthesisProcessor::StatResGainCB( TControlData value )
	{
		mStatResGain = value;

		return 0;
	}

	int SynthesisProcessor::UseSinesCB( TControlData value )
	{
		if( value > 0 )		mUseSines = true;
		else				mUseSines = false;

		return 0;
	}

	int SynthesisProcessor::UseAttackResidualCB( TControlData value )
	{
		if( value > 0 )		mUseAttackResidual = true;
		else				mUseAttackResidual = false;

		return 0;
	}

	int SynthesisProcessor::UseStatResidualCB( TControlData value )
	{
		if( value > 0 )		mUseStatResidual = true;
		else				mUseStatResidual = false;

		return 0;
	}

	int SynthesisProcessor::PlayFrameOnlyCB( TControlData value )
	{
		if( value > 0 )		mPlayFrameOnly = true;
		else				mPlayFrameOnly = false;

		return 0;
	}

	int SynthesisProcessor::UsePhaseAlignmentCB( TControlData value )
	{
		if( value > 0 )		mUsePhaseAlignment = true;
		else				mUsePhaseAlignment = false;

		return 0;
	}

	int SynthesisProcessor::BreathOnlySoundCB( TControlData value )
	{
		if( value > 0 )		mBreathOnlySound = true;
		else				mBreathOnlySound = false;

		return 0;
	}

	bool SynthesisProcessor::Do()
	{
		return true;
	}

	bool SynthesisProcessor::Do( SynthesisState &synthState)
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

		return true;
		// -- Spectral Synthesis
//		mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
		
	}

	void SynthesisProcessor::AttackResidualSynthesis( SynthesisState   &synthState )
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
			mOutResGain.SendControl( 0.0 );
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
					mOutResGain.SendControl( synthState.GetAttackResVolume() * 
						                     synthState.GetAttackTimbre().GetLevel() * inv_127 );
//					mpAttackResSynthPO.Do( *pSynthFrame );
				}
				// otherwise we are in the worst case
				else
				{
				//	mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
				//									mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()*inv_127 );
					mOutResGain.SendControl( synthState.GetAttackResVolume()* ipFactor * 
						                     synthState.GetAttackTimbre().GetLevel() * inv_127 );
//					mpAttackResSynthPO.Do( *pSynthFrame );
				}

			}
	}

	void SynthesisProcessor::StationaryResidualSynthesis( SynthesisState &synthState )
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
