#ifndef _SynthesisProcessor_
#define _SynthesisProcessor_

#include "Processing.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"
#include "SynthesisState.hxx"
#include "Frame.hxx"

#include "CSaltoDataManagment.hxx"

using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::OutControl;
using CLAM::TControlData;
using CLAM::InControlTmpl;
using CLAM::Frame;

using CLAM::CSaltoDataManagment;

namespace SALTO
{
	class SynthesisProcessorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE ( SynthesisProcessorConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit(void);
	};

	class SynthesisProcessor: public Processing
	{
	public:

		SynthesisProcessor();

		SynthesisProcessor( SynthesisProcessorConfig& cfg );

		virtual ~SynthesisProcessor();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return " SALTO::SynthesisProcessor ";
		}

		virtual bool Do();

		bool Do( SynthesisState &synthState);

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig &c );

		void AttackResidualSynthesis( SynthesisState   &synthState );

		void StationaryResidualSynthesis( SynthesisState &synthState );

		int StatResFreqCB( TControlData value );

		int StatResGainCB( TControlData value );

		int UseSinesCB( TControlData value );

		int UseAttackResidualCB( TControlData value );

		int UseStatResidualCB( TControlData value );

		int PlayFrameOnlyCB( TControlData value );

		int UsePhaseAlignmentCB( TControlData value );

		int BreathOnlySoundCB( TControlData value );


	private:

		SynthesisProcessorConfig				mConfig;
		
		double									mStatResFreq;

		double									mStatResGain;

		bool									mUseSines;

		bool									mUseAttackResidual;

		bool									mUseStatResidual;

		bool									mPlayFrameOnly;

		bool									mUsePhaseAlignment;

		bool									mBreathOnlySound;

		InControlTmpl< SynthesisProcessor >		mInStatResFreq;

		InControlTmpl< SynthesisProcessor >		mInStatResGain;

		InControlTmpl< SynthesisProcessor >		mInUseSines;

		InControlTmpl< SynthesisProcessor >		mInUseAttackResidual;

		InControlTmpl< SynthesisProcessor >		mInUseStatResidual;

		InControlTmpl< SynthesisProcessor >		mInPlayFrameOnly;

		InControlTmpl< SynthesisProcessor >		mInUsePhaseAlignment;

		InControlTmpl< SynthesisProcessor >		mInBreathOnlySound;

		OutControl								mOutResGain;

		OutControl								mOutResFreq;

		OutControl								mOutBreathOnlySound;

		OutControl								mOutAttackTimbreLevel;

		OutControl								mOutUsePhaseAlignment;

		OutControl								mOutLastAlignedFrame;

	};
}

#endif
