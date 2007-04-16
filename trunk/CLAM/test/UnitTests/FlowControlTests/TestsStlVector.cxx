
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <list>
//#include <deque> VC7.1 compiler chokes on that.


namespace CLAMTest {

class TestsStlVector ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStlVector );

class TestsStlVector : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsStlVector );

	CPPUNIT_TEST( testReserve );
	CPPUNIT_TEST( testAt );
	CPPUNIT_TEST( testResize_beforeReserve );
	CPPUNIT_TEST( testList_isCircularWithPhantomElement );
	CPPUNIT_TEST( testDeque_elementsNotInContiguousSpace );
	CPPUNIT_TEST( testVectorInsertInTheMiddle );

	CPPUNIT_TEST_SUITE_END();

	void testReserve()
	{
		std::vector<int> vector;
		vector.reserve(5);
		CPPUNIT_ASSERT(0 == vector.size());

	}
	void testAt()
	{
		std::vector<int> vector;
		vector.reserve(5);
		try {
			vector.at(1); // en canvi l'operator[] no fa comprovacions
			CPPUNIT_FAIL("hauria d'haver llençat exception"); //funciona tant en debug com en release
		} catch (std::exception&)
		{
		}
	}

	void testResize_beforeReserve()
	{
		std::vector<int> vector;
		vector.resize(5);
		CPPUNIT_ASSERT(5 == vector.size());
		CPPUNIT_ASSERT(5 <= vector.capacity());

		vector.at(4)=1;
		CPPUNIT_ASSERT(1==vector.at(4));
	}

	void testList_isCircularWithPhantomElement()
	{
		typedef std::list<int> List;
		List list;
		list.push_back(0);
		list.push_back(1);
		list.push_back(2);

		List::iterator it=list.begin();
		for(int i=0; i<3; i++,it++);

		CPPUNIT_ASSERT(it==list.end());

		it++; // we are out of range: let's see that magically we're in the beginning
		CPPUNIT_ASSERT(it!=list.end());
		(*it)=66;
		CPPUNIT_ASSERT(66==*list.begin());

	}

	void testDeque_elementsNotInContiguousSpace()
	{
/*		VC7 Compiler problem with deque
 
		std::deque<int> deque;
		
		deque.push_back(1);
		deque.push_back(2);
		deque.push_back(3);
		deque.push_back(4);
		deque.push_back(5);
		deque.push_front(0);

		CPPUNIT_ASSERT_EQUAL(5, deque.at(5));
		CPPUNIT_ASSERT(5 != *(&(deque.at(0))+5) );
*/
}

	void testVectorInsertInTheMiddle()
	{
		std::vector<char> buff;
		buff.resize(8);
		buff[0]='h';
		buff[1]='e';
		buff[2]='o';
		buff[3]=' ';
		buff[4]='a';
		buff[5]='l';
		buff[6]='l';
		buff[7]='\0';
		std::vector<char>::iterator it  = buff.begin();
		it += 2;
		buff.insert(it, 2, 'l');

		CPPUNIT_ASSERT_EQUAL( 10, int(buff.size()) );
		CPPUNIT_ASSERT_EQUAL( std::string("hello all"), 
				std::string((char *)(&buff[0])) );
	}
};


} // namespace CLAMTest 

