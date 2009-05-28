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
		: mStrictStreaming( true )
		, mFramesLastRead( 0 )
	{
	}

	Stream::~Stream()
	{
	}

	bool Stream::AllChannelsProduced()
	{
		for (unsigned i=0; i<mChannelsProduced.size(); i++)
			if (not mChannelsProduced[i]) return false;
		return true;
	}

	void Stream::MarkAllChannelsAsProduced()
	{
		for (unsigned i=0; i<mChannelsProduced.size(); i++)
			mChannelsProduced[i]=true;
	}

	void Stream::ResetProducedChannels()
	{
		for (unsigned i=0; i<mChannelsProduced.size(); i++)
			mChannelsProduced[i]=false;
	}

	bool Stream::AllChannelsConsumed()
	{
		for (unsigned i=0; i<mChannelsConsumed.size(); i++)
			if (not mChannelsConsumed[i]) return false;
		return true;
	}

	void Stream::MarkAllChannelsAsConsumed()
	{
		for (unsigned i=0; i<mChannelsConsumed.size(); i++)
			mChannelsConsumed[i]=true;
	}

	void Stream::ResetConsumedChannels()
	{
		for (unsigned i=0; i<mChannelsConsumed.size(); i++)
			mChannelsConsumed[i]=false;
	}

	bool Stream::HandleReAllocation( std::vector<TData>& buffer, TSize newSize )
	{
		bool increasing = newSize > buffer.size();
		buffer.resize( newSize );
		return increasing;
	}

	void Stream::CheckForFileReading( TSize howmany )
	{
		if ( StrictStreaming() and not AllChannelsConsumed()) return;
		ResetConsumedChannels();
		
		if ( HandleReAllocation( mInterleavedData, howmany*mChannels ) )
			mFramesToRead = howmany;

		// Acquire samples from file 

		DiskToMemoryTransfer();
	}

	bool Stream::ReadData( int channel, TData* ptr, TSize howmany )
	{
		CheckForFileReading( howmany );

		if ( mFramesLastRead == 0 ) return mEOFReached;

		// Actual data reading
		int channelCount = mChannels;
		
		const TData* begin = &mInterleavedData[0];
		const TData* end = begin + mInterleavedData.size();
		const int stride = channelCount;
		
		for (const TData* i = begin + channel; i < end; i+=stride, ptr++ )
			*ptr = *i;
		
		mChannelsConsumed[ channel ] = true;
		
		return mEOFReached;
		       
	}

	bool Stream::ReadData( int* channels, int nchannels,
			       TData** samples, TSize howmany )
	{
		CheckForFileReading( howmany );

		if ( mFramesLastRead == 0 ) return mEOFReached;

		// Actual data reading
		int channelCount = mChannels;
		
		const TData* begin = &mInterleavedData[0];
		const TData* end = begin + mInterleavedData.size();
		//Unused variable TData** const samplesEnd = samples + nchannels;
		const int* endChannels = channels + nchannels;
		std::vector<bool>::iterator cIt = mChannelsConsumed.begin();
		
		for( int* currentChannel = channels;
			 currentChannel != endChannels;
			 currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			// mark channel as consumed
			*(cIt + channelIndex ) = true;
			const int stride = channelCount;
			TData* pSamples = *(samples + channelIndex);
			
			for ( const TData* i = begin + channelIndex; i<end; i+=stride, pSamples++ )
			{
				*pSamples = *i;
			}
			
		}

		return mEOFReached;
		
	}

	void Stream::PrepareFileWriting( TSize howmany )
	{
		if ( not AllChannelsProduced() ) return;
		ResetProducedChannels();
			
		if ( HandleReAllocation( mInterleavedDataOut, howmany * mChannels ) )
			mFramesToWrite = howmany;
	}

	void Stream::WriteData( int channel, const TData* ptr, TSize howmany )
	{
		PrepareFileWriting( howmany );
		int channelCount = mChannels;
		TData* beginData = &mInterleavedDataOut[0];
		TData* endData = beginData+mInterleavedDataOut.size();
		const int stride = channelCount;

		for (TData* data = beginData + channel;
		      data < endData;
		      data += stride, ptr++ )
			*data = *ptr;

		mChannelsProduced[channel] = true;

		if ( AllChannelsProduced() )
			MemoryToDiskTransfer();
	}

	void Stream::WriteData( int* channels, int nchannels,
				TData** const samples, TSize howmany )
	{
		PrepareFileWriting( howmany );
	
		int channelCount = mChannels;

		TData* begin = &mInterleavedDataOut[0];
		TData* end = begin + mInterleavedDataOut.size();
		const int* endChannels = channels + nchannels;
		//Unused variable TData** const samplesEnd = samples + nchannels;
		std::vector<bool>::iterator cIt = mChannelsProduced.begin();

		for( int* currentChannel = channels;
		     currentChannel != endChannels;
		     currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			// mark channel as consumed
			*(cIt + channelIndex ) = true;
			const int stride = channelCount;
			const TData* pSamples = *(samples + channelIndex);

			for ( TData* i = begin + channelIndex;
			      i<end;
			      i+=stride, pSamples++ )
			{
				*i = *pSamples; 
			}

		}

		if ( AllChannelsProduced() )
			MemoryToDiskTransfer();
	}

	void Stream::SetChannels( TSize nChannels )
	{
		mChannels = nChannels;
		mChannelsConsumed.resize( nChannels );
		mChannelsProduced.resize( nChannels );
	}

}

}

