#include <cppunit/extensions/HelperMacros.h>
#include "FileHelper.hxx"
#include <iostream>

extern "C"
{
#	include "parser.h"
#	include "includepaths.h"
#	include "config_parser.h"
#	include "dsp_parser.h"
}

namespace srcdepsTest
{


class FunctionalTestWithCLAM;

CPPUNIT_TEST_SUITE_REGISTRATION( FunctionalTestWithCLAM );

/*
 * This test suit is formed by totally functional tests. They are basically
 * generating a dsp from a settings.cfg and then comparing it with an 
 * 'expected' one that was
 * hand checked previously. 
 * They test srcdeps in a *real life* project as is compiling some CLAM examples
 *
 * WARNING: they maight take some time to run. So it is likely you'd want to take it
 * out of the srcdeps test suite while developing it.
 * And of course this test suite doesn't have sense if srcdeps is used independently
 * of CLAM.
 *
 */
class FunctionalTestWithCLAM : public CppUnit::TestFixture, public FileHelper
{
	CPPUNIT_TEST_SUITE( FunctionalTestWithCLAM );

//	CPPUNIT_TEST( test_generate_NonSupervisedSystemExample_passing_dsp );
	CPPUNIT_TEST( test_generate_NonSupervisedSystemExample_but_not_passing_dsp );
	
	CPPUNIT_TEST_SUITE_END();


public:
	FunctionalTestWithCLAM() : FileHelper("../Examples/FlowControlExamples/NonSupervisedSystemExample/")
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

private:

	void test_generate_NonSupervisedSystemExample_passing_dsp()
	{
		gendepend = 0;
		recursesrcs = 1;
		config_init();
			
		listhash_add_item_str(config,"OS_WINDOWS","1");
		listhash_add_item_str(config,"OS_LINUX","0");
		listhash_add_item_str(config,"OS_MACOSX","0");
		config_parse( helper_filename("settings.cfg") );
	
		parser_init();
			
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}

		dsp_parse_inplace( helper_filename("NonSupervisedSystemExamples.dsp") );

		//CPPUNIT_ASSERT( AreFilesEqual("expected_from_with_flags.dsp", "NonSupervisedSystemExamples.dsp") );

		parser_exit();
		config_exit();

	}

	void test_generate_NonSupervisedSystemExample_but_not_passing_dsp()
	{
				
		gendepend = 0;
		recursesrcs = 1;
		config_init();
			
		listhash_add_item_str(config,"OS_WINDOWS","1");
		listhash_add_item_str(config,"OS_LINUX","0");
		listhash_add_item_str(config,"OS_MACOSX","0");
		config_parse( helper_filename("settings.cfg") );
	
		parser_init();
			
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}

		dsp_parse_from_empty( helper_filename("NonSupervisedSystemExamples.dsp") );

		//CPPUNIT_ASSERT( AreFilesEqual("expected_from_with_flags.dsp", "NonSupervisedSystemExamples.dsp") );

		parser_exit();
		config_exit();


	}
};


} // namespace srcdepsTest
