#include "MIDISink.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <cstdlib>
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
		: mMIDIMessage("MIDI Message In", this, &MIDISink::DoCallback)
	{
		// Create RtMidiIn Object
		try {
			mMIDIout = new RtMidiOut();
		}
		catch ( RtError &error ) {
			error.printMessage();
			std::exit( EXIT_FAILURE );
		}

		// Open Virtual Port
		try {
			mMIDIout->openVirtualPort("CLAM - MIDISink Out");
		}
		catch ( RtError &error ) {
			error.printMessage();
		}
		
		// Make default message
		mLastMessage.Update(0,0,0,0);
		Configure( config );
	}

	MIDISink::~MIDISink() {
		if ( mMIDIout )
			delete mMIDIout;
	}

	void MIDISink::DoCallback(MIDI::Message inMessage){
		inMessage = mMIDIMessage.GetLastValue();
		
		std::vector< unsigned char > message;

		message.push_back((inMessage)[0]);
		message.push_back((inMessage)[1]);
		message.push_back((inMessage)[2]);
// 		message.push_back((inMessage)[3]);

		mMIDIout->sendMessage( &message );
	};

}
