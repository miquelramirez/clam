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
#include "PhantomBuffer.hxx"
#include <iostream>

namespace CLAMTest {

	template<class T, unsigned int N>
	class DataChunk
	{
		T data[N];
	public:
		DataChunk() {};
		DataChunk(T val)
		{
			for (unsigned i=0; i<N; i++)
				data[i]=val;
		}
		T* Address() { return data; }
		T &operator[](int i) { return data[i]; }
	};


	/**
	 * This test is not a proper unit test
	 * is just an old test (Enrique's style) that uses CppUnit framework
	 */

	class PhantomBufferTest ;
	CPPUNIT_TEST_SUITE_REGISTRATION( PhantomBufferTest );

	class PhantomBufferTest : public CppUnit::TestFixture
	{

		CLAM::PhantomBuffer<double> *mpObject1;
		CLAM::PhantomBuffer<double> *mpObject2;

	public:
		CPPUNIT_TEST_SUITE( PhantomBufferTest );
		CPPUNIT_TEST( TestUsage );
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp()
		{
			mpObject1 = new CLAM::PhantomBuffer<double>();
			mpObject2 = new CLAM::PhantomBuffer<double>(*mpObject1);
		}

		void tearDown()
		{
			delete mpObject1;
			delete mpObject2;
		}

		void PhantomBufferTest::TestUsage()
		{
			mpObject1->Resize(0,0,0);
			mpObject1->Resize(1,1,0);
			mpObject1->Resize(1,1,1);
			mpObject1->Resize(2,1,0);
			mpObject1->Resize(2,1,1);
			mpObject1->Resize(2,1,2);
			mpObject1->Resize(2,2,2);
			mpObject1->Resize(2,2,1);
			mpObject1->Resize(2,2,0);


			mpObject2->Resize(0,0,0);
			mpObject2->Resize(10,1,0);
			mpObject2->Resize(10,1,1);
			mpObject2->Resize(20,1,0);
			mpObject2->Resize(20,1,1);
			mpObject2->Resize(20,1,2);
			mpObject2->Resize(20,2,2);
			mpObject2->Resize(20,2,1);
			mpObject2->Resize(20,2,0);

			int i;
			double *read;
			for (i = 0; i<22-5; i++) {
				DataChunk<double,5> data(double(100.0+i));
				mpObject2->Write(i,5,data.Address());
				read = mpObject2->Read(i,5);
				for (int j=0; j<5; j++)
					CPPUNIT_ASSERT_MESSAGE( "TestUsage(): Written and read data differ!",
							data[j] == read[j] );
			}

			for (i = 22-9; i>=0; i--) {
				DataChunk<double,8> data(double(2000+i));
				mpObject2->FulfilsInvariant();
				mpObject2->Write( i,8,data.Address() );
				mpObject2->FulfilsInvariant();
				read = mpObject2->Read(i,8);
				for (int j=0; j<8; j++)
					CPPUNIT_ASSERT_MESSAGE( "TestUsage(): Written and read data differ!", 
						data[j] == read[j] );
				mpObject2->FulfilsInvariant();
			}

			int logical_size = 21;
			int phantom_size = 2;
			for (i=0; i<100; i++) {
				DataChunk<double,10> data(double(30000+i));
				mpObject2->FulfilsInvariant();
				mpObject2->Write(0,10,data.Address());
				mpObject2->FulfilsInvariant();
				mpObject2->Resize(logical_size++, phantom_size++, 0);
				mpObject2->FulfilsInvariant();
				read = mpObject2->Read(1,10);
				mpObject2->FulfilsInvariant();
				for (int j=0; j<10; j++)
					CPPUNIT_ASSERT_MESSAGE( "TestUsage(): Written and read data differ!", 
						data[j] == read[j] );
			}

			int size_increment = 1;
			int write_pos = 0;
			for (i=0; i<100; i++) {
				DataChunk<double,10> data(double(40000+i));
				mpObject2->FulfilsInvariant();
				mpObject2->Write(write_pos,10,data.Address());
				mpObject2->FulfilsInvariant();
				mpObject2->Resize(logical_size, phantom_size, write_pos);
				mpObject2->FulfilsInvariant();
				read = mpObject2->Read(write_pos + size_increment,10);
				mpObject2->FulfilsInvariant();
				for (int j=0; j<10; j++)
					CPPUNIT_ASSERT_MESSAGE( "TestUsage(): Written and read data differ!", 
						data[j] == read[j] );

				size_increment++;
				logical_size += size_increment;
				phantom_size += 2;
				write_pos += size_increment;
			}
		}
	};
} // namespace CLAMTest

