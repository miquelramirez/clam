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

#include "mtgsstream.h" // An alias for <sstream>
#include "Polar.hxx"
#include "Assert.hxx"
#include <iostream>

int main () {
	std::stringstream s;
	CLAM::Polar p1(1,3), p2(7,7);
	CLAM_ASSERT(p1!=p2, "");
	std::stringstream ss;
	std::cout << p1 << " " << p2 << std::endl;
	
	ss << p1;
	ss >> p2;
	std::cout << p1 << " " << p2 << std::endl;
	CLAM_ASSERT(p1==p2, "Test Failed. Extraction operator over a complex had no efect");
	if ( p1==p2 )
		std::cout << "Test Passed" << std::endl;
	return 0;
}




