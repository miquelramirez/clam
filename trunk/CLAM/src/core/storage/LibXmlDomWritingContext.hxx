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

#ifndef _LibXmlDomWritingContext_hxx_
#define _LibXmlDomWritingContext_hxx_

#include <libxml++/nodes/textnode.h>
#include <libxml++/nodes/element.h>
#include <libxml++/document.h>
#include "LibXmlEncodings.hxx"

namespace CLAM
{
class LibXmlDomDocumentHandler;

/**
 * Keeps the booking when mapping CLAM data into a single libxml++ DOM element.
 * @ingroup XmlBackends_LibXml
 */
class LibXmlDomWritingContext
{
	xmlpp::Element * _currentElement;
	LibXmlDomWritingContext * _parent;
public:
	LibXmlDomWritingContext(xmlpp::Element * element)
	{
		_parent = 0;
		_currentElement = element;
	}
	LibXmlDomWritingContext(LibXmlDomDocumentHandler & docHandler);

	LibXmlDomWritingContext(LibXmlDomWritingContext * parent, const char * name)
	{
		_parent = parent;
		xmlpp::Element * parentElement = parent->_currentElement;
		_currentElement = parentElement->add_child(U(name));
	}

	LibXmlDomWritingContext * release()
	{
		return _parent;
	}
	
	void addAttribute(const char * name, const char * value)
	{
		_currentElement->set_attribute(U(name),U(value));
	}
	void addContent(const char * content)
	{
		_currentElement->add_child_text(U(content));
	}
};

}

#endif//_LibXmlDomWritingContext_hxx_


