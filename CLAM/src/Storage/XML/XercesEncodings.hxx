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
#ifndef XERCES_ENCODINGS_HXX
#define XERCES_ENCODINGS_HXX


#include <xercesc/util/XMLString.hpp>

namespace xercesc=XERCES_CPP_NAMESPACE;

namespace CLAM
{
/**
 * Converts local encoding char* into Xerces Unicode String
 * @ingroup XmlBackends_Xerces
 */
class XercesLocal2Unicode
{
public :
	XercesLocal2Unicode(const char* const toTranscode)
	{
		// Call the private transcoding method
		_unicodeForm = xercesc::XMLString::transcode(toTranscode);
	}

	~XercesLocal2Unicode()
	{
		xercesc::XMLString::release(&_unicodeForm);
	}

	const XMLCh* unicodeForm() const
	{
		return _unicodeForm;
	}

private :
	XMLCh*   _unicodeForm;
};

/**
 * Converts Xerces Unicode String into local encoding char* 
 * @ingroup XmlBackends_Xerces
 */
class XercesUnicode2Local
{
public :
	XercesUnicode2Local(const XMLCh* const toTranscode)
	{
		// Call the private transcoding method
		_localForm = xercesc::XMLString::transcode(toTranscode);
	}

	~XercesUnicode2Local()
	{
		xercesc::XMLString::release(&_localForm);
	}

	const char* localForm() const
	{
		return _localForm;
	}

private :
	char*   _localForm;
};

#define U(str) ::CLAM::XercesLocal2Unicode(str).unicodeForm()
#define L(str) ::CLAM::XercesUnicode2Local(str).localForm()

}

#endif// XERCES_ENCODINGS_HXX

