#ifndef __MpegAudioStream__
#define __MpegAudioStream__


#include "AudioCodecs_Stream.hxx"
#include "MpegBitstream.hxx"
#include <deque>
#include "Array.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class MpegAudioStream 
		: public Stream
	{
	public:

		MpegAudioStream();
		MpegAudioStream( const AudioFile& file );
		
		~MpegAudioStream();

		void SetFOI( const AudioFile& file );

		void PrepareReading();
		void PrepareWriting();
		void PrepareReadWrite();
		void Dispose();

	protected:

		void AudioFileToNative( const AudioFile& file );
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

		void ConsumeDecodedSamples();

	protected:
		
		std::string   mName;
		FILE*         mpHandle;
		MpegBitstream mBitstream;
		int           mEncodedSampleRate;
		int           mEncodedChannels;

		static const TSize                    mMaxDecodedBlockSize;
		std::vector<std::deque<mad_fixed_t> > mDecodeBuffer;
		TSize                                 mSamplesDecoded;
		TSize                                 mSamplesTransferred;
	};
}

}


#endif // MpegAudioStream.hxx
