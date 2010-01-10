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
		_mp3Frame = 0;
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
			unsigned long filePos = mBitstream.CurrentFrameFileOffset();
			if (_mp3Frame>=_seekCache.size())
				_seekCache.push_back(filePos);
			else
			{
/*
				std::cout
					<< "Mp3 frame "
					<< mFramePosition << " "
					<< _mp3Frame << " R:"
					<< filePos << " E:"
					<< _seekCache[_mp3Frame] << " R-E:"
					<< int(filePos)-int(_seekCache[_mp3Frame])
					<< std::endl;
*/
				CLAM_WARNING(filePos==_seekCache[_mp3Frame],
					"MP3 indexing not matching");
			}
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
			_mp3Frame++;
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
			unsigned currOffset = 0;
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

	void MpegAudioStream::SeekTo(unsigned long framePosition)
	{
		// Warning: two 'frames' here, mp3's and AudioStream's (items*channels)
		if (framePosition==mFramePosition) return;
		unsigned mp3FrameSize = 32*MAD_NSBSAMPLES(&mBitstream.CurrentFrame().header);
		unsigned targetMp3Frame = framePosition/mp3FrameSize;
		std::cout << "targetMp3Frame: " << targetMp3Frame << std::endl;
		// TODO: Jumps beyond the last played frame are too expensive to be frame accurate
		unsigned maxForwarJump = _seekCache.size()+100;
		if (targetMp3Frame>maxForwarJump) targetMp3Frame=maxForwarJump;
		if (targetMp3Frame>=_seekCache.size())
		{
			// Construct the index beyond current limits, til the target frame
			fseek(mpHandle, _seekCache[_seekCache.size()-2], SEEK_SET);
			mBitstream.Init();
			for (unsigned mp3Frame=_seekCache.size();
				mp3Frame<=targetMp3Frame; mp3Frame++)
			{
				if (not mBitstream.NextFrame()) return;
				unsigned long filePos = mBitstream.CurrentFrameFileOffset();
/*
				std::cout << "fwd "
					<< _seekCache.back() << " "
					<< filePos << " " 
					<< int(filePos)-_seekCache.back() << std::endl;
*/
				if (filePos<=_seekCache.back()) continue; // filtering already done
				_seekCache.push_back(filePos);
			}
		}
		unsigned skip = targetMp3Frame>4?3:0;
		unsigned jumpingFrame = targetMp3Frame-skip;
		std::cout << "jumpingFrame:" << jumpingFrame << std::endl;
		fseek(mpHandle, _seekCache[jumpingFrame], SEEK_SET);
		mBitstream.Init();
		_mp3Frame = targetMp3Frame;
		// TODO: Because the already buffered bins this is not true
		mFramePosition = mp3FrameSize * _mp3Frame;
		if (targetMp3Frame == 0) return;
		unsigned long previousFrameFilePos = _seekCache[targetMp3Frame-1];
		while (true)
		{
			mBitstream.NextFrame();
			unsigned long filePos = mBitstream.CurrentFrameFileOffset();
/*
			std::cout << "seek "
				<< filePos << " "
				<< previousFrameFilePos << std::endl;
*/
			if (filePos >= previousFrameFilePos) break;
		}
		mBitstream.SynthesizeCurrent();

		unsigned long filePos = mBitstream.CurrentFrameFileOffset();
/*
		std::cout << "Jump n target: " 
			<< _mp3Frame << " "
			<< targetMp3Frame << " "
			<< filePos << " "
			<< _seekCache[targetMp3Frame-1]
			<< std::endl;
*/
		// TODO: Sample accurate seek (now it is frame accurate)
/*
		CLAM_ASSERT(_seekCache[targetMp3Frame-1]==filePos,
			"File postions don't match");
		CLAM_ASSERT(_mp3Frame==targetMp3Frame, "Seek mp3 frames don't match");
		// We should synthesize the previous frame to the one we want.
*/	}
}
	
}

