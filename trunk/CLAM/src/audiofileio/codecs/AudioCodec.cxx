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

#include "AudioCodec.hxx"
#include "Assert.hxx"
#include <cstdio>

namespace CLAM
{

namespace AudioCodecs
{
	Codec::~Codec()
	{
		
	}

	bool Codec::FileExists( std::string filename )
	{
		//CLAM_DEBUG_ASSERT( filename.length() > 0, "Unnamed files never exist" );

		// MRJ: Quite weak file existence check

		if ( filename.length() <= 0 )
			return false;

		FILE* fileHandle = NULL;

		fileHandle = fopen( filename.c_str(), "rb" );
		
		if ( !fileHandle )
			return false;
		
		fclose( fileHandle );

		return true;
	}

	void   Codec::RetrieveTextDescriptors( std::string uri, AudioTextDescriptors& tdesc )
	{
		//CLAM_WARNING( false, "Text descriptors retrieval is not implemented for this kind of files!" );
	}
}

}

