#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"
#include "CharCopierExtractor.hxx"


namespace CLAMTest
{


class ExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ExtractorTest );

class ExtractorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExtractorTest );
	CPPUNIT_TEST(testInScopeBinding);
	CPPUNIT_TEST(testIndirectedBinding);
	CPPUNIT_TEST(testDoubleIndirectedBinding);
	CPPUNIT_TEST(testRangeIndirectBinding);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<char>       ( "Referenced","Input");
		mScheme.AddAttribute<char>       ( "Referenced","Output");
		mScheme.AddAttribute<unsigned>   ( "Referenced","ReverseReference");
		mScheme.AddAttribute<unsigned>   ( "Referencer","BadReference");
		mScheme.AddAttribute<unsigned>   ( "Referencer","Reference");
		mScheme.AddAttribute<char>       ( "Referencer","Output");
		mScheme.AddAttribute<std::string>( "Referencer","Concatenations");

		mPool = new CLAM::DescriptionDataPool(mScheme);
		mPool->SetNumberOfContexts("Referenced",10);
		mPool->SetNumberOfContexts("Referencer",3);
		{
			char * inputBuffer = mPool->GetWritePool<char>("Referenced","Input");
			for (unsigned i = 0; i<10; i++)
				inputBuffer[i]='a'+i;
		}
		{
			unsigned * inputBuffer = mPool->GetWritePool<unsigned>("Referenced","ReverseReference");
			for (unsigned i = 0; i<10; i++)
				inputBuffer[i]=9-i;
		}
		{
			unsigned * inputBuffer = mPool->GetWritePool<unsigned>("Referencer","Reference");
			for (unsigned i = 0; i<3; i++)
				inputBuffer[i]=3*i;
		}
		{
			unsigned * inputBuffer = mPool->GetWritePool<unsigned>("Referencer","BadReference");
			for (unsigned i = 0; i<3; i++)
				inputBuffer[i]=3*i;
			inputBuffer[0]=20;
		}
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{ 
		delete mPool;
	}

private:
	CLAM::DescriptionScheme mScheme;
	CLAM::DescriptionDataPool * mPool;

	void testInScopeBinding()
	{
		CharCopierExtractor extractor;

		CLAM::WriteHook<char> outputHook;
		extractor.GetOutHook()
			.Bind("Referenced","Output");
		extractor.GetInHook()
			.Bind("Referenced","Input");

		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}
		
		std::string expected(mPool->GetWritePool<char>("Referenced","Input"),3);
		std::string result(mPool->GetWritePool<char>("Referenced","Output"),3);
		
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}


	void testIndirectedBinding()
	{
		CharCopierExtractor extractor;

		CLAM::WriteHook<char> outputHook;
		extractor.GetOutHook()
			.Bind("Referencer","Output");
		extractor.GetInHook()
			.Bind("Referenced","Input")
			.Indirect("Referencer","Reference");

		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}
		std::string expected("adg",3);
		std::string result(mPool->GetWritePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testDoubleIndirectedBinding()
	{
		CharCopierExtractor extractor;

		CLAM::WriteHook<char> outputHook;
		extractor.GetOutHook()
			.Bind("Referencer","Output");
		extractor.GetInHook()
			.Bind("Referenced","Input")
			.Indirect("Referenced","ReverseReference")
			.Indirect("Referencer","Reference");

		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}
		std::string expected("jgd",3);
		std::string result(mPool->GetWritePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testRangeIndirectBinding()
	{
		CharJoinExtractor extractor;

		extractor.GetInHook()
			.Range(4)
			.Bind("Referenced","Input")
			.Indirect("Referencer","Reference");

		extractor.GetOutHook()
			.Bind("Referencer","Concatenations");


		for (extractor.Init(*mPool); extractor.IsInsideScope(); extractor.Next())
		{
			extractor.Extract();
		}

		std::string expected0("abcd",4);
		std::string expected1("defg",4);
		std::string expected2("ghij",4);
		std::string * results = mPool->GetWritePool<std::string>("Referencer","Concatenations");
		CPPUNIT_ASSERT_EQUAL(expected0,results[0]);
		CPPUNIT_ASSERT_EQUAL(expected1,results[1]);
		CPPUNIT_ASSERT_EQUAL(expected2,results[2]);
	}


};


} // namespace CLAMTest
