#include "MpegCodec.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFile.hxx"
#include "AudioFileHeader.hxx"
#include "MpegBitstream.hxx"
#include "MpegAudioStream.hxx"
#include <mad.h>
#ifdef HAVE_STANDARD_UNICODE
#include <id3/tag.h>
#endif
#include <cstdio>
#include <iostream>

namespace CLAM
{

namespace AudioCodecs
{


	MpegCodec::MpegCodec()
	{

	}

	MpegCodec::~MpegCodec()
	{
	}

	MpegCodec& MpegCodec::Instantiate()
	{
		static MpegCodec theInstance;

		return theInstance;
	}

	bool MpegCodec::IsReadable( std::string uri ) const
	{
		// We will just check there is a Mpeg frame in the given
		// file

		FILE* handle = fopen( uri.c_str(), "rb" );

		if ( !handle ) // File doesn't exists / not readable
			return false;

		MpegBitstream bitstream( handle );

		bitstream.Init();

		bool   foundSomeMpegFrame = false;
		int    status = 0;

		while( !foundSomeMpegFrame 
		       && !bitstream.EOS() && !bitstream.FatalError() )
			foundSomeMpegFrame = bitstream.NextFrame();

		bitstream.Finish();

		fclose( handle );

		return foundSomeMpegFrame;
	}

	bool MpegCodec::IsWritable( std::string uri, const AudioFileHeader& header ) const
	{
		// CLAM does not encode Mpeg
		return false;
	}

	Stream* MpegCodec::GetStreamFor( const AudioFile& file )
	{
		return new MpegAudioStream(file);
	}

	void MpegCodec::RetrieveHeaderData( std::string uri, AudioFileHeader& hdr )
	{

		FILE* handle = fopen( uri.c_str(), "rb" );

		if ( !handle ) // File doesn't exists / not readable
			return;

		hdr.AddSampleRate();
		hdr.AddChannels();
		hdr.AddFormat();
		hdr.AddEncoding();
		hdr.AddEndianess();
		hdr.AddLength();
		hdr.UpdateData();
		
		MpegBitstream bitstream( handle );

		bitstream.Init();

		int frameCount = 0;
		
		while ( !bitstream.FatalError() && bitstream.NextFrame() )
		{
			if ( frameCount == 0 ) // first frame
			{
				RetrieveMPEGFrameInfo( bitstream.CurrentFrame(),
						       hdr );
			}
			frameCount++;
		}

		TTime length = bitstream.Finish();

		hdr.SetLength( length );
		hdr.SetEndianess( EAudioFileEndianess::eDefault );

		fclose( handle );
		
	}

	void MpegCodec::RetrieveMPEGFrameInfo( const struct mad_frame& MPEGFrame,
					       AudioFileHeader& header )
	{
		switch( MPEGFrame.header.layer )
		{
		case MAD_LAYER_I:
			header.SetFormat( EAudioFileFormat::eMpegLayer1 );
			break;
		case MAD_LAYER_II:
			header.SetFormat( EAudioFileFormat::eMpegLayer2 );
			break;
		case MAD_LAYER_III:
			header.SetFormat( EAudioFileFormat::eMpegLayer3 );
			break;
		default:
			break;	       
		}

		switch( MPEGFrame.header.emphasis )
		{
		case MAD_EMPHASIS_NONE:
			header.SetEncoding( EAudioFileEncoding::eDefault );
			break;
		case MAD_EMPHASIS_50_15_US:
			header.SetEncoding( EAudioFileEncoding::e5015US );
			break;
		case MAD_EMPHASIS_CCITT_J_17:
			header.SetEncoding( EAudioFileEncoding::eCCITTJ17 );
			break;
		default:
			break;
		}
		
		header.SetSampleRate( MPEGFrame.header.samplerate );
		header.SetChannels( MAD_NCHANNELS(&MPEGFrame.header) );
	}


	void   MpegCodec::RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& txt )
	{
#ifdef HAVE_STANDARD_UNICODE
		ID3_Tag fileTag;

		fileTag.Link( uri.c_str() );

		ID3_Frame* artistFrame = fileTag.Find( ID3FID_LEADARTIST );

		if ( artistFrame != NULL )
		{
			txt.AddArtist();
			txt.UpdateData();
			ID3_Field* artistStr = artistFrame->GetField( ID3FN_TEXT );
			
			if ( artistStr != NULL )
				txt.SetArtist( artistStr->GetRawText() );
		}

		ID3_Frame* titleFrame = fileTag.Find( ID3FID_TITLE );

		if ( titleFrame != NULL )
		{
			txt.AddTitle();
			txt.UpdateData();
			ID3_Field* titleStr = titleFrame->GetField( ID3FN_TEXT );

			if ( titleStr!=NULL )
				txt.SetTitle( titleStr->GetRawText() );
		}

		ID3_Frame* albumFrame = fileTag.Find( ID3FID_ALBUM );

		if ( albumFrame != NULL )
		{
			txt.AddAlbum();
			txt.UpdateData();
			ID3_Field* albumStr = albumFrame->GetField( ID3FN_TEXT );

			if ( albumStr != NULL )
				txt.SetAlbum( albumStr->GetRawText() );
		}

		ID3_Frame* tracknumFrame = fileTag.Find( ID3FID_TRACKNUM );

		if ( tracknumFrame != NULL )
		{
			txt.AddTrackNumber();
			txt.UpdateData();

			ID3_Field* tracknumStr = tracknumFrame->GetField( ID3FN_TEXT );
			
			if ( tracknumStr != NULL )
				txt.SetTrackNumber( tracknumStr->GetRawText() );
		}

		ID3_Frame* composerFrame = fileTag.Find( ID3FID_COMPOSER );

		if ( composerFrame != NULL )
		{
			txt.AddComposer();
			txt.UpdateData();

			ID3_Field* composerStr = composerFrame->GetField( ID3FN_TEXT );

			if ( composerStr != NULL )
				txt.SetComposer( composerStr->GetRawText() );
		}

		ID3_Frame* performerFrame = fileTag.Find( ID3FID_CONDUCTOR );

		if ( performerFrame != NULL )
		{
			txt.AddPerformer();
			txt.UpdateData();

			ID3_Field* performerStr = performerFrame->GetField( ID3FN_TEXT );

			if ( performerStr != NULL )
				txt.SetPerformer( performerStr->GetRawText() );
		}
#endif
	}

}

}
