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

#include "TestControlsDummyClasses.hxx"
#include <cstdlib>
#include <exception>
#include <algorithm>

using std::cout;
using std::endl;
using std::for_each;

using namespace CLAM;
namespace CLAMTest {

void TestBasics()
{
 	StaticControls receiver;  // 3 incontrols (first with callback) and 2 outc.
	StaticControls sender; // 2 out controls

	sender.out1.AddLink(&receiver.in1); 
	sender.out2.AddLink(&receiver.in2);
	sender.out2.AddLink(&receiver.in3);

	// Testing basic functionality:
	sender.out1.SendControl(11.11f);
	sender.out2.SendControl(22.33f);

	if (receiver.in1.GetLastValue() != 11.11f) 
		throw Err();
		else if (receiver.in2.GetLastValue() != 22.33f) 
			throw Err();
			else if (receiver.in3.GetLastValue() != 22.33f) 
				throw Err();
	// test static-controls publication order:
	for (int i=3; i<=7; i++) {
		InControl* ic = sender.GetInControl(i);
		int strIndex = atoi( ic->GetName().c_str() );
		if (strIndex != i-3) throw Err("Failed in static-controls publication order");
	}
}

void TestDynamicControls()
{
	DynamicControls receiver;
	DynamicControls sender;

	receiver.Configure(ProcessingControlConfig());
	sender.Configure(ProcessingControlConfig());
	
	// test that is impossible to get the 'copy-constructor-bug':
	// can't be tested in a normal way because, the copy-constructor is "protected"
	// by an assertion.
	
	// test dynamic controls connection:
	for (int i=0; i<sender.outCtrls.size(); i++) 
		sender.outCtrls.at(i)->SendControl(i);
}


// forward declaration
void PrintPublishedControls(const Processing& p);

void TestPublishedControls()
{
	// Test that all the names are reachables.
	// Using the In/Out-Controls iterators
	DynamicControls proc1;
	proc1.Configure(ProcessingControlConfig());

	StaticControls proc2;

	PrintPublishedControls(proc1);
	PrintPublishedControls(proc2);
}

template <class Control>class PrintControlName {
public:
	void operator() (const Control* c) {
		std::cout << c->GetName() << std::endl;
	}
};

void PrintPublishedControls(const Processing& p)
{
	cout << "\nPrinting control names of processing :" << p.GetName() << endl;
	cout << "InControls:\n";
	for_each(p.FirstInControl(), p.LastInControl(), PrintControlName<InControl>() );
	cout << "outControls:\n";
	for_each(p.FirstOutControl(), p.LastOutControl(), PrintControlName<OutControl>() );
}

int main(void)
{
	try {
		TestBasics();
		TestDynamicControls();
		TestPublishedControls();
		// @todo: TestControlLinker();
		// This test has to be done when the ControlLinker will be finished.
	}
	catch (Err e) {
		e.Print();
		cout << "Failed.'" << endl;
		return 1;
	}
	catch (...) {
		cout << "catched a nice '...'" << endl;
		return 1;
	}
	std::cout << "Test Passed!" << std::endl;

	return 0;
}  // main
}; // end namespace CLAMTest

int main (void)
{
	return CLAMTest::main();
}
