#include "MIDIInControl.hxx"

namespace CLAM {

MIDIInControl::MIDIInControl():MIDIIn(false)
{ 
	mpDevice = 0;
	mMessageSize = mControllingBytes = 0;
	mMsgByteIdToControlId = 0;
	Configure(MIDIIOConfig());
}

MIDIInControl::MIDIInControl(const MIDIIOConfig &c):MIDIIn(false)
{
	mpDevice = 0;
	mMessageSize = mControllingBytes = 0;
	mMsgByteIdToControlId = 0;
	Configure(c);
}


bool MIDIInControl::ConcreteConfigure(const ProcessingConfig& c)
	throw(ErrProcessingObj)
{
	bool ret = MIDIIn::ConcreteConfigure(c);
	if (ret==false) return false;

	MIDI::Message m = MIDI::Message(mConfig.GetMessage());

	int mMessageSize = MIDI::GetMessageInfo(m).length;

	/* the amount of controlled bytes is the lenght of the midi message,
	 * but...: */
	mControllingBytes = mMessageSize;
	/* ... one less if we predefine the channel ... */
	if (mConfig.GetChannel()!=0) mControllingBytes--;
	/* ... and one less if we predefine the first data byte,
	 * which is particularly useful for control change messages */
	if (mConfig.GetFirstData()!=128) mControllingBytes--;

	if (mMsgByteIdToControlId) delete mMsgByteIdToControlId;
	mMsgByteIdToControlId = new unsigned char[mControllingBytes];

	int ctrlid = 0;
	
	bool singlePitchBendValue = false;
	
	/* create the InControls */
	for (int i=0;i<mMessageSize;i++)
	{
		char* fieldname = 0;
		/* if in this switch we set the fieldname, the control
		 * will be added */
		switch (i)
		{
			case 0:
				if (mConfig.GetMessage()==MIDI::eSystem)
				{
					if (mConfig.GetChannel()==0)
						fprintf(stderr,"ERROR: sysex in not yet implemented\n");
					else
						/* channel is not predefined */
						fieldname = MIDI::GetMessageInfo(m).field[i];
				}else{
					if (mConfig.GetChannel()==0)
						/* channel is not predefined */
						fieldname = MIDI::GetMessageInfo(m).field[i];
				}
				break;
			case 1:
				if (mConfig.GetFirstData()==128)
				{
					/* first data byte is not predefined */
					fieldname = MIDI::GetMessageInfo(m).field[i];
				}
				/* we make an exception for pitchbend: instead of putting
				 * out to values (LSB, MSB), we prefer 1 14bit value.
				 *	see also the code in Handle
				 */	
				/* nb: does a FirstData make sense with pitchbend??
				 * I don't think so, so we'll just ignore it. 
				 * (silently...)
				 */
				if (mConfig.GetMessage()==MIDI::ePitchbend)
				{
					fieldname = "Value";
					singlePitchBendValue = true;
				}
					
				break;
			default:
				/* all other fields will be controlled */
				if (!singlePitchBendValue)
				{
					fieldname = MIDI::GetMessageInfo(m).field[i];
				}
				break;
		}
		if (fieldname)
		{
			char tmp[256];
			snprintf(tmp,256,"%s:%s",MIDI::GetMessageInfo(m).name,fieldname);
			/* add the InControl, and remember which message byte it will
			 * control */
			mMsgByteIdToControlId[i] = ctrlid++;
			mMyOutControls.AddElem(new OutControl(tmp,this));
		}else{
			mMsgByteIdToControlId[i] = 0xFF;
		}
	}

	return true;
}

void MIDIInControl::Handle(unsigned char* msg,int size)
{
	/* The device has passed the message to this MIDIInControl.
	 * We now need dispatch the message to the resp. OutControls
	 */
	for (int i=size-1;i>=0;i--)
	{
		if (i==0 && (msg[0]&0xF0) == 0xF0) // system message
		{
			/* TODO: this now only handles correctly system realtime
			 * messages, where SetChannel is used to specify the
			 * type of message. Maybe this can be done more elegantly? */
			GetOutControls().GetByNumber(0).SendControl(1);
		}
		else
		{
			if (mMsgByteIdToControlId[i] != 0xFF)
			{
				if (i==1 && (msg[0]&0xF0)==0xE0)
				/* we make an exception for pitchbend: instead of putting
				 * out to values (LSB, MSB), we prefer 1 14bit value.
				 * see also the code in ConcreteConfigure
				*/
				{
					GetOutControls().GetByNumber(mMsgByteIdToControlId[1]).
						SendControl(msg[1] + (msg[2]<<7));
				}
				else
				if (i==0)
				{
					GetOutControls().GetByNumber(mMsgByteIdToControlId[0]).
						SendControl((msg[0]&0x0F)+1);
				}
				else
				{	
					GetOutControls().GetByNumber(mMsgByteIdToControlId[i]).
						SendControl(msg[i]);
				}
			}
		}
	}
}

} // namespace CLAM

