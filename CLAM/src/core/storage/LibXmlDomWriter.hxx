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
#ifndef _LibXmlDomWriter_hxx_
#define _LibXmlDomWriter_hxx_

#include <iosfwd>
#include <libxml++/document.h>

namespace xmlpp
{
	class Document;
	class Node;
}

namespace CLAM
{

/**
 * Controls the libxml++ DOM serializer.
 * @ingroup XmlBackends_LibXml
 * @todo dumping selections
 */
class LibXmlDomWriter
{
	public:
		LibXmlDomWriter()
		{
			mShouldIndent=false;
			mShouldCanonicalize=false;
		}

		void write(std::ostream & target, xmlpp::Node * node)
		{
			xmlpp::Document document;
			document.create_root_node_by_import(node);
			write(target, &document);
		}

		void write(std::ostream & target, xmlpp::Document * node)
		{
			if (mShouldIndent)
				node->write_to_stream_formatted(target,"UTF-8");
			else
				node->write_to_stream(target,"UTF-8");
		}

		void DoCanonicalFormat(bool shouldDo = true)
		{
			mShouldCanonicalize=shouldDo;
		}
		void DoIndentedFormat(bool shouldDo = true)
		{
			mShouldIndent=shouldDo;
		}
	private:
		bool mShouldIndent;
		bool mShouldCanonicalize;
};



}
#endif//_LibXmlDomWriter_hxx_

