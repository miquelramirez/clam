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

#include "BasicXMLable.hxx"
#include <iostream>

using namespace CLAM;

BasicXMLable::~BasicXMLable() 
{
}

BasicXMLable::BasicXMLable (const char * name, bool isXMLElement)
{
	CLAM_BEGIN_CHECK
	if (name) {
		CLAM_ASSERT(*name, "Empty XML label.");
		for (const char * c=name; *c; c++) {
			if (isalpha(*c)) continue;
			// Add here chars to allow as first letter
			CLAM_ASSERT (c!=name, "Illegal start for an XML label, it needs a letter.");
			
			if (isdigit(*c)) continue;
			if (*c=='-') continue;
			if (*c=='_') continue;
			if (*c==':') continue;
			// TODO: Add here chars to allow only as non-first letter
			CLAM_ASSERT (false, "XML label defined with illegal character");
		}
	}
	CLAM_END_CHECK
	amIXMLElement=isXMLElement;
	myXMLName=name;
}

//* Check the internal status for a class instance is valid
bool BasicXMLable::FulfilsInvariant() 
{
	if (myXMLName || !amIXMLElement) {
		std::cerr << "BasicXMLable Invariant failed: An element without name." << std::endl;
		return false;
	}
	return true;
};

