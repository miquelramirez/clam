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
#ifndef LibXmlDomReader_hxx
#define LibXmlDomReader_hxx

#include "XmlStorageErr.hxx"

#include <string>
#include <list>
#include <sstream>
#include "Assert.hxx"
#include <libxml++/parsers/domparser.h>

namespace CLAM
{

/**
 * Controls the libxml++ DOM parser.
 * @ingroup XmlBackends_LibXml
 */
class LibXmlDomReader
{
	xmlpp::DomParser * parser;
	public:
		LibXmlDomReader()
		{
			// TODO: This is a hack for xerces bug on adoptNode, parser should be local variable
			// This should be done in the read method itself when the xerces but is solved
			parser = new xmlpp::DomParser;
		}
		~LibXmlDomReader()
		{
			// TODO: This is a hack for xerces bug on adoptNode, parser should be local variable
			if (parser) delete parser;
		}
		// TODO: This is a hack parser should be local variable
		xmlpp::DomParser * adoptParser()
		{
			xmlpp::DomParser * temp = parser;
			parser = 0;
			return temp;
		}
		xmlpp::Document * read(std::istream & target)
		{
			if (target.fail()) throw XmlStorageErr("Unable to open the document source");
			parser->set_validate(false);
			parser->set_substitute_entities();
			try
			{
				parser->parse_stream(target);
			}
			catch (xmlpp::parse_error & e)
			{
				throw XmlStorageErr(
					std::string("\nXML Parser Errors:\n")+e.what()+"\n");
			}
			xmlpp::Document *doc = parser->get_document();
			CLAM_ASSERT(doc,"No errors but document not loaded!");
			return doc;
		}
};



}
#endif//LibXmlDomReader_hxx

