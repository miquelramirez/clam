#ifndef _Synthesis_
#define _Synthesis_

#include "ProcessingObjectComposite.hxx"

#include "Array.hxx"
#include "Audio.hxx"
#include "AudioCircularBuffer.hxx"
#include "config.h"
#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Err.hxx"
#include "ErrProcessingObj.hxx"
#include "EWindowingMethod.hxx"
#include "IFFT.hxx"
#include "IFFT_rfftw.hxx"
#include "Frame.hxx"
#include "InControl.hxx"
#include "MultiBandFrame.hxx"
#include "MultiChannelFrame.hxx"
#include "LFSpectrum.hxx"
#include "MultiBandFrame.hxx"
#include "MultiChannelFrame.hxx"
#include "Multirate.hxx"
#include "OverlapAdd.hxx"
#include "Port.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingObject.hxx"
#include "TimeMachine.hxx"
#include "TDWindowing.hxx"
#include "WindowGen.hxx"
#include "ZeroPadding.hxx"
#include "ZeroPhaseWindowing.hxx"

#include <stdlib.h>

namespace MTG {

	/** Big configuration class */
	class SynthesisConfig : public ProcessingObjectConfig {
	public:
		REG_NUM_ATTR_USING_INTERFACE (SynthesisConfig, 6,ProcessingObjectConfig);
    REGISTER (0, Name, std::string);
		REGISTER (1, WindowSize, TSize);
		REGISTER (2, WindowType, TSize);
		REGISTER (3, FrameSize, TSize);
		REGISTER (4, UseZeroPhase, bool);
		REGISTER (5, ZeroPaddingFactor, TData);
		void Init();
	};

	/** This is the processing object class we are using to group several
	 *  other processing objects. 
	 */
	class Synthesis : public ProcessingObjectComposite {

		// Configuration data
		SynthesisConfig mConfig;

		// We define the internal data in our system.
		TSize  mFrameSize;
		TSize  mHopSize;
		TSize  mWindowSize;
		int    mWindowType;
		TSize  mSynthBufferSize;
		TSize  mOverlapWindowSize;
		TSize  mFFTSize;
		TInt16 mZeroPaddingFactor;
		SpecTypeFlags mtypeMagPhase;

		//internal Processing Data
		Audio                     Audio0, Audio1, Audio2;
		//Spectrum                  Spec;
		Window                    SynthWindow;

		AudioCircularBuffer<TData>   *mpCircBuffer;

		//internal Processing Objects
		WindowGen                 PO_WindowGen;
		ZeroPadding               PO_ZeroPadding;
		IFFT_rfftw                PO_IFFT;
		TDWindowing               PO_TDWindowing;
		ZeroPhaseWindowing        PO_ZeroPhaseWindowing;
		OverlapAdd							  PO_OverlapAdd;


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
		Synthesis(SynthesisConfig&);
		Synthesis();
		~Synthesis();


		const ProcessingObjectConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void);

		/** Unsupervised mode execution */
		bool Do(Spectrum& in, Audio &out);

		bool Do(TM::Frame& in, Audio& out);

		bool Do(TM::MultiBandFrame& in, Audio& out);
		
		bool Do(TM::MultiChannelFrame& in, Audio& out);

	};

}; //end of namespace

#endif //_Synthesis_
