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

#ifndef __SoundFileIO__
#define __SoundFileIO__

#include "SoundHeader.hxx"
#include "ErrSoundFileIO.hxx"
#include <stdio.h>
#include <string.h>

#ifdef linux
#include <byteswap.h>
#endif

#include "ByteOrder.hxx"

#ifdef CLAM_BIG_ENDIAN
#define SOUNDFILEIO_BIG_ENDIAN
#else
#define SOUNDFILEIO_LITTLE_ENDIAN
#endif


namespace CLAM{

/** @ingroup AudioFileIO */
class SoundFileIO
{
public:
	enum EMode{
		eNone = 0,
		eRead = 1,
		eWrite = 2,
		eDuplex = 3
	};

	SoundFileIO();
	virtual ~SoundFileIO();
	void Init(void);

	void Open(const char* filename,EMode mode) throw ( ErrSoundFileIO );
	void Create(const char* filename,EMode mode,const SoundHeader& header);
	void Close(void);

	void Seek(int pos);
	int Tell(void);
	void SeekFrame(int frame);
	int TellFrame(void);
	virtual int Read(float *data,int& size);
	virtual int Write(const short *data,int size);
	virtual int Read(int *data,int size);
	virtual int Write(const int *data,int size);

	int Size(void) { return mSize; }
	int SizeFrame(void) { return mSize/mHeader.mChannels; }
	int SampleRate(void) { return mHeader.mSamplerate; }
	
	const SoundHeader& Header(void) const { return mHeader; }
protected:
	EMode mMode;
	SoundHeader mHeader;
	FILE* mFile;
	int   mOffset;
	int   mSize;
	int		mPos;
	bool	mSwap;
	bool	mStdIO;

	virtual void ReadHeader(void) {};
	virtual void WriteHeader(void) {};
	virtual void InitSelf(void) {}

	void Swap(unsigned short& val)
	{
	#ifdef linux
		val = bswap_16(val);
	#else
		unsigned char* ptr=(unsigned char*) &val;
		static unsigned char tmp;
		tmp=ptr[0]; ptr[0]=ptr[1]; ptr[1]=tmp;
	#endif
	}

	void Swap(short& val)
	{
		Swap( (unsigned short&) val);
	}

	void Swap(unsigned int& val)
	{
	#ifdef linux
		val = bswap_32(val);
	#else
		unsigned char* ptr=(unsigned char*) &val;
		static unsigned char tmp;
		tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
	#endif
	}

	void Swap( float& val )
	{
		unsigned char* ptr=(unsigned char*) &val;
		static unsigned char tmp;
		tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
		tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;

	}

	void Swap(int& val)
	{
		Swap( (unsigned int&) val);
	}
};

};//CLAM
#endif

