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

#ifndef __AIFFFileIO__
#define __AIFFFileIO__

#include "SoundFileIO.hxx"

namespace CLAM{

/** @ingroup AudioFileIO */
class AIFFFileIO:public SoundFileIO
{
public:
	~AIFFFileIO()
	{
		Close();
	}
private:
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
		unsigned int len;
	};
	struct extended{
		unsigned char bytes[10];
	};

#pragma pack (2)

	struct AIFFCommChunk 
	{
		short			channels;
		unsigned int numSampleFrames;
		short			sampleWidth;
		extended		samplerate;
	};

#pragma pack ()

	int ReadChunkHeader(ChunkHeader& h);
	int ReadID(ID& id);

	int WriteChunkHeader(const ChunkHeader& h);
	int WriteID(ID& id);

	bool CheckID(const ID& id,const ID& cmp);

	void InitSelf(void);
};

};//CLAM
#endif

