#include "MIDIController.hxx"
#include <CLAM/ProcessingFactory.hxx>

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
		: mMIDIMessage("MIDI Message", this),
			mMIDIData1("MIDI Data 1", this),
			mMIDIData2("MIDI Data 2", this),
			mTrigger("Trigger", this),
			mMIDIControllerValue("Control Output", this)
	{
		Configure( mConfig );
	}

	MIDIController::~MIDIController() {}

	bool MIDIController::Do()
	{ 
		if(mTrigger.GetLastValue())
		{
			std::bitset<CHAR_BIT> statusByte;
			statusByte = (std::bitset<CHAR_BIT>)((unsigned char)mMIDIMessage.GetLastValue());
			if(statusByte[7] == 1 && statusByte[6] == 0 && statusByte[5] == 1 && statusByte[4] == 1)
			{
				if(((int)mMIDIData1.GetLastValue()) == mConfig.GetControlNumber()){
					mMIDIControllerValue.SendControl((float)mMIDIData2.GetLastValue());
				}
			}
		}		
		return true;
		 }

	bool MIDIController::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}
}
