#include "ReleaseHandler.hxx"

namespace SALTO
{
	void ReleaseHandlerConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	ReleaseHandler::ReleaseHandler():
		mUseRandomLoop( false ), mUseRandomDeviations( false ),
		mUsePitchCorrection( true ), mRandomRange( 8 ), mUseSines( true ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ),
		mFrameCounterRelease( 1 ), mNumFramesRelease( 6 ),
		mInUseRandomLoop( "Use Random Loop In Control", this, &ReleaseHandler::UseRandomLoopCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &ReleaseHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this ),
		mInUsePicthCorrection( "Use Pitch Correction In Control", this ),
		mInUseSines( "Use Sines In Control", this, &ReleaseHandler::UseSinesCB ),
		mInUsePhaseAlignment( "Use Phase Alignment In Control", this, &ReleaseHandler::UsePhaseAlignmentCB ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &ReleaseHandler::BreathOnlySoundCB ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Use Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
		mOut_InLoopSynthesis( "In Loop Synthesis Out Control", this ),
		mOutUseRandomLoop( "Use Random Loop Out Control", this ),
		mOutUseRandomDeviations( "Use Random Deviations Out Control", this ),
		mOutPitchFactor( "Pitch Factor Out Control", this ),
		mOutRandomRange( "Random Range Out Control", this ),
		mOutTargetFreq( "Target Frequency Out Control", this ),
		mOutMagInterpolFactor( "Magnitude Interpolation Factor Out Control", this ),
		mOutMagGain( "Magnitude Gain Out Control", this ),
		mOutState( "State Out Control", this )
	{
		ReleaseHandlerConfig cfg;

		Configure( cfg );
	}

	ReleaseHandler::ReleaseHandler( ReleaseHandlerConfig &cfg ):
		mUseRandomLoop( false ), mUseRandomDeviations( false ),
		mUsePitchCorrection( true ), mRandomRange( 8 ), mUseSines( true ),
		mUsePhaseAlignment( true ), mBreathOnlySound( false ),
		mFrameCounterRelease( 1 ), mNumFramesRelease( 6 ),
		mInUseRandomLoop( "Use Random Loop In Control", this, &ReleaseHandler::UseRandomLoopCB ),
		mInUseRandomDeviations( "Use Random Deviations In Control", this, &ReleaseHandler::UseRandomDeviationsCB ),
		mInRandomRange( "Random Range In Control", this ),
		mInUsePicthCorrection( "Use Pitch Correction In Control", this ),
		mInUseSines( "Use Sines In Control", this, &ReleaseHandler::UseSinesCB ),
		mInUsePhaseAlignment( "Use Phase Alignment In Control", this, &ReleaseHandler::UsePhaseAlignmentCB ),
		mInBreathOnlySound( "Breath Only Sound In Control", this, &ReleaseHandler::BreathOnlySoundCB ),
		mOutBreathOnlySound( "Breath Only Sound Out Control", this ),
		mOutAttackTimbreLevel( "Attack Timbre Level Out Control", this ),
		mOutUsePhaseAlignment( "Use Phase Alignment Out Control", this ),
		mOutLastAlignedFrame( "Last Aligned Frame Out Control", this ),
		mOut_InLoopSynthesis( "In Loop Synthesis Out Control", this ),
		mOutUseRandomLoop( "Use Random Loop Out Control", this ),
		mOutUseRandomDeviations( "Use Random Deviations Out Control", this ),
		mOutPitchFactor( "Pitch Factor Out Control", this ),
		mOutRandomRange( "Random Range Out Control", this ),
		mOutTargetFreq( "Target Frequency Out Control", this ),
		mOutMagInterpolFactor( "Magnitude Interpolation Factor Out Control", this ),
		mOutMagGain( "Magnitude Gain Out Control", this ),
		mOutState( "State Out Control", this )
	{
		Configure( cfg );
	}

	ReleaseHandler::~ReleaseHandler()
	{}

	bool ReleaseHandler::ConcreteConfigure( const ProcessingConfig &c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	bool ReleaseHandler::ConcreteStart()
	{
		return true;
	}

	bool ReleaseHandler::ConcreteStop()
	{
		return true;
	}

	int ReleaseHandler::UseRandomLoopCB( TControlData value )
	{
		if( value > 0 )		mUseRandomLoop = true;
		else				mUseRandomLoop = false;

		return 0;
	}

	int ReleaseHandler::UseRandomDeviationsCB( TControlData value )
	{
		if( value > 0 )		mUseRandomDeviations = true;
		else				mUseRandomDeviations = false;

		return 0;
	}

	int ReleaseHandler::UsePitchCorrectionCB( TControlData value )
	{
		if( value > 0 )		mUsePitchCorrection = true;
		else				mUsePitchCorrection = false;

		return 0;
	}

	int ReleaseHandler::UseSinesCB( TControlData value )
	{
		if( value > 0 )		mUseSines = true;
		else				mUseSines = false;

		return 0;
	}

	int ReleaseHandler::BreathOnlySoundCB( TControlData value )
	{
		if( value > 0 )		mBreathOnlySound = true;
		else				mBreathOnlySound = false;

		return 0;
	}

	int ReleaseHandler::UsePhaseAlignmentCB( TControlData value )
	{
		if( value > 0 )		mUsePhaseAlignment = true;
		else				mUsePhaseAlignment = false;

		return 0;
	}

	int ReleaseHandler::RandomRangeCB( TControlData value )
	{
		mRandomRange = value;

		return 0;
	}

	bool ReleaseHandler::Do()
	{
		return true;
	}

	void ReleaseHandler::DoInterpolatingReleaseSynthesis( CSaltoSynthFrame &pSynthFrame,
														  SynthesisState   &synthState,
														  double gain )
	{
		pSynthFrame.ClearSpectrum();

		mOut_InLoopSynthesis.SendControlAsBoolean( synthState.GetInLoopSynthesis() );
		mOutUseRandomDeviations.SendControlAsBoolean( mUseRandomDeviations );
		mOutUseRandomLoop.SendControlAsBoolean( mUseRandomLoop );
		mOutRandomRange.SendControl( mRandomRange );
		mOutTargetFreq.SendControl( synthState.GetCurrentStatTemplateFundFreq() );
		mOutMagInterpolFactor.SendControl( synthState.GetIPFactor() );
		mOutMagGain.SendControl( synthState.GetIndividualGain() / ( mFrameCounterRelease * mFrameCounterRelease ) );

	/*	mpInterpolPO.DoMagInterpolation( *mpCurrSpectralFrameBase,
										  *mpCurrPeakArrayTarget,
	//									  mpParams->GetCurrentStatTemplateFundFreq(),
	//									  mIPFactor,
	//									  mIndividualGain / ( currRelFrame * currRelFrame ),
										  *mpSynthFrame );
*/
		double pitchCorrectionFactor=1;
		
		if ( mUsePitchCorrection && pSynthFrame.GetFundFreq()!=0)
			pitchCorrectionFactor= synthState.GetPitchReference() / pSynthFrame.GetFundFreq();

		mOutPitchFactor.SendControl( synthState.GetPitchModFactor() * pitchCorrectionFactor );
//		mpInterpolPO.DoPitchMod( *pSynthFrame);
	}

	void ReleaseHandler::DoReleaseSynthesisProcess( SynthesisState &synthState )
	{

		 // from here on, the content of the synthframe shouldn't be changed anymore
		
		// -- Sinusoidal Synthesis --
		if	( mUseSines )
		{
	//		mpSineSynthPO->DoSineSynthesis( *pSynthFrame );
			mOutAttackTimbreLevel.SendControl( synthState.GetAttackTimbre().GetLevel() );
			mOutUsePhaseAlignment.SendControlAsBoolean( mUsePhaseAlignment );
			mOutLastAlignedFrame.SendControlAsBoolean( synthState.GetLastAlignedFrame() );
			mOutBreathOnlySound.SendControlAsBoolean( mBreathOnlySound );

//			mpSineSynthPO.Do( *pSynthFrame );
		}

		// -- Spectral Synthesis
//		mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
		
	}

	void ReleaseHandler::InitReleaseSynthesis( SynthesisState &synthState )
	{
		mNumFramesRelease = 6;
		mFrameCounterRelease = 1;
		synthState.SetTransitionMode( false );
	}

	bool ReleaseHandler::Do( CSaltoSynthFrame &pSynthFrame,
							 SynthesisState   &synthState,
							 double gain )
	{
		DoInterpolatingReleaseSynthesis( pSynthFrame, synthState, gain );

		DoReleaseSynthesisProcess( synthState );
	
		mFrameCounterRelease++;
	
		if ( mFrameCounterRelease >= mNumFramesRelease )
			mOutState.SendControl( 0 );
		//	mState = Idle; 

		return true;
	}

}
