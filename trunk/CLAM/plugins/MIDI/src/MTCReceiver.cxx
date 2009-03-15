#include "MTCReceiver.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cstdlib>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MTCReceiver",
		"category", "MIDI",
		0
		};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MTCReceiver> registrator(metadata);	
}

	void MTCReceiver::RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
	{
		MTCReceiver* source = (MTCReceiver*)userData;
		source->Do(message);
	}

	MTCReceiver::MTCReceiver(const Config& config) 
		: _millisecondsOut("MIDI Message Out", this)
		, _mtcFrameIndex(-1)
		, _frames(0)
		, _seconds(0)
		, _minutes(0)
		, _hours(0)
		, _lastReceivedTime(0)
	{
		// Create RtMidiIn Object
		try {
			_MIDIin = new RtMidiIn();
		}
		catch ( RtError &error ) {
			error.printMessage();
			std::exit( EXIT_FAILURE );
		}

		// Don't ignore sysex, timing, or active sensing messages.
		_MIDIin->ignoreTypes( false, false, false );

		// Open Virtual Port
		try {
			_MIDIin->openVirtualPort("CLAM - MTCReceiver In");

			// Set our callback function.  This should be done immediately after 
			// opening the port to avoid having incoming messages written to the
			// queue.
			_MIDIin->setCallback( &RtMidiCallback, this );
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		Configure( config );
	}

	MTCReceiver::~MTCReceiver() {
		if ( _MIDIin )
			delete _MIDIin;
	}

}
