#include "Control2MIDIController.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cstdlib>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Control2MIDIController",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Control2MIDIController> registrator(metadata);	
}

	Control2MIDIController::Control2MIDIController(const Control2MIDIControllerConfig& config) 
		: _InControlValue("Controller Value",this)
//		, _InControlChannelNumber("MIDI Channel",this)
//		, _InControlControllerNumber("Controller Number",this)
		, _MIDIMessage("MIDI Message Out", this)
	{
		Configure( config );
	}

	Control2MIDIController::~Control2MIDIController() {}
	bool Control2MIDIController::ConcreteConfigure(const CLAM::ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config,c);
		_expectedMin = _config.GetExpectedMinValue();
		_expectedMax = _config.GetExpectedMaxValue();
		double maxInt = 127.; //TODO: change when enable 14 bit messages
		_scale = maxInt / (_expectedMax - _expectedMin);
		return true;
	}
}
