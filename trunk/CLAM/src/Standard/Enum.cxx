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

#include <iostream>
#include "Enum.hxx"

namespace CLAM {

std::ostream & operator << (std::ostream & os, const Enum & e) throw (IllegalValue) {
	os << e.GetString();
	return os;
}

/**
 * Loads a symbolic value from the input stream onto an Enum.
 * @param os The input stream
 * @param e The Enum
 * @returns The input stream
 */
std::istream & operator >> (std::istream & os, Enum & e) throw (IllegalValue) {
	std::string s;
	os >> s;
	e.SetValue(s);
	return os;
}

}
