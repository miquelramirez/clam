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

#ifndef __AUDIOCODEC__
#define __AUDIOCODEC__

#include <string>

namespace CLAM
{

	class AudioFile;
	class AudioFileHeader;
	class AudioTextDescriptors;

namespace AudioCodecs
{
	class Stream;

	/**
	 * A codec implements a file audio format support.
	 * Subclasses should reimplement its methods to support
	 * a concrete audio file format.
	 */
	class Codec
	{
	public:
		virtual          ~Codec();
		static bool      FileExists( std::string filename );
		/** Returns whether the filename is readable by the codec */
		virtual bool     IsReadable( std::string filename ) const = 0;
		/** Returns whether codec can write a file with such name and header parameters */
		virtual bool     IsWritable( std::string filename, const AudioFileHeader& ) const = 0;
		/** Returns an Stream object to read or write on */
		virtual Stream*  GetStreamFor( const AudioFile& ) = 0;
		/** Returns a header structure containing file information (length, encoding...) */
		virtual void     RetrieveHeaderData( std::string uri, AudioFileHeader& ) = 0;
		/** Returns textual meta information embedded on the audio file such ID3 tags */
		virtual void     RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& );
	};
}

}


#endif // AudioCodec.hxx

