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
		DYNAMIC_TYPE_USING_INTERFACE (MIDIControllerConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, ControlNumber);
		DYN_ATTRIBUTE (1, public, bool, Enable14BitMessage);
		DYN_ATTRIBUTE (2, public, int, ControlNumberMSB);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetControlNumber(74);
			SetEnable14BitMessage(false);
			SetControlNumberMSB(42);
		}

	};
	
	class MIDIController : public CLAM::Processing
	{ 
	protected:
		TypedInControl<MIDI::Message> _MIDIMessage;

		FloatOutControl _MIDIControlValue;

		MIDIControllerConfig _config;

		bool _MSBReceived;
		unsigned char _MSBValue;
		
	public:
		const char* GetClassName() const { return "MIDIController"; }
		
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const { return _config;}
		
		MIDIController();
		~MIDIController();

		bool Do();
		
		void DoCallback(MIDI::Message inMessage);
	};
	
} // End namespace
#endif // MIDIController_hxx
