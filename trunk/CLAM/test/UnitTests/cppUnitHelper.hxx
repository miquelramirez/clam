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
#include <typeinfo>
//#include <cstddef> seems not necessary //definition of std::size_t
#include <cppunit/TestSuite.h>
#include <cppunit/TestAssert.h>


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
			
			const CppUnit::TestSuite * suite = dynamic_cast<const CppUnit::TestSuite*>( test );
			if (suite) {
				std::cout << std::endl;
				indent(ind);
				std::cout << "+ " << suite->getName() << std::endl;
				Tests::const_iterator it;
				for (it=suite->getTests().begin(); it!=suite->getTests().end(); it++ )
					printTestNames(*it, ind+4);
			} else {
				std::string testName = test->getName();
				indent(ind);
				std::cout << ". "
					<< testName.replace(0,testName.find(".")+1,"")
					<< std::endl;
			}
			if (ind==0) {
				std::cout << "\n\n";
			}
		} 
	};

#define CLAMTEST_ASSERT_EQUAL_RTTYPES( expected, actual ) \
	CPPUNIT_ASSERT_EQUAL( \
		typeid(expected), \
		typeid(actual) )

} //namespace CLAMTest


// Helper traits for assertions
namespace CppUnit
{
	template<>
	struct assertion_traits< std::type_info >
	{
		static bool equal( const std::type_info& x, const std::type_info& y )
		{
			return 0!=(x == y);
		}

		static std::string toString( const std::type_info& x )
		{
			std::string text = std::string("'") + x.name() + "'";    // adds quote around the string to see whitespace
			CppUnit::OStringStream ost;
			ost << text;
			return ost.str();
		}
	};

	// traits for avoiding warning message.
	template<>
	struct assertion_traits< std::size_t >
	{
		static bool equal( const std::size_t& x, const std::size_t& y )
		{
			return (x==y);
		}
		static std::string toString( const std::size_t& x)
		{
			CppUnit::OStringStream ost;
			ost << int(x);
			return ost.str();
		}
	};

	// traits for type bool
	template<>
	struct assertion_traits< bool >
	{
		static bool equal( const bool& x, const bool& y )
		{
			return (x==y);
		}
		static std::string toString( const bool& x)
		{
			CppUnit::OStringStream ost;
			if (x) 
				ost << "true";
			else 
				ost << "false";
			return ost.str();
		}
	};
} //namespace CppUnit

#endif
