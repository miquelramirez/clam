#include "MiniCppUnit.hxx"
#include "TableParser.hxx"
#include <sstream>

/*
TODO: Trailing text tokens (parse as string what it is left in the line)
TODO: Optional token decorator (if not enough content, a default value is given. pe Optional<Token<int> > initialized 
TODO: Further testing is needed on the 'ignore errors and continue parsing' mode
*/

class TableParserTest : public TestFixture<TableParserTest>
{
	typedef spectral::TableParser TableParser;

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
		TEST_CASE( test_spaceOnlyLine_isIgnoredAsEmpty );
		TEST_CASE( test_commentLine_notAtFirstPosition );
		TEST_CASE( test_comment_afterContent );
		TEST_CASE( test_dosLineFeeds );

		TEST_CASE( test_twoIntColumns_singleLine );
		TEST_CASE( test_errorMessage_errorOnFirstTokenOfTwo );
		TEST_CASE( test_errorMessage_errorOnSecondToken );
		TEST_CASE( test_twoIntColumns_twoLines );
		TEST_CASE( test_twoIntColumns_complexValidInput );

		TEST_CASE( test_singlefloat_singleLine );
		TEST_CASE( test_singlefloat_badFloat );
		TEST_CASE( test_singleString_singleLine );
		TEST_CASE( test_singleString_spacesArround );
		TEST_CASE( test_singleString_commentJoined );
		TEST_CASE( test_singleString_manyHashes );

		TEST_CASE( test_label_normally );
		TEST_CASE( test_label_multiword );
		TEST_CASE( test_label_twoLines );
		TEST_CASE( test_label_missingColon );
		TEST_CASE( test_labelValue_normally );
		TEST_CASE( test_labelValue_missingValue );
		TEST_CASE( test_valueLabel_normally );
		TEST_CASE( test_quotedToken_singleQuoted );
		TEST_CASE( test_quotedToken_unquoted );
		TEST_CASE( test_quotedToken_doubleQuoted );
		TEST_CASE( test_quotedToken_singleQuotedWithSpaces );
		TEST_CASE( test_quotedToken_missingColumn );
		TEST_CASE( test_quotedToken_unfinishedQuote );
		TEST_CASE( test_quotedToken_secondLine );
		TEST_CASE( test_quotedToken_trailingContent );
		TEST_CASE( test_filenameBased_badFileName );
		TEST_CASE( test_filenameBased_goodFile );
		TEST_CASE( test_remaining_whenEmpty );
		TEST_CASE( test_remaining_whenSingleWord );
		TEST_CASE( test_remaining_whenSingleWord );
		TEST_CASE( test_remaining_withTwoLines );
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
		ASSERT_EQUALS("", parser.errorMessage());
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
		ASSERT_EQUALS(
			"Error in line 1, field 1: Expected field of type int\n",
			parser.errorMessage());
		ASSERT( not ok );
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
		ASSERT_EQUALS(
			"Error in line 2, field 1: Expected field of type int\n",
			parser.errorMessage());
		ASSERT( not ok );
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
		ASSERT_EQUALS(
			"Error in line 1, field 1: Expected field of type int\n"
			"Error in line 3, field 1: Expected field of type int\n",
			parser.errorMessage());
		ASSERT( not ok3 );
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
			"Error in line 4, field 1: Expected field of type int\n",
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
	void test_spaceOnlyLine_isIgnoredAsEmpty()
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
	void test_commentLine_notAtFirstPosition()
	{
		std::istringstream os(
			" \t# comment\n"
			"1\n"
		);
		SingleIntColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.intColumn());
		ASSERT( not parser.hasError() );
	}

	void test_comment_afterContent()
	{
		std::istringstream os(
			"1# comment\n"
		);
		SingleIntColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.intColumn());
		ASSERT( not parser.hasError() );
	}
	void test_dosLineFeeds()
	{
		std::istringstream os(
			"1\x0d\x0a"
			"2\x0d\x0a"
		);
		SingleIntColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS( "", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.intColumn());
		bool ok2 = parser.feedLine();
		ASSERT( ok2 );
		ASSERT_EQUALS(2, parser.intColumn());
		bool ok3 = parser.feedLine();
		ASSERT( not parser.hasError() );
		ASSERT( not ok3 );
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
		TwoIntColumns(const std::string & filename)
			: TableParser(filename)
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
			"Error in line 1, field 1: Expected field of type int\n",
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
			"Error in line 1, field 2: Expected field of type int\n",
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

	void test_singlefloat_badFloat()
	{
		std::istringstream os(
			"bad"
		);
		SingleFloatColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( not ok1 );
		ASSERT_EQUALS(
			"Error in line 1, field 1: Expected field of type float\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
	}

	class SingleWordColumn : public TableParser 
	{
	public:
		Token<std::string> column1;
		SingleWordColumn(std::istream & stream)
			: TableParser(stream)
			, column1(this)
		{
		}
	};

	void test_singleString_singleLine()
	{
		std::istringstream os(
			"one"
		);
		SingleWordColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.column1());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}
	void test_singleString_spacesArround()
	{
		std::istringstream os(
			" one \n"
		);
		SingleWordColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.column1());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}
	void test_singleString_commentJoined()
	{
		std::istringstream os(
			" one# comment"
		);
		SingleWordColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.column1());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}
	void test_singleString_manyHashes()
	{
		std::istringstream os(
			" one#### ## comment"
		);
		SingleWordColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.column1());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}

	class SingleLabel : public TableParser 
	{
	public:
		LabelToken label;
		SingleLabel(std::istream & stream)
			: TableParser(stream)
			, label(this)
		{
		}
	};

	void test_label_normally()
	{
		std::istringstream os(
			"one:"
		);
		SingleLabel parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.label());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}
	void test_label_multiword()
	{
		std::istringstream os(
			"one two:"
		);
		SingleLabel parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one two", parser.label());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}
	void test_label_twoLines()
	{
		std::istringstream os(
			"one:\n"
			"two:\n"
		);
		SingleLabel parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.label());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( ok2 );
		ASSERT_EQUALS("two", parser.label());
		ASSERT( not parser.hasError() );
		bool ok3 = parser.feedLine();
		ASSERT( not ok3 );
		ASSERT( not parser.hasError() );
	}
	void test_label_missingColon()
	{
		std::istringstream os(
			"one"
		);
		SingleLabel parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			"Error in line 1, field 1: Label should end with a colon\n",
			parser.errorMessage());
		ASSERT( not ok1 );
		ASSERT( parser.hasError() );
	}

	class LabelValue : public TableParser 
	{
	public:
		LabelToken label;
		Token<std::string> value;
		LabelValue(std::istream & stream)
			: TableParser(stream)
			, label(this)
			, value(this)
		{
		}
	};

	void test_labelValue_normally()
	{
		std::istringstream os(
			"one: two"
		);
		LabelValue parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( ok1 );
		ASSERT_EQUALS("one", parser.label());
		ASSERT_EQUALS("two", parser.value());
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}

	void test_labelValue_missingValue()
	{
		std::istringstream os(
			"one:"
		);
		LabelValue parser(os);
		bool ok1 = parser.feedLine();
		ASSERT( not ok1 );
		ASSERT_EQUALS(
			"Error in line 1, field 2: Expected field of type std::string\n",
			parser.errorMessage());
		ASSERT( parser.hasError() );
	}

	class ValueLabel : public TableParser 
	{
	public:
		Token<std::string> value;
		LabelToken label;
		ValueLabel(std::istream & stream)
			: TableParser(stream)
			, value(this)
			, label(this)
		{
		}
	};

	void test_valueLabel_normally()
	{
		std::istringstream os(
			"one two:"
		);
		ValueLabel parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("one", parser.value());
		ASSERT_EQUALS("two", parser.label());
		ASSERT( ok1 );
		ASSERT( not parser.hasError() );
		bool ok2 = parser.feedLine();
		ASSERT( not ok2 );
		ASSERT( not parser.hasError() );
	}

	class QuotedColumn : public TableParser 
	{
	public:
		QuotedToken value1;
		QuotedToken value2;
		QuotedToken value3;
		QuotedColumn(std::istream & stream)
			: TableParser(stream)
			, value1(this)
			, value2(this)
			, value3(this)
		{
		}
	};

	void test_quotedToken_singleQuoted()
	{
		std::istringstream os(
			"'one' 'two' 'three'"
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("one", parser.value1());
		ASSERT_EQUALS("two", parser.value2());
		ASSERT_EQUALS("three", parser.value3());
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		
	}
	void test_quotedToken_unquoted()
	{
		std::istringstream os(
			"one two three"
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("one", parser.value1());
		ASSERT_EQUALS("two", parser.value2());
		ASSERT_EQUALS("three", parser.value3());
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		
	}
	void test_quotedToken_doubleQuoted()
	{
		std::istringstream os(
			"\"one\" \"two\" \"three\""
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("one", parser.value1());
		ASSERT_EQUALS("two", parser.value2());
		ASSERT_EQUALS("three", parser.value3());
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		
	}
	void test_quotedToken_singleQuotedWithSpaces()
	{
		std::istringstream os(
			"' one word ' 'two' 'three'"
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(" one word ", parser.value1());
		ASSERT_EQUALS("two", parser.value2());
		ASSERT_EQUALS("three", parser.value3());
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		
	}
	void test_quotedToken_missingColumn()
	{
		std::istringstream os(
			"'one' 'two' "
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			"Error in line 1, field 3: Expected quoted string\n"
			, parser.errorMessage());
		ASSERT_EQUALS("one", parser.value1());
		ASSERT_EQUALS("two", parser.value2());
		ASSERT( not ok1 );
		
	}
	void test_quotedToken_unfinishedQuote()
	{
		std::istringstream os(
			"'one' 'two "
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			"Error in line 1, field 2: Unclosed quote\n"
			, parser.errorMessage());
		ASSERT_EQUALS("one", parser.value1());
		ASSERT( not ok1 );
	}
	void test_quotedToken_secondLine()
	{
		std::istringstream os(
			"one 'two' 'three'\n"
			"four five six\n"
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		bool ok2 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS("four", parser.value1());
		ASSERT_EQUALS("five", parser.value2());
		ASSERT_EQUALS("six", parser.value3());
		
	}
	void test_quotedToken_trailingContent()
	{
		std::istringstream os(
			"one 'two' 'three' more\n"
		);
		QuotedColumn parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			"Error in line 1: Unexpected content at the end of the line\n"
			, parser.errorMessage());
		ASSERT( not ok1 );
	}

	void test_filenameBased_badFileName()
	{
		TwoIntColumns parser("nonexisting");
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			"Could not open file 'nonexisting'\n"
			, parser.errorMessage());
		ASSERT( parser.hasError() );
	}
	void test_filenameBased_goodFile()
	{
		{
			std::ofstream os("fileToParse");
			os << "2 4";
		}
		TwoIntColumns parser("fileToParse");
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS(
			""
			, parser.errorMessage());
		ASSERT( not parser.hasError() );
		ASSERT_EQUALS(2, parser.column1());
		ASSERT_EQUALS(4, parser.column2());

		unlink("fileToParse");
	}

	class RemainingParser : public TableParser
	{
	public:
		Token<int> column;
		RemainingToken remaining;
		RemainingParser(std::istream & stream)
			: TableParser(stream)
			, column(this)
			, remaining(this)
		{}
	};

	void test_remaining_whenEmpty()
	{
		std::istringstream os(
			"1\n"
		);
		RemainingParser parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column());
		ASSERT_EQUALS("", parser.remaining());
	}
	void test_remaining_whenSingleWord()
	{
		std::istringstream os(
			"1 word\n"
		);
		RemainingParser parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column());
		ASSERT_EQUALS("word", parser.remaining());
	}

	void test_remaining_whenMultipleWord()
	{
		std::istringstream os(
			"1 many words\n"
		);
		RemainingParser parser(os);
		bool ok1 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok1 );
		ASSERT_EQUALS(1, parser.column());
		ASSERT_EQUALS("many words", parser.remaining());
	}
	void test_remaining_withTwoLines()
	{
		std::istringstream os(
			"1 many words\n"
			"2 other words\n"
		);
		RemainingParser parser(os);
		parser.feedLine();
		bool ok2 = parser.feedLine();
		ASSERT_EQUALS("", parser.errorMessage());
		ASSERT( ok2 );
		ASSERT_EQUALS(2, parser.column());
		ASSERT_EQUALS("other words", parser.remaining());
	}
	

};


REGISTER_FIXTURE(TableParserTest);

