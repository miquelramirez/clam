#ifndef __MIDIINCONTROL__
#define __MIDIINCONTROL__

#include "MIDIEnums.hxx"
#include "Array.hxx"
#include "MIDIIn.hxx"

namespace CLAM {

class MIDIInControl:public MIDIIn
{
private:
	Array<OutControl*> mMyOutControls;

	int mMessageSize;
	int mControllingBytes;
	
	unsigned char* mMsgByteIdToControlId;

public:
	MIDIInControl();
	MIDIInControl(const MIDIIOConfig &c);
	const char * GetClassName() const {return "MIDIInControl";}

	bool ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj);

private:
	void Handle(unsigned char* msg,int size);
};

} // namespace CLAM

#endif // MIDIInControl.hxx
