#ifndef _TransitionHandler_
#define _TransitionHandler_

#include "SynthesisProcessor.hxx"

#include "CSaltoDataManagment.hxx"
#include "CSaltoSynthFrame.hxx"

using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::Frame;
using CLAM::OutControl;
using CLAM::InControlTmpl;
using CLAM::TFloat64;
using CLAM::TIndex;
using CLAM::TControlData;

using CLAM::CSaltoDataManagment;
using CLAM::CSaltoSynthFrame;

namespace SALTO
{
	class TransitionHandlerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (TransitionHandlerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
	protected:
		void DefaultInit(void);
	};

	class TransitionHandler: public Processing
	{
	public:
	
		InControlTmpl< TransitionHandler >		mInTransitionInterval;

		InControlTmpl< TransitionHandler >		mInTransitionUpwards;

		InControlTmpl< TransitionHandler >		mInUseRandomLoop;

		InControlTmpl< TransitionHandler >		mInUseRandomDeviations;

		InControlTmpl< TransitionHandler >		mInRandomRange;

		TransitionHandler();

		TransitionHandler( TransitionHandlerConfig &cfg );

		virtual ~TransitionHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::TransitionHandler";
		}
	
		inline void SetAssociated( SynthesisProcessor &synthProcessor )
		{
			mSynthProcessor = &synthProcessor;
		}

		virtual bool Do();

		void InitTransitionSynthesis(  SynthesisState &synthState,
									   Frame		  *pCurrSpectralFrameBase);

		bool Do( CSaltoSynthFrame   &pSynthFrame,
				 SynthesisState     &synthState,
				 Frame			    *pTransitionFrameTarget,
				 Frame              *pCurrSpectralFrameBase);

		void EndTransitionSynthesis( SynthesisState &synthState,
									 Frame          *pCurrSpectralFrameBase);

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig &c );

		void DoTransitionSynthesis( CSaltoSynthFrame &pSynthFrame,
									SynthesisState   &synthState,
									Frame			 *pTransitionFrameTarget);

		int TransitionIntervalCB( TControlData value );

		int TransitionUpwardsCB( TControlData value );

		int UseRandomLoopCB( TControlData value );

		int UseRandomDeviationsCB( TControlData value );

		int RandomRangeCB( TControlData value );

	private:

		TransitionHandlerConfig					mConfig;

		SynthesisProcessor*						mSynthProcessor;

		Frame*									mpTransitionFrameBase;

		Frame*									mpTransitionFrameTarget;

		TIndex									mSegPositionTransition;

		TIndex									mNumTransitionFrames;

		TIndex									mTransitionInterval;

		double									mLastIndividualGain;

		bool									mTransitionUpwards;

		bool									mUseRandomLoop;

		bool									mUseRandomDeviations;

		double									mRandomRange;

		OutControl								mOutTransitionInit;

		OutControl								mOutTransitionFrequency;

		OutControl								mOutDisplayedValuesChanged;

		OutControl								mOut_InLoopSynthesis;

		OutControl								mOutUseRandomDeviations;

		OutControl								mOutUseRandomLoop;

		OutControl								mOutRandomRange;

		OutControl								mOutTargetFreq;
		
		OutControl								mOutMagInterpolFactor;

		OutControl								mOutMagGain;

		OutControl								mOutFreqInterpolFactor;

		OutControl								mOutMagInterpolFactor2;

		OutControl								mOutPitchFactor;

		OutControl								mOutState;
	};
}

#endif
