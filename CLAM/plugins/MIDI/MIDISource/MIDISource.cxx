#include "MIDISource.hxx"
#include <CLAM/ProcessingFactory.hxx>

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

	inline void RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
	{
		MIDISource* monitor = (MIDISource*)userData;
		monitor->Do(message);
	}

	MIDISource::MIDISource(const Config& config) 
		: mMIDIMessage("MIDI Message", this),
			mMIDIData1("MIDI Data 1", this),
			mMIDIData2("MIDI Data 2", this),
			mTrigger("Trigger", this)
	{
		// Create RtMidiIn Object
		try {
			mMIDIin = new RtMidiIn();
		}
		catch ( RtError &error ) {
			error.printMessage();
			exit( EXIT_FAILURE );
		}

		// Don't ignore sysex, timing, or active sensing messages.
		mMIDIin->ignoreTypes( false, false, false );

		// Open Virtual Port
		try {
			mMIDIin->openVirtualPort("CLAM - MIDIMon In");

			// Set our callback function.  This should be done immediately after 
			// opening the port to avoid having incoming messages written to the
			// queue.
			mMIDIin->setCallback( &RtMidiCallback, this );
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		Configure( config );
	}

	MIDISource::~MIDISource() {
		if ( mMIDIin )
			delete mMIDIin;
	}

}
