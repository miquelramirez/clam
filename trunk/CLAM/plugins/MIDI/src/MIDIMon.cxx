// Version beyond 1.1.0
#include "MIDIMon.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cstdlib>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDIMon",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MIDIMon> registrator(metadata);	
}

	void MIDIMon::RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
	{
		MIDIMon* monitor = (MIDIMon*)userData;
		monitor->Do(message);
	}

	MIDIMon::MIDIMon(const Config& config) 
		: mMIDIMessage("MIDI Message", this),
			mMIDIData1("MIDI Data 1", this),
			mMIDIData2("MIDI Data 2", this)
	{
		// Create RtMidiIn Object
		try {
			mMIDIin = new RtMidiIn();
		}
		catch ( RtError &error ) {
			error.printMessage();
			std::exit( EXIT_FAILURE );
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

	MIDIMon::~MIDIMon() {
		if ( mMIDIin )
			delete mMIDIin;
	}

}
