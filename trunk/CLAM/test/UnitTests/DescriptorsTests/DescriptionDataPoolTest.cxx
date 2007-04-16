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

#include "Pool.hxx" // CLAM
#include "DataTypes.hxx" // CLAM

namespace CLAMTest
{

class DescriptionDataPoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionDataPoolTest );

class DescriptionDataPoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionDataPoolTest );
	CPPUNIT_TEST( testUnpopulatedScopeIsNotFreed );
	CPPUNIT_TEST( testSetNumberOfContexts_withANonExistingScope );
	CPPUNIT_TEST( testInstanciateAttribute_withinUnexistingScope );
	CPPUNIT_TEST( testInstanciateAttribute_withinUnpopulatedScope );
	CPPUNIT_TEST( testInstanciateAttribute_whenTheAttributeDoesNotExist );
	CPPUNIT_TEST( testInstanciateAttribute_whenTheAttributeDoesNotExistButTheScopeIsNotPopulated );
	CPPUNIT_TEST( testGetAttribute_withDifferentAttributes );
	CPPUNIT_TEST( testGetAttribute_withTheSameAttributeTwice );
	CPPUNIT_TEST( testGetAttribute_fromAUnpopulatedScope );
	CPPUNIT_TEST( testGetAttribute_fromUnexistingScope );
	CPPUNIT_TEST( testGetAttribute_withNonInstantiatedAttribute );
	CPPUNIT_TEST( testInsertElement );
	CPPUNIT_TEST( testRemoveElement );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp()
	{
		mScheme.AddAttribute<CLAM::TData>   ("AudioSample","Level");
		mScheme.AddAttribute<unsigned>      ("Frame","Center");
		mScheme.AddAttribute<unsigned>      ("Frame","Size");
		mScheme.AddAttribute<std::string>   ("Frame","MyLabel");
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	CLAM::DescriptionScheme mScheme;
private:
	void testUnpopulatedScopeIsNotFreed()
	{
		CLAM::DescriptionDataPool data(mScheme);
		// Should not be a memory leak
	}

	void testSetNumberOfContexts_withANonExistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.SetNumberOfContexts("NonExistingScope",10);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Attribute scope 'NonExistingScope' not found";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_withinUnexistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("UnexistingScope","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Attribute scope 'UnexistingScope' not found";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_withinUnpopulatedScope()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Instantianting 'Frame:Center' but the scope is not yet populated";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_whenTheAttributeDoesNotExist()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",10);
		try
		{
			data.InstantiateAttribute("Frame","UnexistingAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an unexisting attribute 'Frame':'UnexistingAttribute'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInstanciateAttribute_whenTheAttributeDoesNotExistButTheScopeIsNotPopulated()
	{
		CLAM::DescriptionDataPool data(mScheme);
		try
		{
			data.InstantiateAttribute("Frame","UnexistingAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an unexisting attribute 'Frame':'UnexistingAttribute'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetAttribute_withDifferentAttributes()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",30);
		data.InstantiateAttribute("Frame","Center");
		data.InstantiateAttribute("Frame","Size");

		unsigned * centers = data.GetWritePool<unsigned>("Frame","Center");
		unsigned * sizes   = data.GetWritePool<unsigned>("Frame","Size");

		CPPUNIT_ASSERT(centers!=sizes);
	}

	void testGetAttribute_withTheSameAttributeTwice()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",30);
		data.InstantiateAttribute("Frame","Center");

		unsigned * centers = data.GetWritePool<unsigned>("Frame","Center");
		unsigned * centers2 = data.GetWritePool<unsigned>("Frame","Center");

		CPPUNIT_ASSERT_EQUAL(centers,centers2);
	}

	void testGetAttribute_fromAUnpopulatedScope()
	{
		CLAM::DescriptionDataPool data(mScheme);

		try
		{
			data.GetWritePool<unsigned>("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Writting data on 'Frame:Center' but the scope is not yet populated";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetAttribute_fromUnexistingScope()
	{
		CLAM::DescriptionDataPool data(mScheme);

		try
		{
			data.GetWritePool<unsigned>("UnexistingScope","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Attribute scope 'UnexistingScope' not found";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetAttribute_withNonInstantiatedAttribute()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",30);

		try
		{
			data.GetReadPool<unsigned>("Frame","Center");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting data from a non instanciated attribute 'Frame':'Center'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testInsertElement()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",10);
		{
			unsigned * centers = data.GetWritePool<unsigned>("Frame","Center");
			for (unsigned i=0; i<data.GetNumberOfContexts("Frame"); i++)
				centers[i] = 10+i;
		}

		data.Insert("Frame",4);

		std::ostringstream os;
		const unsigned * centers = data.GetReadPool<unsigned>("Frame","Center");
		for (unsigned i=0; i<data.GetNumberOfContexts("Frame"); i++)
		{
			os << centers[i] << " ";
		}
		std::string result = os.str();

		CPPUNIT_ASSERT_EQUAL(std::string("10 11 12 13 0 14 15 16 17 18 19 "),result);
	}
	void testRemoveElement()
	{
		CLAM::DescriptionDataPool data(mScheme);
		data.SetNumberOfContexts("Frame",10);
		{
			unsigned * centers = data.GetWritePool<unsigned>("Frame","Center");
			for (unsigned i=0; i<data.GetNumberOfContexts("Frame"); i++)
				centers[i] = 10+i;
		}

		data.Remove("Frame",4);

		std::ostringstream os;
		const unsigned * centers = data.GetReadPool<unsigned>("Frame","Center");
		for (unsigned i=0; i<data.GetNumberOfContexts("Frame"); i++)
		{
			os << centers[i] << " ";
		}
		std::string result = os.str();

		CPPUNIT_ASSERT_EQUAL(std::string("10 11 12 13 15 16 17 18 19 "),result);
	}
};


	// TODO: Insert when not instanciated



} // namespace CLAMTest

