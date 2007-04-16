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


class IndirectBindingTest;

CPPUNIT_TEST_SUITE_REGISTRATION( IndirectBindingTest );

class IndirectBindingTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( IndirectBindingTest );
	CPPUNIT_TEST(testInit_PointsToThePlaceReferencedByTheFirstReference);
	CPPUNIT_TEST(testNext_PointsToThePlaceReferencedByTheSecondReference);
	CPPUNIT_TEST(testIsInsideScope_returnsTrueBeforeEnd);
	CPPUNIT_TEST(testIsInsideScope_returnsFalseAfterLastReference);
	CPPUNIT_TEST(testGetForReading_failsWhenInvalidReference);
	CPPUNIT_TEST(testExtraction_usingHooks);
	CPPUNIT_TEST(testDoubleIndirection_usingHooks);

	CPPUNIT_TEST(testRangeInit_PointsToTheFirstRange);
	CPPUNIT_TEST(testGetRangeForReading_failsWhenInvalidReference);
	CPPUNIT_TEST(testRangeExtraction_usingHooks);

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<char>       ( "Referenced","Input");
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

	void testInit_PointsToThePlaceReferencedByTheFirstReference()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);
		const void * result = & (hook.GetForReading());
		const void * expected = mPool->GetReadPool<char>("Referenced","Input");

		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testNext_PointsToThePlaceReferencedByTheSecondReference()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);
		hook.Next();
		const void * result = & (hook.GetForReading());
		const void * thirdPosition =
			mPool->GetReadPool<char>("Referenced","Input")+3;

		CPPUNIT_ASSERT_EQUAL(thirdPosition,result);
	}
	
	void testIsInsideScope_returnsTrueBeforeEnd()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);

		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
		hook.Next();
		CPPUNIT_ASSERT(hook.IsInsideScope());
	}

	void testIsInsideScope_returnsFalseAfterLastReference()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);

		hook.Next();
		hook.Next();
		hook.Next();
		CPPUNIT_ASSERT(!hook.IsInsideScope());
	}

	void testGetForReading_failsWhenInvalidReference()
	{
		CLAM::ReadHook<char> hook;
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","BadReference");
		hook.Init(*mPool);
		try
		{
			const void * result = & (hook.GetForReading());
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Invalid cross-scope reference";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testExtraction_usingHooks()
	{
		CLAM::WriteHook<char> outputHook;
		CLAM::ReadHook<char> inputHook;
		inputHook.Bind("Referenced","Input");
		inputHook.Indirect("Referencer","Reference");
		outputHook.Bind("Referencer","Output");

		for (inputHook.Init(*mPool),outputHook.Init(*mPool); 
			inputHook.IsInsideScope() && outputHook.IsInsideScope();
			outputHook.Next(),inputHook.Next())
		{
			const char & input = inputHook.GetForReading();
			char & output = outputHook.GetForWriting();
			output = input;
		}
		std::string expected("adg",3);
		std::string result(mPool->GetWritePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testDoubleIndirection_usingHooks()
	{
		CLAM::WriteHook<char> outputHook;
		CLAM::ReadHook<char> inputHook;
		inputHook.Bind("Referenced","Input");
		inputHook.Indirect("Referenced","ReverseReference");
		inputHook.Indirect("Referencer","Reference");
		outputHook.Bind("Referencer","Output");

		for (inputHook.Init(*mPool),outputHook.Init(*mPool); 
			inputHook.IsInsideScope() && outputHook.IsInsideScope();
			outputHook.Next(),inputHook.Next())
		{
			const char & input = inputHook.GetForReading();
			char & output = outputHook.GetForWriting();
			output = input;
		}
		std::string expected("jgd",3);
		std::string result(mPool->GetWritePool<char>("Referencer","Output"),3);
		CPPUNIT_ASSERT_EQUAL(expected,result);
	}

	void testRangeInit_PointsToTheFirstRange()
	{
		CLAM::ReadRangedHook<char> hook;
		hook.Range(4);
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","Reference");
		hook.Init(*mPool);
		const char * begin;
		const char * end;
		hook.GetRangeForReading(begin,end);

		const char * expectedBegin = mPool->GetReadPool<char>("Referenced","Input");
		const char * expectedEnd = expectedBegin + 4;

		CPPUNIT_ASSERT_EQUAL( (void*)expectedBegin, (void*)begin);
		CPPUNIT_ASSERT_EQUAL( (void*)expectedEnd, (void*)end);
	}

	void testGetRangeForReading_failsWhenInvalidReference()
	{
		CLAM::ReadRangedHook<char> hook;
		hook.Range(4);
		hook.Bind("Referenced","Input");
		hook.Indirect("Referencer","BadReference");
		hook.Init(*mPool);
		try
		{
			const char * begin;
			const char * end;
			hook.GetRangeForReading(begin,end);
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Invalid cross-scope reference";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testRangeExtraction_usingHooks()
	{
		CLAM::ReadRangedHook<char> inputHook;
		inputHook.Range(4);
		inputHook.Bind("Referenced","Input");
		inputHook.Indirect("Referencer","Reference");

		CLAM::WriteHook<std::string> outputHook;
		outputHook.Bind("Referencer","Concatenations");

		for (inputHook.Init(*mPool),outputHook.Init(*mPool); 
			inputHook.IsInsideScope() && outputHook.IsInsideScope();
			outputHook.Next(),inputHook.Next())
		{
			const char * input;
			const char * inputEnd;
			inputHook.GetRangeForReading(input, inputEnd);
			std::string & output = outputHook.GetForWriting();
			for (output = ""; input<inputEnd; input++)
				output += *input;
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

