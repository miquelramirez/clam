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

#include "InControl.hxx"
#include "Processing.hxx"
#include "ProcessingComposite.hxx"

#include <cppunit/extensions/HelperMacros.h>

namespace CLAMTest {

using CLAM::TControlData;


class ControlsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ControlsTest );
	CPPUNIT_TEST( NonPublishedControls );
	CPPUNIT_TEST( InControlBasics );
	CPPUNIT_TEST( ProcessingWithControlArray );
#ifdef HAVE_STANDARD_VECTOR_AT 
	CPPUNIT_TEST_EXCEPTION( ProcessingSidePublishedContainers, std::out_of_range );
#endif
	CPPUNIT_TEST( ProcessingSideInterface );


	CPPUNIT_TEST_SUITE_END();

private:
	void NonPublishedControls();
	void InControlBasics();
	void ProcessingWithControlArray();
	void ProcessingSidePublishedContainers();
	void ProcessingSideInterface();
	
};


CPPUNIT_TEST_SUITE_REGISTRATION( ControlsTest );

	// dummy classes for testing
	class DummyConfig : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (DummyConfig, 1, CLAM::ProcessingConfig)
		DYN_ATTRIBUTE (0, public, std::string, Name);
	};

	class DummyProcessing : public CLAM::Processing
	{
	public:
		DummyProcessing() { Configure( DummyConfig() ); }
		const char* GetClassName() const { return "DummyProcessing"; }
		bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return true; }
		const CLAM::ProcessingConfig &GetConfig() const { static DummyConfig _c; return _c; }
		bool Do() { return true; }
	};

	void ControlsTest::NonPublishedControls() {
		// construction outside the processing is possible
		CLAM::InControl in("i'm a non published in" );
		CLAM::OutControl out("i'm a non published out");
	}

void ControlsTest::InControlBasics() {
	TControlData _3(3), _1_1( TControlData(1.1) );
	CLAM::InControl in("i'm an in control");
	CLAM::OutControl out("i'm an out control");
	in.DoControl( _1_1 );
	CPPUNIT_ASSERT_EQUAL( _1_1, in.GetLastValue() );
	CPPUNIT_ASSERT_EQUAL(std::string("i'm an in control"), in.GetName() );

	DummyProcessing parentProc;
	CLAM::InControl published( "not used name", &parentProc );
	parentProc.DoControl(0, _3);
	CPPUNIT_ASSERT_EQUAL( _3, parentProc.GetInControl(0)->GetLastValue() );
}

	// dummy class for testing
	class DummyWithDynamicInControls : public DummyProcessing
	{
		std::vector<CLAM::InControl*> _inCtls;
		CLAM::OutControl _out;
	public:
		enum { size=6 };
		DummyWithDynamicInControls() : _out ("i'm an out", this)
		{
			for (int i=0; i<size; i++) {
				std::stringstream strm;
				strm << "i'm an in_" << i;
				_inCtls.push_back( new CLAM::InControl(strm.str(), this) ); 
				// new controls are published by default
			}
		}
	};

void ControlsTest::ProcessingSidePublishedContainers()
{
	DummyWithDynamicInControls dum;
	const int last = DummyWithDynamicInControls::size - 1;
	// rise an exepected std::exception
	// some compiler uses insecure operator[] instead of .at
#ifdef HAVE_STANDARD_VECTOR_AT
	dum.GetInControl(last+1);
#endif
}

void ControlsTest::ProcessingSideInterface()
{
	DummyWithDynamicInControls sender, receiver;
	const int last = DummyWithDynamicInControls::size - 1;

	CPPUNIT_ASSERT_EQUAL( std::string("i'm an in_0"), receiver.GetInControl(0)->GetName() );
	CPPUNIT_ASSERT_EQUAL( std::string("i'm an in_1"), receiver.GetInControl(1)->GetName() );
	std::stringstream strm;
	strm << "i'm an in_" << last;
	CPPUNIT_ASSERT_EQUAL( strm.str(), receiver.GetInControl(last)->GetName() );

	sender.LinkOutWithInControl(0, &receiver, 0);
	sender.LinkOutWithInControl(0, &receiver, last);
	
	const TControlData _3(3);
	sender.SendControl( 0, _3 );
	CPPUNIT_ASSERT_EQUAL( _3, receiver.GetInControl(0)->GetLastValue() );
	CPPUNIT_ASSERT_EQUAL( _3, receiver.GetInControl(last)->GetLastValue() );
	const TControlData _0(0);
	CPPUNIT_ASSERT_EQUAL( _0, receiver.GetInControl(1)->GetLastValue() );
	// in control 1 wasn't connected. So it remains with the init value

}

/// we make this dummy class a composite for testing controls in composites
class DummyProcArray : public CLAM::ProcessingComposite
{
public:
	typedef CLAM::InControlTmplArray<DummyProcArray> CtlArray;
	typedef CLAM::InControlTmpl<DummyProcArray> Ctl;
	Ctl _control;
	CtlArray _controls;
	
	enum {ctlSize=4};
	// constructor
	DummyProcArray() :
		_control( "control", this, &DummyProcArray::fun, true ),
		_controls( ctlSize, "myName1", this, &DummyProcArray::fun, true)
	{}

private:	
	const char* GetClassName() const { return "DummyProcArray"; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return true; }
	const CLAM::ProcessingConfig &GetConfig() const { return _c;}
	bool Do() { return true; }

	int fun( int id, TControlData val ) { return id; }
	int fun( TControlData val ) { return 0; }

	DummyConfig _c;
	

};

void ControlsTest::ProcessingWithControlArray() {
	DummyProcArray proc;
	CPPUNIT_ASSERT_EQUAL( std::string("myName1_2"),
			proc._controls[2].GetName() );
	/// \todo test publishing in arrays

	/// \todo test callback functions.
}

} // namespace 
