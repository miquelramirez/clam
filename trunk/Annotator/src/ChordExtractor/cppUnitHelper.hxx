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


#ifndef CPPUNIT_NS_BEGIN
#define CPPUNIT_NS_BEGIN namespace CppUnit {
#endif
#ifndef CPPUNIT_NS_END
#define CPPUNIT_NS_END }
#endif

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

	inline std::string GetTestDataDirectory(std::string postfix = "")
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( pathToTestData ) return std::string(pathToTestData)+postfix;
		return std::string("../../../../CLAM-TestData/")+postfix;
	}

} //namespace CLAMTest



CPPUNIT_NS_BEGIN

// KLUDGE: until cppunit 1.10 Win binaries are available
#if defined( _MSC_VER )
namespace TestAssert {
#endif // MSVC 6


	// Colorizing string diferences
	inline int firstMismatch(const std::string & one, const std::string & other)
	{
		int minLen=one.length()<other.length()?
			one.length():other.length();
		for ( int index = 0; index < minLen; index++ )
			if (one[index] != other[index]) return index;
		return minLen;
	}

#if !defined( _MSC_VER ) || _MSC_VER > 1310
// MRJ: This causes all CPPUNIT_ASSERT_EQUALS not to compile under VC6
#ifdef CPPUNIT_ENABLE_SOURCELINE_DEPRECATED
	template <>
	inline void assertEquals( const std::string& expected,
			   const std::string& actual,
			   long lineNumber,
			   std::string fileName )
	{
		if ( !assertion_traits<std::string>::equal(expected,actual) )
		{
			unsigned int index = firstMismatch(expected, actual);
			assertNotEqualImplementation(
				expected.substr(0,index)+"\033[32;1m"+expected.substr(index)+"\033[0m",
				actual.substr(0,index)+"\033[31;1m"+actual.substr(index)+"\033[0m",
				lineNumber,
				fileName );
		}
	}
#else
	template <>
	inline void assertEquals( const std::string& expected,
			   const std::string& actual,
			   SourceLine sourceLine,
			   const std::string &message)
	{
		if ( !assertion_traits<std::string>::equal(expected,actual) )
		{
			unsigned int index = firstMismatch(expected, actual);
			::CppUnit::Asserter::failNotEqual(
				expected.substr(0,index)+"\033[32;1m"+expected.substr(index)+"\033[0m",
				actual.substr(0,index)+"\033[31;1m"+actual.substr(index)+"\033[0m",
				sourceLine,
				message );
		}
	}
#endif

#endif // end of VC6 guard 'ifdef'

// KLUDGE: until cppunit 1.10 Win binaries are available
#if defined( _MSC_VER )
} // namespace TestAssert
#endif // MSVC

// Helper traits for assertions

	// type_info traits
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
#define CLAMTEST_ASSERT_EQUAL_RTTYPES( expected, actual ) \
	CPPUNIT_ASSERT_EQUAL( \
		typeid(expected), \
		typeid(actual) )


	// traits for avoiding warning messages with size_t
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

CPPUNIT_NS_END

#endif

