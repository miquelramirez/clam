#ifndef __MPEGCODEC__
#define __MPEGCODEC__

#include "AudioCodec.hxx"
#include "DataTypes.hxx"

struct mad_frame;

namespace CLAM
{

namespace AudioCodecs
{


	class MpegCodec : public Codec
	{
	protected:
		MpegCodec();

	public:
		virtual ~MpegCodec();
		
		static MpegCodec& Instantiate();

		virtual bool     IsReadable( std::string uri ) const;
		virtual bool     IsWritable( std::string uri, const AudioFileHeader& ) const;
		virtual Stream*  GetStreamFor( const AudioFile& );
		virtual void     RetrieveHeaderData( std::string uri, AudioFileHeader& );
		virtual void     RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& );

	protected:

		void RetrieveMPEGFrameInfo( const struct mad_frame& MPEGframe,
					    AudioFileHeader& header );

	};
}

}


#endif // MpegCodec.hxx
