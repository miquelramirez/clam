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
#ifndef _cppUnitHelper_hxx_
#define _cppUnitHelper_hxx_

#include <iostream>
#include <cppunit/TestSuite.h>

namespace CLAMTest
{
	class Helper
	{
	private:
		static void indent(unsigned n){
			for (unsigned i=0; i<n; i++) std::cout << " ";
		}
		typedef std::vector<CppUnit::Test *> Tests;

	public:
		static void printTestNames(const CppUnit::Test* test, const unsigned ind=0)
		{
			indent(ind);
			const CppUnit::TestSuite * suite = dynamic_cast<const CppUnit::TestSuite*>( test );
			if (suite) {
				std::cout << "+ " << suite->getName() << std::endl;
				Tests::const_iterator it;
				for (it=suite->getTests().begin(); it!=suite->getTests().end(); it++ )
					printTestNames(*it, ind+4);
			} else {
				std::cout << ". " << test->getName() << std::endl;
			}
			if (ind==0) {
				std::cout << "\n\n";
			}
		} 
	};

}; //namespace CLAMTest

#endif
