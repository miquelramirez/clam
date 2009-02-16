#include "MIDINote2Freq.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cmath>
#include <climits> // for CHAR_BIT
namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDINote2Freq",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDINote2Freq> registrator(metadata);	
}

	MIDINote2Freq::MIDINote2Freq() 
		: mMIDIMessage("MIDI Message Input", this, &MIDINote2Freq::DoCallback)
		, mFreq("Frequency Output", this)
		, mAmplitude("Amplitude Output", this)
	{
		Configure( mConfig );
	}

	MIDINote2Freq::~MIDINote2Freq() {}

	bool MIDINote2Freq::Do()
	{ 
		return true;
	}
	
	void MIDINote2Freq::DoCallback(MIDI::Message inMessage)
	{
		std::bitset<CHAR_BIT> statusByte;
		statusByte = (std::bitset<CHAR_BIT>)((unsigned char)inMessage[0]);
		if(statusByte[7] == 1 && statusByte[6] == 0 && statusByte[5] == 0 && statusByte[4] == 1)
		{
			float frequency;
			frequency = 440 * pow(2.0,((float)(inMessage[1])-69.0)/12.0);
			mFreq.SendControl(frequency);
			mAmplitude.SendControl(inMessage[2] * mConfig.GetScaleAmplitude() / 127.0); 
		}
	}
	bool MIDINote2Freq::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}
}
