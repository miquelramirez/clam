/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioFileFormats.hxx"
#include <algorithm>
#include <cctype>

namespace CLAM
{
	EAudioFileFormat EAudioFileFormat::FormatFromFilename( std::string filename )
	{
		std::string::iterator dotPos = std::find( filename.begin(), filename.end(), '.' );
		if ( dotPos == filename.end() )
			return eWAV;
		std::string extension( dotPos+1, filename.end() );
		for(unsigned i=0; i<extension.size();++i)
			extension[i] = std::tolower(extension[i]);

		static struct { 
			const char * extension;
			EAudioFileFormat format;
		} extensionMap[] =
		{
			{"wav",  eWAV},
			{"ogg",  eVorbisMk1},
			{"aif",  eAIFF},
			{"aiff", eAIFF},
			{"au",   eAU},
			{"snd",  eAU},
			{"raw",  eRAW},
			{"paf",  ePAF},
			{"svx",  eSVX},
			{"nist", eNIST},
			{"voc",  eVOC},
			{"ircam", eIRCAM},
			{"w64",  eW64},
			{"mat4", eMAT4},
			{"mat5", eMAT5},
			{"mat",  eMAT5},
			{"mp1",  eMpegLayer1},
			{"mp2",  eMpegLayer2},
			{"mp3",  eMpegLayer3},
			{0,0}
		};
		for (unsigned i=0; extensionMap[i].extension; i++)
			if (extension==extensionMap[i].extension)
				return extensionMap[i].format;
		return eWAV;
	}


}

