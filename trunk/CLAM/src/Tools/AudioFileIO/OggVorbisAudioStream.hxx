#ifndef __OGGVORBISAUDIOSTREAM__
#define __OGGVORBISAUDIOSTREAM__

#include <string>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#include "CLAM_deque.hxx"
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
