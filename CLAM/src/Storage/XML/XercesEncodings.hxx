/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

class Local2Unicode
{
public :
	Local2Unicode(const char* const toTranscode)
	{
		// Call the private transcoding method
		_unicodeForm = xercesc::XMLString::transcode(toTranscode);
	}

	~Local2Unicode()
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

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class Unicode2Local
{
public :
    Unicode2Local(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        _localForm = xercesc::XMLString::transcode(toTranscode);
    }

    ~Unicode2Local()
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

#define X(str) Local2Unicode(str).unicodeForm()
#define L(str) Unicode2Local(str).localForm()

#endif// XERCES_ENCODINGS_HXX

