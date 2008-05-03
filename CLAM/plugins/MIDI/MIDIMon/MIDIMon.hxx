#ifndef MIDIMon_hxx
#define MIDIMon_hxx

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

#include "RtMidi.hxx"

 namespace CLAM {

	class MIDIMon : public CLAM::Processing
	{ 
		OutControl mMIDIMessage;
		OutControl mMIDIData1;
		OutControl mMIDIData2;

		RtMidiIn *mMIDIin;
		std::vector<unsigned char> mMessage;
		int mNBytes;

	public:
		const char* GetClassName() const { return "MIDIMon"; }

		MIDIMon(const Config& config = Config()) 
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
				exit( EXIT_FAILURE );
			}

			// Don't ignore sysex, timing, or active sensing messages.
			mMIDIin->ignoreTypes( false, false, false );

			// Open Virtual Port
			try {
				mMIDIin->openVirtualPort("CLAM - MIDIMon In");
			}
			catch ( RtError &error ) {
				error.printMessage();
			}
			Configure( config );
		}
		
		bool Do()
		{
			mMIDIin->getMessage( &mMessage );
		    mNBytes = mMessage.size();
			
			if (mNBytes>0)
			{
				mMIDIMessage.SendControl((float)mMessage[0]);
				mMIDIData1.SendControl((float)mMessage[1]);
				mMIDIData2.SendControl((float)mMessage[2]);
			}
			return true;
		}
		
	};
	
} // End namespace
#endif // MIDIMon_hxx
