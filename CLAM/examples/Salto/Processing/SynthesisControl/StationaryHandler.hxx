#ifndef _StationaryHandler_
#define _StationaryHandler_

#include "Processing.hxx"
#include "CSaltoSynthFrame.hxx"
#include "CSaltoDataManagment.hxx"
#include "SynthesisState.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Frame.hxx"

using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::InControlTmpl;
using CLAM::OutControl;
using CLAM::TControlData;
using CLAM::TFloat32;
using CLAM::Frame;

using CLAM::CSaltoSynthFrame;
using CLAM::CSaltoDataManagment;

namespace SALTO
{

	class StationaryHandlerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (StationaryHandlerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit(void);
	};

	class StationaryHandler: public Processing
	{
	public:
				
		InControlTmpl< StationaryHandler >		mInUseRandomLoop;

		InControlTmpl< StationaryHandler >		mInInterpolFactor;

		InControlTmpl< StationaryHandler >		mInUseRandomDeviations;

		InControlTmpl< StationaryHandler >		mInRandomRange;

		InControlTmpl< StationaryHandler >		mInUsePitchCorrection;

		InControlTmpl< StationaryHandler >		mInUseSines;

		InControlTmpl< StationaryHandler >		mInUsePhaseAlignment;

		InControlTmpl< StationaryHandler >		mInBreathOnlySound;

		InControlTmpl< StationaryHandler >		mInPlayFrameOnly;

		InControlTmpl< StationaryHandler >		mInUseAttackResidual;

		InControlTmpl< StationaryHandler >		mInUseStatResidual;

		InControlTmpl< StationaryHandler >		mInStatResGain;

		InControlTmpl< StationaryHandler >		mInStatResFreq;


		StationaryHandler();

		StationaryHandler( StationaryHandlerConfig& cfg );

		virtual ~StationaryHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::StationaryHandler";
		}

		virtual bool Do();

		virtual bool Do( CSaltoSynthFrame  &pSynthFrame,
						   Frame			 *pCurrSpectralFrameBase,										
						   SynthesisState    &synthState,
						   Frame			 *pCurrSpectralFrameResidual,
						   SpectralPeakArray *pCurrPeakArrayTarget);
	protected:

		bool ConcretStart();

		bool ConcretStop();

		bool ConcreteConfigure( const ProcessingConfig& c );

		void DoInterpolatingSynthesis( CSaltoSynthFrame  &pSynthFrame,
									   Frame			 *pCurrSpectralFrameBase,										
									   SynthesisState    &synthState,
									   Frame			 *pCurrSpectralFrameResidual,
									   SpectralPeakArray *pCurrPeakArrayTarget);

		void DoStationarySynthesisProcess( SynthesisState &synthState );

		void AttackResidualSynthesis( SynthesisState &synthState );

		void StationaryResidualSynthesis( SynthesisState &synthState );

		int UseRandomLoopCB( TControlData value );

		int InterpolFactorCB( TControlData value );

		int UseRandomDeviationsCB( TControlData value );

		int RandomRangeCB( TControlData value );

		int UsePitchCorrectionCB( TControlData value );

		int UseSinesCB( TControlData value );

		int UsePhaseAlignmentCB( TControlData value );

		int BreathOnlySoundCB( TControlData value );

		int PlayFrameOnlyCB( TControlData value );

		int UseAttackResidualCB( TControlData value );

		int UseStatResidualCB( TControlData value );

		int StatResGainCB( TControlData value );

		int StatResFreqCB( TControlData value );

		StationaryHandlerConfig					mConfig;

	private:

		bool									mUseRandomLoop;

		double									mInterpolFactor;

		bool									mUseRandomDeviations;

		double									mRandomRange;

		bool									mUsePitchCorrection;

		bool									mUseSines;

		bool									mUsePhaseAlignment;

		bool									mBreathOnlySound;

		bool									mPlayFrameOnly;

		bool									mUseAttackResidual;

		bool									mUseStatResidual;

		double									mStatResGain;

		double									mStatResFreq;


		OutControl								mOut_InLoopSynthesis;

		OutControl								mOutUseRandomDeviations;

		OutControl								mOutUseRandomLoop;
		
		OutControl								mOutRandomRange;

		OutControl								mOutTargetFreq;
		
		OutControl								mOutMagInterpolFactor;

		OutControl								mOutMagGain;

		OutControl								mOutMagInterpolFactor2;

		OutControl								mOutFreqInterpolFactor;

		OutControl								mOutPitchFactor;

		OutControl								mOutBreathOnlySound;

		OutControl								mOutAttackTimbreLevel;

		OutControl								mOutUsePhaseAlignment;

		OutControl								mOutLastAlignedFrame;

		OutControl								mOutResGain;

		OutControl								mOutResFreq;

		OutControl								mOutAttackResGain;

	};
}

#endif
