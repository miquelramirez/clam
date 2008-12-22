#ifndef MIDIController_hxx
#define MIDIController_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedInControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>
#include <bitset>

namespace CLAM {

	class MIDIControllerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIControllerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, ControlNumber);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetControlNumber(74);
		}

	};
	
	class MIDIController : public CLAM::Processing
	{ 
	protected:
		CascadingTypedInControl<MIDI::Message, MIDIController> mMIDIMessage;

		FloatOutControl mMIDIControlValue;

		MIDIControllerConfig mConfig;
		
	public:
		const char* GetClassName() const { return "MIDIController"; }
		
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const { return mConfig;}
		
		MIDIController();
		~MIDIController();

		bool Do();
		
		int DoCallback(MIDI::Message inMessage);
	};
	
} // End namespace
#endif // MIDIController_hxx
