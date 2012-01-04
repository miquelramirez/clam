#ifndef MIDISource_hxx
#define MIDISource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/MIDIMessage.hxx>

namespace CLAM { class MIDISource; }
class RtMidiIn;

class CLAM::MIDISource : public CLAM::Processing
{ 
	/** Controls **/
	OutControl<MIDI::Message> mMIDIMessage;
	RtMidiIn *mMIDIin;

public:
	MIDISource( const Config& config = Config() );
	~MIDISource();
	const char* GetClassName() const { return "MIDISource"; }
	bool Do() { return true; }
	bool Do(std::vector< unsigned char > *message);
protected:
	bool ConcreteConfigure(const ProcessingConfig & config);
};

#endif // MIDISource_hxx
