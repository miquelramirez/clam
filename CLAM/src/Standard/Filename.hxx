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

#ifndef _FILENAME_H_
#define _FILENAME_H_
#include <exception>
#include <iosfwd>
#include <string>
#include "Assert.hxx"
#include "Component.hxx"
#include "TypeInfo.hxx"

namespace CLAM {

/**
 * This class represents a filename. 
 * You can do with it nearly whatever you can do with a string
 * but it has filename semantics in order to be diferentiated by
 * by type discriminators.
 * @todo Build a filename specific interface
 */
class Filename : public std::string 
{
	typedef std::string std_string;	
public:
		Filename() {}

		Filename(const std_string &s) : std_string(s) {} 

		Filename(const char * s) : std_string(s) {}
};

CLAM_TYPEINFOGROUP(BasicCTypeInfo, Filename);

}

#endif // _FILENAME_H_

