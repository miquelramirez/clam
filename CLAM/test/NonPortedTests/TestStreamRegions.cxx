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

#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "InplaceStreamRegion.hxx"
#include "DelayStreamRegion.hxx"


#include <iostream>
#include <exception>

namespace CLAMTest {

	class StreamRegionsTest {
		CLAM::ReadStreamRegion  *mpReader;
		CLAM::WriteStreamRegion  *mpWriter;
		CLAM::InplaceStreamRegion *mpInplace;
		CLAM::DelayStreamRegion   *mpDelay;  

		bool TestConstruction();
		bool TestManipulation();
		bool TestDestruction();
	public:
		bool Do();
	};


	bool StreamRegionsTest::TestConstruction()
	{
		mpWriter  = new CLAM::WriteStreamRegion(10,10);
		mpReader  = new CLAM::ReadStreamRegion(10,10,mpWriter);
		mpInplace = new CLAM::InplaceStreamRegion(10,10,10,mpWriter);
		mpDelay   = new CLAM::DelayStreamRegion(10,10,mpWriter);
		return true;
	}


	bool StreamRegionsTest::TestManipulation()
	{
		return true;
	}

	bool StreamRegionsTest::TestDestruction()
	{
		delete mpReader; 
		delete mpWriter; 
		delete mpInplace;
		delete mpDelay;  
		return true;
	}

	bool StreamRegionsTest::Do()
	{
		bool res, ok = true;

		res = TestConstruction();
		if (!res) {
			std::cerr << "Construction failed." << std::endl;
			ok = false;
		}
		res = TestManipulation();
		if (!res) {
			std::cerr << "Manipulation failed." << std::endl;
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


int main()
{
	try {
		CLAMTest::StreamRegionsTest test;
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

