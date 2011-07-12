#include "MiniCppUnit.hxx"
#include "TableParser.hxx"
#include <sstream>

/*
	TODO: Error always says it expected an int
	TODO: No test for filename based constructor
	TODO: Comments not at the first char
	TODO: Trailing tokens (parse as string what it is left in the line)
	TODO: Label tokens (parse as string until a stop token is found)
	TODO: Optional token decorator (if not enough content, a default value is given. pe Optional<Token<int> > initialized 
*/

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
		TEST_CASE( test_errorMessage_trailingContent );
		TEST_CASE( test_trailingSpacesIsNotError );
		TEST_CASE( test_spaceOnliLine_isIgnoredAsEmpty );

		TEST_CASE( test_twoIntColumns_singleLine );
		TEST_CASE( test_errorMessage_errorOnFirstTokenOfTwo );
		TEST_CASE( test_errorMessage_errorOnSecondToken );
		TEST_CASE( test_twoIntColumns_twoLines );
		TEST_CASE( test_twoIntColumns_complexValidInput );

		TEST_CASE( test_singlefloat_singleLine );
	}

	class SingleIntColumn : public TableParser 
	{
	public:
		Token<int> intColumn;
		SingleIntColumn(std::istream & stream)
			: TableParser(stream)
			, intColumn(this)
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
		ASSERT( not ok );
	}
	void test_singleint_singleLine()
	{
		std::istringstream os(
			"1"
		);
		SingleIntColumn parser(os);
		bool ok = parser.feedLine();
		ASSERT( ok );
		ASSERT_EQUALS(1, parser.intColumn());
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
	}
	void test_singleint_beyondSingleLine()
	{
		std::istringstream os(
			"1"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok );
	}
	void test_singleint_beyondWithEmptyLine()
	{
		std::istringstream os(
			"1\n"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok );
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
		ASSERT( ok );
		ASSERT_EQUALS(2, parser.intColumn());
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
			parser.errorMessage());
		ASSERT( not parser.hasError() );
	}
	void test_errorMessage_inTheFirstLine()
	{
		std::istringstream os(
			"a"
		);
		SingleIntColumn parser(os);
		parser.feedLine();
		bool ok = parser.feedLine();
		ASSERT( not ok );
		ASSERT_EQUALS(
			"Error in line 1, token 1: Expected an int\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
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
		ASSERT( not ok );
		ASSERT_EQUALS(
			"Error in line 2, token 1: Expected an int\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
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
		ASSERT( not ok1 );
		bool ok2 = parser.feedLine();
		ASSERT( ok2 );
		bool ok3 = parser.feedLine();
		ASSERT( not ok3 );
		ASSERT_EQUALS(
			"Error in line 1, token 1: Expected an int\n"
			"Error in line 3, token 1: Expected an int\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
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
		ASSERT( ok );
		ASSERT_EQUALS(2, parser.intColumn());
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
		ASSERT( not ok );
		ASSERT_EQUALS(
			"Error in line 4, token 1: Expected an int\n",
			parser.errorMessage());
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
		ASSERT( ok );
		ASSERT_EQUALS(2, parser.intColumn());
	}
	void test_errorMessage_trailingContent()
	{
		std::istringstream os(
			"1 2\n"
		);
		SingleIntColumn parser(os);
		bool ok = parser.feedLine();
		ASSERT( not ok );
		ASSERT_EQUALS(
			"Error in line 1: Unexpected content at the end of the line\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
	}
	void test_trailingSpacesIsNotError()
	{
		std::istringstream os(
			"1 \t  \t \n"
		);
		SingleIntColumn parser(os);
		bool ok = parser.feedLine();
		ASSERT( ok );
		ASSERT( not parser.hasError() );
	}
	void test_spaceOnliLine_isIgnoredAsEmpty()
	{
		std::istringstream os(
			" \t  \t \n"
			"1\n"
		);
		SingleIntColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.intColumn());
		ASSERT( not parser.hasError() );
	}


	class TwoIntColumns : public TableParser 
	{
	public:
		Token<int> column1;
		Token<int> column2;
		TwoIntColumns(std::istream & stream)
			: TableParser(stream)
			, column1(this)
			, column2(this)
		{
		}
	};

	void test_twoIntColumns_singleLine()
	{
		std::istringstream os(
			"1 2"
		);
		TwoIntColumns parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column1());
		ASSERT_EQUALS(2, parser.column2());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
	}
	void test_errorMessage_errorOnFirstTokenOfTwo()
	{
		std::istringstream os(
			"a 1"
		);
		TwoIntColumns parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( not ok1 );
		ASSERT_EQUALS(
			"Error in line 1, token 1: Expected an int\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
	}

	void test_errorMessage_errorOnSecondToken()
	{
		std::istringstream os(
			"1 a"
		);
		TwoIntColumns parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( not ok1 );
		ASSERT_EQUALS(
			"Error in line 1, token 2: Expected an int\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
	}

	void test_twoIntColumns_twoLines()
	{
		std::istringstream os(
			"1 2\n"
			"3 4"
		);
		TwoIntColumns parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column1());
		ASSERT_EQUALS(2, parser.column2());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( ok2 );
		ASSERT_EQUALS(3, parser.column1());
		ASSERT_EQUALS(4, parser.column2());
		bool ok3 = parser.feedLine();
		ASSERT( not ok3 );
		ASSERT( not parser.hasError() );
	}

	void test_twoIntColumns_complexValidInput()
	{
		std::istringstream os(
			"# comment\n"
			"1 2\n"
			"\n"
			"3 4\n"
			"\n"
		);
		TwoIntColumns parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column1());
		ASSERT_EQUALS(2, parser.column2());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok2 );
		ASSERT_EQUALS(3, parser.column1());
		ASSERT_EQUALS(4, parser.column2());
		bool ok3 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( not ok3 );
		ASSERT( not parser.hasError() );
	}

	class SingleFloatColumn : public TableParser 
	{
	public:
		Token<float> floatColumn;
		SingleFloatColumn(std::istream & stream)
			: TableParser(stream)
			, floatColumn(this)
		{
		}
	};

	void test_singlefloat_singleLine()
	{
		std::istringstream os(
			"1.3"
		);
		SingleFloatColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1.3f, parser.floatColumn());
		ASSERT( not parser.hasError() );
	}

};


REGISTER_FIXTURE(TableParserTest);

