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
#ifndef _XercesDomDocumentHandler_hxx_
#define _XercesDomDocumentHandler_hxx_

#include "XercesEncodings.hxx"
#include "XercesDomReadingContext.hxx"
#include "XercesDomWritingContext.hxx"
#include "XercesDomReader.hxx"
#include "XercesDomWriter.hxx"
#include "XercesInitializer.hxx"


#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

namespace CLAM 
{
class XercesDomReadingContext;
class XercesDomWritingContext;

/**
 * XML implementation backend for the Xerces-C library.
 * @ingroup XmlBackends_Xerces
 */
class XercesDomDocumentHandler
{
public:
	typedef XercesDomWritingContext WritingContext;
	typedef XercesDomReadingContext ReadingContext;
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
	xercesc::DOMDocument * _document;
	xercesc::DOMElement * _selection;
	xercesc::XercesDOMParser * _parser;
public:
	XercesDomDocumentHandler()
	{
		_document = 0;
		_selection = 0;
		_parser = 0;
	}
	~XercesDomDocumentHandler()
	{
		releaseIfAnyDocument();
	}
	void setDocument(xercesc::DOMDocument * document)
	{
		_document = document;
		_selection = _document->getDocumentElement();
	}
	void selectPath(const char * path)
	{
		if (path[0]!='/') 
			_selection = recursiveSelection(_selection, path, 0);
		else
			_selection = absoluteSelection(path);
		return;
	}
	
	xercesc::DOMElement * getSelection()
	{
		return _selection;
	}
	void create(const char * rootName)
	{
		XercesInitializer::require();
		releaseIfAnyDocument();
		xercesc::DOMImplementation * imp =
			xercesc::DOMImplementation::getImplementation();
		xercesc::DOMDocument * domDoc = imp->createDocument(0,U(rootName),0);
		setDocument(domDoc);
	}
	void read(std::istream & stream)
	{
		XercesDomReader reader;
		xercesc::DOMDocument * domDoc;
		domDoc = reader.read(stream);
		setDocument(domDoc);
		_parser = reader.adoptParser();
	}
	void writeDocument(std::ostream & os, bool useIndentation=false)
	{
		XercesDomWriter writer;
		writer.DoIndentedFormat(useIndentation);
		writer.write(os,_document);
	}
	void writeSelection(std::ostream & os, bool useIndentation=false)
	{
		XercesDomWriter writer;
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
			_document->release();
		_parser=0;
		_document=0;
	}
	xercesc::DOMElement * absoluteSelection(const std::string & path)
	{
		xercesc::DOMElement * root = (xercesc::DOMElement*) _document->getDocumentElement();
		unsigned int nextSlash = std::string(path).find('/',1);
		std::string rootStep = std::string(path).substr(1,nextSlash-1);
		std::string rootName = L(root->getNodeName());
		if (rootStep=="") return root;

		if (rootName== rootStep)
			return recursiveSelection(root, path, nextSlash+1);

		throw XmlStorageErr("Wrong root name, expected '"+rootStep+"' but found '"+rootName+"'");
	}
	xercesc::DOMElement * recursiveSelection(xercesc::DOMElement * current, const std::string & path, unsigned int pos)
	{
		if (pos >= path.length()) return current;
		unsigned int slashPosition = path.find('/', pos);
		unsigned int stepSize = 
			slashPosition == std::string::npos ? 
				std::string::npos : slashPosition-pos;
		std::string step = path.substr(pos, stepSize);
		for (
			xercesc::DOMNode * child = current->getFirstChild();
			child;
			child = child->getNextSibling())
		{
			if (child->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue;
			std::string nodeName(L(child->getNodeName()));
			if (nodeName != step) continue;
			if (slashPosition==std::string::npos) return (xercesc::DOMElement *) child;
			return recursiveSelection((xercesc::DOMElement *) child, path, slashPosition+1);
		}
		std::string msg = "Wrong path step '" + step + "'";
		throw XmlStorageErr(msg);
	}

};


}

/**
 * @defgroup XmlBackends_Xerces XML Backend for Apache's Xerces-C
 * @ingroup XmlBackends
 */

#endif//_XercesDomDocumentHandler_hxx_

