#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "BaseLoggable.hxx"

namespace CLAMTest
{

class cppUnitHelperTest;
CPPUNIT_TEST_SUITE_REGISTRATION( cppUnitHelperTest );

class cppUnitHelperTest : public CppUnit::TestFixture, public BaseLoggable
{
	CPPUNIT_TEST_SUITE( cppUnitHelperTest );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoToString_NotEquals );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoToString_Equals );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoEqual_Equals );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoEqual_DifferentUnrelatedClasses );
	CPPUNIT_TEST( testAssertionTraitsTypeInfoEqual_DifferentSuperAndSubClass );
	// tests for class BaseLoggable
	CPPUNIT_TEST( testLoggable_InsertAndThenGetLog );
	CPPUNIT_TEST( testLoggable_InsertAndThenGetLog_AfterClearLog );

	CPPUNIT_TEST_SUITE_END();

private:
	
	// fixture definitions
	class Base { 
	public:
		virtual ~Base(){} 
	};
	class ConcreteFoo : public Base{};
	class ConcreteBar : public Base{};
	class NothingToDo {};
	// fixture data
	Base* baseConcrete;
	Base* base;
	ConcreteFoo *concrete;
	NothingToDo *nothingToDo;

	std::string baseConcreteStr, baseStr, concreteStr;

public:
	/// @name Fixture Implementation
	/// @{
	void setUp()
	{	
		baseConcrete = new ConcreteFoo;
		concrete = new ConcreteFoo;
		base = new Base;
		nothingToDo = new NothingToDo;
		
		baseConcreteStr = CppUnit::assertion_traits<std::type_info>::toString( typeid(*baseConcrete) );
		baseStr = CppUnit::assertion_traits<std::type_info>::toString( typeid(*base) );
		concreteStr = CppUnit::assertion_traits<std::type_info>::toString( typeid(*concrete) );
	}

	void tearDown()
	{
		delete baseConcrete;
		delete concrete;
		delete base;
		delete nothingToDo;
	}
	/// @}

private:
	void testAssertionTraitsTypeInfoToString_NotEquals()
	{
		CPPUNIT_ASSERT_MESSAGE("typeid(...).name() of different classes should be differents",
			concreteStr != baseStr );
	}

	void testAssertionTraitsTypeInfoToString_Equals()
	{
		CPPUNIT_ASSERT_EQUAL( concreteStr, baseConcreteStr );
	}

	void testAssertionTraitsTypeInfoEqual_Equals()
	{
		bool eq = CppUnit::assertion_traits<std::type_info>::equal(
			typeid(*baseConcrete), typeid(*concrete) );

		CPPUNIT_ASSERT_MESSAGE(
			"assertion_traits of type_info, method eq. should return true if dyn types are equal ",
			eq );
	}

	void testAssertionTraitsTypeInfoEqual_DifferentUnrelatedClasses()
	{
		CPPUNIT_ASSERT_MESSAGE(
			"assertion_traits of type_info, method eq. should return false if dyn types are different",
			!CppUnit::assertion_traits<std::type_info>::equal(
			typeid(*baseConcrete), typeid(*nothingToDo) ) );
	}

	void testAssertionTraitsTypeInfoEqual_DifferentSuperAndSubClass()
	{
		CPPUNIT_ASSERT_MESSAGE(
			"assertion_traits of type_info, method eq. should return false if dyn types are different",
			!CppUnit::assertion_traits<std::type_info>::equal(
			typeid(baseConcrete), typeid(concrete) ) );
	}
	
	void testLoggable_InsertAndThenGetLog()
	{
		ToLog() << "text inserted to the log";
		CPPUNIT_ASSERT_EQUAL( std::string("text inserted to the log"), GetLog() );
	}

	void testLoggable_InsertAndThenGetLog_AfterClearLog()
	{
		ToLog() << "first insertion";
		ClearLog();
		ToLog() << "text inserted to the log";
		CPPUNIT_ASSERT_EQUAL( std::string("text inserted to the log"), GetLog() );
	}
};
} // namespace
 
