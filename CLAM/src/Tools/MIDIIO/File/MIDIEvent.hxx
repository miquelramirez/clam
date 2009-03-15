/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 * UNIVERSITAT POMPEU FABRA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * MIDIFileReader C++ classes
 * This code is part of the CLAM library, but also usable stand-alone.
 * Maarten de Boer <mdeboer@iua.upf.es>
 *
 */
#ifndef __MIDIEvent__
#define __MIDIEvent__

#include <CLAM/MIDIMessage.hxx>

namespace MIDI
{

	class Event
	/* a midi event is a time-stamped midi message */
	{
	private:
		Message mMessage;
		Ticks   mTicks;
	public:
		Event()
		{
			mTicks = 0;
		}

		Event(const Message& m,const Ticks& t):mMessage(m),mTicks(t)
		{
		}
		
		Byte operator [] (int i) const { return mMessage[i]; }
		
		Ticks GetTicks(void) const { return mTicks; }
	};


	/* meta and sysex events are really different, can be any length, etc
	** so we subclass Event for them
	*/
	
	class MetaEvent:public Event
	{
	public:
		Byte *mData;
		MetaEvent(const Message& m,const Ticks& t,int datasize):Event(m,t)
		{
			mData = new Byte[datasize];
		}
	};

	class SysExEvent:public Event
	{
	public:
		Byte *mData;
		SysExEvent(const Message& m,const Ticks& t,int datasize):Event(m,t)
		{
			mData = new Byte[datasize];
		}
	};

}

#endif

