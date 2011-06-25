/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _TEXT_HXX_
#define _TEXT_HXX_
#include <iosfwd>
#include <string>
#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )
#include <sstream>
#endif
#include "TypeInfo.hxx"

namespace CLAM
{
	/**
	 * This class represents an string that can contain any kind of separators.
	 * Use it instead std::string if you want to deserialize
	 * It inherits all the std::string methods.
	 */
	class Text : public std::basic_string<char>
	{
		public:
			Text()
			{
			}
			Text(const std::string & value)
				: std::basic_string<char>(value)
			{
			}
			Text(const char * value)
				: std::basic_string<char>(value)
			{
			}
	};

	std::istream & operator >> (std::istream & stream, Text & text);

#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )
	/**
	* This is a kludge to make windows work
	*/

	inline std::istream& operator>>( std::istringstream& stream, CLAM::Text& text )
	{
		std::istream & basestream = stream;
		return basestream >> text;
	}
#endif

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Text);
}


#endif // _TEXT_HXX_

