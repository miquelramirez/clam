#include "AudioFileHeader.hxx"

namespace CLAM
{
	void AudioFileHeader::DefaultInit()
	{
	
	}

	void AudioFileHeader::SetValues( TData rate, TSize nChannels,
					 const char* fmtString )
	{
		SetValues( rate, nChannels, EAudioFileFormat( fmtString ) );
	}

	void AudioFileHeader::SetValues( TData rate, TSize nChannels,
					 EAudioFileFormat format )
	{
		AddAll();
		UpdateData();
		SetSampleRate( rate );
		SetChannels( nChannels );
		SetFormat( format );
		SetEndianess( EAudioFileEndianess::eDefault );
		SetLength( TTime( 0 ) );

		// Default encoding
		if ( GetFormat() == EAudioFileFormat::eWAV )
			SetEncoding( EAudioFileEncoding::ePCM_24 );
		else if ( GetFormat() == EAudioFileFormat::eAIFF )
			SetEncoding( EAudioFileEncoding::ePCM_24 );
		else
			SetEncoding( EAudioFileEncoding::eDefault );
	}


}
