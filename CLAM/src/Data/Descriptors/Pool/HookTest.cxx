#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Extractor.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{


class HookTest;

CPPUNIT_TEST_SUITE_REGISTRATION( HookTest );

class HookTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( HookTest );
	CPPUNIT_TEST(testInit_PointsToThePoolBegin);
	CPPUNIT_TEST(testNext_PointsToTheNextPoolData);
	CPPUNIT_TEST(testIsInsideScope_ReturnsTrueWhileInsideTheScope);
	CPPUNIT_TEST(testIsInsideScope_ReturnsFalseBeyondTheScope);

	CPPUNIT_TEST(testWriteInit_PointsToThePoolBegin);
	CPPUNIT_TEST(testWriteNext_PointsToTheNextPoolData);
	CPPUNIT_TEST(testWriteIsInsideScope_ReturnsTrueWhileInsideTheScope);
	CPPUNIT_TEST(testWriteIsInsideScope_ReturnsFalseBeyondTheScope);

	CPPUNIT_TEST(testTransformUsingHooks);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<char>(
				"TestScope1","InputData");
		mScheme.AddAttribute<char>(
				"TestScope1","OutputData");

		mPool = new CLAM::DescriptionDataPool(mScheme);
		mPool->SetNumberOfContexts("TestScope1",3);

		char * inputBuffer = mPool->GetWritePool<char>("TestScope1","InputData");
		for (unsigned i = 0; i<3; i++)
			inputBuffer[i]='a'+i;

		mInputBuffer = inputBuffer;

	}

	/// Common clean up, executed after each test method
	void tearDown()
	{ 
		delete mPool;
	}

private:
	CLAM::DescriptionScheme mScheme;
	CLAM::DescriptionDataPool * mPool;
	const char * mInputBuffer;

	void testInit_PointsToThePoolBegin()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);
		const char & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testNext_PointsToTheNextPoolData()
	{
		const char * expected = mInputBuffer;
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);
		hook.Next();
		const char & result = hook.GetForReading();

		CPPUNIT_ASSERT_EQUAL(expected+1, &result);
	}

	void testIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("TestScope1","InputData");
		hook.Init(*mPool);

		// Advance until the end
		hook.Next();
		hook.Next();
		// Go Beyond
		hook.Next();

		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testWriteInit_PointsToThePoolBegin()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);
		char & result = hook.GetForWriting();

		char * expected = mPool->GetWritePool<char>("TestScope1","OutputData");

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteNext_PointsToTheNextPoolData()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);
		hook.Next();
		char & result = hook.GetForWriting();
		char * expected = mPool->GetWritePool<char>("TestScope1","OutputData") + 1;

		CPPUNIT_ASSERT_EQUAL(expected, &result);
	}

	void testWriteIsInsideScope_ReturnsTrueWhileInsideTheScope()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testWriteIsInsideScope_ReturnsFalseBeyondTheScope()
	{
		CLAM::WriteHook<char> hook;
		hook.Bind("TestScope1","OutputData");
		hook.Init(*mPool);

		// Advance until the end
		hook.Next();
		hook.Next();
		// Go Beyond
		hook.Next();

		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testTransformUsingHooks()
	{
		CLAM::WriteHook<char> outputHook;
		CLAM::ReadHook<char> inputHook;
		inputHook.Bind("TestScope1","InputData");
		outputHook.Bind("TestScope1","OutputData");
		inputHook.Init(*mPool);
		outputHook.Init(*mPool);

		for (; inputHook.IsInsideScope() && outputHook.IsInsideScope(); outputHook.Next(), inputHook.Next())
		{
			char & output = outputHook.GetForWriting();
			const char  & input = inputHook.GetForReading();
			output = input;
		}
		
		std::string expected(mPool->GetWritePool<char>("TestScope1","InputData"),3);
		std::string result(mPool->GetWritePool<char>("TestScope1","OutputData"),3);
		

		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

};


} // namespace CLAMTest
