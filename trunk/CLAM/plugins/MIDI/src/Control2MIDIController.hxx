#ifndef Control2MIDIController_hxx
#define Control2MIDIController_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>

#include "../RtMidi.hxx"

namespace CLAM {


	class Control2MIDIControllerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Control2MIDIControllerConfig,4,ProcessingConfig);
		DYN_ATTRIBUTE (0,public,int,ChannelNumber);
		DYN_ATTRIBUTE (1,public,int,ControllerNumber);
		DYN_ATTRIBUTE (2,public,float,ExpectedMinValue);
		DYN_ATTRIBUTE (3,public,float,ExpectedMaxValue);
	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetChannelNumber(0);
			SetControllerNumber(0);
			SetExpectedMinValue(0.);
			SetExpectedMaxValue(127.);
		}
	};

	class Control2MIDIController : public CLAM::Processing
	{ 
	protected:
		/** Controls **/
//		InControl <float> _InControlValue; //TODO: WHY THIS GetLastValue() DOESN'T WORK IN THIS WAY?????
		FloatInControl _InControlValue;
//		InControl <unsigned int> _InControlChannelNumber;
//		InControl <unsigned int> _InControlControllerNumber;
		OutControl<MIDI::Message> _MIDIMessage;

		Control2MIDIControllerConfig _config;

		float _expectedMin;
		float _expectedMax;
		double _scale;

		unsigned int Float2QuantizedInt (float originalFloat) const
			{ return (originalFloat - _expectedMin) * _scale; }

		float QuantizedInt2Float (unsigned int originalInt) const
			{ return (originalInt / _scale) + _expectedMin; }


	public:
		const char* GetClassName() const { return "Control2MIDIController"; }
		bool ConcreteConfigure(const ProcessingConfig& c);
		const ProcessingConfig &GetConfig() const {return _config; }
		Control2MIDIController( const Control2MIDIControllerConfig& config = Control2MIDIControllerConfig() );
		~Control2MIDIController();
		bool Do() 
		{
			if (_InControlValue.HasBeenRead())
				return true;
			unsigned char statusByte,dataByte0,dataByte1;

			statusByte = 0xB0 | (_config.GetChannelNumber() & 0x0F); // Set status byte: control change, with channel
			dataByte0 = 0x7F & (unsigned char)_config.GetControllerNumber();

			const unsigned char scaledCharValue=Float2QuantizedInt(_InControlValue.GetLastValue());

			dataByte1 = 0x7F & scaledCharValue;
			// Send Message
//			std::cout<< "status: "<<statusByte<<" ; data0: "<<dataByte0<<" ; data1: "<<dataByte1<<std::endl;
				MIDI::Message tmpMessage( statusByte,dataByte0,dataByte1);
				_MIDIMessage.SendControl(tmpMessage);
			return true;
		}
	};
	
} // End namespace
#endif // Control2MIDIController_hxx
