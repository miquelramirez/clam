#include "PCMCodec.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFile.hxx"
#include "AudioFileHeader.hxx"
#include "DataTypes.hxx"
#include <sndfile.h>
#include "PCMAudioStream.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	PCMCodec::PCMCodec()
	{
	}

	PCMCodec::~PCMCodec()
	{
	}

	PCMCodec& PCMCodec::Instantiate()
	{
		static PCMCodec theInstance;

		return theInstance;
	}

	bool PCMCodec::IsReadable( std::string uri ) const
	{
		SNDFILE*   fileHandle = NULL;
		SF_INFO   fileHeaderInfo;
		// MRJ: Done as libsndfile doc says
		fileHeaderInfo.format = 0;

		fileHandle = sf_open( uri.c_str(), SFM_READ, &fileHeaderInfo );

		if ( fileHandle != NULL )
		{
			sf_close( fileHandle );
			return true;
		}

		// TODO: An exception should be thrown
		return false;

	}

	bool PCMCodec::IsWritable( std::string uri, const AudioFileHeader& header ) const
	{
		if ( !header.HasSampleRate() )
			return false;
		if ( !header.HasChannels() )
			return false;
		if ( !header.HasFormat() )
			return false;
		if ( !header.HasEncoding() )
			return false;
		if ( !header.HasEndianess() )
			return false;

		SF_INFO fileInfo;
		fileInfo.samplerate = (int)header.GetSampleRate();
		fileInfo.channels = (int)header.GetChannels();
		fileInfo.format = header.GetFormat() | header.GetEncoding() | header.GetEndianess();

		return sf_format_check( &fileInfo ) == 1;
	}

	Stream* PCMCodec::GetStreamFor( const AudioFile& file )
	{
		return new PCMAudioStream(file);
	}

	void PCMCodec::RetrieveHeaderData( std::string uri, AudioFileHeader& hdr )
	{
		SNDFILE* fileHandle = NULL;
		SF_INFO  fileHeaderInfo;
		fileHeaderInfo.format = 0;
		
		fileHandle = sf_open( uri.c_str(), SFM_READ, &fileHeaderInfo );
		
		if ( fileHandle != NULL )
		{
			hdr.AddSampleRate();
			hdr.AddChannels();
			hdr.AddFormat();
			hdr.AddEncoding();
			hdr.AddEndianess();
			hdr.AddLength();
			hdr.UpdateData();
						
			hdr.SetSampleRate( (TData)fileHeaderInfo.samplerate );
			hdr.SetChannels( (TSize)fileHeaderInfo.channels );
			hdr.SetFormat( fileHeaderInfo.format & SF_FORMAT_TYPEMASK );
			hdr.SetEncoding( fileHeaderInfo.format & SF_FORMAT_SUBMASK );
			hdr.SetEndianess( fileHeaderInfo.format & SF_FORMAT_ENDMASK );
			hdr.SetLength( TTime(fileHeaderInfo.frames/fileHeaderInfo.channels) / hdr.GetSampleRate() );
			hdr.SetLength( hdr.GetLength() * 1000. );
		}

	}
}

}
