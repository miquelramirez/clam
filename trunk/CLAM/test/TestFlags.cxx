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


#include "Flags.hxx"
#include "mtgsstream.h" // An alias for <sstream>
#include <string>
#include "Assert.hxx"
#include "XMLTestHelper.hxx"

using namespace CLAM;

namespace CLAMTest {
	class MyFlags : public Flags<5> {
	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		static tValue sDefault;
		virtual Component * Species() const {
			return new MyFlags();
		}
		typedef enum {
			eFlag0=0,
			eFlag1=1,
			eFlag2=2,
			eFlag3=3,
			eFlag4=4
		} tFlag;
		MyFlags () :
			Flags<5>(sFlagValues),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{
			flag4=true;
		};
		template <class T> MyFlags(const T &t) : 
			Flags<5>(sFlagValues,t),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{};
		template <class T1, class T2> MyFlags(const T1 &t1, const T2 &t2) : 
			Flags<5>(sFlagValues,t1,t2),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{}
		reference flag0;
		reference flag1;
		reference flag2;
		reference flag3;
		reference flag4;

		bool Matches(bool f0, bool f1, bool f2, bool f3, bool f4) {
			// This check is done in this weird way in order not to
			// mask type errors with phantom implicit conversions
			bool ok = FullfilsInvariant();
//			ok &= (f2? flag2: !flag2);
			if (f0!=operator[](0)) ok=ReportNotMatch(0,f0,operator[](0));
			if (f1!=operator[](1)) ok=ReportNotMatch(1,f1,operator[](1));
			if (f2!=operator[](2)) ok=ReportNotMatch(2,f2,operator[](2));
			if (f3!=operator[](3)) ok=ReportNotMatch(3,f3,operator[](3));
			if (f4!=operator[](4)) ok=ReportNotMatch(4,f4,operator[](4));
			return ok;
		}
		bool ReportNotMatch(const int place, const bool expected, const bool found) {
			const bool verbose = true;
			if (verbose) {
				std::cout 
					<< "Flag at positon " << place 
					<< " expected: " << expected 
					<< " and found: " << found
					<< std::endl;
			}
			return false;
		}
		bool FullfilsInvariant() {
			bool ok = true;
			if (flag0!=operator[](0)) ok=ReportReferencesDoNotMatch(0,flag0,operator[](0));
			if (flag1!=operator[](1)) ok=ReportReferencesDoNotMatch(1,flag1,operator[](1));
			if (flag2!=operator[](2)) ok=ReportReferencesDoNotMatch(2,flag2,operator[](2));
			if (flag3!=operator[](3)) ok=ReportReferencesDoNotMatch(3,flag3,operator[](3));
			if (flag4!=operator[](4)) ok=ReportReferencesDoNotMatch(4,flag4,operator[](4));
			return ok;
		}
		bool ReportReferencesDoNotMatch(const int place, const bool ref, const bool op) {
			const bool verbose = true;
			if (verbose) {
				std::cout 
					<< "Internal bit reference inconsistency"
					<< " at positon " << place 
					<< " by reference: " << ref 
					<< " by [] operator: " << op << " "
					<< std::endl;
			}
			return false;
		}
		static void TestInputOutput ();
		static void TestOperations ();
	};

	Flags<5>::tFlagValue MyFlags::sFlagValues[] = {
		{MyFlags::eFlag0, "flag0"},
		{MyFlags::eFlag1, "flag1"},
		{MyFlags::eFlag2, "flag2"},
		{MyFlags::eFlag3, "flag3"},
		{MyFlags::eFlag4, "flag4"},
		{0,NULL}
	};

	void MyFlags::TestOperations () {
		std::cout << "--Testing Flags Operations" << std::endl;
	
		std::cout << ".Testing the default constructor" << std::endl;
		MyFlags A;
		CLAM_ASSERT(A.Matches(0,0,0,0,1),
				"Failed: All but Flag4 must be off by default" );
		
		std::cout << ".Testing the int constructor" << std::endl;
		MyFlags B=3;
		CLAM_ASSERT(B.Matches(1,1,0,0,0),
			"Failed: Only Flags0 and Flags1 must be on" );
		
		std::cout << ".Testing the copy constructor" << std::endl;
		MyFlags C(3);
		CLAM_ASSERT(C.Matches(1,1,0,0,0),
			"Failed: Only Flags0, Flags2 and Flag3 mult be on" );

		std::cout << ".Testing the or-assignment operation" << std::endl;
		C|=A;
		std::cout << A << B << C << std::endl;
		CLAM_ASSERT(C.Matches(1,1,0,0,1),
			"Failed: OR-Assignment operator" );

		std::cout << ".Testing the bitset interface (reset)" << std::endl;
		A.reset();
		CLAM_ASSERT(A.Matches(0,0,0,0,0),
			"Failed: Reset does not set all the flags to zero" );
		
		std::cout << ".Testing direct flag setting" << std::endl;
		A.flag1=true;
		CLAM_ASSERT(A.Matches(0,1,0,0,0),
			"Failed: Setting flag1 to true failed" );
		A.flag3=false;
		CLAM_ASSERT(A.Matches(0,1,0,0,0),
			"Failed: Setting flag0 to false failed" );

		std::cout << ".Testing direct bit flip" << std::endl;
		C.flag3.flip();
		CLAM_ASSERT(C.Matches(1,1,0,1,1),
			"Failed: Flip does not flip one single bit (3)" );
		C.flag0.flip();
		CLAM_ASSERT(C.Matches(0,1,0,1,1),
			"Failed: Flip does not flip one single bit (0)" );


		CLAM_ASSERT(XMLInputOutputMatches(A,"FlagA.xml"),
			   "Failed: The loaded version of A stores differently than the original A");
		CLAM_ASSERT(XMLInputOutputMatches(B,"FlagB.xml"),
			   "Failed: The loaded version of B stores differently than the original B");
		CLAM_ASSERT(XMLInputOutputMatches(C,"FlagC.xml"),
			   "Failed: The loaded version of C stores differently than the original C");
	}

	void MyFlags::TestInputOutput () {
		std::cout << "Testing formating with std streams" << std::endl;
		char * inputstring[] = {
			"{flag0 flag2 flag3}",
			"{ flag0 flag2 flag3 }",
			" { flag0 flag2 flag3 } ",
			" { flag0 flag2 flag3 } ",
			"{flag0 fla2 flag3 }",
			"{fla0 flag2 flag3 }",
			NULL
		};
		for (int i=0; inputstring[i]; i++) {
			std::stringstream is(inputstring[i]);
			MyFlags flag;
			try {
				is >> flag;
				CLAM_ASSERT(flag.Matches(1,0,1,1,0), "Failed: Readed value does not match");
				CLAM_ASSERT(i!=4 && i!=5, "Failed: A correct flag input did not fail");
			}
			catch(...) {
				CLAM_ASSERT(i==4 || i==5, "Failed a correct flag input");
				std::cerr << "Catching a expected exception" << std::endl;
			}
			std::cout << "'" << inputstring[i] << "' readed as '" << flag << "'" <<std::endl;
		}
	}
}


int main () {
	CLAMTest::MyFlags::TestOperations();
	CLAMTest::MyFlags::TestInputOutput();
	return 0;
}


