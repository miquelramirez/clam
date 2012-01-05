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

#ifndef __MIDIEnums__
#define __MIDIEnums__

namespace CLAM {

class MIDI
{
public:
	/**
	* Channel message IDs. This corresponds with the 4 most significant 
	* bits of the MIDI Channel message.
	*/
	enum Message {
		eNoteOff = 0,
		eNoteOn = 1,
		ePolyAftertouch = 2,
		eControlChange = 3,
		eProgramChange = 4,
		eAftertouch = 5,
		ePitchbend = 6,
		eSystem = 7,
		// and some special stuff
		eNoteOnOff = 8
	};

	struct MessageInfo
	{
		int length;
		const char* name;
		const char* field[3];
	};

	/**
	* System message IDs. This corresponds with the 4 least significant 
	* bits of the MIDI System message (#Fx).
	*/
	enum SysMsg {
		eExclusiveStart = 0,
		eQuarterFrams = 1,
		eSongPosition = 2,
		eSongSelect = 3,
		eUndefined1 = 4,
		eUndefined2 = 5,
		eTuneRequest = 6,
		eExclusiveEnd = 7,
		eTimingCloce = 8,
		eTimingTice = 9,
		eStart = 10,
		eContinue = 11,
		eStop = 12,
		eUndefined3 = 13,
		eActiveSensing = 14,
		eSystemReset = 15
	};

	/**
	* Convert the first byte of a MIDI Channel Message to a EMessage.
	* @param b The midi byte
	*/
	static Message StatusByteToMessage(const unsigned char& byte)
	{
		return (Message)((byte >> 4)&7);
	}

	/**
	* Convert the first byte of a MIDI Channel Message to a ECLAMSysMsg.
	* @param b The midi byte, which should be have the hexadecimal value #Fx
	*/
	static SysMsg StatusByteToSysMsg(const unsigned char& b)
	{
		return (SysMsg)(b&15);
	}

	static const MessageInfo& GetMessageInfo(Message msg)
	{
		return sMessageInfo[int(msg)];
	}

	static const MessageInfo& GetMessageInfo(int msg)
	{
		return sMessageInfo[msg];
	}
private:
	static const MessageInfo sMessageInfo[9];

friend class MIDIDevice;

	static const int sNBytesPerSysMsg[16];

};

}

#endif

