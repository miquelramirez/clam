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
		TEST_CASE( test_errorMessage_whenNoError );
		TEST_CASE( test_errorMessage_inTheFirstLine );
		TEST_CASE( test_errorMessage_inTheSecondLine );
		TEST_CASE( test_errorMessage_multipleErrors );
		TEST_CASE( test_feedLine_ignoresEmptyLines );
		TEST_CASE( test_feedLine_ignoresEmptyLinesButKeepsLineNumbers );
		TEST_CASE( test_feedLine_ignoresComments );
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
		ASSERT( not ok2 )
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
	void test_errorMessage_whenNoError()
	{
		std::istringstream os(
			"1"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT_EQUALS(
			"",
			parser.errorMessage())
		ASSERT( not parser.hasError() )
	}
	void test_errorMessage_inTheFirstLine()
	{
		std::istringstream os(
			"a"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
		ASSERT_EQUALS(
			"Error in line 1, token 1: Expected an int\n",
			parser.errorMessage())
		ASSERT( parser.hasError() )
	}
	void test_errorMessage_inTheSecondLine()
	{
		std::istringstream os(
			"1\n"
			"a"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
		ASSERT_EQUALS(
			"Error in line 2, token 1: Expected an int\n",
			parser.errorMessage())
		ASSERT( parser.hasError() )
	}
	void test_errorMessage_multipleErrors()
	{
		std::istringstream os(
			"b\n"
			"1\n"
			"a"
		);
		SingleIntColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( not ok1 )
		bool ok2 = parser.feedLine();
		ASSERT( ok2 )
		bool ok3 = parser.feedLine();
		ASSERT( not ok3 )
		ASSERT_EQUALS(
			"Error in line 1, token 1: Expected an int\n"
			"Error in line 3, token 1: Expected an int\n",
			parser.errorMessage())
		ASSERT( parser.hasError() )
	}
	void test_feedLine_ignoresEmptyLines()
	{
		std::istringstream os(
			"1\n"
			"\n\n"
			"2\n"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( ok )
		ASSERT_EQUALS(2, parser.intColumn())
	}
	void test_feedLine_ignoresEmptyLinesButKeepsLineNumbers()
	{
		std::istringstream os(
			"1\n"
			"\n\n"
			"a\n"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok )
		ASSERT_EQUALS(
			"Error in line 4, token 1: Expected an int\n",
			parser.errorMessage())
	}
	void test_feedLine_ignoresComments()
	{
		std::istringstream os(
			"1\n"
			"# A comment\n"
			"2\n"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( ok )
		ASSERT_EQUALS(2, parser.intColumn())
	}

/*
	TODO:
	- Error on trailing line content
	- 

*/
};

REGISTER_FIXTURE(TableParserTest);

