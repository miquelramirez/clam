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
#include <cstdlib>
#include "Err.hxx"
#include "ErrDynamicType.hxx"
#include "ErrProcessingObj.hxx"
#include "ErrOpenFile.hxx"
#include "ErrOutOfMemory.hxx"
#include "ErrGUI.hxx"

using CLAM::Err;
using CLAM::ErrDynamicType;
using CLAM::ErrProcessingObj;
using CLAM::ErrOpenFile;
using CLAM::ErrOutOfMemory;
using CLAMVM::ErrGUI;

namespace CLAMTest {

	using namespace CLAM;

	template <class T>
	class ExceptionTester {

		void Ok() 
		{ 
			std::cout << "."; 
		}

		void Fail()
		{ 
			std::cout << "\nFailed!" << std::endl; 
			exit(1);
		}
		
	public:
		ExceptionTester() {
			PlainTest();
			GeneralTest();
			EmbedTest();
		}

		void PlainTest()
		{
			try {
				throw T("Plain");
			}
			catch (T& e) {
				e.Print();
				Ok();
			}
			catch(...) {
				Fail();
			}
		}

		void GeneralTest()
		{
			try {
				throw T("General");
			}
			catch (Err& e) {
				e.Print();
				Ok();
			}
			catch(...) {
				Fail();
			}
		}

		void EmbedTest()
		{
			try {
				try {
					throw T("Embed");
				}
				catch (T &orig) {
					Err myErr("Trying nested exceptions");
					myErr.Embed(orig);
					throw(myErr);
				}
				catch (...) {
					Fail();
				}
			}
			catch (Err &e) {
				e.Print();
				Ok();
			}
			catch (...) {
				Fail();
			}
					
		}

	};

}



int main()
{
	using namespace CLAMTest;
	try {
		ExceptionTester<Err>();
		ExceptionTester<ErrDynamicType>();
		ExceptionTester<ErrProcessingObj>();
		ExceptionTester<ErrOpenFile>();
//	ExceptionTester<ErrOutOfMemory>();
//	ExceptionTester<ErrGUI>();
		std::cout << "Passed." << std::endl;
		return 0;
	}
	catch (Err&e) {
		e.Print();
		std::cout << "Failed." << std::endl;
		return 1;
	}
	catch (...) {
		std::cout << "Unknown exception!. Failed." << std::endl;
		return 1;
	}
}

