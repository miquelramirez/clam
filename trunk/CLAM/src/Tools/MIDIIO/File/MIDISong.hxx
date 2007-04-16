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
#ifndef __MIDISong__
#define __MIDISong__

namespace MIDI
{

	class Track;

	class Song
	/* a midi song is a collection of tracks */
	{
	private:
		class SongImpl* mImpl; /* hide implementation */
	public:
		Song(void);
		~Song(void);
		
		int Tracks(void) const;
		Track* GetTrack(int i) const;
		void AddTrack(Track* t);
		unsigned short GetTicksPerQ(void) const;
		void SetTicksPerQ(unsigned int v);
	};

}

#endif

