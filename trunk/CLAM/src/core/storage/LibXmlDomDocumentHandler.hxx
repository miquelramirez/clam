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
#ifndef _LibXmlDomDocumentHandler_hxx_
#define _LibXmlDomDocumentHandler_hxx_

#include "LibXmlDomReadingContext.hxx"
#include "LibXmlDomWritingContext.hxx"
#include "LibXmlDomReader.hxx"
#include "LibXmlDomWriter.hxx"


#include <libxml++/libxml++.h>

namespace CLAM 
{
class LibXmlDomReadingContext;
class LibXmlDomWritingContext;
/**
 * XML implementation backend for the libxml++ library.
 * @ingroup XmlBackends_LibXml
 */
class LibXmlDomDocumentHandler
{
public:
	typedef LibXmlDomWritingContext WritingContext;
	typedef LibXmlDomReadingContext ReadingContext;
	ReadingContext * GetReadingContext()
	{
		return _currentReadContext;
	}
	void SetReadingContext(ReadingContext * context)
	{
		_currentReadContext = context;
	}
	WritingContext * GetWritingContext()
	{
		return _currentWriteContext;
	}
	void SetWritingContext(WritingContext * context)
	{
		_currentWriteContext = context;
	}
private:
	ReadingContext * _currentReadContext;
	WritingContext * _currentWriteContext;
	xmlpp::Document * _document;
	xmlpp::Element * _selection;
	xmlpp::DomParser * _parser;
public:
	LibXmlDomDocumentHandler()
	{
		_document = 0;
		_selection = 0;
		_parser = 0;
	}
	~LibXmlDomDocumentHandler()
	{
		releaseIfAnyDocument();
	}
	void setDocument(xmlpp::Document * document)
	{
		_document = document;
		_selection = _document->get_root_node();
	}
	void selectPath(const char * path)
	{
		if (path[0]!='/') 
			_selection = recursiveSelection(_selection, path, 0);
		else
			_selection = absoluteSelection(path);
		return;
	}
	xmlpp::Element * getSelection()
	{
		return _selection;
	}
	void create(const char * rootName)
	{
		releaseIfAnyDocument();
		
		xmlpp::Document * domDoc = new xmlpp::Document;
		domDoc->create_root_node(rootName);
		setDocument(domDoc);
	}
	void read(std::istream & stream)
	{
		LibXmlDomReader reader;
		xmlpp::Document * domDoc;
		domDoc = reader.read(stream);
		setDocument(domDoc);
		_parser = reader.adoptParser();
	}
	void writeDocument(std::ostream & os, bool useIndentation=false)
	{
		LibXmlDomWriter writer;
		writer.DoIndentedFormat(useIndentation);
		writer.write(os,_document);
	}
	void writeSelection(std::ostream & os, bool useIndentation=false)
	{
		LibXmlDomWriter writer;
		writer.DoIndentedFormat(useIndentation);
		writer.write(os,_selection);
	}
private:
	void releaseIfAnyDocument()
	{
		if (!_document) return;

		if (_parser)
			delete _parser;
		else
			delete _document;
		_parser=0;
		_document=0;
	}
	xmlpp::Element * absoluteSelection(const std::string & path)
	{
		xmlpp::Element * root = _document->get_root_node();
		unsigned int nextSlash = std::string(path).find('/',1);
		std::string rootStep = std::string(path).substr(1,nextSlash-1);
		std::string rootName = root->get_name();
		if (rootStep=="") return root;

		if (rootName== rootStep)
			return recursiveSelection(root, path, nextSlash+1);

		throw XmlStorageErr("Wrong root name, expected '"+rootStep+"' but found '"+rootName+"'");
	}
	xmlpp::Element * recursiveSelection(xmlpp::Element * current, const std::string & path, unsigned int pos)
	{
		if (pos >= path.length()) return current;
		unsigned int slashPosition = path.find('/', pos);
		unsigned int stepSize = 
			slashPosition == std::string::npos ? 
				std::string::npos : slashPosition-pos;
		std::string step = path.substr(pos, stepSize);
		xmlpp::Element::NodeList children = current->get_children();
		for (
			xmlpp::Element::NodeList::iterator child = children.begin();
			child!=children.end();
			child++)
		{
			xmlpp::Element * element = dynamic_cast<xmlpp::Element*>(*child);
			if (!element) continue;
			std::string nodeName= element->get_name();
			if (nodeName != step) continue;
			if (slashPosition==std::string::npos) return element;
			return recursiveSelection(element, path, slashPosition+1);
		}
		std::string msg = "Wrong path step '" + step + "'";
		throw XmlStorageErr(msg);
	}
};


}

/**
 * @defgroup XmlBackends_LibXml XML Backend for Gnome's libxml++
 * @ingroup XmlBackends
 */
#endif//_LibXmlDomDocumentHandler_hxx_

