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
#ifndef XercesDomReader_hxx
#define XercesDomReader_hxx

#include "XercesEncodings.hxx"
#include "XercesInitializer.hxx"
#include "XmlStorageErr.hxx"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <string>
#include <list>
#include <sstream>
#include "Assert.hxx"

namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{

/**
 * Controls the Xerces-C DOM parser.
 * @ingroup XmlBackends_Xerces
 */
class XercesDomReader : private xercesc::HandlerBase
{
	xercesc::XercesDOMParser * parser;
	public:
		XercesDomReader()
		{
			// This should be done in the read method itself when the xerces but is solved
			XercesInitializer::require();
			parser = new xercesc::XercesDOMParser();
		}
		~XercesDomReader()
		{
			// TODO: This is a hack for xerces bug on adoptNode, parser should be local variable
			if (parser)
				delete parser;
		}
		// TODO: This is a hack parser should be local variable
		xercesc::XercesDOMParser * adoptParser()
		{
			xercesc::XercesDOMParser * temp = parser;
			parser = 0;
			return temp;
		}
		xercesc::DOMDocument * read(std::istream & target)
		{
			if (target.fail()) throw XmlStorageErr("Unable to open the document source");
			parser->setErrorHandler(this);
			parser->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
			parser->setValidationSchemaFullChecking(true);
			parser->setDoNamespaces(true);
			parser->setDoSchema(true);
			parser->setCreateEntityReferenceNodes(true);

			std::ostringstream stream;
			char c;
			while (true)
			{
				target.get(c);
				if(!target.good()) break;
				stream.put(c);
			}
			std::string temp = stream.str();
			unsigned length = temp.length();
			const char * documentText = temp.c_str();
			xercesc::MemBufInputSource xercesInputSource (
				(const XMLByte*)documentText
				, length
				, "CLAMParser"
				, false
				);

			xercesInputSource.setCopyBufToStream(false);

			parser->parse(xercesInputSource);

			if (parser->getErrorCount())
				throw XmlStorageErr(
					(std::string("\nXML Parser Errors:\n")+
					 RecopilaErrors()).c_str());
			xercesc::DOMDocument *doc = parser->getDocument();
			CLAM_ASSERT(doc,"No errors but document not loaded!");
			return doc;
		}
	private:
		typedef std::list<std::string> Missatges;
		Missatges _errors;
		void error(const xercesc::SAXParseException& e)
		{
			report("Error", e);
		}

		void fatalError(const xercesc::SAXParseException& e)
		{
			report("Fatal Error", e);
		}

		void warning(const xercesc::SAXParseException& e)
		{
			report("Warning", e);
		}

		void report(const std::string & level, const xercesc::SAXParseException& e)
		{
			std::ostringstream stream;
			stream << level << " at file " << L(e.getSystemId())
				<< ", line " << e.getLineNumber()
				<< ", col " << e.getColumnNumber()
				<< ":\n" << L(e.getMessage());
			_errors.push_back(stream.str());
		}
	public:
		std::string RecopilaErrors()
		{
			std::string result;
			Missatges::iterator it = _errors.begin();
			for (; it!=_errors.end(); it++)
				result += *it + "\n";
			return result;
		}
};



}
#endif//XercesDomReader_hxx

