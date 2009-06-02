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

#include "AudioCodecs_Stream.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	Stream::Stream()
		: mFramesLastRead(0)
		, mFramePosition(0)
	{
	}

	Stream::~Stream()
	{
	}

	bool Stream::ReadData( int channel, TData* buffer, unsigned nFrames )
	{
		AdjustInterleavedBuffer( nFrames );
		DiskToMemoryTransfer();
		if ( mFramesLastRead == 0 ) return mEOFReached;

		// Actual data reading
		const TData* begin = &mInterleavedData[0];
		const TData* end = begin + mInterleavedData.size();
		for (const TData* i = begin + channel; i < end; i+=mChannels, buffer++ )
			*buffer = *i;
		
		return mEOFReached;
		       
	}
	bool Stream::ReadData(TData** buffers, unsigned nFrames )
	{
		AdjustInterleavedBuffer( nFrames );
		DiskToMemoryTransfer();
		if ( mFramesLastRead == 0 ) return mEOFReached;

		const TData * interleaved = &mInterleavedData[0];
		for (unsigned iFrame=0; iFrame<nFrames; iFrame++)
			for (unsigned iChannel=0; iChannel<mChannels; iChannel++)
				buffers[iChannel][iFrame] = *interleaved++;

		return mEOFReached;
	}

	bool Stream::ReadData( int* channels, int nchannels, TData** buffers, unsigned nFrames )
	{
		AdjustInterleavedBuffer( nFrames );
		DiskToMemoryTransfer();
		if ( mFramesLastRead == 0 ) return mEOFReached;

		// Actual data reading
		const TData* begin = &mInterleavedData[0];
		const TData* end = begin + mInterleavedData.size();
		const int* endChannels = channels + nchannels;
		
		for( int* currentChannel = channels;
			 currentChannel != endChannels;
			 currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			TData* pSamples = buffers[channelIndex];
			for ( const TData* i = begin + channelIndex; i<end; i+=mChannels)
			{
				*pSamples++ = *i;
			}
		}
		return mEOFReached;
	}

	void Stream::AdjustInterleavedBuffer( unsigned nFrames )
	{
		unsigned newSize = nFrames * mChannels;
		if (newSize == mInterleavedData.size()) return;
		mInterleavedData.resize( nFrames * mChannels );
	}

	void Stream::WriteData( int channel, const TData* buffer, unsigned nFrames )
	{
		AdjustInterleavedBuffer( nFrames );
		TData* beginData = &mInterleavedData[0];
		TData* endData = beginData + mInterleavedData.size();
		for (TData* data = beginData+channel; data < endData; data += mChannels)
			*data = *buffer++;

		MemoryToDiskTransfer();
	}

	void Stream::WriteData(TData ** const buffers, unsigned nFrames)
	{
		AdjustInterleavedBuffer( nFrames );

		TData * interleaved = &mInterleavedData[0];
		for (unsigned iFrame=0; iFrame<nFrames; iFrame++)
			for (unsigned iChannel=0; iChannel<mChannels; iChannel++)
				*interleaved++ = buffers[iChannel][iFrame];

		MemoryToDiskTransfer();
	}

	void Stream::WriteData( int* channels, int nchannels,
				TData** const samples, unsigned nFrames )
	{
		AdjustInterleavedBuffer( nFrames );

		TData* begin = &mInterleavedData[0];
		TData* end = begin + mInterleavedData.size();
		const int* endChannels = channels + nchannels;

		for( int* currentChannel = channels;
		     currentChannel != endChannels;
		     currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			const TData* pSamples = *(samples + channelIndex);
			for ( TData* i = begin + channelIndex; i<end; i+=mChannels, pSamples++ )
			{
				*i = *pSamples; 
			}
		}

		MemoryToDiskTransfer();
	}

	void Stream::SetChannels( unsigned nChannels )
	{
		mChannels = nChannels;
	}

}

}

