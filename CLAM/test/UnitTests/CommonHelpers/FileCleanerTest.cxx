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

#include "FileCleaner.hxx"
#include <cppunit/extensions/HelperMacros.h>
#include <fstream>

namespace CLAMTest{

class FileCleanerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FileCleanerTest );

class FileCleanerTest : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE( FileCleanerTest );

	CPPUNIT_TEST( test_exists_whenExists );
	CPPUNIT_TEST( test_destructor_whenEmpty );
	CPPUNIT_TEST( test_destructor_whenAddedOne );
	CPPUNIT_TEST( test_destructor_whenAddedMany );
	CPPUNIT_TEST( test_destructor_whenNotCreated );
	CPPUNIT_TEST( test_addTemp_withoutExtension );

	CPPUNIT_TEST_SUITE_END();

private:
	bool exists(const std::string & filename)
	{
		return std::ifstream(filename.c_str());
	}
	std::string tempfilename()
	{
		return std::string(std::tmpnam(0));
	}
	void create(const std::string & filename)
	{
		std::ofstream(filename.c_str());
	}
	void remove(const std::string & filename)
	{
		std::remove(filename.c_str());
	}

public:
	void setUp()
	{
	}

public:
	FileCleanerTest()
	{
	}

private:
	void test_exists_whenExists()
	{
		std::string filename = tempfilename();
		CPPUNIT_ASSERT(not exists(filename));

		create(filename);
		CPPUNIT_ASSERT(exists(filename));

		remove(filename);
		CPPUNIT_ASSERT(not exists(filename));
	}

	void test_destructor_whenEmpty()
	{
		std::string filename = tempfilename();
		{
			FileCleaner cleaner;
			create(filename);
		}
		CPPUNIT_ASSERT(exists(filename));
		remove(filename);
	}

	void test_destructor_whenAddedOne()
	{
		std::string filename = tempfilename();
		{
			FileCleaner cleaner;
			create(filename);
			cleaner.add(filename);
		}
		CPPUNIT_ASSERT(not exists(filename));
	}

	void test_destructor_whenAddedMany()
	{
		std::string filename1 = tempfilename();
		std::string filename2 = tempfilename();
		{
			FileCleaner cleaner;
			create(filename1);
			cleaner.add(filename1);
			create(filename2);
			cleaner.add(filename2);
		}
		CPPUNIT_ASSERT(not exists(filename1));
		CPPUNIT_ASSERT(not exists(filename2));
	}

	void test_destructor_whenNotCreated()
	{
		std::string filename1 = tempfilename();
		{
			FileCleaner cleaner;
			cleaner.add(filename1);
		}
		CPPUNIT_ASSERT(not exists(filename1));
	}

	void test_addTemp_withoutExtension()
	{
		std::string filename;
		{
			FileCleaner cleaner;
			filename = cleaner.addTemp();
			create(filename);
		}
		CPPUNIT_ASSERT(not exists(filename));
	}

};

}

