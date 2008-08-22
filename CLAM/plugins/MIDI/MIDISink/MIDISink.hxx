#ifndef MIDISink_hxx
#define MIDISink_hxx

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>

#include "RtMidi.hxx"

namespace CLAM {

	class MIDISink : public CLAM::Processing
	{ 
		/** Controls **/
		InControl mMIDIMessage;
		InControl mMIDIData1;
		InControl mMIDIData2;

		/*
		* TODO: mTrigger should be an InControlTmpl 
		*/
		InControl mTrigger; 

		RtMidiOut *mMIDIout;

		//static void RtMidiCallback( double deltatime, std::vector< unsigned char > *message, void *userData );

	public:
		const char* GetClassName() const { return "MIDISink"; }

		MIDISink( const Config& config = Config() );

		~MIDISink();

		bool Do()
		{
			std::vector< unsigned char > message;
			if((bool)(mTrigger.GetLastValue())){
				message.push_back(mMIDIMessage.GetLastValue());
				message.push_back(mMIDIData1.GetLastValue());
				message.push_back(mMIDIData2.GetLastValue());
				
				mMIDIout->sendMessage( &message );
				
				mMIDIMessage.DoControl(0);
				mMIDIData1.DoControl(0);
				mMIDIData2.DoControl(0);
				mTrigger.DoControl(0);
			}
			return true;
		}
	};
	
} // End namespace
#endif // MIDISink_hxx
