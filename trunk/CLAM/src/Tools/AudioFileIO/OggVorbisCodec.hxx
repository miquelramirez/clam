#ifndef __OGGVORBISCODEC__
#define __OGGVORBISCODEC__

#include "AudioCodec.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class OggVorbisCodec : public Codec
	{
	protected:
		OggVorbisCodec();
	public:
		virtual                ~OggVorbisCodec();
		static OggVorbisCodec& Instantiate();
		virtual bool           IsReadable( std::string uri ) const;
		virtual bool           IsWritable( std::string uri, const AudioFileHeader& ) const;
		virtual Stream*        GetStreamFor( const AudioFile& );
		virtual void           RetrieveHeaderData( std::string uri, AudioFileHeader& );
		virtual void           RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& );
	};
}
}

#endif // OggVorbisCodec.hxx
