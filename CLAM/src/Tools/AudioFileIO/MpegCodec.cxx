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


/* XING parsing is from the MAD winamp input plugin */
/* Ripped mercilessly from mpg321 */

	struct xing {
		int flags;
		unsigned long frames;
		unsigned long bytes;
		unsigned char toc[100];
		long scale;
	};
	
	enum {
		XING_FRAMES = 0x0001,
		XING_BYTES  = 0x0002,
		XING_TOC    = 0x0004,
		XING_SCALE  = 0x0008
	};
	
# define XING_MAGIC     (('X' << 24) | ('i' << 16) | ('n' << 8) | 'g')
	
	static
	int parse_xing(struct xing *xing, struct mad_bitptr ptr, unsigned int bitlen)
	{
		if (bitlen < 64 || mad_bit_read(&ptr, 32) != XING_MAGIC)
			goto fail;
		
		xing->flags = mad_bit_read(&ptr, 32);
		bitlen -= 64;
		
		if (xing->flags & XING_FRAMES) {
			if (bitlen < 32)
				goto fail;
			
			xing->frames = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}
		
		if (xing->flags & XING_BYTES) {
			if (bitlen < 32)
				goto fail;
			
			xing->bytes = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}
		
		if (xing->flags & XING_TOC) {
			int i;
			
			if (bitlen < 800)
				goto fail;
			
			for (i = 0; i < 100; ++i)
				xing->toc[i] = mad_bit_read(&ptr, 8);
			
			bitlen -= 800;
		}
		
		if (xing->flags & XING_SCALE) {
			if (bitlen < 32)
				goto fail;
			
			xing->scale = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}
		
		return 1;
		
	fail:
		xing->flags = 0;
		return 0;
	}



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
		struct xing xingHeader;
		bool   hasXingHeader = false;
		bool   isVBR = false;
		int    bitrate = 0;
		
		/* There are three ways of calculating the length of an mp3:
		   1) Constant bitrate: One frame can provide the information
		   needed: # of frames and duration. Just see how long it
		   is and do the division.
		   2) Variable bitrate: Xing tag. It provides the number of 
		   frames. Each frame has the same number of samples, so
		   just use that.
		   3) All: Count up the frames and duration of each frames
		   by decoding each one. We do this if we've no other
		   choice, i.e. if it's a VBR file with no Xing tag.
		*/
		

		while ( !bitstream.FatalError() && bitstream.NextFrame() )
		{
			if ( frameCount == 0 ) // first frame, for retrieving info about encoding
			{
				RetrieveMPEGFrameInfo( bitstream.CurrentFrame(),
						       hdr );
				if ( parse_xing( &xingHeader, 
						 bitstream.StreamState().anc_ptr, 
						 bitstream.StreamState().anc_bitlen ) )
				{
					isVBR = true;
					
					if ( xingHeader.flags & XING_FRAMES )
					{
						/* We use the Xing tag only for frames. If it doesn't have that
						   information, it's useless to us and we have to treat it as a
						   normal VBR file */
						hasXingHeader = true;
						break;					
					}
				}	
				bitrate = bitstream.CurrentFrame().header.bitrate;
			}
			
			if ( frameCount <= 20 )
			{
				if ( bitstream.CurrentFrame().header.bitrate != bitrate )
					isVBR = true;
				else
					bitrate = bitstream.CurrentFrame().header.bitrate;
			}		       
			
			if ( !isVBR && frameCount > 20 )
				break;
			
			frameCount++;
		}

		long numFrames = 0;
		mad_timer_t   madFmtTime;

		if ( !isVBR )
		{
			double time = ( 40. * 8192. ) / bitstream.CurrentFrame().header.bitrate;
			double timeFrac = (double)time - ((long)(time));
			long   nsamples = 32 * MAD_NSBSAMPLES(&bitstream.CurrentFrame().header); // samples per frame
			numFrames = ( long) ( time * bitstream.CurrentFrame().header.samplerate / nsamples );
			
			mad_timer_set( &madFmtTime, (long)time, (long)(timeFrac*100), 100 );

			if ( hasXingHeader )
			{
				mad_timer_multiply( &bitstream.CurrentFrame().header.duration,
						    numFrames );
				madFmtTime = bitstream.CurrentFrame().header.duration;
			}

			bitstream.Finish();

			hdr.SetLength( (TTime)mad_timer_count( madFmtTime, MAD_UNITS_MILLISECONDS ) );
		}
		else
		{ 

			TTime decodedFramesLength = bitstream.Finish();
			hdr.SetLength( decodedFramesLength );
		}

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
			{
				if ( artistStr->GetRawText() != NULL )
					txt.SetArtist( artistStr->GetRawText() );
			}
		}

		ID3_Frame* titleFrame = fileTag.Find( ID3FID_TITLE );

		if ( titleFrame != NULL )
		{
			txt.AddTitle();
			txt.UpdateData();
			ID3_Field* titleStr = titleFrame->GetField( ID3FN_TEXT );

			if ( titleStr!=NULL )
				if ( titleStr->GetRawText() != NULL )
					txt.SetTitle( titleStr->GetRawText() );
		}

		ID3_Frame* albumFrame = fileTag.Find( ID3FID_ALBUM );

		if ( albumFrame != NULL )
		{
			txt.AddAlbum();
			txt.UpdateData();
			ID3_Field* albumStr = albumFrame->GetField( ID3FN_TEXT );

			if ( albumStr != NULL )
				if ( albumStr->GetRawText() != NULL )
					txt.SetAlbum( albumStr->GetRawText() );
		}

		ID3_Frame* tracknumFrame = fileTag.Find( ID3FID_TRACKNUM );

		if ( tracknumFrame != NULL )
		{
			txt.AddTrackNumber();
			txt.UpdateData();

			ID3_Field* tracknumStr = tracknumFrame->GetField( ID3FN_TEXT );
			
			if ( tracknumStr != NULL )
				if ( tracknumStr->GetRawText() != NULL )
					txt.SetTrackNumber( tracknumStr->GetRawText() );
		}

		ID3_Frame* composerFrame = fileTag.Find( ID3FID_COMPOSER );

		if ( composerFrame != NULL )
		{
			txt.AddComposer();
			txt.UpdateData();

			ID3_Field* composerStr = composerFrame->GetField( ID3FN_TEXT );

			if ( composerStr != NULL )
				if ( composerStr->GetRawText() != NULL )
					txt.SetComposer( composerStr->GetRawText() );
		}

		ID3_Frame* performerFrame = fileTag.Find( ID3FID_CONDUCTOR );

		if ( performerFrame != NULL )
		{
			txt.AddPerformer();
			txt.UpdateData();

			ID3_Field* performerStr = performerFrame->GetField( ID3FN_TEXT );

			if ( performerStr != NULL )
				if ( performerStr->GetRawText() != NULL )
					txt.SetPerformer( performerStr->GetRawText() );
		}
#endif
	}

}

}
