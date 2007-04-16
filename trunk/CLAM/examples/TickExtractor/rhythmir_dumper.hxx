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

#ifndef __rhythmir_dumper__
#define __rhythmir_dumper__

#include <string>
#include "Pool.hxx"

namespace CLAM
{
	class Pulse;
}

extern void DumpExtractedData( std::string filename, 
			       std::string analyzedFile,
			       const CLAM::Pulse& ticksData,
			       const CLAM::Pulse& beatsData );

namespace RhythmIR
{
	extern void DumpToSimacXML( CLAM::DescriptionDataPool& pool,
				    std::string outXMLFile );

	extern void DumpToWavesurferLabs( CLAM::DescriptionDataPool& pool, std::string outputFilename );					  
}

#endif // rhythmir_dumper.hxx

