#include <cppunit/extensions/HelperMacros.h>

extern "C"
{
#	include "parser.h"
#	include "includepaths.h"
#	include "config_parser.h"
}

namespace srcdepsTest
{


class ConfigParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigParserTest );

class ConfigParserTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ConfigParserTest );

	CPPUNIT_TEST( still_TODO_but_somehow_tested_in_DspParserTest );

	CPPUNIT_TEST_SUITE_END();

	std::string mTestPath;
	std::string mCurrentFileName;

public:
	ConfigParserTest() : mTestPath("unittests/source_files_for_testing/")
	{
	}
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		config_init();
		parser_init();
		gendepend = 0;
		recursesrcs = 1;
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{
		parser_exit();
		config_exit();
	}

private:
	void still_TODO_but_somehow_tested_in_DspParserTest()
	{
	/*	CPPUNIT_ASSERT_EQUAL( 
			std::string("this should be a test about parsing settings.cfg file"), 
			std::string("To Do") );
*/
	}



};









} // namespace srcdepsTest