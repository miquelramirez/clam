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

/** @file
 * This file provides an skeleton for processing classes test
 * files. You can use it as an starting point to make a new processing
 * class test, replacing "MyClass" with the name of your class all
 * along the file, and the "MyDataN" classes in the execution test.
 * <p>
 * Of course, it is a very limited example. You should complicate it
 * as much as possible to really stress the tested class.
 * <p>
 * If you know about any useful general testing code which should fit
 * in here, or have any good idea about how to organise things, please
 * add it!
 */

// The following file defines some useful things, such a TestError
// class for exceptions, and functions to compare Spectrum and Audio
// objects
#include "TestUtils.hxx"

// Here you should include the class you want to test!
#include "MyClass.hxx"

// And you may want to provide some feedback.
#include <iostream>

namespace CLAMTest {

	class MyClassTest {
		CLAM::MyClass *mpObject1;
		CLAM::MyClass *mpObject2;

		// Basic Test methods. You should make them bigger, and maybe
		// add some more.
		// You can notify a test failure either by throwing an
		// exception inside them or by making them return a false
		// value. In the first case no more checks will be made.  In
		// the second case tests will continue, but at the end a test
		// failure will be reported.
		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		bool Do();
	};


	bool MyClassTest::TestConstruction()
	{
		CLAM::MyClassConfig cfg;
		// You should add more constructions here,
		// using different configuration parameters, or
		// other constructors the class may provide.
		mpObject1 = new CLAM::MyClass();
		mpObject2 = new CLAM::MyClass(cfg);
		return true;
	}

	bool MyClassTest::TestConfiguration()
	{
		CLAM::MyClassConfig cfg,cfg2;
		// You should add more reconfigurations here,
		// using different configuration parameters.
		mpObject1->Configure(cfg);
		mpObject2->Configure(cfg);
		mpObject1->Configure(cfg2);
		mpObject2->Configure(cfg2);
		return true;
	}

	bool MyClassTest::TestExecution()
	{
		MyData1 data1;
		MyData2 data2;
		MyData3 data3;

		mpObject1->Start();
		mpObject2->Start();

		// You should add more Do calls here, using different data
		// values, AND YOU SHOULD CHECK IF THE RESULTS ARE CORRECT

		mpObject1->Do(data1,data2,data3);
		mpObject2->Do(data1,data2,data3);

		mpObject1->Stop();
		mpObject2->Stop();

		return true;
	}

	bool MyClassTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool MyClassTest::Do()
	{
		bool res, ok = true;

		res = TestConstruction();
		if (!res) {
			std::cerr << "Construction failed." << std::endl;
			ok = false;
		}
		res = TestConfiguration();
		if (!res) {
			std::cerr << "Configuration failed." << std::endl;
			ok = false;
		}
		res = TestExecution();
		if (!res) {
			std::cerr << "Execution failed." << std::endl;
			ok = false;
		}
		res = TestDestruction();
		if (!res) {
			std::cerr << "Destruction failed." << std::endl;
			ok = false;
		}
		return ok;
	}

}

// main() return values:
//    0:        Test sucessful.
//    non-zero: Test Failed.
int main()
{
	try {
		CLAMTest::MyClassTest test;
		bool result = test.Do();
		if (result==false) {
			std::cerr << "Failed." << std::endl;
			return 1;
		}
	}
	catch (std::exception &e) {
		std::cerr << "ERROR: Excepton: " << e.what() << std::endl
				  << "Failed." << std::endl;
		return 1;
	}
	std::cerr << "Passed." << std::endl;
	return 0;
}
