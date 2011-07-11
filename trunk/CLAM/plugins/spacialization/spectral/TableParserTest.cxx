#include "MiniCppUnit.hxx"
#include "TableParser.hxx"
#include <sstream>


class TableParserTest : public TestFixture<TableParserTest>
{
public:
	TEST_FIXTURE( TableParserTest )
	{
		TEST_CASE( test_singleint_emptyContent );
		TEST_CASE( test_singleint_singleLine );
		TEST_CASE( test_singleint_beyondSingleLine );
		TEST_CASE( test_singleint_beyondWithEmptyLine );
		TEST_CASE( test_singleint_twoLines );
		TEST_CASE( test_singleint_withNotInt );
	}

	class SingleIntColumn : public TableParser 
	{
	public:
		SingleIntColumn(std::istream & stream)
			: TableParser(stream)
		{
		}
	};

	void test_singleint_emptyContent()
	{
		std::istringstream os(
			""
		);
		SingleIntColumn parser(os);
		bool ok = parser.feedLine();
		ASSERT( not ok )
	}
	void test_singleint_singleLine()
	{
		std::istringstream os(
			"1"
		);
		SingleIntColumn parser(os);
		bool ok = parser.feedLine();
		ASSERT( ok )
		ASSERT_EQUALS(1, parser.intColumn())
		bool ok2 = parser.feedLine();
		ASSERT( ok )
	}
	void test_singleint_beyondSingleLine()
	{
		std::istringstream os(
			"1"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
	}
	void test_singleint_beyondWithEmptyLine()
	{
		std::istringstream os(
			"1\n"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
	}
	void test_singleint_twoLines()
	{
		std::istringstream os(
			"1\n"
			"2"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( ok )
		ASSERT_EQUALS(2, parser.intColumn())
	}
	void test_singleint_withNotInt()
	{
		std::istringstream os(
			"a"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
		ASSERT( not parser.hasError() )
		ASSERT_EQUALS("Error in line 1, token 1: Expected an int", parser.errorMessage())
	}
};

REGISTER_FIXTURE(TableParserTest);

