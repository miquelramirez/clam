/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

	const MIDI::MessageInfo MIDI::sMessageInfo[8] =
	{
		// mask    nBytes name              field[0]   field[1]   
		{  0x0001, 3, "NoteOff",        {"Key",    "Vel"}   }, // eNoteOff
		{  0x0002, 3, "NoteOn",         {"Key",    "Vel"}   }, // eNoteOn
		{  0x0004, 3, "PolyAftertouch", {"Key",    "Value"} }, // ePolyAftertouch
		{  0x0008, 3, "ControlChange",  {"CtrlNr", "Value"} }, // eControlChange
		{  0x0010, 2, "ModeChange",     {0,0}               }, // eModeChange
		{  0x0020, 3, "Aftertouch",     {"Channel","Value"} }, // eAftertouch
		{  0x0040, 3, "Pitchbend",      {"LSB",    "MSB"}   }, // ePitchbend
		{  0x0080, 0, "System",         {0,        0}       }  // eSystem
	};

	const unsigned short MIDI::sChannelMask[16] = 
	{
		0x0001,0x0002,0x0004,0x0008,
		0x0010,0x0020,0x0040,0x0080,
		0x0100,0x0200,0x0400,0x0800,
		0x1000,0x2000,0x4000,0x8000
	};

	const unsigned short MIDI::sAllChannelMask = 0xFFFF;
	const unsigned short MIDI::sAllMessageMask = 0x00FF;

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
