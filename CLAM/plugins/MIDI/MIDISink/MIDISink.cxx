#include "MIDISink.hxx"
#include <CLAM/ProcessingFactory.hxx>

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

	MIDISink::MIDISink(const Config& config) 
		: mMIDIMessage("MIDI Message", this),
			mMIDIData1("MIDI Data 1", this),
			mMIDIData2("MIDI Data 2", this),
			mTrigger("Trigger", this)
	{
		// Create RtMidiIn Object
		try {
			mMIDIout = new RtMidiOut();
		}
		catch ( RtError &error ) {
			error.printMessage();
			exit( EXIT_FAILURE );
		}

		// Open Virtual Port
		try {
			mMIDIout->openVirtualPort("CLAM - MIDISink Out");
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		Configure( config );
	}

	MIDISink::~MIDISink() {
		if ( mMIDIout )
			delete mMIDIout;
	}

}
