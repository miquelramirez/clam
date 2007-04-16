/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "cppUnitHelper.hxx"

#include "Extractor.hxx" // CLAM
#include "DataTypes.hxx" // CLAM


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

