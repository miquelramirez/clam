#include "AttackHandler.hxx"

namespace SALTO
{

	AttackHandler::AttackHandler():
		mOutResetSineSynth( "Reset Sine Synthesis Out Control", this )
	{
		StationaryHandlerConfig cfg;

		Configure( cfg );
	}

	AttackHandler::AttackHandler( StationaryHandlerConfig &cfg ):
		mOutResetSineSynth( "Reset Sine Synthesis Out Control", this )
	{
		Configure( cfg );
	}

	AttackHandler::~AttackHandler()
	{
	}

	bool AttackHandler::ConcreteConfigure( const ProcessingConfig &c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	bool AttackHandler::ConcreteStart()
	{
		return true;
	}

	bool AttackHandler::ConcreteStop()
	{
		return true;
	}

	bool AttackHandler::Do()
	{
		return true;
	}

	bool AttackHandler::Do( CSaltoSynthFrame  &pSynthFrame,
						   Frame			 *pCurrSpectralFrameBase,										
						   SynthesisState    &synthState,
						   Frame			 *pCurrSpectralFrameResidual,
						   SpectralPeakArray *pCurrPeakArrayTarget)
	{
		pSynthFrame.SetSynthTime( 0.0 );
	
		//mpSineSynthPO.ResetSineSynthesis();

		mOutResetSineSynth.SendControlAsBoolean( true );

		InitInterpolatingSynthesis( synthState );

		DoInterpolatingSynthesis( pSynthFrame, pCurrSpectralFrameBase,
								  synthState, pCurrSpectralFrameResidual, pCurrPeakArrayTarget );

	//	DoStationarySynthesisProcess( synthState );

		return true;
	}

	void AttackHandler::InitInterpolatingSynthesis( SynthesisState &synthState )
	{
		CSaltoDataManagment* handleDB = CSaltoDataManagment::GetSaltoDBHandle();
		
		// remember : after a transition we dont step into here again
		// this means we have do the necessary inits in CSaltoDSP::EndTransitionSynthesis too !!!
		synthState.SetInLoopSynthesis(false);// we're not yet in the loop
		synthState.SetTransitionMode(false); // we're not yet in transition mode
		synthState.SetLoopDirectionFW( true );// start the loop always in forwards direction
		synthState.SetStatResLoopDirectionFW( true ); // start stat res loop in forwards direction
		synthState.SetFrameCounterTransition( 0 );
	//	mSinAttackOffsetCounter = 0; 
		
		//mTimbreVektorBase = mpParams->GetAttackTimbre();
		CSaltoTimbreVektor timbreVektorBase = synthState.GetAttackTimbre();
		
		// inital values for interpolation
		//mSegPositionBase = mTimbreVektorBase.GetPosition();
		synthState.SetSegPositionBase( timbreVektorBase.GetPosition() );
		
		// for security reasons only, can go out later
		if ( synthState.GetSegPositionBase() >= SPECTRAL_SEGMENTS_IN_USE ) 
			synthState.SetSegPositionBase( 0 );
		
		// num frames in the current sdif segment
		synthState.SetNumFramesBase( handleDB->GetFramesInSeg( synthState.GetSegPositionBase() ) );
		
		synthState.SetFrameCounterBase( ( synthState.GetSegDataByPos( 
				   synthState.GetSegPositionBase() ) ).GetPlayBackStart() );
		
		double dB = synthState.GetSegDataByPos( synthState.GetSegPositionBase() ).GetIndividualVolume(); // CHANGE THIS NAME !!!!
		
		double linear_value = pow(10.,dB/20.);


		synthState.SetIndividualGain( linear_value );
		
		synthState.SetLimitLoopStart( ( synthState.GetSegDataByPos( 
										synthState.GetSegPositionBase() ) ).GetLoopStart() );
		synthState.SetLimitLoopEnd( ( synthState.GetSegDataByPos( synthState.GetSegPositionBase() ) ).GetLoopEnd() );
		if ( synthState.GetLimitLoopEnd() < synthState.GetLimitLoopStart() )
			synthState.SetLimitLoopEnd( synthState.GetLimitLoopStart() ); // security
		
		synthState.SetLoopStart( synthState.GetLimitLoopStart() ); // variable borders for Random Mirror Looping
		synthState.SetLoopEnd( synthState.GetLimitLoopEnd() ); // dto.
		
		synthState.SetInterpolStart( ( synthState.GetSegDataByPos( 
									   synthState.GetSegPositionBase() ) ).GetInterpolStart() );
		synthState.SetInterpolEnd( ( synthState.GetSegDataByPos(
									 synthState.GetSegPositionBase() )).GetInterpolEnd() );
		synthState.SetInterpolDuration( synthState.GetInterpolEnd() - synthState.GetInterpolStart() );
		
		synthState.SetResFadeStart( ( synthState.GetSegDataByPos( 
									  synthState.GetSegPositionBase() )).GetResFadeStart() );
		synthState.SetResFadeEnd( ( synthState.GetSegDataByPos( 
									synthState.GetSegPositionBase() )).GetResFadeEnd() );
		synthState.SetAttackResVolume( (double)( synthState.GetSegDataByPos( 
										synthState.GetSegPositionBase() )).GetAttackResVolume() );
		CLAM_ASSERT ( synthState.GetAttackResVolume() >= 0, "mAttackResVolume < 0!!");
		
	//	mSinAttackOffset =0;
		
		synthState.SetFrameCounterStatRes( 100 );// manually set for now... should be adjustable
		synthState.SetNumFramesStatRes( handleDB->GetFramesInStatResidualSegment() );
		synthState.SetStatResFadeInFrom( synthState.GetResFadeStart() );
		synthState.SetStatResFadeInTo( synthState.GetResFadeEnd() );
		
		synthState.SetStatResVolume( (double)( synthState.GetSegDataByPos( 
									 synthState.GetSegPositionBase() )).GetStatResVolume() );
		CLAM_ASSERT( synthState.GetStatResVolume() >= 0, "mStatResVolume <0!");
		
		synthState.SetFrameCounterAttackResidual( 0 );//(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
		
		// pitch correction (fund.freq for pitch 0) use for tuning the instrument
		// and correcting individual deviation of samples
		
		//mPitchReference = LOWEST_PITCH *pow(2.0,mTimbreVektorBase.GetPitch()/12.0);
		synthState.SetPitchReference( LOWEST_PITCH *pow(2.0,timbreVektorBase.GetPitch()/12.0) );

	}

}
