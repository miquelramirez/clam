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

#if USE_MAD != 1
#error with_mad was not set to 1 with scons, but you are including files that require this. Please fix your settings.cfg
#endif

#if USE_ID3 != 1
#error with_id3 was not set to 1 with scons, but you are including files that require this. Please fix your settings.cfg
#endif


#include "MpegCodec.hxx"
#include "AudioFileFormats.hxx"
#include "AudioFile.hxx"
#include "AudioFileHeader.hxx"
#include "MpegBitstream.hxx"
#include "MpegAudioStream.hxx"
#include <mad.h>
#include <id3/tag.h>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

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
		if (bitlen < 64) goto fail;
		if (mad_bit_read(&ptr, 32) != XING_MAGIC) goto fail;

		xing->flags = mad_bit_read(&ptr, 32);
		bitlen -= 64;
		
		if (xing->flags & XING_FRAMES) {
			if (bitlen < 32) goto fail;
			
			xing->frames = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}
		
		if (xing->flags & XING_BYTES) {
			if (bitlen < 32) goto fail;
			
			xing->bytes = mad_bit_read(&ptr, 32);
			bitlen -= 32;
		}
		
		if (xing->flags & XING_TOC) {
			if (bitlen < 800) goto fail;
			
			// MRJ: We just need the 8 least significant bits
			for (unsigned i = 0; i < 100; ++i)
				xing->toc[i] = (unsigned char)mad_bit_read(&ptr, 8);
			bitlen -= 800;
		}
		
		if (xing->flags & XING_SCALE) {
			if (bitlen < 32) goto fail;
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
		// If has extension and it is not mp3 reject it
		std::string::size_type startExt = uri.rfind( '.' );
		if ( startExt != std::string::npos )
		{
			std::string ext = uri.substr(startExt+1);
			if ( ext!="mp3" && ext!="mpg" && ext!="mpeg")
				return false;
		}

		// If it is not readable reject it
		FILE* handle = fopen( uri.c_str(), "rb" );
		if ( !handle ) // File doesn't exists / not readable
			return false;

		// Look for an Mpeg frame
		MpegBitstream bitstream;
		bitstream.Init(handle);
		bool   foundSomeMpegFrame = false;
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

		struct stat fileStats;

		if ( stat( uri.c_str(), &fileStats ) != 0 )
		{
			// Error reading stats from file
			fclose(handle);
			return;
		}

		unsigned long fileLength = fileStats.st_size;
		
		if ( fseek( handle, -128, SEEK_END ) < 0 )
		{
			/* File empty */
			fclose(handle);
			return;
		}

		char buffer[3];

		if ( fread( buffer, 1, 3, handle ) != 3 )
		{
			fclose(handle);
			return;
		}

		if ( !strncmp( buffer, "TAG", 3 ) )
		{
			fileLength -=128;
		}

		fclose( handle );
		handle = fopen( uri.c_str(), "rb" );

		hdr.AddSampleRate();
		hdr.AddChannels();
		hdr.AddSamples();
		hdr.AddFormat();
		hdr.AddEncoding();
		hdr.AddEndianess();
		hdr.AddLength();
		hdr.UpdateData();
		
		MpegBitstream bitstream;

		bitstream.Init(handle);

		int frameCount = 0;
		struct xing xingHeader;
		xingHeader.frames=0;
		bool   hasXingHeader = false;
		bool   isVBR = false;
		unsigned int    bitrate = 0;
		
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
		
		long numFrames = 0;

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
                        numFrames = xingHeader.frames;
						break;					
					}
				}
				bitrate = bitstream.CurrentFrame().header.bitrate;
			}
			
			if ( frameCount <= 20 )
			{
				if ( bitstream.CurrentFrame().header.bitrate != bitrate )
					isVBR = true;
			}		       
			
			if ( !isVBR && frameCount > 20 )
				break;
			
			frameCount++;
		}

		if ( !isVBR )
		{
			double time = ( fileLength * 8.0 ) / bitstream.CurrentFrame().header.bitrate;
			double timeFrac = (double)time - ((long)(time));
			long   nsamples = 32 * MAD_NSBSAMPLES(&bitstream.CurrentFrame().header); // samples per frame
			numFrames = ( long) ( time * bitstream.CurrentFrame().header.samplerate / nsamples );
			mad_timer_t   madFmtTime;
			mad_timer_set( &madFmtTime, (long)time, (long)(timeFrac*100), 100 );
			bitstream.Finish();

            //std::cerr << "Not VBR: " << (TTime)mad_timer_count( madFmtTime, MAD_UNITS_MILLISECONDS )/1000.  << std::endl;
			hdr.SetLength( (TTime)mad_timer_count( madFmtTime, MAD_UNITS_MILLISECONDS ) );
		}
		else if ( hasXingHeader )
		{
			mad_timer_multiply( &bitstream.CurrentFrame().header.duration, numFrames );
			mad_timer_t madFmtTime = bitstream.CurrentFrame().header.duration;
			bitstream.Finish();

            //std::cerr << "Has XING Header: "<< (TTime)mad_timer_count( madFmtTime, MAD_UNITS_MILLISECONDS )/1000.  << std::endl;
			hdr.SetLength( (TTime)mad_timer_count( madFmtTime, MAD_UNITS_MILLISECONDS ) );
		}
		else
		{ 

			TTime decodedFramesLength = bitstream.Finish();
            //std::cerr << "Brute force time guessing: " <<  decodedFramesLength/1000. << " s" << std::endl;
			hdr.SetLength( decodedFramesLength );
		}
		
		// @TODO@: Find a way to estimate reasonably well the actual
		// number of samples.
		hdr.SetSamples(  TSize((hdr.GetLength()/1000.)*hdr.GetSampleRate()) );
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
		
		header.SetSampleRate( TData(MPEGFrame.header.samplerate) );
		header.SetChannels( MAD_NCHANNELS(&MPEGFrame.header) );
	}


	static const char * getField(ID3_Tag & tag, const ID3_FrameID & field)
	{
		ID3_Frame* fieldFrame = tag.Find( field );
		if (not fieldFrame) return 0;
		ID3_Field* fieldString = fieldFrame->GetField( ID3FN_TEXT );
		if (not fieldString) return 0;
		return fieldString->GetRawText();
	}

	void   MpegCodec::RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& txt )
	{
		ID3_Tag fileTag;
		fileTag.Link( uri.c_str() );

		const char * artist = getField(fileTag, ID3FID_LEADARTIST);
		const char * title = getField(fileTag, ID3FID_TITLE);
		const char * album = getField(fileTag, ID3FID_ALBUM);
		const char * tracknum = getField(fileTag, ID3FID_TRACKNUM);
		const char * composer = getField(fileTag, ID3FID_COMPOSER);
		const char * performer = getField(fileTag, ID3FID_CONDUCTOR);

		if (artist) txt.AddArtist();
		if (title) txt.AddTitle();
		if (album) txt.AddAlbum();
		if (tracknum) txt.AddTrackNumber();
		if (composer) txt.AddComposer();
		if (performer) txt.AddPerformer();

		txt.UpdateData();

		if (artist) txt.SetArtist(artist);
		if (title) txt.SetTitle(title);
		if (album) txt.SetAlbum(album);
		if (tracknum) txt.SetTrackNumber(tracknum);
		if (composer) txt.SetComposer(composer);
		if (performer) txt.SetPerformer(performer);
	}
}

}

