/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <iostream>
#include <exception>
#include <string>
#include "Err.hxx"
#include "Segment.hxx"
#include "XMLStorage.hxx"

void getFileFromArgs( int argc, char** argv, std::string& filename )
{
	if ( argc == 1 ) // no arguments provided
	{
		std::cerr << "Insufficient parameters: syntax:" << std::endl;
		std::cerr << "TakingSnapshots_2 <xml file>"<< std::endl;
		exit(-1);
	}
	else if ( argc == 2 )
	{
		filename = argv[1];
	}
	else
	{
		std::cerr << "Too many parameters: syntax:" << std::endl;
		std::cerr << "TakingSnapshots_2 <xml file>" << std::endl;
		exit(-1);
	}
}

void restoreObject( const std::string& filename, CLAM::Segment& obj )
{
	CLAM::XMLStorage loader;

	loader.Restore( obj, filename );
}

int main( int argc, char** argv )
{
	try
	{
		// the filename to the .xml file which contains the stored
		// CLAM::Segment object
		std::string   filename;
		// the 'live' object onto we will restore the Segment object
		// in filename
		CLAM::Segment restoredSegment;

		// we retrieve from the program arguments the name of the xml file
		getFileFromArgs( argc, argv, filename );

		// we restore the Segment contained in filename
		restoreObject( filename, restoredSegment );
		

	}
	catch( CLAM::Err& e )
	{
		std::cerr << "CLAM exception caught:" << std::endl;
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << "Standard Library exception caught" << std::endl;
		std::cerr << e.what() << std::endl;
		exit(-1);
	}
	catch( ... )
	{
		std::cerr << "Unknown exception caught" << std::endl;
		exit(-1);
	}

	return 0;
}
