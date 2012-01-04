#include "MIDISource.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include "../RtMidi.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDISource",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDISource> registrator(metadata);	
}
}

static void RtMidiCallback(
	double deltatime, 
	std::vector< unsigned char > *message,
	void *userData )
{
	CLAM::MIDISource* source = (CLAM::MIDISource*)userData;
	source->Do(message);
}


CLAM::MIDISource::MIDISource(const Config& config) 
	: mMIDIMessage("MIDI Message Out", this)
	, mMIDIin(0)
{
	Configure( config );
}

bool CLAM::MIDISource::ConcreteConfigure(const ProcessingConfig & config)
{
	if ( mMIDIin )
	{
		delete mMIDIin;
		mMIDIin = 0;
	}
	// Create RtMidiIn Object
	try
	{
		mMIDIin = new RtMidiIn();
	}
	catch ( RtError &error )
	{
		return AddConfigErrorMessage(error.getMessage());
	}

	// Don't ignore sysex, timing, or active sensing messages.
	mMIDIin->ignoreTypes( false, false, false );

	// Open Virtual Port
	try
	{
		mMIDIin->openVirtualPort("CLAM - MIDISource In");

		// Set our callback function.
		// This should be done immediately after opening
		// the port to avoid having incoming messages written
		// to the queue.
		mMIDIin->setCallback( &RtMidiCallback, this );
	}
	catch ( RtError &error )
	{
		return AddConfigErrorMessage(error.getMessage());
	}
	return true;
}

CLAM::MIDISource::~MIDISource()
{
	if ( mMIDIin )
		delete mMIDIin;
}

bool CLAM::MIDISource::Do(std::vector< unsigned char > *message)
{
	// TODO: deal with many messages in the vector/queue and take timestamp into account

	unsigned int nBytes = message->size();
	if (nBytes>0)
	{
		// Send Message
		MIDI::Message tmpMessage( (*message)[0] , (*message)[1] , (*message)[2] , (*message)[3] );
		mMIDIMessage.SendControl(tmpMessage);
	}
	return true;
}

