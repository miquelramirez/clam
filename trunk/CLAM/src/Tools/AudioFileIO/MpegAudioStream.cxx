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

	MpegAudioStream::MpegAudioStream( const AudioFile& file )
		: mpHandle( NULL )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mChannels = (int)file.GetHeader().GetChannels();
		mDecodeBuffer.resize( mChannels );
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

		mSamplesDecoded = 0;
		mFramePosition = 0;
	}

	void MpegAudioStream::PrepareWriting()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}

	void MpegAudioStream::Dispose()
	{
		mBitstream.Finish();
	}

	void MpegAudioStream::DiskToMemoryTransfer()
	{
		unsigned nFrames = mInterleavedData.size()/mChannels;
		while( mDecodeBuffer[0].size() < nFrames
		       && mBitstream.NextFrame() )
		{
			mBitstream.SynthesizeCurrent();
			
			CLAM_ASSERT( mChannels == MAD_NCHANNELS( &mBitstream.CurrentFrame().header ),
				     "MpegAudioStream: A frame had not the expected channels." );
			CLAM_ASSERT( mChannels == mBitstream.CurrentSynthesis().pcm.channels,
				     "MpegAudioStream: Synthesis result had not the expected number of channels" );
			
			TSize samplesDecodedThisTime = mBitstream.CurrentSynthesis().pcm.length;
			for(unsigned i = 0; i < mChannels; i++ )
			{
				mad_fixed_t* channelData = mBitstream.CurrentSynthesis().pcm.samples[i];
				mDecodeBuffer[i].insert( mDecodeBuffer[i].end(),
							 channelData,
							 channelData + samplesDecodedThisTime );
			}
			mSamplesDecoded += samplesDecodedThisTime;
		}
		
		mFramesLastRead = mDecodeBuffer[0].size();
		mEOFReached = mBitstream.EOS() && mDecodeBuffer[0].empty();

		if (mDecodeBuffer[0].empty()) return;

		for (unsigned i = 0; i < mChannels; i++ )
		{
			if ( mDecodeBuffer[i].size() < nFrames )
			{
				mDecodeBuffer[i].insert(
					mDecodeBuffer[i].end(),
					nFrames - mDecodeBuffer[i].size(),
					mad_fixed_t(0) );
			}
		}
		ConsumeDecodedSamples();
	}

	void MpegAudioStream::ConsumeDecodedSamples()
	{
		unsigned nFrames = mInterleavedData.size()/mChannels;
		for (unsigned i = 0; i < mChannels; i++ )
		{
			TIndex currOffset = 0;
			for ( std::deque<mad_fixed_t>::iterator j = mDecodeBuffer[i].begin();
			      currOffset < mInterleavedData.size(); 
			      j++, currOffset+=mChannels )
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
				mDecodeBuffer[i].begin() + nFrames );
		}

		mFramePosition += nFrames;
	}

	void MpegAudioStream::MemoryToDiskTransfer()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}
}
	
}

