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
#include "DynamicTypeBasicTest.hxx"

namespace CLAMTest {

CPPUNIT_TEST_SUITE_REGISTRATION( DynamicTypeBasicTest  );


//implementation of Fixture setUp
void DynamicTypeBasicTest::setUp()
{
	dyn = new Dyn;
}

void DynamicTypeBasicTest::tearDown()
{
	delete dyn;
}

// Unit tests:

void DynamicTypeBasicTest::InstantiateAttributes ()
{
	dyn->AddInt();
	dyn->AddMyA();
	CPPUNIT_ASSERT( !dyn->HasInt() && !dyn->HasMyB() && !dyn->HasInt() );
	dyn->UpdateData();
	CPPUNIT_ASSERT( dyn->HasInt() && dyn->HasMyA() && !dyn->HasMyB() );
	dyn->FullfilsInvariant();
}

void DynamicTypeBasicTest::RepetitiveOperations()
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
void DynamicTypeBasicTest::NonUpdatedAddsAndRemoves ()
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

void DynamicTypeBasicTest::ChainedMethods ()
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

void DynamicTypeBasicTest::ShallowDataAccess ()
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

void DynamicTypeBasicTest::ShallowDeletion ()
{
	Dyn *p = new Dyn;
	CPPUNIT_ASSERT( p );
	dyn->FullfilsInvariant();
	delete p;
}

void DynamicTypeBasicTest::DeepDataAccess ()
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

void DynamicTypeBasicTest::CopyConstructor()
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

void DynamicTypeBasicTest::DeepCopy()
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

void DynamicTypeBasicTest::Assignation()
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
}; //namespace
