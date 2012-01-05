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

#include "MIDIEnums.hxx"

namespace CLAM {

	const MIDI::MessageInfo MIDI::sMessageInfo[9] =
	{
//  nBytes  name        field[0]  field[1]  field[2]   Enum
		{ 
		3, "NoteOff",        {"Chn", "Key",    "Vel"  } // eNoteOff
		},{
		3, "NoteOn",         {"Chn", "Key",    "Vel"  } // eNoteOn
		},{
		3, "PolyAftertouch", {"Chn", "Key",    "Value"} // ePolyAftertouch
		},{
		3, "ControlChange",  {"Chn", "Ctrl",   "Value"} // eControlChange
		},{
		2, "ProgramChange",  {"Chn", "Value",  0      } // eProgramChange
		},{
		3, "Aftertouch",     {"Chn", "Value",  0      } // eAftertouch
		},{
		3, "Pitchbend",      {"Chn", "LSB",    "MSB"  } // ePitchbend
		},{
		1, "System",         {"Msg", 0,				0     } // eSystem
		},{
		3, "NoteOnOff",      {"Chn", "Key",    "Vel"  } // eSystem
		}	
	};

	const int MIDI::sNBytesPerSysMsg[16] =
	{
			0, // eExclusiveStart
			1, // eQuarterFrams
			2, // eSongPosition
			1, // eSongSelect
			1, // eUndefined1
			1, // eUndefined2
			1, // eTuneRequest
			1, // eExclusiveEnd
			1, // eTimingCloce
			1, // eTimingTice
			1, // eStart
			1, // eContinue
			1, // eStop
			1, // eUndefined3
			1, // eActiveSensing
			1  // eSystemReset
	};
}

