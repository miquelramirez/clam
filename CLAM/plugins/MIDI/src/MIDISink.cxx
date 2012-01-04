#include "MIDISink.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <iostream>
#include "../RtMidi.hxx"

namespace CLAM
{ 
namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDISink",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDISink> registrator(metadata);	
}
}

CLAM::MIDISink::MIDISink(const Config& config) 
	: mMIDIMessage("MIDI Message In", this, &MIDISink::DoCallback)
	, mMIDIout(0)
{
	Configure(config);
}
bool CLAM::MIDISink::ConcreteConfigure(const ProcessingConfig & config)
{
	if (mMIDIout)
	{
		delete mMIDIout;
		mMIDIout = 0;
	}
	// Create RtMidiIn Object
	try
	{
		mMIDIout = new RtMidiOut();
	}
	catch ( RtError &error )
	{
		return AddConfigErrorMessage(error.getMessage());
	}

	// Open Virtual Port
	try
	{
		mMIDIout->openVirtualPort("CLAM - MIDISink Out");
	}
	catch ( RtError &error )
	{
		return AddConfigErrorMessage(error.getMessage());
	}
	
	// Make default message
	mLastMessage.Update(0,0,0,0);
	return true;
}

CLAM::MIDISink::~MIDISink()
{
	if ( mMIDIout )
		delete mMIDIout;
}

void CLAM::MIDISink::DoCallback(MIDI::Message inMessage)
{
	inMessage = mMIDIMessage.GetLastValue();
	_message3bytes[0] = inMessage[0];
	_message3bytes[1] = inMessage[1];
	_message3bytes[2] = inMessage[2];

	mMIDIout->sendMessage( &_message3bytes );
};
