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


#ifndef _PointTmplDef_
#define _PointTmplDef_

#include <string>
#include <iostream>
#include <sstream>

namespace CLAM
{

	/**
	* @todo Not yet implemented
	* @todo Error conditions management
	*/
	template <class TX,class TY>
	inline std::istream& operator >> (std::istream & is,
			PointTmpl<TX,TY> & a)
	{
		if (is.flags() & std::ios::skipws) {
			char c = '\0';
			do
				is.get(c);
			while (is && isspace(c));
			if (is)	is.putback(c);
		}
		char c = '\0';
		is >> c;
		if (c!='{') {
			if (is)	is.putback(c);
//			std::cerr << "A point starting with '" << c << "'" << std::endl;
			return is;
		}
		TX x; 
		TY y;
		if (!(is >> x)) return is;
		if (!(is >> y)) return is;
		if (is.flags() & std::ios::skipws) {
			char c = '\0';
			do
				is.get(c);
			while (is && isspace(c));
			if (is) is.putback(c);
		}
		if (!is.get(c) || c!='}') return is;

		a.SetX(x);
		a.SetY(y);
		return is;
	}

	template <class TX,class TY>
	std::ostream& operator << (std::ostream& myStream, const PointTmpl<TX,TY>& a)
	{
		return myStream 
			<< "{"
			<< a.GetX()
			<< " "
			<< a.GetY()
			<< "}";
	}

} // namespace CLAM


#endif // _PointTmplDef_

