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

#ifndef _XercesDomReadingContext_hxx_
#define _XercesDomReadingContext_hxx_

#include "Assert.hxx"
#include "XercesEncodings.hxx"
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <sstream>
#include <list>
#include <string>

namespace CLAM 
{
class XercesDomDocumentHandler;

/**
 * Keeps the booking when loading a single Xerces-C DOM element into CLAM data.
 * @ingroup XmlBackends_Xerces
 */
class XercesDomReadingContext
{
	xercesc::DOMElement * _context;
	xercesc::DOMNodeList * _children;
	xercesc::DOMNamedNodeMap * _attributes;
	std::stringstream _plainContentToParse;
	unsigned int _currentChild;
	XercesDomReadingContext * _parentContext;
	std::list<std::string> _errors;

//	std::list<std::string> & _currentPath;
public:
	XercesDomReadingContext(xercesc::DOMElement * element)
	{
		_parentContext=0;
		setAt(element);
	}
	// TODO: Test this
	XercesDomReadingContext(XercesDomDocumentHandler & docHandler);

	XercesDomReadingContext(XercesDomReadingContext * oldContext, const char * name)
	{
		_parentContext=oldContext;
		setAt(oldContext->fetchElement(name));
	}
	void setAt(xercesc::DOMElement * element)
	{
		_context = element;
		_children = _context->getChildNodes();
		_attributes = _context->getAttributes();
		_currentChild=0;
		fetchContent();
	}

	/**
	 * Returns true when the next DOM element to be read is an element
	 * with the required name.
	 * Returns false when all the nodes have been read.
	 * Returns false when there is content left before the next element.
	 * Returns false when the next DOM element has a different name.
	 * @pre There is only DOMElements and already fetched DOMText
	 */
	bool findElement(const char * name)
	{
		if (contentLeft()) return false;
		if (_currentChild==_children->getLength()) return false; // No nodes left

		xercesc::DOMNode * child = _children->item(_currentChild);
		CLAM_ASSERT(child->getNodeType() == xercesc::DOMNode::ELEMENT_NODE,
			"Can't change the context to a non element node");
		if (!xercesc::XMLString::equals(child->getNodeName(), U(name))) return false; // Name mismatch
		return true;
	}

	/**
	 * Preconditions are asured when findElement returns true
	 * @pre There is no non-space content left
	 * @pre There is a current node left to explore
	 * @pre The current node is an element
	 * @pre The current node has name as xml name
	 */
	xercesc::DOMElement * fetchElement(const char * name)
	{
		bool hasContentLeft;
		hasContentLeft = contentLeft();
		CLAM_ASSERT(!hasContentLeft, "Fetching element with content left");
		CLAM_ASSERT(_currentChild!=_children->getLength(),
			"Accessing beyond DOM nodes");
		xercesc::DOMNode * child = _children->item(_currentChild);
		CLAM_ASSERT(child->getNodeType() == xercesc::DOMNode::ELEMENT_NODE,
			"Can't change the context to a non element node");
		CLAM_ASSERT(xercesc::XMLString::equals(child->getNodeName(), U(name)),
			"XML element name should be the one expected");
		_currentChild++;
		fetchContent();
		return dynamic_cast<xercesc::DOMElement *>(child);
	}

	XercesDomReadingContext * release()
	{
		checkNoContentLeftOrError();
		checkNoElementLeftOrError();
		return _parentContext;
	}

	void checkNoContentLeftOrError()
	{
		if (!contentLeft()) return;
		std::ostringstream os;
		os << "Unexpected content: '";
		for (int c=_plainContentToParse.get(); c!=EOF; c=_plainContentToParse.get())
			os.put(c);
		os << "' at position ";
		os << getPath();
		_errors.push_back(os.str());
	}

	void checkNoElementLeftOrError()
	{
		if (_currentChild>=_children->getLength()) return;
		xercesc::DOMNode * child = _children->item(_currentChild);
		/*
		if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
		{
			_errors.push_back("Unexpected node type");
		}
		*/

		std::ostringstream os;
		os << "Unexpected Element: '";
		os << L(child->getNodeName());
		os << "' at position ";
		os << getPath();

		_errors.push_back(os.str());
	}

	bool extractAttribute(const char * attributeName, std::ostream & os)
	{
		xercesc::DOMNode * attribute =
			_attributes->getNamedItem(U(attributeName));
		if (!attribute) return false;
		os << L(attribute->getNodeValue()) << std::flush;
		return true;
	}

	std::istream & reachableContent()
	{
		return _plainContentToParse;
	}

	/**
	 * Dumps the reachable content of text nodes onto the content stream.
	 * Reachable means continuous Text which may have XML comments inside.
	 * As side effect trims initial spaces on content
	 */
	void fetchContent()
	{
//		_plainContentToParse.clear(); // Clear any error flag
		for (; _currentChild<_children->getLength(); _currentChild++)
		{
			xercesc::DOMNode * child= _children->item(_currentChild);
			if (child->getNodeType() == xercesc::DOMNode::COMMENT_NODE) continue;
			if (child->getNodeType() != xercesc::DOMNode::TEXT_NODE) break;
			_plainContentToParse << L(child->getNodeValue());
		}
		_plainContentToParse << std::flush;
		contentLeft();
	}

	/**
	 * Returns true if there is non-space content on the content stream.
	 * As side effect it skips any space (including \n \t \r...) caracters.
	 */
	bool contentLeft()
	{
		int c = _plainContentToParse.peek();
		while (c != EOF)
		{
			if (!isspace(c)) return true;
			_plainContentToParse.ignore();
			c = _plainContentToParse.peek();
		}
		_plainContentToParse.clear();
		return false;
	}
	std::list<std::string> errors()
	{
		return _errors;
	}

	std::string getPath()
	{
		std::string path;
		if (_parentContext) path=_parentContext->getPath();
		path += '/';
		path += L(_context->getNodeName());
		return path;
	}

};

} // Namespace CLAM

#endif//_XercesDomReadingContext_hxx_



