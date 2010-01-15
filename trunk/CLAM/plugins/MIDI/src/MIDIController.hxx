#ifndef MIDIController_hxx
#define MIDIController_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>
#include <bitset>

namespace CLAM {

	class MIDIControllerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIControllerConfig, 5, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, int, ControlNumber);
		DYN_ATTRIBUTE (1, public, bool, Enable14BitMessage);
		DYN_ATTRIBUTE (2, public, int, ControlNumberMSB);
		DYN_ATTRIBUTE (3, public, float, ExpectedMinValue);
		DYN_ATTRIBUTE (4, public, float, ExpectedMaxValue);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetControlNumber(74);
			SetEnable14BitMessage(false);
			SetControlNumberMSB(42);
			SetExpectedMinValue(0);
			SetExpectedMaxValue(127);
		}

	};
	
	class MIDIController : public CLAM::Processing
	{ 
	protected:
		InControl<MIDI::Message> _MIDIMessage;

		FloatOutControl _MIDIControlValue;

		MIDIControllerConfig _config;

		bool _MSBReceived;
		unsigned char _MSBValue;
		float _expectedMin;
		float _expectedMax;
		double _scale;

		unsigned int Float2QuantizedInt (float originalFloat) const
			{ return (originalFloat - _expectedMin) * _scale; }

		float QuantizedInt2Float (unsigned int originalInt) const
			{ return (originalInt / _scale) + _expectedMin; }

		void DoCallback(MIDI::Message inMessage);
		
	public:
		const char* GetClassName() const { return "MIDIController"; }
		
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const { return _config;}
		
		MIDIController();
		~MIDIController();

		bool Do();
		
	};
	
} // End namespace
#endif // MIDIController_hxx
