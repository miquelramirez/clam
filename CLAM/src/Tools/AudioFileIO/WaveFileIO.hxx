/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __WaveFileIO__
#define __WaveFileIO__

#include "SoundFileIO.hxx"

namespace CLAM{

/** @ingroup AudioFileIO */
class WaveFileIO : public SoundFileIO
{
public:
	~WaveFileIO()
	{
		Close();
		if (mCuePoints)
			delete mCuePoints;
	}
private:
	void InitSelf(void);
	void ReadHeader(void);
	void WriteHeader(void);

	struct ID {
		ID(const char* str = "\0\0\0\0")
		{
			memcpy(id,str,4);
		}
		char id[4];
	};

	struct ChunkHeader {
		ChunkHeader()
		{
			len = 0;
		}
		ChunkHeader(const ID& _id,int _len = 0):id(_id),len(_len)
		{ 
		}
		ID  id;
		int len;
	};

	struct CuePoint {
	  int    identifier;
	  int    position;
	  ID      fccChunk;
	  int    chunkStart;
	  int    blockStart;
	  int    offset;
	};
	
#pragma pack (2)

	struct WaveFmtChunk 
	{
		short formatTag;
		short channels;
		int   samplerate;
		int   bytesPerSec;
		short blockAlign;
		short sampleWidth;
	};

#pragma pack ()

	int ReadChunkHeader(ChunkHeader& h);
	int ReadID(ID& id);

	int WriteChunkHeader(const ChunkHeader& h);
	int WriteID(ID& id);

	bool CheckID(const ID& id,const ID& cmp);

public:
	CuePoint* mCuePoints; // read only for now
	int mNCuePoints; 
};

};//CLAM

#endif

