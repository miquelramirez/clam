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

#include "StreamBuffer.hxx"
#include "CircularStreamImpl.hxx"
#include <iostream>
#include <vector>

namespace CLAMTest {

	template<class T, class B>
	class StreamBufferTest {

		// Test flags
		unsigned int MaxHop;
		unsigned int MaxExtraLength;
		unsigned int NumRegions;
		bool verbose;

		// Testees

		struct RegParam {
			unsigned int hop;
			unsigned int length;
		};

		typedef std::vector<RegParam> RegionsParams;
		CLAM::WriteStreamRegion* mWriter;
		std::vector<CLAM::ReadStreamRegion*> mReaders;

		void DumpHops   (RegionsParams &regions);
		void DumpLengths(RegionsParams &regions);
		void DumpRegions(RegionsParams &regions);

		CLAM::StreamBuffer<T,B> *mpObject1;
		CLAM::StreamBuffer<T,B> *mpObject2;

		// Test methods

		unsigned long SchedLength();
		unsigned int  WindowLength();

		short Schedule(unsigned long pos);

		T DataFunction(unsigned long pos);

		void FillData (T* data,
		               unsigned long data_pos,
		               unsigned int data_length);
		bool CheckData(const T* data,
		               unsigned long data_pos,
		               unsigned int data_length);

		bool Execute(unsigned short region);

		bool TestConstruction();
		bool TestConfiguration();
		bool TestDestruction();

		// This is the real test method. All the following methods are
		// used to generate a long secuence of different region sets
		// with different parameter combinations. For each of this 
		// region sets this method is called.
		bool TestExecution();

		bool DoTestRegions(RegionsParams &regions);

		bool TestConfiguration(RegionsParams &regions);

		bool TestEqualLengths(RegionsParams &regions);
		bool TestGrowingLengths(RegionsParams &regions);
		bool TestShrinkingLengths(RegionsParams &regions);
		bool TestRandomLengths(RegionsParams &regions);

		bool TestLengths(RegionsParams &regions);

		bool TestEqualHops(RegionsParams &regions);
		bool TestGrowingHops(RegionsParams &regions);
		bool TestShrinkingHops(RegionsParams &regions);
		bool TestRandomHops(RegionsParams &regions);

		bool TestHops(RegionsParams &regions);

		bool TestFixedRegions();
		bool TestChangingRegions();
	public:
		StreamBufferTest();
		bool Do();
	};

	template<class T, class B>
	StreamBufferTest<T,B>::StreamBufferTest()
		: mpObject1(0),
		  mpObject2(0)
	{}

	template<class T, class B>
	void StreamBufferTest<T,B>::DumpHops(RegionsParams &regions)
	{
		unsigned int size = regions.size();
		for (unsigned int i=0; i<size; i++)
			std::cerr << regions[i].hop << " ";
		std::cerr << std::endl;
	}

	template<class T, class B>
	void StreamBufferTest<T,B>::DumpLengths(RegionsParams &regions)
	{
		unsigned int size = regions.size();
		for (unsigned int i=0; i<size; i++)
			std::cerr << regions[i].length << " ";
		std::cerr << std::endl;
	}

	template<class T, class B>
	void StreamBufferTest<T,B>::DumpRegions(RegionsParams &regions)
	{
		unsigned int size = regions.size();
		for (unsigned int i=0; i<size; i++)
			std::cerr << regions[i].hop    << ","
			          << regions[i].length << " ";
		std::cerr << std::endl;
	}

	template<class T, class B>
	unsigned long StreamBufferTest<T,B>::SchedLength()
	{
		// TODO: MCM of hop sizes.
		return 500;
	}

	template<class T, class B>
	unsigned int StreamBufferTest<T,B>::WindowLength()
	{
		// TODO: Calculate window length.
		return 100;
	}

	template<class T, class B>
	short StreamBufferTest<T,B>::Schedule(unsigned long pos)
	{
		unsigned long write_pos = mWriter -> Pos();
		short reader;
		int Nregions = mReaders.size();
		unsigned int AvailableData;
		for (reader=1; reader < Nregions; reader++)
		{
			AvailableData = write_pos - mReaders[reader]->Pos();
			if (AvailableData >= mReaders[reader]->MaxLength())
				return reader;
		}
		return 0;
	}

	template<class T, class B>
	T StreamBufferTest<T,B>::DataFunction(unsigned long pos)
	{
		return T(pos);
	}

	template<class T, class B>
	void StreamBufferTest<T,B>::FillData (T* data,
	                                      unsigned long data_pos,
	                                      unsigned int length)
	{
		unsigned int i;
		for (i=0; i<length; i++)
			data[i] = DataFunction(data_pos+i);
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::CheckData(const T* data,
	                                      unsigned long data_pos,
	                                      unsigned int length)
	{
		unsigned int i;
		for (i=0; i<length; i++)
			if (data[i] != DataFunction(data_pos+i))
				return false;
		return true;
	}


	template<class T, class B>
	bool StreamBufferTest<T,B>::Execute(unsigned short region)
	{
		bool res = true;
		if (region == 0) // Writer
		{
			CLAM::Array<T> data;
			mpObject2->GetAndActivate(mWriter,data);
			FillData(data.GetPtr(),mWriter->Pos(),mWriter->Len());
			mpObject2->LeaveAndAdvance(mWriter);
			return true;
		}
		// Reader
		CLAM::Array<T> data;
		CLAM::ReadStreamRegion *reader = mReaders[region];
		mpObject2->GetAndActivate(reader,data);
		res = CheckData(data.GetPtr(),reader->Pos(),reader->Len());
		mpObject2->LeaveAndAdvance(reader);
		return res;
	}



	template<class T, class B>
	bool StreamBufferTest<T,B>::TestConstruction()
	{
		mpObject1 = new CLAM::StreamBuffer<T,B>();
		mpObject2 = new CLAM::StreamBuffer<T,B>(*mpObject1);
		return true;
	}

	
	template<class T, class B>
	bool StreamBufferTest<T,B>::TestConfiguration(RegionsParams &regions)
	{
		bool ok=true;
		int Nregions = regions.size();
		int i;

		mReaders.resize(Nregions);
		mWriter = mpObject2->NewWriter(regions[0].hop, regions[0].length);
		mReaders[0]=0;
		for (i=1; i<Nregions; i++)
			mReaders[i] = mpObject2->NewReader(regions[i].hop, regions[i].length);
		mpObject2->Configure(WindowLength());
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestExecution()
	{
		bool ok=true;
		unsigned long sched_length = SchedLength();
		unsigned long step;

		for (step = 0; step < sched_length * 2; step++)
		{
			short next_reg = Schedule(step);
			if (!Execute(next_reg)) {
				std::cerr << "StreamBufferTest<T,B>::TestExecution(): "
				             "Execution failed at step "
				          << step << "." << std::endl;
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}


	template<class T, class B>
	bool StreamBufferTest<T,B>::DoTestRegions(RegionsParams &regions)
	{
		bool res, ok=true;

		if (verbose)
			DumpRegions(regions);

		res = TestConstruction();
		if (!res) {
			std::cerr << "Construction failed." << std::endl;
			ok = false;
		}
		res = TestConfiguration(regions);
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


	template<class T, class B>
	bool StreamBufferTest<T,B>::TestEqualLengths(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int extra_length;
		unsigned int i;
		unsigned int Nregions = regions.size();

		if (verbose) std::cerr << "Testing Equal Lengths" << std::endl;

		for (extra_length = 0; extra_length < MaxExtraLength; extra_length++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].length = regions[i].hop + extra_length;
			res = DoTestRegions(regions);
			if (!res) {
				std::cerr << "Test failed with lengths: " << std::endl;
				DumpLengths(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestGrowingLengths(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int extra_length;
		unsigned int i;
		unsigned int Nregions = regions.size();

		if (verbose) std::cerr << "Testing Growing Lengths" << std::endl;

		for (extra_length = 0; extra_length < MaxExtraLength; extra_length++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].length = regions[i].hop + 1
					+ (i+1)*extra_length/(Nregions+1);
			res = DoTestRegions(regions);
			if (!res) {
				std::cerr << "Test failed with lengths: " << std::endl;
				DumpLengths(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestShrinkingLengths(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int extra_length;
		unsigned int i;
		unsigned int Nregions = regions.size();

		if (verbose) std::cerr << "Testing Shrinking Lengths" << std::endl;

		for (extra_length = 0; extra_length < MaxExtraLength; extra_length++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].length = regions[i].hop + 1
					+ (Nregions-i)*extra_length/(Nregions+1);
			res = DoTestRegions(regions);
			if (!res) {
				std::cerr << "Test failed with lengths: " << std::endl;
				DumpLengths(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestRandomLengths(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int extra_length;
		unsigned int i;
		unsigned int Nregions = regions.size();

		if (verbose) std::cerr << "Testing Random Lengths" << std::endl;

		for (extra_length = 0; extra_length < MaxExtraLength*3; extra_length++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].length = regions[i].hop + 1 + 
					unsigned(double(MaxExtraLength)*rand()/(RAND_MAX+1.0));
			res = DoTestRegions(regions);
			if (!res) {
				std::cerr << "Test failed with lengths: " << std::endl;
				DumpLengths(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestLengths(RegionsParams &regions)
	{
		bool res, ok=true;

		res = TestEqualLengths(regions);
		if (!res) {
			std::cerr << "EqualHop test failed." << std::endl;
			ok = false;
		}
		res = TestGrowingLengths(regions);
		if (!res) {
			std::cerr << "GrowingHop test failed." << std::endl;
			ok = false;
		}
		res = TestShrinkingLengths(regions);
		if (!res) {
			std::cerr << "ShrinkingHop test failed." << std::endl;
			ok = false;
		}
		res = TestRandomLengths(regions);
		if (!res) {
			std::cerr << "RandomHop test failed." << std::endl;
			ok = false;
		}
		return ok;
	}


	template<class T, class B>
	bool StreamBufferTest<T,B>::TestEqualHops(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int hop;
		unsigned int Nregions = regions.size();
		unsigned int i;

		std::cerr << "   Testing Equal Hops" << std::endl;

		for (hop = 1; hop < MaxHop; hop++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].hop = hop;
			res = TestLengths(regions);
			if (!res) {
				std::cerr << "Test failed with hops: " << std::endl;
				DumpHops(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestGrowingHops(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int hop;
		unsigned int Nregions = regions.size();
		unsigned int i;

		std::cerr << "   Testing Growing Hops" << std::endl;

		for (hop = 1; hop < MaxHop; hop++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].hop = 1 + (i+1)*hop/(Nregions+1);
			res = TestLengths(regions);
			if (!res) {
				std::cerr << "Test failed with hops: " << std::endl;
				DumpHops(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestShrinkingHops(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int hop;
		unsigned int Nregions = regions.size();
		unsigned int i;

		std::cerr << "   Testing Shrinking Hops" << std::endl;

		for (hop = 1; hop < MaxHop; hop++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].hop = 1 + (Nregions-i)*hop/(Nregions+1);
			res = TestLengths(regions);
			if (!res) {
				std::cerr << "Test failed with hops: " << std::endl;
				DumpHops(regions);
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestRandomHops(RegionsParams &regions)
	{
		bool res, ok=true;
		unsigned int hop;
		unsigned int Nregions = regions.size();
		unsigned int i;

		std::cerr << "   Testing Random Hops" << std::endl;

		for (hop = 1; hop < MaxHop*3; hop++)
		{
			for (i=0; i<Nregions; i++)
				regions[i].hop = 1+unsigned(double(MaxHop)*rand()/(RAND_MAX+1.0));
			res = TestLengths(regions);
			if (!res) {
				std::cerr << "Test failed with hops: " << std::endl;
				DumpHops(regions);
				ok = false;
			}
		}
		return ok;
	}


	template<class T, class B>
	bool StreamBufferTest<T,B>::TestHops(RegionsParams &regions)
	{
		bool res, ok = true;

		res = TestEqualHops(regions);
		if (!res) {
			std::cerr << "EqualHop test failed." << std::endl;
			ok = false;
		}
		res = TestGrowingHops(regions);
		if (!res) {
			std::cerr << "GrowingHop test failed." << std::endl;
			ok = false;
		}
		res = TestShrinkingHops(regions);
		if (!res) {
			std::cerr << "ShrinkingHop test failed." << std::endl;
			ok = false;
		}
		res = TestRandomHops(regions);
		if (!res) {
			std::cerr << "RandomHop test failed." << std::endl;
			ok = false;
		}

		return ok;
	}



	template<class T, class B>
	bool StreamBufferTest<T,B>::TestFixedRegions()
	{
		bool res, ok = true;
		unsigned int Nregions;

		for (Nregions = 2; Nregions < NumRegions; Nregions++)
		{
			RegionsParams regions(Nregions);

			std::cerr << "Testing " << Nregions 
			          << " Fixed Regions" << std::endl;

			res = TestHops(regions);
			if (!res) {
				std::cerr << "TestHops failed." << std::endl;
				ok = false;
			}
		}
		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::TestChangingRegions()
	{
		bool ok=true;

		std::cerr << " Warning: Changing regions test not implemented!" << std::endl;

		return ok;
	}

	template<class T, class B>
	bool StreamBufferTest<T,B>::Do()
	{
		MaxHop = 10;
		NumRegions = 5;
		MaxExtraLength = 6;
		verbose=false;

		bool res, ok = true;

		res = TestFixedRegions();
		if (!res) {
			std::cerr << "Fixed hop test failed." << std::endl;
			ok = false;
		}

		res = TestChangingRegions();
		if (!res) {
			std::cerr << "Changing hop test failed." << std::endl;
			ok = false;
		}

		return ok;
	}

}


int main()
{
	try {
		CLAMTest::StreamBufferTest<double,CLAM::CircularStreamImpl<double> > test;
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

