#include "MIDIController.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <iostream>
#include <climits> // FOR CHAR_BIT
namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDIController",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDIController> registrator(metadata);	
}

	MIDIController::MIDIController() 
		: _MIDIMessage("MIDI Message Input", this, &MIDIController::DoCallback)
		, _MIDIControlValue("Control Output", this)
	{
		Configure( _config );
	}

	MIDIController::~MIDIController() {}

	bool MIDIController::Do()
	{ 
		return true;
	}
	
	void MIDIController::DoCallback(MIDI::Message inMessage)
	{
		unsigned char controllerNumber=(unsigned char)inMessage[1];
		if( ((unsigned char)inMessage[0] & 0xF0) == 0xB0 )
		{
			// 14 bit control message:
			if (_config.HasEnable14BitMessage() and _config.GetEnable14BitMessage()==true)
			{
				if (controllerNumber==_config.GetControlNumberMSB())
				{
					_MSBValue=(unsigned char)inMessage[2] & 0x7F;
					_MSBReceived=true;
					return;
				}
				if (controllerNumber==_config.GetControlNumber() and _MSBReceived)
				{
					unsigned int completeValue=(_MSBValue<<7) | ((unsigned char)inMessage[2] & 0x7F);
					_MIDIControlValue.SendControl(QuantizedInt2Float(completeValue));
					_MSBValue=0;
					_MSBReceived=false;
					return;
				}
				return; // if received LSB without MSB 
			}
			// 7 bit control message:
			if(controllerNumber == _config.GetControlNumber())
			{
				_MIDIControlValue.SendControl(QuantizedInt2Float((unsigned int)inMessage[2]));
			}
		}
	}
	bool MIDIController::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		_MSBReceived=false;
		_expectedMin = _config.HasExpectedMinValue() ? _config.GetExpectedMinValue() : 0;
		_expectedMax = _config.HasExpectedMaxValue() ? _config.GetExpectedMaxValue() : 127;
		double maxInt = (_config.HasEnable14BitMessage() && _config.GetEnable14BitMessage()) ? 16383. : 127.;
		_scale = maxInt / (_expectedMax - _expectedMin);
		return true;
	}
}

