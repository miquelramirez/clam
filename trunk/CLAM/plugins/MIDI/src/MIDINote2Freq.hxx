#ifndef MIDINote2Freq_hxx
#define MIDINote2Freq_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>
#include <bitset>

namespace CLAM {

	class MIDINote2FreqConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDINote2FreqConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, ScaleAmplitude);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetScaleAmplitude(1);
		}

	};
	
	class MIDINote2Freq : public CLAM::Processing
	{ 
	protected:
		InControl<MIDI::Message> mMIDIMessage;

		FloatOutControl mFreq;
		FloatOutControl	mAmplitude;
		
		MIDINote2FreqConfig mConfig;
		
	public:
		const char* GetClassName() const { return "MIDINote2Freq"; }
		
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const { return mConfig;}
		
		MIDINote2Freq();
		~MIDINote2Freq();

		bool Do();
		
		void DoCallback(MIDI::Message inMessage);
	};
	
} // End namespace
#endif // MIDINote2Freq_hxx
