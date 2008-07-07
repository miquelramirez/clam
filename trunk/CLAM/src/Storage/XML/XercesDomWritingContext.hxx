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

#ifndef _XercesDomWritingContext_hxx_
#define _XercesDomWritingContext_hxx_

#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include "XercesEncodings.hxx"

namespace CLAM
{
class XercesDomDocumentHandler;

/**
 * Keeps the booking when mapping CLAM data into a single Xerces-C DOM element.
 * @ingroup XmlBackends_Xerces
 */
class XercesDomWritingContext
{
	xercesc::DOMElement * _currentElement;
	XercesDomWritingContext * _parent;
public:
	XercesDomWritingContext(xercesc::DOMElement * element)
	{
		_parent = 0;
		_currentElement = element;
	}
	XercesDomWritingContext(XercesDomDocumentHandler & docHandler);

	XercesDomWritingContext(XercesDomWritingContext * parent, const char * name)
	{
		_parent = parent;
		xercesc::DOMElement * parentElement = parent->_currentElement;
		_currentElement = parentElement->getOwnerDocument()->createElement(U(name));
		parentElement->appendChild(_currentElement);
	}

	XercesDomWritingContext * release()
	{
		return _parent;
	}
	
	void addAttribute(const char * name, const char * value)
	{
		_currentElement->setAttribute(U(name),U(value));
	}
	void addContent(const char * content)
	{
		xercesc::DOMText * domContent = _currentElement->getOwnerDocument()->createTextNode(U(content));
		_currentElement->appendChild(domContent);
	}
};

}

#endif//_XercesDomWritingContext_hxx_


