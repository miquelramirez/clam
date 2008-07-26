#ifndef MIDISource_hxx
#define MIDISource_hxx

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

#include "RtMidi.hxx"

namespace CLAM {

	class MIDISource : public CLAM::Processing
	{ 
		/** Controls **/
		OutControl mMIDIMessage;
		OutControl mMIDIData1;
		OutControl mMIDIData2;

		/**
		* mTrigger should be connected to an InControlTmpl to inform that all MIDI Data is placed and can be used.
		*/
		OutControl mTrigger; 

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
				mMIDIMessage.SendControl((float) ( (*message)[0]) );
				mMIDIData1.SendControl((float) ( (*message)[1]));
				mMIDIData2.SendControl((float) ( (*message)[2]));
				mTrigger.SendControl(1);
			}
			return true;
		}
	};
	
} // End namespace
#endif // MIDISource_hxx
