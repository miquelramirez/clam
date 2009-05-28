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

#include "MpegAudioStream.hxx"
#include "AudioFile.hxx"
#include "Assert.hxx"
#include <iostream>

namespace CLAM
{

namespace AudioCodecs
{
	// A reasonable multiple of four
	const TSize MpegAudioStream::mMaxDecodedBlockSize = 8192;

	MpegAudioStream::MpegAudioStream()
		: mpHandle( NULL )
	{
	}

	MpegAudioStream::MpegAudioStream( const AudioFile& file )
		: mpHandle( NULL )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mEncodedChannels = (int)file.GetHeader().GetChannels();
		mDecodeBuffer.resize( mEncodedChannels );
	}

	MpegAudioStream::~MpegAudioStream()
	{
		
		if ( not mpHandle ) return;
		fclose(mpHandle);
	}
	
	void MpegAudioStream::PrepareReading()
	{
		mpHandle = fopen( mName.c_str(), "rb");

		if ( !mpHandle )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString += " for reading!";

			CLAM_ASSERT( false, msgString.c_str() );
			
		}

		mBitstream.Init( mpHandle );

		SetChannels( mEncodedChannels );
		MarkAllChannelsAsConsumed();

		mSamplesDecoded = 0;
		mSamplesTransferred = 0;
	}

	void MpegAudioStream::PrepareWriting()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}

	void MpegAudioStream::PrepareReadWrite()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}

	void MpegAudioStream::Dispose()
	{
		mBitstream.Finish();
	}

	void MpegAudioStream::DiskToMemoryTransfer()
	{
		unsigned samplesToRead = mInterleavedData.size()/mEncodedChannels;

		while( mDecodeBuffer[0].size() < samplesToRead
		       && mBitstream.NextFrame() )
		{
			mBitstream.SynthesizeCurrent();
			
			CLAM_ASSERT( mEncodedChannels == MAD_NCHANNELS( &mBitstream.CurrentFrame().header ),
				     "This frame hasn't mEncodedChannels channels!" );
			
			CLAM_ASSERT( mEncodedChannels == mBitstream.CurrentSynthesis().pcm.channels,
				     "Synthesis result does not have the expected number of channels" );
			
			TSize samplesDecodedThisTime = mBitstream.CurrentSynthesis().pcm.length;
			
			for( int i = 0; i < mEncodedChannels; i++ )
			{
				mad_fixed_t* channelData = mBitstream.CurrentSynthesis().pcm.samples[i];
				
				mDecodeBuffer[i].insert( mDecodeBuffer[i].end(),
							 channelData,
							 channelData + samplesDecodedThisTime );
			}
			
			mSamplesDecoded += mBitstream.CurrentSynthesis().pcm.length;

		}
		
		mFramesLastRead = mDecodeBuffer[0].size();

		if ( !mDecodeBuffer[0].empty() )
		{

			for ( int i = 0; i < mEncodedChannels; i++ )
				if ( mDecodeBuffer[i].size() < samplesToRead )
				{
					mDecodeBuffer[i].insert( mDecodeBuffer[i].end(),
								 samplesToRead - mDecodeBuffer[i].size(),
								 mad_fixed_t(0) );
					
				}
			
			ConsumeDecodedSamples();
		}

		mEOFReached = mBitstream.EOS() && mDecodeBuffer[0].empty();
	}

	void MpegAudioStream::ConsumeDecodedSamples()
	{
		TSize samplesToRead = mInterleavedData.size()/mEncodedChannels;

		for ( int i = 0; i < mEncodedChannels; i++ )
		{
			TIndex currOffset = 0;

			for ( std::deque<mad_fixed_t>::iterator j = mDecodeBuffer[i].begin();
			      currOffset < mInterleavedData.size(); 
			      j++, currOffset+=mEncodedChannels )
			{
				double sampleValue = mad_f_todouble(*j);

				// :TODO: Finding a nicer way to clamp things
				// to the -1,1 could be necessary
				// clipping
				if ( sampleValue > 1.0 )
					sampleValue = 1.0;
				else if ( sampleValue < -1.0 )
					sampleValue = -1.0;
					
				
				mInterleavedData[ currOffset + i ] = TData(sampleValue);
			}
			
			mDecodeBuffer[i].erase( mDecodeBuffer[i].begin(),
						mDecodeBuffer[i].begin() + samplesToRead );
		}

		mSamplesTransferred += samplesToRead;
	}

	void MpegAudioStream::MemoryToDiskTransfer()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}
}
	
}

