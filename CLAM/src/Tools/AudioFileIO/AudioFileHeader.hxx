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

#ifndef __AUDIOFILEHEADER__
#define __AUDIOFILEHEADER__
#include <typeinfo>

#include "AudioFileFormats.hxx"
#include "DynamicType.hxx"

namespace CLAM
{
	/** @ingroup AudioFileIO */
	class AudioFileHeader
		: public DynamicType
	{
	public:
		DYNAMIC_TYPE( AudioFileHeader, 7 );
		DYN_ATTRIBUTE( 0, public, TData,               SampleRate );
		DYN_ATTRIBUTE( 1, public, TSize,               Samples    );
		DYN_ATTRIBUTE( 2, public, TSize,               Channels   );
		DYN_ATTRIBUTE( 3, public, TTime,               Length     );
		DYN_ATTRIBUTE( 4, public, EAudioFileFormat,    Format     );
		DYN_ATTRIBUTE( 5, public, EAudioFileEncoding,  Encoding   );
		DYN_ATTRIBUTE( 6, public, EAudioFileEndianess, Endianess  );

	protected:
		void DefaultInit();

	public:

		void SetValues( TData rate, TSize numberOfChannels,
				EAudioFileFormat fmt );
		
		void SetValues( TData rate, TSize numberOfChannels,
				const char* fmtString );

	};

}

#endif // AudioFileHeader.hxx

