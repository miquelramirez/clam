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
#ifndef __MIDIReader__
#define __MIDIReader__

#include <stdio.h>
#include <CLAM/MIDIDataTypes.hxx>
#include "MIDIChunkType.hxx"

namespace MIDI
{
	class Song;

	class Reader
	/* class to read midi data from a file, parse it and store it in a song;
	*/
	{
	private:
		FILE* mFile;
		int mCnt;
	public:
		Reader(const char* filename)
		{
			mFile = fopen(filename,"rb");
		}
		bool Ok(void)
		{
			return mFile!=NULL;
		}
		Byte GetByte(void)
		{
			mCnt++;
			return fgetc(mFile);
		}
		unsigned int GetInt(void)
		{
			unsigned int val;

			val = GetByte();
			val <<=8;
			val |= GetByte();
			val <<=8;
			val |= GetByte();
			val <<=8;
			val |= GetByte();
			
			return val;
		}
		unsigned short GetShort(void)
		{
			unsigned short val;

			val = GetByte();
			val <<= 8;
			val |= GetByte();

			return val;
		}

		ChunkType GetChunkType(void)
		{
			Byte bytes[4];
			for (int i=0;i<4;i++) bytes[i] = GetByte();
			return ChunkType(bytes);
		}

		unsigned int GetVarLength(void)
		{
			/* read a variable lenght value. see midi file spec */
			unsigned int ret = 0;
			Byte tmp;

			tmp = GetByte();
			ret = tmp&0x7F;
			while (tmp&0x80)
			{
				tmp = GetByte();
				ret <<= 7;
				ret |= (tmp&0x7F);
			}
			return ret;
		}

		class Error
		{
		public:
			const char* mStr;
			Error(const char* str):mStr(str) { printf(str); }
		};

		void Read(Song& s);
	};

}

#endif

