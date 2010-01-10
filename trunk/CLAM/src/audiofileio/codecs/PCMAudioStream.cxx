/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "PCMAudioStream.hxx"
#include "AudioFile.hxx"
#include "Assert.hxx"

#ifdef CLAM_DOUBLE
#define CLAM_sf_readf sf_readf_double
#define CLAM_sf_writef sf_writef_double
#else
#define CLAM_sf_readf sf_readf_float
#define CLAM_sf_writef sf_writef_float
#endif

namespace CLAM
{

namespace AudioCodecs
{
	PCMAudioStream::PCMAudioStream( const AudioFile& file )
		: mFileHandle( NULL )
	{
		mName = file.GetLocation();
		mNativeFileParams.channels = file.GetHeader().GetChannels();
		mNativeFileParams.samplerate = (int) file.GetHeader().GetSampleRate();
		mNativeFileParams.format = file.GetHeader().GetFormat() | file.GetHeader().GetEncoding() | file.GetHeader().GetEndianess();
		SetChannels( mNativeFileParams.channels );
	}

	PCMAudioStream::~PCMAudioStream()
	{
		Dispose();
	}

	void PCMAudioStream::PrepareReading()
	{
		mFileHandle = sf_open( mName.c_str(), 
				       SFM_READ, 
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			"Cannot open file for reading!!!" );
		mEOFReached = false;
		mFramePosition = 0;
	}

	void PCMAudioStream::PrepareWriting()
	{
		mFileHandle = sf_open( mName.c_str(),
				       SFM_WRITE,
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			"Cannot open file for writing!!!" );
	}

	void PCMAudioStream::Dispose()
	{
		if ( mFileHandle )
		{
			sf_close( mFileHandle );
			mFileHandle = NULL;
		}
	}

	void PCMAudioStream::DiskToMemoryTransfer()
	{
		int nChannels = mNativeFileParams.channels;
		unsigned nFrames = mInterleavedData.size()/nChannels;

		TData* begin = &mInterleavedData[0];
		const TData* end = begin + mInterleavedData.size();

		sf_count_t framesRead = CLAM_sf_readf( mFileHandle, begin, nFrames );
		mFramesLastRead = (TSize)framesRead;
		mFramePosition += mFramesLastRead;

		if (not framesRead) // No more data to read - EOF reached
		{
			mEOFReached = true;
			return;
		}
		if (framesRead<nFrames) // EOF reached
		{
			mEOFReached = true;
			for (TData * p=begin+framesRead*nChannels; p!=end; p++)
				*p= 0.0;
		}
	}

	void PCMAudioStream::MemoryToDiskTransfer()
	{
		unsigned nFrames = mInterleavedData.size()/mChannels;
		const TData* begin = &mInterleavedData[0];
		sf_count_t samplesWritten = CLAM_sf_writef( mFileHandle,
							   begin,
							   nFrames );

		CLAM_DEBUG_ASSERT( samplesWritten == nFrames,
			     "Could not write all samples to disk!" );
	}

	void PCMAudioStream::SeekTo(long unsigned framePosition)
	{
		mFramePosition = sf_seek(mFileHandle, framePosition, SEEK_SET);
		mEOFReached = false;
	}
}
}

