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
#ifndef __MIDIChunkType__
#define __MIDIChunkType__

#include "MIDIDataTypes.hxx"

namespace MIDI
{
	class ChunkType
	{
	public:
		Byte mBytes[4];
		ChunkType(Byte bytes[4])
		{
			for (int i=0;i<4;i++) { mBytes[i] = bytes[i]; }
		}
		ChunkType(const char* str)
		{
			for (int i=0;i<4;i++) { mBytes[i] = (Byte) str[i]; }
		}
		bool operator == (const char* str)
		{
			for (int i=0;i<4;i++) { if (mBytes[i] != (Byte) str[i]) return false; }
			return true;
		}
		bool operator != (const char* str)
		{
			return ! (*this==str);
		}
	};
	
}

#endif

