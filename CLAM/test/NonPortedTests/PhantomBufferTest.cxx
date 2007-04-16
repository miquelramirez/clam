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

#include "PhantomBuffer.hxx"
#include <iostream>

namespace CLAMTest {

	template<class T, unsigned int N>
	class DataChunk {
		T data[N];
	public:
		DataChunk() {};
		DataChunk(T val);
		T* Address() { return data; }
		T &operator[](int i) { return data[i]; }
	};

	template<class T, unsigned int N>
	DataChunk<T,N>::DataChunk(T val)
	{
		for (unsigned i=0; i<N; i++)
			data[i]=val;
	}

	template<class T>
	class PhantomBufferTest {
		CLAM::PhantomBuffer<T> *mpObject1;
		CLAM::PhantomBuffer<T> *mpObject2;

		bool TestConstruction();
		bool TestUsage();
		bool TestDestruction();
	public:
		bool Do();
	};


	template<class T>
	bool PhantomBufferTest<T>::TestConstruction()
	{
		mpObject1 = new CLAM::PhantomBuffer<T>();
		mpObject2 = new CLAM::PhantomBuffer<T>(*mpObject1);
		return true;
	}

	template<class T>
	bool PhantomBufferTest<T>::TestUsage()
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
			DataChunk<T,5> data(double(100.0+i));
			mpObject2->FulfilsInvariant();
			mpObject2->Write(i,5,data.Address());
			mpObject2->FulfilsInvariant();
			read = mpObject2->Read(i,5);
			for (int j=0; j<5; j++)
				if (data[j] != read[j]) {
					std::cerr << "TestUsage(): Written and read data differ!\n";
					return false;
				}
			mpObject2->FulfilsInvariant();
		}

		for (i = 22-9; i>=0; i--) {
			DataChunk<T,8> data(double(2000+i));
			mpObject2->FulfilsInvariant();
			mpObject2->Write(i,8,data.Address());
			mpObject2->FulfilsInvariant();
			read = mpObject2->Read(i,8);
			for (int j=0; j<8; j++)
				if (data[j] != read[j]) {
					std::cerr << "TestUsage(): Written and read data differ!\n";
					return false;
				}
			mpObject2->FulfilsInvariant();
		}

		int logical_size = 21;
		int phantom_size = 2;
		for (i=0; i<100; i++) {
			DataChunk<T,10> data(double(30000+i));
			mpObject2->FulfilsInvariant();
			mpObject2->Write(0,10,data.Address());
			mpObject2->FulfilsInvariant();
			mpObject2->Resize(logical_size++, phantom_size++, 0);
			mpObject2->FulfilsInvariant();
			read = mpObject2->Read(1,10);
			mpObject2->FulfilsInvariant();
			for (int j=0; j<10; j++)
				if (data[j] != read[j]) {
					std::cerr << "TestUsage(): Written and read data differ!\n";
					return false;
				}
		}

		int size_increment = 1;
		int write_pos = 0;
		for (i=0; i<100; i++) {
			DataChunk<T,10> data(double(40000+i));
			mpObject2->FulfilsInvariant();
			mpObject2->Write(write_pos,10,data.Address());
			mpObject2->FulfilsInvariant();
			mpObject2->Resize(logical_size, phantom_size, write_pos);
			mpObject2->FulfilsInvariant();
			read = mpObject2->Read(write_pos + size_increment,10);
			mpObject2->FulfilsInvariant();
			for (int j=0; j<10; j++)
				if (data[j] != read[j]) {
					std::cerr << "TestUsage(): Written and read data differ!\n";
					return false;
				}
			size_increment++;
			logical_size += size_increment;
			phantom_size += 2;
			write_pos += size_increment;
		}

		return true;
	}

	template<class T>
	bool PhantomBufferTest<T>::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	template<class T>
	bool PhantomBufferTest<T>::Do()
	{
		bool res, ok = true;

		res = TestConstruction();
		if (!res) {
			std::cerr << "Construction failed." << std::endl;
			ok = false;
		}
		res = TestUsage();
		if (!res) {
			std::cerr << "Usage failed." << std::endl;
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
		CLAMTest::PhantomBufferTest<double> test;
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

