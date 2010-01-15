#ifndef MIDISource_hxx
#define MIDISource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>

#include "../RtMidi.hxx"

namespace CLAM {

	class MIDISource : public CLAM::Processing
	{ 
		/** Controls **/
		OutControl<MIDI::Message> mMIDIMessage;
		
		RtMidiIn *mMIDIin;

		static void RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData );

	public:
		const char* GetClassName() const { return "MIDISource"; }

		MIDISource( const Config& config = Config() );

		~MIDISource();

		bool Do() { return true; }

		bool Do(std::vector< unsigned char > *message)
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
	};
	
} // End namespace
#endif // MIDISource_hxx
