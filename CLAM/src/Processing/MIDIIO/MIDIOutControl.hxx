#ifndef __MIDIOUTCONTROL__
#define __MIDIOUTCONTROL__

#include "MIDIEnums.hxx"
#include "Array.hxx"
#include "MIDIOut.hxx"
#include "MIDIIOConfig.hxx"

namespace CLAM {

class MIDIOutControl:public MIDIOut
{
	private:
		Array<InControl*> mMyInControls;

		/* keep a unique id for each message we are constructing,
		 * to make sure we have each controlled byte only once
		 */
		unsigned char mUniqId;
		
		/* buffer to store the message */
		unsigned char* mMessage;
		unsigned char  mStatusByte;

		/* buffer to store the uniq id for each controlled byte */
		unsigned char* mReceivedUniqId;
		
		/* table to map InControl ID to byte-in-message index */
		unsigned char* mControlIdToMsgByteId;

		/* size of the midi message in bytes */
		unsigned char mMessageSize;

		/* number of bytes in the message that are controlled, and the
		 * number of of those that have been received for the current 
		 * message construction (current unique ID)
		 * (when these are equal the message has been constructed fully)
		 */
		unsigned char mControlledBytes;
		unsigned char mControlsReceived;
		
		void InitMembers(void);

		void Handle(unsigned char* msg,int size);
		
		/* callback function for each received control value */
		int DoControl(int id,TControlData val);
	public:
		MIDIOutControl();
		MIDIOutControl(const MIDIIOConfig &c);

		const char * GetClassName() const {return "MIDIOutControl";}

		bool ConcreteConfigure(const ProcessingConfig& c)
			throw(ErrProcessingObj);
};

} // namespace CLAM

#endif // MIDIOutControl.hxx
