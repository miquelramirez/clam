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
#ifndef _XERCES_DOM_WRITER_HXX_
#define _XERCES_DOM_WRITER_HXX_

#include <iosfwd>
#include <libxml++/document.h>

namespace xmlpp
{
	class Document;
}

namespace CLAM
{

class LibXmlDomWriter
{
	public:
		LibXmlDomWriter()
		{
			mShouldIndent=false;
			mShouldCanonicalize=false;
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
#endif//_XERCES_DOM_WRITER_HXX_

