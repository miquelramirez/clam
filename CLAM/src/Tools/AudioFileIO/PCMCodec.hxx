#ifndef __PCMCODEC__
#define __PCMCODEC__

#include "AudioCodec.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class PCMCodec : public Codec
	{
	protected:
		PCMCodec();
	public:
		virtual          ~PCMCodec();
		static PCMCodec& Instantiate();
		virtual bool     IsReadable( std::string uri ) const;
		virtual bool     IsWritable( std::string uri, const AudioFileHeader& ) const;
		virtual Stream*  GetStreamFor( const AudioFile& );
		virtual void     RetrieveHeaderData( std::string uri, AudioFileHeader& );

	};
}
}

#endif // PCMCodec.hxx
