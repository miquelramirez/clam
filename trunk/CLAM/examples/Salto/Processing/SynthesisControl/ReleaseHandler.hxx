#ifndef _ReleaseHandler_
#define _ReleaseHandler_

#include "Processing.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"

#include "CSaltoSynthFrame.hxx"
#include "SynthesisState.hxx"

using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::Processing;
using CLAM::OutControl;
using CLAM::InControlTmpl;
using CLAM::TControlData;

using CLAM::CSaltoSynthFrame;

namespace SALTO
{
	class ReleaseHandlerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ReleaseHandlerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit(void);
	};

	class ReleaseHandler: public Processing
	{
	public:

		InControlTmpl< ReleaseHandler >			mInUseRandomLoop;

		InControlTmpl< ReleaseHandler >			mInUsePicthCorrection;

		InControlTmpl< ReleaseHandler >			mInRandomRange;

		InControlTmpl< ReleaseHandler >			mInUseRandomDeviations;

		InControlTmpl< ReleaseHandler >			mInUseSines;

		InControlTmpl< ReleaseHandler >			mInUsePhaseAlignment;

		InControlTmpl< ReleaseHandler >			mInBreathOnlySound;

		ReleaseHandler();

		ReleaseHandler( ReleaseHandlerConfig &cfg );

		virtual ~ReleaseHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::ReleaseHandler";
		}

		virtual bool Do();

		bool Do( CSaltoSynthFrame &pSynthFrame,
				 SynthesisState   &synthState,
				 double gain );

		void InitReleaseSynthesis( SynthesisState &synthState );

	protected:
		 
		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig &c );

		void DoInterpolatingReleaseSynthesis(  CSaltoSynthFrame &pSynthFrame,
											   SynthesisState   &synthState,
											   double gain);

		void DoReleaseSynthesisProcess( SynthesisState &synthState );

		int UseRandomLoopCB( TControlData value );

		int UseRandomDeviationsCB( TControlData value );

		int RandomRangeCB( TControlData value );

		int UsePitchCorrectionCB( TControlData value );

		int UseSinesCB( TControlData value );

		int UsePhaseAlignmentCB( TControlData value );

		int BreathOnlySoundCB( TControlData value );

	private:

		ReleaseHandlerConfig					mConfig;

		TIndex									mFrameCounterRelease;

		TIndex									mNumFramesRelease;

		bool									mUseRandomLoop;

		bool									mUseRandomDeviations;

		double									mRandomRange;

		bool									mUsePitchCorrection;

		bool									mUseSines;

		bool									mUsePhaseAlignment;

		bool									mBreathOnlySound;


		OutControl								mOut_InLoopSynthesis;

		OutControl								mOutUseRandomLoop;

		OutControl								mOutUseRandomDeviations;

		OutControl								mOutPitchFactor;

		OutControl								mOutRandomRange;

		OutControl								mOutTargetFreq;

		OutControl								mOutMagInterpolFactor;

		OutControl								mOutMagGain;

		OutControl								mOutAttackTimbreLevel;

		OutControl								mOutUsePhaseAlignment;

		OutControl								mOutLastAlignedFrame;

		OutControl								mOutBreathOnlySound;

		OutControl								mOutState;
	};
}

#endif
