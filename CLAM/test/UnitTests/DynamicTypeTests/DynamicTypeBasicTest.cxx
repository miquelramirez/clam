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
#include <cppunit/extensions/HelperMacros.h>

#include "DynamicType.hxx" // CLAM
#include "DummyDynamicTypes.hxx"


namespace CLAMTest {

// forward declaration:
class DynamicTypeBasicTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DynamicTypeBasicTest  );

class DynamicTypeBasicTest : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE( DynamicTypeBasicTest );
	CPPUNIT_TEST( InstantiateAttributes );
	CPPUNIT_TEST( RepetitiveOperations );
	CPPUNIT_TEST( NonUpdatedAddsAndRemoves );
	CPPUNIT_TEST( ChainedMethods );
	CPPUNIT_TEST( ShallowDataAccess );
	CPPUNIT_TEST( ShallowDeletion );
	CPPUNIT_TEST( DeepDataAccess );
//  We would like to have the following test, but valid
//  accessors are managed with breakpoints, not exceptions.
//	CPPUNIT_TEST_EXCEPTION( InvalidAccessors, Err );
	CPPUNIT_TEST( CopyConstructor );
	CPPUNIT_TEST( DeepCopy );
	CPPUNIT_TEST( Assignation );
	CPPUNIT_TEST( Assignation_WhenIsNotUpdated );
	CPPUNIT_TEST( Assignation_WhenIsSelfAssigning );
	CPPUNIT_TEST_SUITE_END();

public:
	// implementation of TestFixture interface
	void setUp()
	{
		dyn = new Dyn;
	}

	void tearDown()
	{
		delete dyn;
	}

private:
	
	Dyn * dyn;
	





	// Unit tests:

	void InstantiateAttributes ()
	{
		dyn->AddInt();
		dyn->AddMyA();
		CPPUNIT_ASSERT( !dyn->HasInt() && !dyn->HasMyB() && !dyn->HasInt() );
		dyn->UpdateData();
		CPPUNIT_ASSERT( dyn->HasInt() && dyn->HasMyA() && !dyn->HasMyB() );
		dyn->FullfilsInvariant();
	}

	void RepetitiveOperations()
	{
		CPPUNIT_ASSERT( !dyn->HasInt() );
		dyn->AddInt();
		dyn->AddInt();
		CPPUNIT_ASSERT( !dyn->HasInt()&& !dyn->HasMyA() );
		dyn->UpdateData();
		dyn->RemoveInt();
		dyn->RemoveInt();
		CPPUNIT_ASSERT( !dyn->HasInt() );
		dyn->UpdateData();
		CPPUNIT_ASSERT( !dyn->HasInt() );
		dyn->FullfilsInvariant();
	}
	void NonUpdatedAddsAndRemoves ()
	{
		CPPUNIT_ASSERT( !dyn->HasInt() );
		dyn->AddInt();
		dyn->UpdateData();
		CPPUNIT_ASSERT( dyn->HasInt() );
		dyn->GetInt() = 777;
		CPPUNIT_ASSERT( dyn->GetInt() == 777 );
		dyn->RemoveInt();
		CPPUNIT_ASSERT( !dyn->HasInt() );
		dyn->AddInt(); 
		CPPUNIT_ASSERT( dyn->HasInt() ); 
		//Notice: UpdateData() hasn't been necessary.
		CPPUNIT_ASSERT( dyn->GetInt() == 777 ); 
		dyn->FullfilsInvariant();
	}

	void ChainedMethods ()
	{
		dyn->AddAll();
		CPPUNIT_ASSERT( !dyn->HasInt() && !dyn->HasMyA() && !dyn->HasMyB() );
		dyn->UpdateData();
		CPPUNIT_ASSERT( dyn->HasInt() && dyn->HasMyA() && dyn->HasMyB() );
		dyn->RemoveAll();
		CPPUNIT_ASSERT( !dyn->HasInt() && !dyn->HasMyA() && !dyn->HasMyB() );
		dyn->UpdateData();
		CPPUNIT_ASSERT( !dyn->HasInt() && !dyn->HasMyA() && !dyn->HasMyB() );
		dyn->FullfilsInvariant();
	}

	void ShallowDataAccess ()
	{
		const double delta = 0.00000001; // for doubles equal

		dyn->AddAll();
		dyn->UpdateData();
		dyn->SetInt( 5 );
		CPPUNIT_ASSERT_EQUAL( 5, dyn->GetInt() );
		dyn->GetInt() = 9;
		CPPUNIT_ASSERT_EQUAL( 9, dyn->GetInt() );
		CompWithBasics8 dummyA;
		dummyA.mA = -2;
		dummyA.mB = float( 3.1415 );
		dummyA.mC = double( 0.123456789 );
		dyn->SetMyA( dummyA );
		CPPUNIT_ASSERT_EQUAL( -2, dyn->GetMyA().mA );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.1415, dyn->GetMyA().mB, delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.123456789, dyn->GetMyA().mC, delta );

		dyn->GetMyB().mA = -1;
		CompWithBasics9 dummyB( dyn->GetMyB() );
		CPPUNIT_ASSERT_EQUAL( -1, dummyB.mA );

		dyn->FullfilsInvariant();
	}

	void ShallowDeletion ()
	{
		Dyn *p = new Dyn;
		CPPUNIT_ASSERT( p );
		dyn->FullfilsInvariant();
		delete p;
	}

	void DeepDataAccess ()
	{
		dyn->AddAll();
		dyn->UpdateData();
		dyn->SetInt( 1 );
		CPPUNIT_ASSERT( !dyn->GetSubDyn().HasInt() );
		Dyn *p = &(dyn->GetSubDyn());
		p->AddAll();
		p->UpdateData();
		CPPUNIT_ASSERT( dyn->GetSubDyn().HasInt() );
		p->SetInt( 2 );
		p = &(p->GetSubDyn());
		p->AddAll();
		p->UpdateData();
		p->SetInt( 3 );
		
		p = &(dyn->GetSubDyn()); // start over to read.
		CPPUNIT_ASSERT_EQUAL( 2, p->GetInt() );
		p = &(p->GetSubDyn());
		CPPUNIT_ASSERT_EQUAL( 3, p->GetInt() );
		p = &(p->GetSubDyn());
		CPPUNIT_ASSERT( !p->HasInt() );
		
		p->FullfilsInvariant();
		dyn->FullfilsInvariant();
	}

	void CopyConstructor()
	{
		dyn->AddInt();
		dyn->AddSubDyn();
		dyn->UpdateData();
		dyn->SetInt( 9 );
		
			Dyn foo;
			foo.AddInt();
			foo.UpdateData();
			foo.SetInt( 10 );

		dyn->SetSubDyn( foo );
		Dyn *dynCopy = new Dyn( *dyn );
		CPPUNIT_ASSERT( !dynCopy->HasMoreInt() );
		CPPUNIT_ASSERT( dynCopy->HasInt() );
		CPPUNIT_ASSERT_EQUAL( 9, dynCopy->GetInt());
		CPPUNIT_ASSERT( !dynCopy->GetSubDyn().HasSubDyn() );
		CPPUNIT_ASSERT( dynCopy->GetSubDyn().HasInt() );
		CPPUNIT_ASSERT_EQUAL( 10, dynCopy->GetSubDyn().GetInt() );
		
		dynCopy->GetSubDyn().SetInt(-1);
		CPPUNIT_ASSERT_EQUAL( 10, dyn->GetSubDyn().GetInt() );
		
		dynCopy->FullfilsInvariant();
		delete dynCopy;
	}

	void DeepCopy()
	{
		// the same test as before but copying with explicit DeepCopy
		dyn->AddInt();
		dyn->AddSubDyn();
		dyn->UpdateData();
		dyn->SetInt( 9 );
		
			Dyn foo;
			foo.AddInt();
			foo.UpdateData();
			foo.SetInt( 10 );

		dyn->SetSubDyn( foo );
		Dyn *dynCopy = dynamic_cast<Dyn*>( dyn->DeepCopy() );
		CPPUNIT_ASSERT( !dynCopy->HasMoreInt() );
		CPPUNIT_ASSERT( dynCopy->HasInt() );
		CPPUNIT_ASSERT_EQUAL( 9, dynCopy->GetInt());
		CPPUNIT_ASSERT( !dynCopy->GetSubDyn().HasSubDyn() );
		CPPUNIT_ASSERT( dynCopy->GetSubDyn().HasInt() );
		CPPUNIT_ASSERT_EQUAL( 10, dynCopy->GetSubDyn().GetInt() );
		dynCopy->GetSubDyn().SetInt(-1);
		CPPUNIT_ASSERT_EQUAL( 10, dyn->GetSubDyn().GetInt() );
		
		dyn->FullfilsInvariant();
		dynCopy->FullfilsInvariant();
		
		delete dynCopy;
	}

	void Assignation()
	{
		// the same as before, now using an operator=
		dyn->AddInt();
		dyn->AddSubDyn();
		dyn->UpdateData();
		dyn->SetInt( 9 );
		
			Dyn foo;
			foo.AddInt();
			foo.UpdateData();
			foo.SetInt( 10 );

		dyn->SetSubDyn( foo );

		Dyn dynCopyPointee = *dyn; // this extra variable is for keeping cut&paste
								// compatibility with the last tests.
		Dyn *dynCopy = &dynCopyPointee; 

		CPPUNIT_ASSERT( !dynCopy->HasMoreInt() );
		CPPUNIT_ASSERT( dynCopy->HasInt() );
		CPPUNIT_ASSERT_EQUAL( 9, dynCopy->GetInt());
		CPPUNIT_ASSERT( !dynCopy->GetSubDyn().HasSubDyn() );
		CPPUNIT_ASSERT( dynCopy->GetSubDyn().HasInt() );
		CPPUNIT_ASSERT_EQUAL( 10, dynCopy->GetSubDyn().GetInt() );
		dynCopy->GetSubDyn().SetInt(-1);
		CPPUNIT_ASSERT_EQUAL( 10, dyn->GetSubDyn().GetInt() );
		
		// don't delete dynCopy, now:  the copy object is a local variable.
	}

	void Assignation_WhenIsNotUpdated()
	{
		dyn->AddInt();
		dyn->AddSubDyn();
		dyn->UpdateData();
		
		Dyn foo;
		dyn->SetSubDyn( foo );
		dyn->RemoveSubDyn();
		try
		{
			Dyn dynCopyPointee = *dyn;
			CPPUNIT_FAIL("non updated copy should have asserted");
		}
		catch( CLAM::ErrAssertionFailed&  ) {}

	}
	void Assignation_WhenIsSelfAssigning()
	{
		dyn->AddInt();
		dyn->UpdateData();
		dyn->SetInt(1);
		*dyn = *dyn;
		CPPUNIT_ASSERT_EQUAL( 1, dyn->GetInt() );
	}
}; // class DynamicTypeBasicTest

} //namespace

