#include <cppunit/extensions/HelperMacros.h>
#include "FileHelper.hxx"

extern "C"
{
#	include "parser.h"
#	include "includepaths.h"
#	include "config_parser.h"
#	include "dsp_parser.h"
}

namespace srcdepsTest
{


class ConfigParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigParserTest );

class ConfigParserTest : public CppUnit::TestFixture , public FileHelper
{
	CPPUNIT_TEST_SUITE( ConfigParserTest );

	CPPUNIT_TEST( test_config_parse_with_equal_inside_a_var );

	CPPUNIT_TEST_SUITE_END();


public:
	ConfigParserTest() : FileHelper("unittests/source_files_for_testing/")
	{
	}
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{
	}


	void test_config_parse_with_equal_inside_a_var()
	{
		gendepend = 0;
		recursesrcs = 1;
		config_init();

		listhash_add_item_str(config,"OS_WINDOWS","1");
		listhash_add_item_str(config,"OS_LINUX","0");
		listhash_add_item_str(config,"OS_MACOSX","0");
		config_parse( helper_filename("test_equalinvar.cfg") );
		
		parser_init();
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}
		dsp_parse_from_empty( helper_filename("test_equalinvar_result.dsp") );
		
		CPPUNIT_ASSERT( AreFilesEqual("test_equalinvar_expected.dsp", "test_equalinvar_result.dsp") );

		parser_exit();
		config_exit();
	}




};









} // namespace srcdepsTest