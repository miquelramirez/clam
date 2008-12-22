#ifndef MIDISink_hxx
#define MIDISink_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/TypedInControl.hxx>
#include <CLAM/MIDIMessage.hxx>
#include "../RtMidi.hxx"
#include <iostream>
namespace CLAM {

	class MIDISink : public CLAM::Processing
	{ 
		/** Controls **/
		CascadingTypedInControl< MIDI::Message, MIDISink > mMIDIMessage;
		MIDI::Message mLastMessage;
		RtMidiOut *mMIDIout;

	public:
		const char* GetClassName() const { return "MIDISink"; }

		MIDISink( const Config& config = Config() );

		~MIDISink();

		bool Do() { return true; };
		
		int DoCallback(MIDI::Message inMessage);
	};
} // End namespace
#endif // MIDISink_hxx
