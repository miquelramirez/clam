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

#if USE_SNDFILE != 1
#error USE_SNDFILE was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

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
			hdr.AddSamples();
			hdr.AddChannels();
			hdr.AddFormat();
			hdr.AddEncoding();
			hdr.AddEndianess();
			hdr.AddLength();
			hdr.UpdateData();
						
			hdr.SetSampleRate( (TData)fileHeaderInfo.samplerate );
			hdr.SetSamples( (TSize)fileHeaderInfo.frames );
			hdr.SetChannels( (TSize)fileHeaderInfo.channels );
			hdr.SetFormat( fileHeaderInfo.format & SF_FORMAT_TYPEMASK );
			hdr.SetEncoding( fileHeaderInfo.format & SF_FORMAT_SUBMASK );
			hdr.SetEndianess( fileHeaderInfo.format & SF_FORMAT_ENDMASK );
			hdr.SetLength( TTime(fileHeaderInfo.frames) / hdr.GetSampleRate() );
			hdr.SetLength( hdr.GetLength() * 1000. );
			sf_close(fileHandle);
		}

	}
}

}

