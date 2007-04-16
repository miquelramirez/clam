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
#define CLAM_sf_read sf_readf_double
#define CLAM_sf_write sf_writef_double
#else
#define CLAM_sf_read sf_readf_float
#define CLAM_sf_write sf_writef_float
#endif

namespace CLAM
{

namespace AudioCodecs
{
	PCMAudioStream::PCMAudioStream()
		: mFileHandle( NULL )
	{
		mNativeFileParams.format = 0;
	}

	PCMAudioStream::PCMAudioStream( const AudioFile& file )
		: mFileHandle( NULL )
	{
		SetFOI( file );

	}

	PCMAudioStream::~PCMAudioStream()
	{
		if ( mFileHandle )
		{
			sf_close( mFileHandle );
		}
	}

	void PCMAudioStream::AudioFileToNative( const AudioFile& file)
	{
		mName = file.GetLocation();
		mNativeFileParams.channels = file.GetHeader().GetChannels();
		mNativeFileParams.samplerate = (int) file.GetHeader().GetSampleRate();
		mNativeFileParams.format = file.GetHeader().GetFormat() | file.GetHeader().GetEncoding() | file.GetHeader().GetEndianess();
	}

	void PCMAudioStream::SetFOI( const AudioFile& file )
	{
		Dispose();
		AudioFileToNative(file);
		SetChannels( mNativeFileParams.channels );
	}

	void PCMAudioStream::PrepareReading()
	{
		mFileHandle = sf_open( mName.c_str(), 
				       SFM_READ, 
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for reading!!!" );
		MarkAllChannelsAsConsumed();
		mEOFReached = false;
	}

	void PCMAudioStream::PrepareWriting()
	{
		
		mFileHandle = sf_open( mName.c_str(),
				       SFM_WRITE,
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for writing!!!" );

		MarkAllChannelsAsProduced();
	}

	void PCMAudioStream::PrepareReadWrite()
	{
		mFileHandle = sf_open( mName.c_str(),
				       SFM_RDWR,
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for reading/writing!!!" );

		MarkAllChannelsAsConsumed();
		MarkAllChannelsAsProduced();
		mEOFReached = false;
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
		int channelCount = mNativeFileParams.channels;			
		
		sf_count_t framesRead = CLAM_sf_read( mFileHandle, 
						       mInterleavedData.GetPtr(),
						       mFramesToRead );

		mFramesLastRead = (TSize)framesRead;
		
		if ( framesRead == 0 ) // No more data to read - EOF reached
		{
			mEOFReached = true;
			return;
		}
		
		if ( framesRead < mFramesToRead ) // EOF reached
		{
			// We set the remainder to zero

			const TData* end = mInterleavedData.GetPtr() + mInterleavedData.Size();

			for ( TData* i = mInterleavedData.GetPtr() + ( framesRead * channelCount );
			      i != end;
			      i++ )
				*i = 0.0;
			
			mEOFReached = true;
		}		
	}

	
	void PCMAudioStream::MemoryToDiskTransfer()
	{
		sf_count_t samplesWritten = CLAM_sf_write( mFileHandle,
							   mInterleavedDataOut.GetPtr(),
							   mFramesToWrite );

		CLAM_DEBUG_ASSERT( samplesWritten == mFramesToWrite,
			     "Could not write all samples to disk!" );
	}

}
}

