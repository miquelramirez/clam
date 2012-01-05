/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "MIDIDevice.hxx"
#include "MIDIIn.hxx"
#include "MIDIOut.hxx"
#include "MIDIClocker.hxx"
#include "MIDIEnums.hxx"
#include <algorithm>

using std::find;

using namespace CLAM;

MIDIManager& MIDIDevice::_MIDIManager(void)
{
	if (mMIDIManager==0) 
		throw Err("This MIDIDevice is not associated with any MIDIManager");

	return *mMIDIManager; 
}

void MIDIDevice::_SetMIDIManager(MIDIManager* am)
{
	if (mMIDIManager==0) mMIDIManager = am;
	else if (mMIDIManager!=am)
	{
		throw Err("An MIDIDevice can only be associated with one MIDIManager");
	}
}

bool MIDIDevice::Register(MIDIManager* mm,MIDIIn& in)
{
	/** Register a MIDIIn object as a new input of the device
	*/
	mInputs.push_back(&in);
	_SetMIDIManager(mm);
	in.mpDevice = this;
	return true;
}

bool MIDIDevice::Register(MIDIManager* mm,MIDIClocker& in)
{
	_SetMIDIManager(mm);
	in.mpDevice = this;
	return true;
}

bool MIDIDevice::Register(MIDIManager* mm,MIDIOut& out)
{
	mOutputs.push_back(&out);
	_SetMIDIManager(mm);
	out.mpDevice = this;
	return true;
}

void MIDIDevice::Unregister(MIDIIn& in)
{
	if (in.mpDevice != this)
	{
		throw(Err("MIDIDevice::Unregister(): I am not this MIDIIn object's device."));
	}
	std::vector<MIDIIn*>::iterator it = std::find(mInputs.begin(),mInputs.end(),&in);
	if (it == mInputs.end())
	{
		throw(Err("MIDIDevice::Unregister(): MIDIIn object not registered in this device."));
	}

	/** Unregistering the MIDIIn object from the target table
	*/
	for (int status = 0;status<8;status++)
	{
		for (int channel = 0;channel<16;channel++)
		{	
			for (int i=0;i<128;i++)
			{
				if (target[status][channel][i]==&in)
					target[status][channel][i] = 0;
			}
		}
	}

	mInputs.erase(it);
	in.mpDevice = 0;
}

void MIDIDevice::Unregister(MIDIClocker& in)
{
	in.mpDevice = 0;
}


void MIDIDevice::Unregister(MIDIOut& out)
{
	std::vector<MIDIOut*>::iterator it = std::find(mOutputs.begin(),mOutputs.end(),&out);
	if (it == mOutputs.end())
	{
		throw(Err("MIDIDevice::Unregister(): MIDIOut object not registered in this device."));
	}
	mOutputs.erase(it);
	out.mpDevice = 0;
}


void MIDIDevice::GetInfo(MIDIDevice::TInfo &info)
{
	info.mName = mName;
}

void MIDIDevice::SetTarget(
	MIDI::Message msg,
	unsigned char chn,unsigned char firstData,
	MIDIIn* inp)
{
	if (msg==MIDI::eNoteOnOff)
	{
		SetTarget(MIDI::eNoteOn, chn, firstData, inp);
		SetTarget(MIDI::eNoteOff,chn, firstData, inp);
		return;
	}

	if (firstData==0x80)
	{
		for (firstData=0;firstData<128;firstData++)
		{
			SetTarget(msg,chn,firstData,inp);
		}
		return;
	}

	if (msg==MIDI::eSystem)
	{
		if (chn==0)
		{
			CLAM_WARNING(true,"CLAM::MIDIDevice: SysEx not yet implemented");
			return;
		}
	}else{
		if (chn==0)
		{
			for (chn = 1;chn<=16;chn++)
			{
				SetTarget(msg,chn,firstData,inp);
			}
			return;
		}
		chn--;
	}
	target[msg][chn][firstData] = inp;
}

void MIDIDevice::Start(void)
{
	int status ;

	/** Initialating the target table
	*/
	for (status = 0;status<8;status++)
	{
		for (int channel = 0;channel<16;channel++)
		{	
			for (int byte=0;byte<128;byte++)
			{
							target[status][channel][byte] = 0;
			}
		}
	}
	
	for (unsigned int i=0; i<mInputs.size(); i++)
	{
		const MIDIIOConfig &cfg = 
			dynamic_cast<const MIDIIOConfig&>(mInputs[i]->GetConfig());

		SetTarget(
			MIDI::Message(cfg.GetMessage()),
			cfg.GetChannel(),
			cfg.GetFirstData(),
			mInputs[i]);
	}

	ConcreteStart();
}

void MIDIDevice::Stop(void)
{
	ConcreteStop();
}

int MIDIDevice::GetMessageLength(unsigned char byte)
{
	MIDI::Message cm = MIDI::StatusByteToMessage(byte);
	if (cm == MIDI::eSystem)
	{
		MIDI::SysMsg sm = MIDI::StatusByteToSysMsg(mData[0]);
		return MIDI::sNBytesPerSysMsg[int(sm)];
	}
	else
	{
		mLastStatus = mData[0];
		return MIDI::GetMessageInfo(cm).length;
	}
}

void MIDIDevice::HandleRawByte(unsigned char byte)
{	
	if (byte==0xfe) return;  /** Filter Active Sensing */
	if (mIndex==0)           /** First message byte */
	{
		if (byte&0x80)       /** Status byte */
		{
			mData[0] = byte;
			mIndex = 1;
		}
		else                 /** Data byte */
		{
			if (mLastStatus) 
			{
				mData[0] = mLastStatus;
				mData[1] = byte;
				mIndex = 2;
			}
		}
		mLength = GetMessageLength(mData[0]);
	}else                   /** First or second data byte */
	{
		mData[mIndex++] = byte;
	}
	if (mLength && mIndex == mLength)   /** We have just receive all the bytes of the message */
	{
		/** Extracting information form the status byte */
		int status = ((mData[0]&0xF0)>>4)-8;			/** Message type */
		int channel = mData[0]&0x0F;					/** Number of channel */  
		if (status == MIDI::eNoteOn && mData[2]==0)		/** Note On with velocity 0 */
		{
			status = MIDI::eNoteOff;					/** Mapping Note On status */
			mData[0] = 0x80|channel;					/** byte to a Note Off status  */
														/** byte with the same channel */															
		}
		MIDIIn* t = target[status][channel][mData[1]];  /** Getting the correspondence */
														/** MIDIIn object to this message */
		if (t)
		{
			t->Handle(mData,mLength);
		}
		mIndex = 0;
	}
}

