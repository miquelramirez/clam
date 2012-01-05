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
#ifndef __MIDITempo__
#define __MIDITempo__

namespace MIDI
{

	class Song;
	class Track;

	class Tempo
	/* class to convert midi ticks to time in milliseconds, 
	** according to the tempo information in a certain track 
	** (typically the first) of a midi song
	*/
	{
	private:
		class TempoImpl* mImpl; // hide implementation
	public:
		Tempo(Song* song = 0,Track* track = 0);

		~Tempo();

		void Init(Song* song = 0,Track* track = 0);

		Milliseconds TicksToTime(Ticks t);
	};

}

#endif

