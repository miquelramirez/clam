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

#include "OggVorbisAudioStream.hxx"
#include "AudioFile.hxx"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vorbis/codec.h>
#include <iostream>
#include <algorithm>

#if defined ( __powerpc__ ) || defined ( __POWERPC__ )
#define HOST_ENDIANESS 1
#else
#define HOST_ENDIANESS 0
#endif

namespace CLAM
{

namespace AudioCodecs
{
	const unsigned OggVorbisAudioStream::mMaxBlockSize = 4096 / sizeof(TInt16); // Seems to be the 'reference' value
	const unsigned OggVorbisAudioStream::mAnalysisWindowSize = 1024;
	
	OggVorbisAudioStream::OggVorbisAudioStream( const AudioFile& file )
		: mFileHandle( NULL )
		, mEncoding( false )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mChannels = (int)file.GetHeader().GetChannels();
		mEncodeBuffer.resize( mChannels ); // as many stream buffers as channels
		mBlockBuffer.resize( mMaxBlockSize );
	}

	OggVorbisAudioStream::~OggVorbisAudioStream()
	{
		Dispose();
	}

	void OggVorbisAudioStream::PrepareReading()
	{
		mFileHandle = fopen(mName.c_str(), "rb");
		if (mFileHandle == NULL)
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for reading!";
			CLAM_ASSERT( false, msgString.c_str() );
		}
		int error = ov_open(mFileHandle, &mNativeFileParams, NULL, 0);
		if ( error < 0 )
		{
			fclose( mFileHandle );
			mFileHandle=NULL;
			std::string msgString = mName;
			msgString += " is not a valid Ogg/Vorbis file!";
			CLAM_ASSERT( false, msgString.c_str() );
		}

		vorbis_info* info = ov_info(&mNativeFileParams, -1);
		CLAM_ASSERT(mChannels==unsigned(info->channels),
			"OggVorbisAudioStream: channels info changed before opening");

		mCurrentSection = 0;
		mFramePosition = 0;
		
		// MRJ: Seen on Audacity sources. It seems that
		// not all encoders respect the specs right: sometimes
		// one might stumble on a file with poorly encoded headers
		// having this the effect of reading several frames of zeros
		// at the beginning
		ov_pcm_seek( &mNativeFileParams, 0 );
	}

	void OggVorbisAudioStream::PrepareWriting()
	{
		mFileHandle = fopen(mName.c_str(), "wb");
		if ( mFileHandle==NULL )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for writing!";
			CLAM_ASSERT( false, msgString.c_str() );
		}
		VorbisI_EncoderSetup();
		mEncoding = true;
	}

	void OggVorbisAudioStream::VorbisI_EncoderSetup()
	{

		vorbis_info_init( &mStreamInfo );
		// encoding mode choosing
		int retValue = vorbis_encode_init_vbr(
			&mStreamInfo, mChannels, mEncodedSampleRate, 0.5 );

		CLAM_ASSERT( retValue == 0, "Error trying to initialize Vorbis encoder!" );

		// We add to the comment section who we are
		vorbis_comment_init( &mFileComments );
		vorbis_comment_add_tag( &mFileComments, "ENCODER", "CLAM" );

		// analysis state and auxiliary encoding state storage setup
		vorbis_analysis_init( &mDSPState, &mStreamInfo );
		vorbis_block_init( &mDSPState, &mVorbisBlock );
		
		// packet->stream encoder setup
		// pick random serial number
		// :TODO: this random number thing might be really important...
		ogg_stream_init( &mOggStreamState, rand() );

		WriteBitstreamHeader();
	}

	void OggVorbisAudioStream::WriteBitstreamHeader()
	{
		// Every Vorbis stream begins with 3 headers:
		//   + the initial header ( with codec setup params )
		//   + the header with the comment fields
		//   + the header with the code books
		
		ogg_packet header_codec_setup;
		ogg_packet header_comments;
		ogg_packet header_codebooks;

		// We make the headers from the current Vorbis DSP module state
		// and file comments
		vorbis_analysis_headerout( &mDSPState, &mFileComments, 
					   &header_codec_setup,
					   &header_comments,
					   &header_codebooks );

		// We 'push' each header one at a time into the stream
		ogg_stream_packetin( &mOggStreamState, &header_codec_setup );
		ogg_stream_packetin( &mOggStreamState, &header_comments );
		ogg_stream_packetin( &mOggStreamState, &header_codebooks );

		// Now we ensure that the audio data will begin on a new
		// 'page' as the specs require

		while( ogg_stream_flush( &mOggStreamState, &mOggPage ) > 0 )
		{
			fwrite( mOggPage.header, 1, mOggPage.header_len, mFileHandle );
			fwrite( mOggPage.body, 1, mOggPage.body_len, mFileHandle );
		}
	}

	void OggVorbisAudioStream::Dispose()
	{
		if ( mFileHandle == NULL) return;
		if ( not mEncoding )
		{
			ov_clear( &mNativeFileParams );
			mFileHandle = NULL;
			return;
		}
		// Encoding dispose is more complex
		// if there are yet samples to be processed we assure
		// they are encoded
		if ( !mEncodeBuffer[0].empty() )
			DoVorbisAnalysis();

		// We tell the Vorbis encoder that we are 
		// finished with encoding frames
		vorbis_analysis_wrote( &mDSPState, 0 );
		// push blocks generated by the last call
		// onto the Ogg stream
		PushAnalysisBlocksOntoOggStream(); 
		
		// Encoder cleaning up
		ogg_stream_clear( &mOggStreamState );
		vorbis_block_clear( &mVorbisBlock );
		vorbis_dsp_clear( &mDSPState );
		vorbis_comment_clear( &mFileComments );
		vorbis_info_clear( &mStreamInfo );

		fclose( mFileHandle );
		mFileHandle = NULL;
		mEncoding = false;
	}

	void OggVorbisAudioStream::ConsumeDecodedSamples()
	{
		unsigned nItems = mInterleavedData.size();
		TData* pSamples = &mInterleavedData[0];

		CLAM_ASSERT( mDecodeBuffer.size() >= nItems,
			     "This method cannot be called if the decode buffer"
			     " has less samples than requested by the upper level");

		static const TData norm = 1.0 / 32768.0;

		const TData* pSamplesEnd = pSamples + nItems;
		typedef std::deque<TInt16> sampleDeque;
		for( sampleDeque::iterator i = mDecodeBuffer.begin(); pSamples < pSamplesEnd; i++)
			*pSamples++ = TData(*i)*norm;

		mDecodeBuffer.erase(
			mDecodeBuffer.begin(),
			mDecodeBuffer.begin()+nItems );

		unsigned nFrames = nItems / mChannels;
		mFramePosition+=nFrames;
	}

	void OggVorbisAudioStream::DiskToMemoryTransfer()
	{
		//Unused variable: TSize nBytes = 0;
		unsigned samplesRead = 0;

		while (mDecodeBuffer.size() < mInterleavedData.size())
		{
			mLastBytesRead = ov_read(
				&mNativeFileParams,
				(char*)&mBlockBuffer[0], 
				mMaxBlockSize*sizeof(TInt16),
				HOST_ENDIANESS,
				2, 1, &mCurrentSection );
			
			CLAM_ASSERT( mLastBytesRead >= 0, "Malformed OggVorbis file!" );
			CLAM_ASSERT( mLastBytesRead % mChannels == 0, "BIG Whoops!" );

			if ( mLastBytesRead == 0 ) break;

			samplesRead = mLastBytesRead / sizeof(TInt16 );

			mDecodeBuffer.insert(
				mDecodeBuffer.end(),
				mBlockBuffer.begin(),
				mBlockBuffer.begin() + samplesRead);
		}

		mFramesLastRead = mDecodeBuffer.size();
		mEOFReached = ( mLastBytesRead == 0) && (mDecodeBuffer.empty());

		if (mDecodeBuffer.empty()) return;

		if (mDecodeBuffer.size() < mInterleavedData.size())
		{
			mDecodeBuffer.insert(
				mDecodeBuffer.end(),
				mInterleavedData.size() - mDecodeBuffer.size(),
				0);
		}
		ConsumeDecodedSamples();
	}

	void OggVorbisAudioStream::MemoryToDiskTransfer()
	{
		// Yahoo! The vorbis encoder wants the samples
		// as floats!
		
		// We expose the buffer for submitting data to the encoder

		unsigned currentOffset = 0;
		unsigned i;
		do
		{
			for ( i = mEncodeBuffer[0].size(); 
			      i < mAnalysisWindowSize && currentOffset < mInterleavedData.size(); 
			      i++ )
			{
				for (unsigned j=0; j<mChannels; j++)
					mEncodeBuffer[j].push_front( mInterleavedData[ currentOffset + j ] );

				currentOffset += mChannels;
			}

			if ( i == mAnalysisWindowSize ) // enough samples acquired
				DoVorbisAnalysis();

		} while ( currentOffset < mInterleavedData.size() );

	}

	void OggVorbisAudioStream::PushAnalysisBlocksOntoOggStream()
	{
		int eos = 0;
		while( vorbis_analysis_blockout( &mDSPState, &mVorbisBlock ) == 1 && !eos )
		{
			// we assume we want bitrate management
			vorbis_analysis( &mVorbisBlock, NULL );
			vorbis_bitrate_addblock( &mVorbisBlock );

			while( vorbis_bitrate_flushpacket( &mDSPState, &mOggPacket ) )
			{
				// We push the packet into the bitstream
				ogg_stream_packetin( &mOggStreamState, &mOggPacket );

				// page writeout
				while( ogg_stream_pageout( &mOggStreamState, &mOggPage ) > 0 
					&& !eos)
				{
					fwrite( mOggPage.header, 1, mOggPage.header_len, mFileHandle );
					fwrite( mOggPage.body, 1, mOggPage.body_len, mFileHandle );
					eos = ( ogg_page_eos( &mOggPage ) )? 1 : 0;
				}
			}
		}
	}

	void OggVorbisAudioStream::DoVorbisAnalysis()
	{
		float** encBuffer = vorbis_analysis_buffer(
			&mDSPState, mAnalysisWindowSize);
		
		for (unsigned j = 0; j < mChannels; j++ )
		{
			unsigned i = 0;
			while( !mEncodeBuffer[j].empty() )
			{
				encBuffer[j][i] = mEncodeBuffer[j].back();
				mEncodeBuffer[j].pop_back();				
				i++;
			}

			// Zero padding
			while( i < mAnalysisWindowSize )
			{
				encBuffer[j][i] = 0.0;
				i++;
			}
		}
		vorbis_analysis_wrote( &mDSPState, mAnalysisWindowSize );
		PushAnalysisBlocksOntoOggStream();
	}
	void OggVorbisAudioStream::SeekTo(unsigned long framePosition)
	{
		ov_pcm_seek( &mNativeFileParams, framePosition );
		mFramePosition = ov_pcm_tell(&mNativeFileParams);
	}
}	

}

