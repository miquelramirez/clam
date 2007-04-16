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

	static struct {
		bool writeable;
		const char * description;
		const char * extensions;
	} filters [] = {
		{true,  "Microsoft RIFF/WAVE files",        "*.wav"},
		{true,  "Vorbis I files (Ogg/Vorbis)",      "*.ogg"},
		{false, "Mpeg Audio Layer 1",               "*.mp1 *.mpg"},
		{false, "Mpeg Audio Layer 2",               "*.mp2 *.mpg"},
		{false, "Mpeg Audio Layer 3",               "*.mp3 *.mpg"},
		{true,  "Apple/SGI AIFF files",             "*.aiff *.aif"},
		{true,  "Sun/Next AU files",                "*.snd *.au"},
		{true,  "RAW PCM files",                    "*.raw"},
		{true,  "Ensoniq's PARIS Files",            "*.paf"},
		{true,  "Amiga IFF files",                  "*.svx"},
		{true,  "Sphere NIST files",                "*.nist"},
		{true,  "Creative's VOC files",             "*.voc"},
		{true,  "Berkeley/IRCAM/CARL files",        "*.ircam"},
		{true,  "Sonic Foundry's 64 RIFF/WAV files","*.w64"},
		{true,  "Matlab/GNU Octave files",          "*.mat4 *.mat5 *.mat"},
		{false, 0,0}
	};

	const FileFormatFilterList& EAudioFileFormat::ReadableFormats( )
	{
		static FileFormatFilterList mReadableFormatsList;
		
		if ( mReadableFormatsList.empty() )
		{
			for (unsigned i=0; filters[i].description; i++)
				mReadableFormatsList.push_back(
					FileFormatFilter( filters[i].description, filters[i].extensions));
		}
		return mReadableFormatsList;
	}

	const FileFormatFilterList& EAudioFileFormat::WritableFormats( )
	{
		static FileFormatFilterList mWritableFormatsList;
		
		if ( mWritableFormatsList.empty() )
		{
			for (unsigned i=0; filters[i].description; i++)
			{
				if (! filters[i].writeable) continue;
				mWritableFormatsList.push_back(
					FileFormatFilter( filters[i].description, filters[i].extensions));
			}
		}
		return mWritableFormatsList;
	}

}

