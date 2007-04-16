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

#if USE_OGGVORBIS != 1
#error USE_OGGVORBIS was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#ifndef __OGGVORBISAUDIOSTREAM__
#define __OGGVORBISAUDIOSTREAM__

#include <string>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#include <deque>
#include <vector>
#include "AudioCodecs_Stream.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class OggVorbisAudioStream : public Stream
	{

	public:
		OggVorbisAudioStream();
		OggVorbisAudioStream( const AudioFile& file );

		~OggVorbisAudioStream();

		void SetFOI( const AudioFile& file );

		void PrepareReading();
		void PrepareWriting();
		void PrepareReadWrite();
		void Dispose();

	protected:
		void AudioFileToNative( const AudioFile& file );
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

		void VorbisI_EncoderSetup();
		void WriteBitstreamHeader();

		void DoVorbisAnalysis();
		void PushAnalysisBlocksOntoOggStream();
		void ConsumeDecodedSamples();
		
	protected:
		std::string      mName;
		FILE*            mFileHandle;
		OggVorbis_File   mNativeFileParams;
		int              mCurrentSection;
		bool             mValidFileParams;

		// Structs needed for Vorbis encoding
		vorbis_info        mStreamInfo;
		vorbis_comment     mFileComments;

		ogg_stream_state   mOggStreamState; // os
		ogg_page           mOggPage;        // og
		ogg_packet         mOggPacket;      // op
		vorbis_dsp_state   mDSPState;       // vd
		vorbis_block       mVorbisBlock;    // vb
		
		int                mEncodedSampleRate;
		int                mEncodedChannels;
		bool               mEncoding;
		int                mOffset;

		static const TSize      mMaxBlockSize;
		static const TSize      mAnalysisWindowSize;
		Array<TInt16>           mBlockBuffer;
		std::vector<std::deque<TData> >       mEncodeBuffer;
		std::deque<TInt16>      mDecodeBuffer;
		
		TSize                   mLastBytesRead;
  	};
}

}

#endif // OggVorbisAudioStream.hxx

