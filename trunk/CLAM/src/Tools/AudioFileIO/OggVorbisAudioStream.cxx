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
	const TSize OggVorbisAudioStream::mMaxBlockSize = 4096 / sizeof(TInt16); // Seems to be the 'reference' value
	const TSize OggVorbisAudioStream::mAnalysisWindowSize = 1024;
	
	OggVorbisAudioStream::OggVorbisAudioStream()
		: mFileHandle( NULL ), mValidFileParams( false ), mEncoding( false )
	{
		mBlockBuffer.Resize( mMaxBlockSize );
		mBlockBuffer.SetSize( mMaxBlockSize );

	}

	OggVorbisAudioStream::OggVorbisAudioStream( const AudioFile& file )
		: mFileHandle( NULL ), mValidFileParams( false ), mEncoding( false )
	{
		SetFOI( file );
		mBlockBuffer.Resize( mMaxBlockSize );
		mBlockBuffer.SetSize( mMaxBlockSize );

	}

	OggVorbisAudioStream::~OggVorbisAudioStream()
	{
		if ( mValidFileParams )
			Dispose();
	}

	void OggVorbisAudioStream::SetFOI( const AudioFile& file )
	{
		if ( mValidFileParams )
			Dispose();
		AudioFileToNative( file );
		
	}

	void OggVorbisAudioStream::AudioFileToNative( const AudioFile& file )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mEncodedChannels = (int)file.GetHeader().GetChannels();
		
		mEncodeBuffer.resize( mEncodedChannels ); // as many stream buffers as channels

	}


	void OggVorbisAudioStream::PrepareReading()
	{
		if ( ( mFileHandle = fopen( mName.c_str(), "rb" ) ) == NULL )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for reading!";
			CLAM_ASSERT( false, msgString.c_str() );
		}

		if ( ov_open( mFileHandle, &mNativeFileParams, NULL, 0 ) < 0 )		
		{
			fclose( mFileHandle );			
			std::string msgString = mName;
			msgString += " is not a valid Ogg/Vorbis file!";
			
			CLAM_ASSERT( false, msgString.c_str() );
		}

		vorbis_info* info = ov_info( &mNativeFileParams, -1 );
		
		SetChannels( info->channels );
		MarkAllChannelsAsConsumed();

		mValidFileParams = true;
		mCurrentSection = 0;
		
		// MRJ: Seen on Audacity sources. It seems that
		// not all encoders respect the specs right: sometimes
		// one might stumble on a file with poorly encoded headers
		// having this the effect of reading several frames of zeros
		// at the beginning
		ov_pcm_seek( &mNativeFileParams, 0 );		
	}

	void OggVorbisAudioStream::PrepareWriting()
	{
		if ( ( mFileHandle = fopen( mName.c_str(), "wb" ) ) == NULL )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for writing!";
			CLAM_ASSERT( false, msgString.c_str() );
		}
		
		VorbisI_EncoderSetup();
		MarkAllChannelsAsProduced();
		mEncoding = true;

	}

	void OggVorbisAudioStream::VorbisI_EncoderSetup()
	{

		vorbis_info_init( &mStreamInfo );

		// encoding mode choosing

		int retValue = vorbis_encode_init_vbr( &mStreamInfo, 
						       mEncodedChannels,
						       mEncodedSampleRate,
						       0.5 );

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

		SetChannels( mEncodedChannels );
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

	void OggVorbisAudioStream::PrepareReadWrite()
	{
		CLAM_ASSERT( false, "Cannot be done!" );
	}

	void OggVorbisAudioStream::Dispose()
	{
		if ( !mEncoding )
		{
			ov_clear( &mNativeFileParams );
			mValidFileParams = false;
		}
		else
		{
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
			
			mEncoding = false;

		}
	}

	void OggVorbisAudioStream::ConsumeDecodedSamples()
	{
		CLAM_ASSERT( mDecodeBuffer.size() >= unsigned(mInterleavedData.Size()),
			     "This method cannot be called if the decode buffer"
			     " has less samples than requested by the upper level");

		static const TData norm = 1.0 / 32768.0;

		TData* pSamples = mInterleavedData.GetPtr();
		const TData* pSamplesEnd = mInterleavedData.GetPtr() + mInterleavedData.Size();
		for( std::deque<TInt16>::iterator i = mDecodeBuffer.begin();
		     pSamples < pSamplesEnd; i++, pSamples++ )
			*pSamples = TData(*i)*norm;		
		
		mDecodeBuffer.erase( mDecodeBuffer.begin(),
				     mDecodeBuffer.begin()+mInterleavedData.Size() );
		
	}

	void OggVorbisAudioStream::DiskToMemoryTransfer()
	{

		//Unused variable: TSize nBytes = 0;
		unsigned samplesRead = 0;

		while ( mDecodeBuffer.size() < unsigned(mInterleavedData.Size()) )
		{
			mLastBytesRead = ov_read( &mNativeFileParams, 
						  (char*)mBlockBuffer.GetPtr(), 
						  mMaxBlockSize*sizeof(TInt16),
						  HOST_ENDIANESS,
						  2, 1, &mCurrentSection );
			
			CLAM_ASSERT( mLastBytesRead >= 0, "Malformed OggVorbis file!" );
			CLAM_ASSERT( mLastBytesRead % mEncodedChannels == 0, "BIG Whoops!" );

			if ( mLastBytesRead == 0 ) break;

			samplesRead = mLastBytesRead / sizeof(TInt16 );

			
			mDecodeBuffer.insert( mDecodeBuffer.end(),
					      mBlockBuffer.GetPtr(),
					      mBlockBuffer.GetPtr() + samplesRead);				
		}

		mFramesLastRead = mDecodeBuffer.size();

		if ( !mDecodeBuffer.empty() )
		{

			if ( mDecodeBuffer.size() < unsigned(mInterleavedData.Size()) )
			{
				mDecodeBuffer.insert( mDecodeBuffer.end(),
						      mInterleavedData.Size() - mDecodeBuffer.size(),
						      0);
			}
			
			ConsumeDecodedSamples();
		}

		mEOFReached = ( mLastBytesRead == 0) && (mDecodeBuffer.empty());
	
	}

	void OggVorbisAudioStream::MemoryToDiskTransfer()
	{
		// Yahoo! The vorbis encoder wants the samples
		// as floats!
		
		// We expose the buffer for submitting data to the encoder

		TIndex currentOffset = 0;
		int i;

		do
		{
			for ( i = mEncodeBuffer[0].size(); 
			      i < mAnalysisWindowSize && currentOffset < mInterleavedDataOut.Size(); 
			      i++ )
			{
				for ( int j = 0; j < mEncodedChannels; j++ )
					mEncodeBuffer[j].push_front( mInterleavedDataOut[ currentOffset + j ] );

				currentOffset += mEncodedChannels;
			}

			if ( i == mAnalysisWindowSize ) // enough samples acquired
				DoVorbisAnalysis();
			
		} while ( currentOffset < mInterleavedDataOut.Size() );

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

		float** encBuffer = vorbis_analysis_buffer( &mDSPState, 
							    mAnalysisWindowSize);
		
		//Unused variable: int samplesWrote = 0;

		for ( int j = 0; j < mEncodedChannels; j++ )
		{
			int i = 0;

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
}	

}

