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
#ifndef __MIDITrack__
#define __MIDITrack__

#include <list>
#include "MIDIEvent.hxx"

namespace MIDI
{

	class Track
	{
	private:
		/* a midi track consists of a list of midi event */
		std::list<Event*> mEventList; 
		
		/* and may have a name */
		char* mName;
		
	public:
		Track(void)
		:mName(0)
		{
		}

		~Track(void)
		{
			if (mName) delete mName;
		}

		void Add(Event* e)
		{
			mEventList.push_back(e);
		}

		void Name(const Byte* ptr, int len)
		{
			if (mName) delete mName;
			mName = new char[len+1];
			for (int i=0;i<len;i++) mName[i] = ptr[i];
			mName[len] = 0;
		}


		/* iterator to traverse the list of events */
		typedef std::list<Event*>::const_iterator EventIterator;

		EventIterator Begin(void) { return mEventList.begin(); }
		EventIterator End(void) { return mEventList.end(); }

		bool HasTempoEvents(void)
		{
			EventIterator it = Begin();
			
			while (it!=End())
			{
				const Event &ev = **it;
        if ( ev[0]==0xFF && ev[1]==0x51 ) return true;
				it++;
			}
			return false;
		}

	};

}

#endif

