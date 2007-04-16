#include "WritingRegion.hxx" // CLAM
#include "ReadingRegion.hxx" // CLAM

#include "PhantomBuffer.hxx" // CLAM
#include "Assert.hxx" // CLAM
#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>
#include "cppUnitHelper.hxx"

namespace CLAMTest {

class TestsRandomStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsRandomStream );

class TestsRandomStream : public CppUnit::TestFixture
{
	typedef CLAM::WritingRegion<char, CLAM::PhantomBuffer > WRegion;
	typedef CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion RRegion;

	WRegion mWriter;
	RRegion mOutputReader;
	std::list< RRegion * > mOtherReaders;
	std::ifstream mInputFile;
	std::string mInputString;
	std::ostringstream mOutput;
	std::ostringstream mBuff;

public:
	TestsRandomStream() :
		mInputFile( GetTestDataDirectory("networkTestsData/testFile.txt").c_str()),
		mInputString( (std::istreambuf_iterator<char>(mInputFile)), std::istreambuf_iterator<char>() )
	{

		mInputFile.seekg( std::ios::beg );
		std::srand(std::time(0));
	}
		
	~TestsRandomStream()
	{
		std::list< RRegion* >::iterator it;
		for(it=mOtherReaders.begin(); it!=mOtherReaders.end(); it++)
			delete *it;
	}

	CPPUNIT_TEST_SUITE( TestsRandomStream );
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();
private:
	bool FillWriterFromInputFile()
	{
		int inchar;
		for (int i=0; i<mWriter.Size(); i++)
		{
			if (i<mWriter.Hop())
			{
				inchar = mInputFile.get();
				if (inchar<0)
				{
					mWriter[i] ='\0';
					return false;
				}
				mWriter[i] = inchar;
			}
			else
				mWriter[i] = 'X';
		}
		return true;
	}
	void DumpOutputReader()
	{
		for (int i=0; i<mOutputReader.Size(); i++)
		{
			char actualToken = mOutputReader[i];
			if (actualToken =='\n')
			{
				mOutput << mBuff.str() << '\n';
				mBuff.str("");
			}
			else
				mBuff << actualToken;
		}
	}

	bool ChanceOf(double chance)
	{
		double rand = std::rand() / float(RAND_MAX);
		return rand < chance;
		return true;
	}

	int RandomIntFromTo(int from, int to)
	{
		int dist = to-from;
		int r = rand();
		int result = from + int( r / double(RAND_MAX) * dist);

		CLAM_DEBUG_ASSERT(result>=from && result <=to, "TestsRandomStream::randomIntFromTo() - "
								 " result must be inside interval marked by parameters" );
		return result;
	}


	void MaybeChangeSizeWriter()
	{
		if (ChanceOf(1-0.3))
			return;

		int newSize = RandomIntFromTo(1, 40);
		mWriter.Size(newSize);
		mWriter.Hop(newSize);
	}
	
	void MaybeChangeHopWriter()
	{
		if (ChanceOf(1-0.3))
			return;

		int newSize = RandomIntFromTo(1, mWriter.Size());
		mWriter.Hop(newSize);
	}

	void MaybeChangeHopAndSizeOutputReader()
	{	
		if (ChanceOf(1-0.3))
			return;

		int newSize = RandomIntFromTo(1, 40);
		mOutputReader.Size(newSize);
		mOutputReader.Hop(newSize);
	}

	void CreateOtherReader()
	{
		int newSize = RandomIntFromTo(1, 40);
		RRegion * newRegion = new RRegion; 
		newRegion->Size(newSize);
		newRegion->Hop(newSize);
		mWriter.LinkRegions( *newRegion );
		mOtherReaders.push_back( newRegion );

	}
	void CreateOtherReadersRandomly()
	{
		int nreaders = RandomIntFromTo(3, 25);
		for( int i=0; i<nreaders; i++)
		{
			CreateOtherReader();
		}
	}
	void MaybeCreateOtherReader()
	{
		if (ChanceOf(0.5))
			return;
		CreateOtherReader();
	}
	
	void MaybeRemoveOtherReader()
	{	
		std::list< RRegion * >::iterator it;
		for(it=mOtherReaders.begin(); it!=mOtherReaders.end(); it++)
		{
			if(ChanceOf(0.1))
			{
				RRegion * toDelete = *it;
				mWriter.RemoveRegion( *toDelete );
				mOtherReaders.remove( toDelete );
				delete toDelete;
				return;
			}
		}
	}
	
	void MaybeModifyHopAndSizeOfOtherReaders()
	{	
		std::list< RRegion * >::iterator it;
		for(it=mOtherReaders.begin(); it!=mOtherReaders.end(); it++)
		{
			if(ChanceOf(0.2))
			{
				RRegion * otherRegion = *it;
				int newSize = RandomIntFromTo(1, 40);
				otherRegion->Size( newSize );
				otherRegion->Hop( newSize );

			}		
			if(ChanceOf(0.2))
			{
				RRegion * otherRegion = *it;
				int newHop = RandomIntFromTo(1, otherRegion->Size() );
				otherRegion->Hop( newHop );
			}

		}
	}

	void MaybeChangeOtherReaders()
	{
		MaybeCreateOtherReader();
		MaybeRemoveOtherReader();
		MaybeModifyHopAndSizeOfOtherReaders();
	}

	void OtherReadersConsume()
	{
		std::list< RRegion * >::iterator it;
		for(it=mOtherReaders.begin(); it!=mOtherReaders.end(); it++)
			if( (*it)->CanConsume() )
			{
				for(int i=0;i<(*it)->Size();i++)
				{
					(*it)->operator[](i);
				}
				(*it)->Consume();
			}
	}

	void test()
	{
		mWriter.Size(2);
		mWriter.Hop(2);
		mWriter.LinkRegions(mOutputReader);
		bool endOfFile = false;
		CreateOtherReadersRandomly();

		while(true)
		{	
			if ( mWriter.CanProduce() && !endOfFile )
			{
				endOfFile =  !FillWriterFromInputFile();
				CLAM_ASSERT(mWriter.CanProduce(), "mWriter should be able to produce");
				mWriter.Produce();
			}
								
			MaybeChangeSizeWriter();
			MaybeChangeHopWriter();
		//	MaybeChangeHopAndSizeOutputReader();
			MaybeChangeOtherReaders();

			OtherReadersConsume();
			
			if( mOutputReader.CanConsume() )
			{
				DumpOutputReader();
				mOutputReader.Consume();
			}
			else
				if( endOfFile ) break;
		}

		CPPUNIT_ASSERT_EQUAL( mInputString, mOutput.str() );
	}
};





} // namespace CLAMTest 

