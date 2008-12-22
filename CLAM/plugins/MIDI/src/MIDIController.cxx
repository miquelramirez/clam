#include "MIDIController.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <iostream>
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
		: mMIDIMessage("MIDI Message Input", this, &MIDIController::DoCallback),
			mMIDIControlValue("Control Output", this)
	{
		Configure( mConfig );
	}

	MIDIController::~MIDIController() {}

	bool MIDIController::Do()
	{ 
		return true;
	}
	
	int MIDIController::DoCallback(MIDI::Message inMessage){
		std::bitset<CHAR_BIT> statusByte;
		statusByte = (std::bitset<CHAR_BIT>)((unsigned char)inMessage[0]);
		if(statusByte[7] == 1 && statusByte[6] == 0 && statusByte[5] == 1 && statusByte[4] == 1)
		{
			if(((int)inMessage[1]) == mConfig.GetControlNumber()){
				mMIDIControlValue.SendControl((float)inMessage[2]);
			}
		}
		
	}
	bool MIDIController::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}
}
