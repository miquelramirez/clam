#ifndef MIDIMon_hxx
#define MIDIMon_hxx

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

#include "RtMidi.hxx"

namespace CLAM {

	class MIDIMon : public CLAM::Processing
	{ 
		/** Controls **/
		OutControl mMIDIMessage;
		OutControl mMIDIData1;
		OutControl mMIDIData2;

		RtMidiIn *mMIDIin;

	public:
		const char* GetClassName() const { return "MIDIMon"; }

		MIDIMon( const Config& config = Config() );

		~MIDIMon();

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
			}
			return true;
		}
	};
	
} // End namespace
#endif // MIDIMon_hxx
