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

#include "Array.hxx"
#include "Assert.hxx"
#include <iostream>
#include <cstdlib> // for rand
#include <ctime>
#include <vector>

using std::cout;
using std::endl;
using std::cerr;
using std::ostream;
using std::istream;
using CLAM::Err;

namespace CLAMTest {

	using namespace CLAM;

	template<class T>
		class MyArray : public std::vector<T> {
		typedef std::vector<T> Super;
	public:
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		MyArray(TSize size = 0) : std::vector<T>(size){}
		void Init() { Super::reserve(0); Super::resize(0); }
		MyArray(T* ptr,int size = 0) {
			throw(Err("MyArray::MyArray(T*): Not implemented"));
		}
		bool OwnsMemory() const { return true; }
		int Size() const { return Super::size(); }
		int SizeInBytes() const { return Super::size() * sizeof(T); }
		int AllocatedSize() const { return Super::capacity(); }
		int AllocatedSizeInBytes() const { return Super::capacity() * sizeof(T); }
		void SetSize(int size) { Super::resize(size); }
		void Resize(int size) { Super::reserve(size); }
		const T* GetPtr(void) const {return &(*this)[0];}
		T* GetPtr(void) {return &(*this)[0];}
//		void SetPtr(T* ptr) {
//			throw(Err("MyArray::SetPtr(T*): Not implemented"));
//		}
		void AddElem(const T& elem) {Super::push_back(elem);}
		void InsertElem(int pos, const T& elem) {Super::insert(iterator(&(Super::operator[](pos))),elem);}
		void DeleteElem(int where) { Super::erase(iterator(&Super::operator[](where)));}
		MyArray<T>& operator += (const MyArray<T>& src) {
			Super::insert(Super::end(),src.begin(),src.end());
			return *this;
		}
		void Apply( T (*f)(T,int),int parameter ){
			int i; 
			int s=Super::size();
			for (i=0; i<s; i++)
				(*this)[i] = f( (*this)[i], parameter );
		}
	};

	class FatOne {
		int *data;
		int val;
	public:
		int Val() const {return val;}
		void Set(int i) {
			val = i;
			for (int j=0; j<32; j++) data[j]=i;
		}
		FatOne() : data(new int[32]) {
			Set(5555);
		}
		FatOne(int i) : data(new int[32]) {
			Set(i);
		}
		FatOne(const FatOne &f) : data(new int[32]), val(f.val) {
			for (int j=0; j<32; j++) data[j]=f.data[j];
		}
		~FatOne() {
			for (int j=0; j<32; j++) {
				if (data[j] != val) {
					cout << "FatOne::Destructor: Inconsistent!";
					cout << endl;
					throw(Err("FatOne::Destructor: Inconsistent!"));
				}
			}
			delete [] data;
		}
		FatOne& operator =(const FatOne& f) {
			val = f.val;
			for (int j=0; j<32; j++) data[j]=f.data[j];
			return *this;
		}
		bool operator ==(const FatOne& src) const {
			if (val != src.val) 
				return false;
			for (int j=0; j<32; j++)
				if (data[j] != src.data[j])
					return false;
			return true;
		}

		bool operator !=(const FatOne& src) const {
			if (val != src.val) 
				return true;
			for (int j=0; j<32; j++) 
				if (data[j] != src.data[j])
					return true;
			return false;
		}
	};
	istream &operator>>(istream &in, FatOne &d)
	{
		unsigned int i;
		in >> i;
		d.Set(i);
		return in;
	}

	ostream &operator<<(ostream &out, const FatOne &d)
	{
		out << d.Val();
		return out;
	}


	class SelfRefering {
		int data;
		SelfRefering *me;
	public:
		// Default constructor: initializes self-reference.
		int Data() const {return data;}
		void Data(int i) {data=i;}
		SelfRefering(int i=0)
			{
				data = i;
				me = this;
			}
		// Copy constructor: must be implemented to keep the self-reference consistent.
		SelfRefering(const SelfRefering& src) {
			data=src.data;
			me = this;
		}
		// Copy operator: same as for the constructor.
		SelfRefering &operator = (const SelfRefering& src) {
			data=src.data;
			me = this;
			return *this;
		}
		// The destructor: is everything ok?
		~SelfRefering() {
			if (me != this)
				throw(CLAM::Err("SelfRefering error: invalid self reference"));
		}

		bool operator ==(const SelfRefering& src) const {
			return Data() == src.Data();
		}

		bool operator !=(const SelfRefering& src) const {
			return Data() != src.Data();
		}

	};
	istream &operator>>(istream &in, SelfRefering &d)
	{
		unsigned int i;
		in >> i;
		d.Data(i);
		return in;
	}

	ostream &operator<<(ostream &out, const SelfRefering &d)
	{
		out << d.Data();
		return out;
	}


	template<class T, class TArray>
	class TestArray {

		TArray &victim;

		static int mMaxSize;
		long mIterations;
		long mMaxIterations;

	public:

		TestArray(TArray &a) : victim(a) {}

		typedef enum {
			eAccess = 0,
			eInsert,
			eRemove,
			eResize,
			eSetSize,
			eStep,
			eCopy,
			eConcat,
			eConstruct,
			NUMTypes
		} ETestType;

		bool TestAccess();
		bool TestInsert();
		bool TestAdd();
		bool TestRemove();
		bool TestResize();
		bool TestSetSize();
		bool TestStep();
		bool TestCopy();
		bool TestConcat();
		bool TestConstruct();

		bool Test(ETestType type) {
			switch(type) {
			case eAccess:
				return TestAccess();
			case eInsert:
				return TestInsert();
			case eRemove:
				return TestRemove();
			case eResize:
				return TestResize();
			case eSetSize:
				return TestSetSize();
			case eStep:
				return TestStep();
			case eCopy:
				return TestCopy();
			case eConcat:
				return TestConcat();
			case eConstruct:
				return TestConstruct();
			default:
				return false;
			}
		}

		void TestFailed()
			{
				cout << "Test Failed" << endl;
			}

		bool Do(int max, int init=0)	{
			mIterations = init;
			mMaxIterations = max;
			int tnum;
			while (mIterations < mMaxIterations) {
				cout << mIterations << ": ";
				tnum = (int) (double(NUMTypes)*rand()/(RAND_MAX+1.0));
				if (!Test(ETestType(tnum)))
					return false;
				mIterations ++;
			}
			return true;
		}
	};

	template<class T,class A>
	int TestArray<T,A>::mMaxSize = 16;

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestAccess()
	{
		int i;
		int size = victim.Size();
		cout << "Testing access with size " << size << endl;
		T * backup = new T[size];
		for (i=0; i<size;i++)
			backup[i] = victim[i];
		for (i=size-1; i>=0;i--) {
			victim[i]=T(0);
			if (victim[i] != T(0)) {
				TestFailed();
				return false;
			}
		}
		for (i=0; i<size;i++) {
			victim[i] = backup[i];
			if (backup[i] != victim[i] || 
				backup[i] != victim.GetPtr()[i] )
			{
				TestFailed();
				return false;
			}
		}
		delete [] backup;
#ifdef CLAM_TEST_DEBUG_CHECKS
		bool thrown = false;
		try {
			victim[size] = 0;
		}
		catch (Err &e) {
			if (strcmp(e.what(),TArray::msgIndexOutOfRange)) {
				cout << "Incorrect exception thrown after overflow access: "
					 << e.what() << endl;
				TestFailed();
				return false;
			}
			thrown = true;
		}
		if (!thrown) {
			cout << " Overflow access accepted in DEBUG mode! " << endl;
			TestFailed();
			return false;
		}
		thrown = false;
		try {
			victim[-1] = 0;
		}
		catch (Err &e) {
			if (strcmp(e.what(),TArray::msgIndexOutOfRange)) {
				cout << "Incorrect exception thrown after overflow -1 access: "
					 << e.what() << endl;
				TestFailed();
				return false;
			}
			thrown = true;
		}
		if (!thrown) {
			cout << " Overflow -1 access accepted in DEBUG mode! " << endl;
			TestFailed();
			return false;
		}
#endif
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestAdd()
	{
		cout << "Testing addition " << endl;
		int pos = victim.Size();
		T elem(rand());
		victim.AddElem(elem);
		if (victim[pos] != elem) {
			TestFailed();
			return false;
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestInsert()
	{
		if (victim.Size() == 0) {
			cout << "Test insertion skipped" << endl;
			return true;
		}
#ifdef CLAM_TEST_DEBUG_CHECKS
		int pos = (int) (double(victim.Size()-1)*1.2*rand()/(RAND_MAX+1.0));
#else
		int pos = (int) (double(victim.Size()-1)*rand()/(RAND_MAX+1.0));
#endif
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		cout << "Testing " << rep << " insertion(s) at pos " << pos << endl;
		try {
			while (rep--) {
				T elem;
				victim.InsertElem(pos,elem);
				if (victim[pos] != elem) {
					TestFailed();
					return false;
				}
			}
		}
		catch (Err &e) {
			if ( strcmp(e.what(),TArray::msgInsertOutOfRange) ) {
				cout << " Exception thrown in InsertElem: " << e.what() << endl;
				TestFailed();
				return false;
			}
			if ( pos < victim.Size() ) {
				cout << " IndexOutOfRange thrown after correct InsertElem call!" << endl;
				TestFailed();
				return false;
			}
			return true;
		}
		if (pos >= victim.Size()) {
			cout << " Exception not thrown after incorrect InsertElem call!" << endl;
			TestFailed();
			return false;
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestRemove()
	{
		int size = victim.Size();
		if (!size) {
			cout << "Test remove skipped" << endl;
			return true;
		}
		int pos = int(double(size)*rand()/(RAND_MAX+1.0));
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		if (rep > (size-pos))
			rep = size-pos;
		cout << "Testing " << rep << " deletion(s) at pos " << pos << endl;
		while (rep--) {
			victim.DeleteElem(pos);
			if (victim.Size() != size-1) {
				TestFailed();
				return false;
			}
			size--;
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestResize()
	{
		int new_size = int(double((victim.AllocatedSize()+1)*2)*rand()/(RAND_MAX+1.0));
		if (new_size > mMaxSize)
			new_size = mMaxSize;
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		cout << "Testing " << rep << " resize(s) to size " << new_size << endl;
		while (rep--) {
			victim.Resize(new_size);
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestSetSize()
	{
#ifdef CLAM_TEST_DEBUG_CHECKS
		int new_size = int(double((victim.AllocatedSize()+1)*2)*rand()/(RAND_MAX+1.0));
#else
		int new_size = int(double(victim.AllocatedSize()  )*rand()/(RAND_MAX+1.0));
#endif
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		cout << "Testing " << rep << " SetSize(s) to size " << new_size << endl;
		try {
			while (rep--) {
				victim.SetSize(new_size);
			}
		}
		catch (Err &e) {
			if ( strcmp(e.what(),TArray::msgSetSizeOutOfRange) ) {
				cout << " Exception thrown in SetSize: " << e.what() << endl;
				TestFailed();
				return false;
			}
			if ( new_size <= victim.AllocatedSize() ) {
				cout << " IndexOutOfRange thrown after correct SetSize call!" << endl;
				TestFailed();
				return false;
			}
			return true;
		}
		if (victim.Size() > victim.AllocatedSize()) {
			cout << " Size is bigger than allocated size!" << endl;
			TestFailed();
			return false;
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestStep()
	{
		cout << "TODO: TestStep not implemented!!!" << endl;
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestConcat()
	{
		int old_size = victim.Size();
		int new_size = int(double((old_size+1)*2)*rand()/(RAND_MAX+1.0));
		int i;
		if (new_size > mMaxSize)
			new_size = mMaxSize;
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		cout << "Testing " << rep << " concat(s) with size " << new_size << endl;
		TArray new_array;
		new_array.Resize(new_size);
		new_array.SetSize(new_size);
		for (i=0;i<new_size;i++)
			new_array[i]=T(rand());
		while (rep--) {
			old_size = victim.Size();
			victim += new_array;
			for (i=0; i<new_size; i++)
				if ( victim[old_size+i] != new_array[i] ) {
					TestFailed();
					return false;
				}
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestCopy()
	{
		TArray new_array;
		int new_size = int(double((victim.Size()+1)*2)*rand()/(RAND_MAX+1.0));
		int i;
		if (new_size > mMaxSize)
			new_size = mMaxSize;
		new_array.Resize(new_size);
		new_array.SetSize(new_size);
		int rep = int(1+(3.0*rand()/(RAND_MAX+1.0)));
		cout << "Testing " << rep << " copy(s) with size " << new_size << endl;
		for (i=0;i<new_size;i++)
			new_array[i]=T(rand());
		while (rep--) {
			victim = new_array;
			if (!(victim == new_array)) {
				TestFailed();
				return false;
			}
		}
		return true;
	}

	template<class T,class TArray>
	bool TestArray<T,TArray>::TestConstruct()
	{
		if (mIterations == mMaxIterations)
			return 1;
		cout << "Testing constructor..." << endl;
		TArray *new_array = new TArray(victim);
		TestArray<T,TArray> new_test(*new_array);
		int end_iter = mIterations + 100;
		if (end_iter > mMaxIterations)
			end_iter = mMaxIterations;
		if (!new_test.Do(end_iter,mIterations))
			return false;
		delete new_array;
		mIterations = end_iter;
		return true;
	}

}

int main()
{
	srand(333);
//	srand(time(0));

	using namespace CLAMTest;
	try {
//  		MyArray<char> w;
//  		TestArray<char,MyArray<char> > testA(w);
//  		if (!testA.Do(50)) {
//  			std::cout << "Failed." << std::endl;
//  			return 1;
//  		}

//  		MyArray<double> x;
//  		TestArray<double,MyArray<double> > testB(x);
//  		if (!testB.Do(50)) {
//  			std::cout << "Failed." << std::endl;
//  			return 1;
//  		}
//  		MyArray<SelfRefering> y;
//  		TestArray<SelfRefering,MyArray<SelfRefering> > testC(y);
//  		if (!testC.Do(50)) {
//  			std::cout << "Failed." << std::endl;
//  			return 1;
//  		}

//  		MyArray<FatOne> z;
//  		TestArray<FatOne,MyArray<FatOne> > testD(z);
//  		if (!testD.Do(50)) {
//  			std::cout << "Failed." << std::endl;
//  			return 1;
//  		}

		std::cout << std::endl << "Testing char array" << std::endl;
		Array<char> a;
		TestArray<char,Array<char> > test1(a);
		if (!test1.Do(10000)) {
			std::cout << "Failed." << std::endl;
			return 1;
		}


		std::cout << std::endl << "Testing double array" << std::endl;
		Array<double> b;
		TestArray<double,Array<double> > test2(b);
		if (!test2.Do(10000)) {
			std::cout << "Failed." << std::endl;
			return 1;
		}

		std::cout << std::endl << "Testing self-referencing array" << std::endl;
		Array<SelfRefering> c;
		TestArray<SelfRefering,Array<SelfRefering> > test3(c);
		if (!test3.Do(10000)) {
			std::cout << "Failed." << std::endl;
			return 1;
		}

		std::cout << std::endl << "Testing dinamic memory array" << std::endl;
		Array<FatOne> d;
		TestArray<FatOne,Array<FatOne> > test4(d);
		if (!test4.Do(10000)) {
			std::cout << "Failed." << std::endl;
			return 1;
		}

		std::cout << "Passed." << std::endl;
		return 0;

	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl
				  << "Failed." << std::endl;
		return 1;
	}
	catch (...) {
		std::cout << "Unknown Exception!!!" << std::endl
				  << "Failed." << std::endl;
		return 1;
	}
}

