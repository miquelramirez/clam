/*
 * Copyright (c) 2008 Fundació Barcelona Media Universitat Pompeu Fabra
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

#include "SndfileWriter.hxx"
#include <CLAM/ProcessingFactory.hxx>


static const char * metadata[] = {
	"key", "SndfileWriter",
	"category", "[plugin] Sndfile Read/Write",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::SndfileWriter> registratorSndfile(metadata);

namespace CLAM
{
	
	EAudioFileWriter EAudioFileWriter::FormatFromFilename( std::string filename )
	{
		std::string::iterator dotPos = std::find( filename.begin(), filename.end(), '.' );
		if ( dotPos == filename.end() )
			return eDefault;
		std::string extension( dotPos+1, filename.end() );
		for(unsigned i=0; i<extension.size();++i)
			extension[i] = std::tolower(extension[i]);

		static struct { 
			const char * extension;
			Enum::tValue format;
		} extensionMap[] =
		{
			{"wav", ePCM_16},
			{"flac", eFLAC_16},
			{0,0}
		};

		for (unsigned i=0; extensionMap[i].extension; i++)
			if (extension==extensionMap[i].extension)
				return extensionMap[i].format;
		return eDefault;
	}
}
