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

#include "Assert.hxx"
#include <iostream>


int main (void) {
	
	CLAM_BEGIN_CHECK
		std::cout << "Executing the check" << std::endl;
	CLAM_END_CHECK

	CLAM_BEGIN_DEBUG_CHECK
		std::cout << "Executing the debug check" << std::endl;
	CLAM_END_DEBUG_CHECK
			
	try {
		CLAM_ASSERT(((std::cout<<"ExecutingAssertTrue"<<std::endl),true),"Casca el true");
	}
	catch (...) {
		std::cout << "True Throws exception" << std::endl;
	}
	try {
		CLAM_ASSERT(((std::cout<<"ExecutingAssertFalse"<<std::endl),false),"Casca el false");
	}
	catch (...) {
		std::cout << "False Throws exception" << std::endl;
	}


	try {
		CLAM_DEBUG_ASSERT(((std::cout<<"ExecutingAssertTrue"<<std::endl),true),"Casca el true");
	}
	catch (...) {
		std::cout << "True Throws exception" << std::endl;
	}
	try {
		CLAM_DEBUG_ASSERT(((std::cout<<"ExecutingAssertFalse"<<std::endl),false),"Casca el false");
	}
	catch (...) {
		std::cout << "False Throws exception" << std::endl;
	}


	
	return 0;
}

