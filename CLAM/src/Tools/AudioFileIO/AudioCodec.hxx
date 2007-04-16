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

	class Codec
	{
	public:
		virtual          ~Codec();
		static bool      FileExists( std::string filename );
		virtual bool     IsReadable( std::string filename ) const = 0;
		virtual bool     IsWritable( std::string filename, const AudioFileHeader& ) const = 0;
		virtual Stream*  GetStreamFor( const AudioFile& ) = 0;
		virtual void     RetrieveHeaderData( std::string uri, AudioFileHeader& ) = 0;
		virtual void     RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& );
	};
}

}


#endif // AudioCodec.hxx

