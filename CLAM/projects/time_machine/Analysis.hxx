#ifndef _Analysis_
#define _Analysis_

#include "ProcessingObjectComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "config.h"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
#include "ErrProcessingObj.hxx"
#include "EWindowType.hxx"
#include "FFT.hxx"
#include "FFT_rfftw.hxx"
#include "Frame.hxx"
#include "InControl.hxx"
#include "MultiBandFrame.hxx"
#include "MultiChannelFrame.hxx"
#include "AudioCircularBuffer.hxx"
#include "LFSpectrum.hxx"
#include "MultiBandFrame.hxx"
#include "MultiChannelFrame.hxx"
#include "Multirate.hxx"
#include "Port.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingObject.hxx"
#include "Spectrum.hxx"
#include "TimeMachine.hxx"
#include "TDWindowing.hxx"
#include "WindowGen.hxx"
#include "ZeroPadding.hxx"
#include "ZeroPhaseWindowing.hxx"

#include <stdlib.h>

namespace MTG {

	/** Big configuration class */
	class AnalysisConfig : public ProcessingObjectConfig {
	public:
		REG_NUM_ATTR_USING_INTERFACE (AnalysisConfig, 7,ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, WindowSize, TSize);
		REGISTER (2, WindowType, MTG::EWindowType);
		REGISTER (3, FrameSize, TSize);
		REGISTER (4, UseZeroPhase, bool);			
		REGISTER (5, ZeroPaddingFactor, TData);
		REGISTER (6, CalculateFirstSpectrumWhenBufferIsFilled, bool);

		void Init();
	};

	/** This is the processing object class we are using to group several
	 *  other processing objects. 
	 */
	class Analysis : public ProcessingObjectComposite {

		// Configuration data
		AnalysisConfig mConfig;

		// We define the internal data in our system.
		TSize                     mFrameSize;
		TSize                     mHopSize;
		TSize                     mWindowSize;
		int                       mWindowType;
		TSize                     mAnalBufferSize;
		TSize                     mFFTSize;
		TInt32                    mZeroPaddingFactor;
		TInt32										mSamplePosition;
		TInt32                    mAbsoluteWriteIndex;
		bool                      mCalculateFirstSpectrumWhenBufferIsFilled;

		Audio                     Audio0, Audio1, Audio2;
		//Spectrum                  Spec;
		Window                    AnalWindow;

		AudioCircularBuffer<TData>   *mpCircBuffer;

		// The internal Processing Objects
		WindowGen                 PO_WindowGen;
		TDWindowing               PO_TDWindowing;
		ZeroPhaseWindowing        PO_ZeroPhaseWindowing;
		ZeroPadding               PO_ZeroPadding;
		FFT_rfftw                 PO_FFT;

		// And the interfaces with the outside world.

		/** Global input port */
		InPort InputAudio;
		/** Global output port */
		OutPort OutputAudio;

		// Internal convenience methods.

		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();

		// Processing Object compliance methods.

		const char *GetClassName() {return "AnlysisComposite";}

		/** Configuration method */
		bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

		TInt32 CalculatePowerOfTwo(TInt32 size);

	public:
		Analysis(AnalysisConfig&);
		Analysis();
		~Analysis();

		const ProcessingObjectConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** Unsupervised mode execution */
		bool Do(Audio &in, Spectrum &out);

		bool Do(Audio &in, TM::Frame &out);

		bool Do(Audio &in, TM::MultiBandFrame &out);

		bool Do(Audio &in, TM::MultiChannelFrame &out);

	};

}; //end of namespace

#endif //_Analysis_
