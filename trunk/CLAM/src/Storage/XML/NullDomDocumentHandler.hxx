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
#ifndef _NullDomDocumentHandler_hxx_
#define _NullDomDocumentHandler_hxx_

#include "NullDomReadingContext.hxx"
#include "NullDomWritingContext.hxx"

namespace CLAM
{

/**
 * Dummy implementation of a DomDocumentHandler when XML is disabled.
 * @ingroup XmlBackends_Null
 */
class NullDomDocumentHandler
{
public:
	typedef NullDomWritingContext WritingContext;
	typedef NullDomReadingContext ReadingContext;
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
public:
	NullDomDocumentHandler()
	{
		CLAM_ASSERT(false, "Using XML support with no xmlbackend. Reconfigure CLAM using the 'xmlbackend' option.");
	}
	~NullDomDocumentHandler()
	{
	}
	void selectPath(const char * path)
	{
	}
	void create(const char * rootName)
	{
	}
	void read(std::istream & stream)
	{
	}
	void writeDocument(std::ostream & os, bool useIndentation=false)
	{
	}
	void writeSelection(std::ostream & os, bool useIndentation=false)
	{
	}
};

}
/**
 * @defgroup XmlBackends_Null XML Backend to compile with disabled XML
 * @ingroup XmlBackends
 */

#endif//_NullDomDocumentHandler_hxx_

