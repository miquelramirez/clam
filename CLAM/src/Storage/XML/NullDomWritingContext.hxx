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

#ifndef _NullDomWritingContext_hxx_
#define _NullDomWritingContext_hxx_

#include "Assert.hxx"

class NullDomDocumentHandler;


namespace CLAM
{

/**
 * Dummy implementation of a DomDocumentWritingContext when XML is disabled.
 * @ingroup XmlBackends_Null
 */
class NullDomWritingContext
{
	NullDomWritingContext * _parent;
public:
	NullDomWritingContext(NullDomDocumentHandler & docHandler)
	{
		CLAM_ASSERT(false, "Using XML support, but it is disabled on compile time (CLAM_USE_XML=0).");
	}

	NullDomWritingContext(NullDomWritingContext * parent, const char * name)
	{
		CLAM_ASSERT(false, "Using XML support, but it is disabled on compile time (CLAM_USE_XML=0).");
	}

	NullDomWritingContext * release()
	{
		return 0;
	}
	
	void addAttribute(const char * name, const char * value)
	{
	}
	void addContent(const char * content)
	{
	}
};

}


#endif//_NullDomWritingContext_hxx_


