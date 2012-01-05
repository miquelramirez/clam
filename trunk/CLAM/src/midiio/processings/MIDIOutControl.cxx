/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "InControl.hxx"
#include "MIDIOutControl.hxx"
#include <string>

namespace CLAM {

MIDIOutControl::MIDIOutControl():MIDIOut(false)
{ 
	mpDevice = 0;
	InitMembers();
	Configure(MIDIIOConfig());
}

MIDIOutControl::MIDIOutControl(const MIDIIOConfig &c):MIDIOut(false)
{
	mpDevice = 0;
	InitMembers();
	Configure(c);
}

void MIDIOutControl::InitMembers(void)
{
	mUniqId = 0;

	mMessage = 0;
	mReceivedUniqId = 0;
	mControlIdToMsgByteId = 0;

	mMessageSize = 0;

	mControlledBytes = 0;
	mControlsReceived = 0;
}

bool MIDIOutControl::ConcreteConfigure(const ProcessingConfig& c)
	throw(ErrProcessingObj)
{
	bool ret = MIDIOut::ConcreteConfigure(c);
	
	if (ret==false) return false;

	MIDI::Message m = MIDI::Message(mConfig.GetMessage());

	mMessageSize = MIDI::GetMessageInfo(m).length;

	/* the amount of controlled bytes is the lenght of the midi message,
	 * but...: */
	mControlledBytes = MIDI::GetMessageInfo(m).length;
	/* ... one less if we predefine the channel ... */
	if (mConfig.GetChannel()!=0) mControlledBytes--;
	/* ... and one less if we predefine the first data byte,
	 * which is particularly useful for control change messages */
	if (mConfig.GetFirstData()!=128) mControlledBytes--;

	mControlsReceived = 0;

	/* allocate arrays */
	if (mMessage) delete [] mMessage;
	mMessage = new unsigned char[mMessageSize];

	if (mReceivedUniqId) delete [] mReceivedUniqId;
	mReceivedUniqId = new unsigned char[mControlledBytes];

	if (mControlIdToMsgByteId) delete mControlIdToMsgByteId;
	mControlIdToMsgByteId = new unsigned char[mControlledBytes];

	/* init uniq-id-per-received-value array */
	for (int i = 0; i < mControlledBytes ; i++ )
	{
		mReceivedUniqId[i] = mUniqId;
	}
	
	if (m==MIDI::eNoteOnOff) m = MIDI::eNoteOn;
	/* init first message byte, based on message type */
	mStatusByte = 0x80|(int(m)<<4);

	int ctrlid = 0;
	
	/* create the InControls */
	for (int i=0;i<MIDI::GetMessageInfo(m).length;i++)
	{
		const char* fieldname = 0;
		/* if in this switch we set the fieldname, the control
		 * will be added */
		switch (i)
		{
			case 0:
				if (mConfig.GetChannel()==0)
					/* channel is not predefined */
					fieldname = "Channel";
				else
					/* channel _is_ predefined, so modify status byte
					 * to contain channel */
					mStatusByte |= (mConfig.GetChannel()+1);
				break;
			case 1:
				if (mConfig.GetFirstData()==128)
					/* first data byte is not predefined */
					fieldname = MIDI::GetMessageInfo(m).field[i-1];
				else
					/* first data byte _is_ predefined, so modify message */	
					mMessage[1] = mConfig.GetFirstData();
				break;
			default:
				/* all other fields will be controlled */
				fieldname = MIDI::GetMessageInfo(m).field[i-1];
				break;
		}
		if (fieldname)
		{
			std::string controlName = std::string(MIDI::GetMessageInfo(m).name) + ":" + fieldname;
			/* add the InControl, and remember which message byte it will
			 * control */
			mControlIdToMsgByteId[ctrlid] = i;
			mMyInControls.AddElem(new FloatInControl(ctrlid++,controlName,this,&MIDIOutControl::DoControl));
		}
	}

	return true;
}

void MIDIOutControl::DoControl(unsigned id,TControlData val)
{
	/* we keep a uniq id to check if each message has been fully 
	 * constructed */
	if (mReceivedUniqId[id]!=mUniqId)
	{
		/* receiving a byte when the prev message was not fully
		 * constructed yet... TODO: should we throw or assert? */
		fprintf(stderr,"ERROR!!!! receiving a byte when the prev message was not fully constructed yet... TODO: should we throw or assert?\n");
		return;
	}
	/* ok, we still needed this byte */
	int i = mControlIdToMsgByteId[id];
	if (i==0)
	{
		/* for the first byte, we need to keep the status, and
		 * modify the channel */
		mStatusByte = (mStatusByte&0xF0) | ((unsigned char)(val)-1);
	}else{
		mMessage[i] = (unsigned char) val;
	}
	mReceivedUniqId[id]++;
	mControlsReceived++;
	if (mControlsReceived==mControlledBytes)
	{
		/* we got all controlled bytes, so we increment the mUniqId
		 * for the next message, reset mControlsReceived, and 
		 * call Handle to send the message to the device */
		mUniqId++;
		mControlsReceived = 0;
					mMessage[0]=mStatusByte;
		Handle(mMessage,mMessageSize);
	}
}

void MIDIOutControl::Handle(unsigned char* msg,int size)
{
	/* write the message to the device */
	CLAM_ASSERT(mpDevice,"MIDIOutControl used without a valid device");
	if ((msg[0]&0xF0)==0x90 && msg[2]==0) msg[0] &=0x8F;
	mpDevice->Write(msg,size);
}

} // namespace CLAM

