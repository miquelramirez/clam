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

#ifndef _LibXmlDomReadingContext_hxx_
#define _LibXmlDomReadingContext_hxx_

#include "Assert.hxx"
#include "LibXmlEncodings.hxx"
#include <libxml++/libxml++.h>
#include <sstream>
#include <list>
#include <string>

namespace CLAM 
{
class LibXmlDomDocumentHandler;

/**
 * Keeps the booking when loading one LibXml DOM single element into CLAM data.
 * @ingroup XmlBackends_LibXml
 */
class LibXmlDomReadingContext
{
	LibXmlDomReadingContext * _parentContext;
	xmlpp::Element * _context;
	xmlpp::Node::NodeList _children;
	xmlpp::Node::NodeList::iterator _currentChild;
	xmlpp::Element::AttributeList _attributes;
	std::stringstream _plainContentToParse;
	std::list<std::string> _errors;

public:
	LibXmlDomReadingContext(xmlpp::Element * element)
	{
		_parentContext=0;
		setAt(element);
	}

	// TODO: Test this
	LibXmlDomReadingContext(LibXmlDomDocumentHandler & docHandler);

	LibXmlDomReadingContext(LibXmlDomReadingContext * oldContext, const char * name)
	{
		_parentContext=oldContext;
		setAt(oldContext->fetchElement(name));
	}
	void setAt(xmlpp::Element * element)
	{
		_context = element;
		_children = _context->get_children();
		_attributes = _context->get_attributes();
		_currentChild=_children.begin();
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
		if (_currentChild==_children.end()) return false; // No nodes left
		xmlpp::Element * child = dynamic_cast<xmlpp::Element*>(*_currentChild);
		CLAM_ASSERT(child,
			"Can't change the context to a non element node");
		if (child->get_name()!=U(name)) return false; // Name mismatch
		return true;
	}
	/**
	 * Preconditions are asured when findElement returns true
	 * @pre There is no non-space content left
	 * @pre There is a current node left to explore
	 * @pre The current node is an element
	 * @pre The current node has name as xml name
	 */
	xmlpp::Element * fetchElement(const char * name)
	{
		bool hasContentLeft = contentLeft();
		CLAM_ASSERT(!hasContentLeft, "Fetching element with content left");
		CLAM_ASSERT(_currentChild!=_children.end(),
			"Accessing beyond DOM nodes");
		xmlpp::Element * child = dynamic_cast<xmlpp::Element *>(*_currentChild);
		CLAM_ASSERT(child,
			"Can't change the context to a non element node");
		CLAM_ASSERT(L(child->get_name())==name,
			"XML element name should be the one expected");
		_currentChild++;
		fetchContent();
		return child;
	}

	LibXmlDomReadingContext * release()
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
		for (int c=_plainContentToParse.get(); not _plainContentToParse.eof(); c=_plainContentToParse.get())
			os.put(c);
		os << "' at position ";
		os << getPath();
		_errors.push_back(os.str());
	}
	void checkNoElementLeftOrError()
	{
		if (_currentChild==_children.end()) return;
		xmlpp::Element * child = dynamic_cast<xmlpp::Element*>(*_currentChild);
		/*
		if (!child)
			_errors.push_back("Unexpected node type");
		*/

		std::ostringstream os;
		os << "Unexpected Element: '";
		os << L(child->get_name());
		os << "' at position ";
		os << getPath();

		_errors.push_back(os.str());
	}

	bool extractAttribute(const char * attributeName, std::ostream & os)
	{
		xmlpp::Attribute * attribute =
			_context->get_attribute(U(attributeName));
		if (!attribute) return false;
		os << L(attribute->get_value()) << std::flush;
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
		for (; _currentChild!=_children.end(); _currentChild++)
		{
			xmlpp::Node * child= *_currentChild;
			if (dynamic_cast<xmlpp::CommentNode*>(child)) continue;
			xmlpp::TextNode * textNode = dynamic_cast<xmlpp::TextNode*>(child);
			if (!textNode) break;
			_plainContentToParse << L(textNode->get_content());
		}
		_plainContentToParse << std::flush;
		contentLeft();
	}

	/**
	 * Returns true if there is non-space content on the content stream.
	 * As side effect it skips any space (including \\n \\t \\r...) caracters.
	 */
	bool contentLeft()
	{
		int c = _plainContentToParse.peek();
		while (not _plainContentToParse.eof())
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
		return L(_context->get_path());
		// TODO: Remove the by hand way
		std::string path;
		if (_parentContext) path=_parentContext->getPath();
		path += '/';
		path += L(_context->get_name());
		return path;
	}
};

} // Namespace CLAM

#endif//_LibXmlDomReadingContext_hxx_



