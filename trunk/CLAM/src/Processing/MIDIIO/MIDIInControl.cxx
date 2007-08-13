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

#include "MIDIInControl.hxx"
#include "OutControl.hxx"
#include <string>
#include "ProcessingFactory.hxx"

namespace CLAM 
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "MIDIInControl",
	//	"category", "MIDI",
	//	"description", "MIDIInControl",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, MIDIInControl> regMIDIInControl("MIDIInControl");
	static FactoryRegistrator<ProcessingFactory, MIDIInControl> reg = metadata;
}

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
			std::string tmp = std::string() + MIDI::GetMessageInfo(m).name + ":" + fieldname;
			/* add the InControl, and remember which message byte it will
			 * control */
			mMsgByteIdToControlId[i] = ctrlid++;
			mMyOutControls.AddElem(new OutControl(tmp.c_str(),this));
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
			if (mMsgByteIdToControlId[i] == 0xFF) continue;

			if (i==1 && (msg[0]&0xF0)==0xE0)
			{
				/* we make an exception for pitchbend: instead of putting
				 * out to values (LSB, MSB), we prefer 1 14bit value.
				 * see also the code in ConcreteConfigure
				*/
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


} // namespace CLAM

