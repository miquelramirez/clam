#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

#include "PhantomBuffer.hxx"
#include "Assert.hxx"
#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>

namespace CLAMTest {

class TestsRandomStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsRandomStream );

class TestsRandomStream : public CppUnit::TestFixture
{
	typedef WritingRegion<char, CLAM::PhantomBuffer > WRegion;
	typedef WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion RRegion;

	WRegion mWriter;
	RRegion mOutputReader;
	std::list< RRegion * > mOtherReaders;
	std::ifstream mInputFile;
	std::string mInputString;
	std::ostringstream mOutput;
	std::ostringstream mBuff;

public:
	TestsRandomStream() :
		mInputFile( "testFile.txt" ),
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
		double rand = random() / float(RAND_MAX);
		return rand < chance;
	}

	int RandomIntFromTo(int from, int to)
	{
		int dist = to-from;
		int result = from + int((random() / double(RAND_MAX)) * dist);

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

	void MaybeCreateOtherReader()
	{
		if(ChanceOf(1-0.3))
			return;

		RRegion * newRegion = new RRegion; 
		mWriter.LinkRegions( *newRegion );
		mOtherReaders.push_back( newRegion );
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
				(*it)->Consume();
	}

	void test()
	{
		mWriter.Size(2);
		mWriter.Hop(2);
		mWriter.LinkRegions(mOutputReader);
		bool endOfFile = false;

		while(true)
		{	
			if ( mWriter.CanProduce() && !endOfFile )
			{
				endOfFile =  !FillWriterFromInputFile();
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
