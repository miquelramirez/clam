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

#include "WaveFileIO.hxx"
#include "ErrSoundFileIO.hxx"

#ifdef SOUNDFILEIO_BIG_ENDIAN
#define SWAP(var) Swap(var)
#else
#define SWAP(var)
#endif

using namespace CLAM;

void WaveFileIO::InitSelf(void)
{
#ifdef SOUNDFILEIO_BIG_ENDIAN
	mSwap = true;
#endif
	mCuePoints = 0;
	mNCuePoints = 0;
}

int WaveFileIO::ReadChunkHeader(ChunkHeader& h)
{
	int ret = int( fread(&h,1,sizeof(h),mFile) );
	if (ret>0) {
		SWAP(h.len);
	}
	return ret;
}

int WaveFileIO::WriteChunkHeader(const ChunkHeader& h)
{
	ChunkHeader cp = h;
	SWAP(cp.len);
	return int( fwrite(&cp,1,sizeof(cp),mFile) );
}

int WaveFileIO::ReadID(ID& id)
{
	return int( fread(&id,1,sizeof(id),mFile) );
}

int WaveFileIO::WriteID(ID& id)
{
	return int( fwrite(&id,1,sizeof(id),mFile) );
}

bool WaveFileIO::CheckID(const ID& id,const ID& cmp)
{
	return !memcmp(&id,&cmp,4);
}

void WaveFileIO::ReadHeader(void)
{
	int i = 0;
	bool fmtFound = false;
	fseek(mFile,0,SEEK_SET);
	ID waveID;
	ChunkHeader riff;
	ReadChunkHeader(riff);
	if (!CheckID(riff.id,"RIFF"))
	{
		UnsupportedSoundFileFormat error( "Given file has not a valid RIFF header \n" );

		throw error;
	}
	i = 0;
	int r = ReadID(waveID);
	i += r;
	
	if (!CheckID(waveID,"WAVE"))
	{
		UnsupportedSoundFileFormat error( "Given file has not a valid WAVE header \n" );

		throw error;
	}

	ChunkHeader h;	
	
	// allowing to read beyond riff len
	while ((r = ReadChunkHeader(h)) > 0) {
		if (i>=riff.len) 
		{
			// WARNING: Reading beyond RIFF chunk. gnoise locates cue-points
			// here. Check with standatd if this is correct
		}
		i += r;
		if (CheckID(h.id,"fmt ")) {
			WaveFmtChunk fmt;
			int j = 0;
			j = int( fread(&fmt,1,sizeof(fmt),mFile) );
			i += j;

			SWAP(fmt.formatTag);
			SWAP(fmt.channels);
			SWAP(fmt.samplerate);
			SWAP(fmt.sampleWidth);

			mHeader.mChannels = fmt.channels;
			mHeader.mSamplerate = fmt.samplerate;
			mHeader.mSampleWidth = fmt.sampleWidth;
			mHeader.mBytesPerSample = (mHeader.mSampleWidth+7)>>3;
			mHeader.mFormatTag = fmt.formatTag;

			// MRJ: We acknowledge here WAVE IEEE Float sample format
			if ( fmt.formatTag == 0x0003 )
			{
				UnsupportedSoundFileSampleEncoding error( "IEEE 32-bit WAV encoding not supported" );

				throw error;
			}
			if (fmt.formatTag!=0x0001 )
			{
				UnsupportedSoundFileSampleEncoding error( "Compressed WAVE modes are not supported!" );

				throw error;
			}
			mHeader.mChannels = fmt.channels;
			mHeader.mSamplerate = fmt.samplerate;
			fmtFound = true;
			
			while (j<h.len)
			{
				char dum[256];
				int n = (h.len-j);
				if (n>256) n = 256;
				j += int( fread(&dum,1,n,mFile) );
			}
		}
		else
		{
		  if (CheckID(h.id,"data")) {
			  mSize += h.len/mHeader.mBytesPerSample;
			  mOffset = sizeof(riff)+i;
		  }
		  if (CheckID(h.id,"cue "))
			{
				if (mCuePoints)
				{
					delete mCuePoints;
				}
				mNCuePoints = (h.len-4)/24;
				int tmp;
				fread(&tmp,1,sizeof(int),mFile);
				SWAP(tmp);
				if (tmp!=mNCuePoints)
				{
					UnsupportedSoundFileFormat error( "Given file is not consistent: \n"
									  "'cue ' chunk len does not match with number of cue points");
					throw error;
				}
				mCuePoints = new CuePoint[mNCuePoints];
				for (int i=0;i<mNCuePoints;i++)
				{
					fread(&mCuePoints[i],1,sizeof(CuePoint),mFile);
					SWAP(mCuePoints[i].identifier);
					SWAP(mCuePoints[i].position);
					SWAP(mCuePoints[i].chunkStart);
					SWAP(mCuePoints[i].blockStart);
					SWAP(mCuePoints[i].offset);
				}
		  }
			i += h.len;
			fseek(mFile, sizeof(riff)+i, SEEK_SET);
		}
	}
	if (!fmtFound)
	{
		UnsupportedSoundFileFormat error( "Given sound file is not consistent: " 
						  "no format 'chunk' was found \n" );
		
		throw error;
	}
	if (mOffset == 0) 
	{
		UnsupportedSoundFileFormat error( "Given sound file is not consistent: "
						  "it does not contain any data \n");

		throw error;
	}
}

void WaveFileIO::WriteHeader(void)
{
	fseek(mFile,0,SEEK_SET);
	mOffset = 0;
	ChunkHeader riffHeader("RIFF");
	ChunkHeader fmtHeader("fmt ",sizeof(WaveFmtChunk));
	ChunkHeader dataHeader("data",mSize*2);

	if (mStdIO && mSize==0)
	{	
		dataHeader.len = 0x7FFFFFFF;
	}
	
	WaveFmtChunk fmtChunk;
	ID waveID("WAVE");

	riffHeader.len = 
		sizeof(waveID) + 
		sizeof(fmtHeader) + fmtHeader.len + 
		sizeof(dataHeader) + dataHeader.len;

	fmtChunk.formatTag = 0x0001;
	fmtChunk.channels = mHeader.mChannels;
	fmtChunk.samplerate = mHeader.mSamplerate;
	fmtChunk.bytesPerSec = 
		mHeader.mSamplerate*mHeader.mBytesPerSample*mHeader.mChannels;
	fmtChunk.blockAlign = mHeader.mBytesPerSample;
	fmtChunk.sampleWidth = mHeader.mSampleWidth;
	
	mOffset += WriteChunkHeader(riffHeader);
	mOffset += WriteID(waveID);
	mOffset += WriteChunkHeader(fmtHeader);

	SWAP(fmtChunk.formatTag);
	SWAP(fmtChunk.channels);
	SWAP(fmtChunk.samplerate);
	SWAP(fmtChunk.bytesPerSec);
	SWAP(fmtChunk.blockAlign);
	SWAP(fmtChunk.sampleWidth);
	
	mOffset += int( fwrite(&fmtChunk,1,sizeof(fmtChunk),mFile) );

	mOffset += WriteChunkHeader(dataHeader);
}

