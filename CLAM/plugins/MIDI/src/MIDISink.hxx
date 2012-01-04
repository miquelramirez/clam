#ifndef MIDISink_hxx
#define MIDISink_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/MIDIMessage.hxx>

namespace CLAM { class MIDISink; }
class RtMidiOut;

class CLAM::MIDISink : public CLAM::Processing
{ 
	/** Controls **/
	InControl< MIDI::Message> mMIDIMessage;
	MIDI::Message mLastMessage;
	RtMidiOut *mMIDIout;
	std::vector<unsigned char> _message3bytes;

public:
	MIDISink( const Config& config = Config() );
	~MIDISink();
	const char* GetClassName() const { return "MIDISink"; }
	bool Do() { return true; };
protected:
	bool ConcreteConfigure(const ProcessingConfig & config);
private:
	void DoCallback(MIDI::Message inMessage);
};

#endif // MIDISink_hxx
