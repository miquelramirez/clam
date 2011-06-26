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

#ifndef _NullDomReadingContext_hxx_
#define _NullDomReadingContext_hxx_


// Dummy implementation that will fail when used

#include "Assert.hxx"
#include <sstream>
#include <list>
#include <string>

namespace CLAM 
{

class NullDomDocumentHandler;

/**
 * Dummy implementation of a DomDocumentReadingContext when XML is disabled.
 * @ingroup XmlBackends_Null
 */
class NullDomReadingContext
{
	std::stringstream _plainContentToParse;

public:
	NullDomReadingContext(NullDomDocumentHandler & docHandler)
	{
		CLAM_ASSERT(false, "Using XML support with no xmlbackend. Reconfigure CLAM using the 'xmlbackend' option.");
	}

	NullDomReadingContext(NullDomReadingContext * oldContext, const char * name)
	{
		CLAM_ASSERT(false, "Using XML support with no xmlbackend. Reconfigure CLAM using the 'xmlbackend' option.");
	}
	bool findElement(const char * name)
	{
		return false;
	}

	NullDomReadingContext * release()
	{
		return this;
	}

	bool extractAttribute(const char * attributeName, std::ostream & os)
	{
		return false;
	}

	std::istream & reachableContent()
	{
		return _plainContentToParse;
	}

	std::list<std::string> errors()
	{
		static std::list<std::string> errors;
		return errors;
	}

	std::string getPath()
	{
		return "Bad Path";
	}

};

} // Namespace CLAM


#endif//_NullDomReadingContext_hxx_



