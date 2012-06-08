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

#include "SerializationController.hxx"

#include <CLAM/Segment.hxx>

using namespace CLAM;

SerializationController::SerializationController(  ) { }

SerializationController::~SerializationController(  ) { }

bool SerializationController::DoSerialization( enum eAction action, Segment& segment, const char* fileName )
{
	switch( action ) {
	case Load:
		switch ( GetExtension( fileName ) ) {
		case XML :
			return mXMLSer.DoLoad( fileName, segment );
		case SDIF:
			return mSDIFSer.DoLoad( fileName, segment );
		default  :
			return false;
		}
		break;
		
	case Store:
		switch ( GetExtension( fileName ) ) {
		case XML :
			return mXMLSer.DoStore( fileName, segment );
		case SDIF:
			return mSDIFSer.DoStore( fileName, segment );
		default  :
			return false;
		}
	default  :
		return false;
	}
}

int SerializationController::GetExtension(const char* fileName )
{
	std::string strFileName( fileName );
	std::string ext = strFileName.substr( strFileName.length()-4, strFileName.length(  ) );

	if( ext == ".xml" )
		return XML;

	else if( ext == "sdif" )
		return SDIF;

	else return 0;
}

