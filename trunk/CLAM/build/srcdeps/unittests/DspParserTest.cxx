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


class DspParserTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DspParserTest );

/*
 * This test suit is kind of if functional tests since we are basically
 * generating a dsp and then comparing it with an 'expected' one that was
 * hand checked previously. 
 * Therefore that these test won't give a very accurate diagnostic of the 
 * problem, but at least they cover a lot of functionality (and have been easy
 * to write)
 */
class DspParserTest : public CppUnit::TestFixture, public FileHelper
{
	CPPUNIT_TEST_SUITE( DspParserTest );

	CPPUNIT_TEST( testhelperCopyFiles );
	CPPUNIT_TEST( test_dsp_parse_passing_an_empty_dsp );
	CPPUNIT_TEST( test_dsp_parse_passing_a_dsp_with_flags );
	CPPUNIT_TEST( test_config_parse_and_dsp_parse_passing_a_config_file );

	CPPUNIT_TEST_SUITE_END();


public:
	DspParserTest() : FileHelper("unittests/source_files_for_testing/")
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


	void testhelperCopyFiles()
	{
		CopyFile( "foo", "copia_foo");
		CPPUNIT_ASSERT( AreFilesEqual("foo", "copia_foo") );

	}

	void test_dsp_parse_passing_an_empty_dsp()
	{
		//CopyFile("empty.dsp", "fortesting.dsp");
			
		gendepend = 0;
		recursesrcs = 1;
		config_init();
		listhash_add_item_str(config, "PROGRAM", "fortesting");
		parser_init();
		list_add_str_once( guessed_sources, helper_filename("a.cxx") );
		list_add_str_once( guessed_sources, helper_filename("d.cxx") );
		list_add_str_once( guessed_sources, helper_filename("e.cxx") );
		list_add_str_once( guessed_sources, helper_filename("j.cxx") );
		includepaths_add_rec( helper_filename("subdir") );
		
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}

		dsp_parse_from_empty( helper_filename("fortesting.dsp") );

		CPPUNIT_ASSERT( AreFilesEqual("expected_from_empty.dsp", "fortesting.dsp") );

		parser_exit();
		config_exit();

	}

	void test_dsp_parse_passing_a_dsp_with_flags()
	{
		CopyFile("with_flags.dsp", "fortesting.dsp");
		gendepend = 0;
		recursesrcs = 1;
		config_init();
		listhash_add_item_str(config, "PROGRAM", "fortesting");
		parser_init();
		list_add_str_once( guessed_sources, helper_filename("a.cxx") );
		list_add_str_once( guessed_sources, helper_filename("d.cxx") );
		list_add_str_once( guessed_sources, helper_filename("e.cxx") );
		list_add_str_once( guessed_sources, helper_filename("j.cxx") );
		includepaths_add_rec( helper_filename("subdir") );
		
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}

		dsp_parse( helper_filename("fortesting.dsp") );

		CPPUNIT_ASSERT( AreFilesEqual("expected_from_with_flags.dsp", "fortesting.dsp") );

		parser_exit();
		config_exit();

	}

	void test_config_parse_and_dsp_parse_passing_a_config_file()
	{
		gendepend = 0;
		recursesrcs = 1;
		config_init();

		listhash_add_item_str(config,"OS_WINDOWS","1");
		listhash_add_item_str(config,"OS_LINUX","0");
		listhash_add_item_str(config,"OS_MACOSX","0");
		config_parse( helper_filename("testsettings.cfg") );
		
		parser_init();
		item* i = guessed_sources->first;
		while (i)
		{
			parser_run(i->str);
			i = i->next;
		}
		dsp_parse_from_empty( helper_filename("fortesting.dsp") );
		
		CPPUNIT_ASSERT( AreFilesEqual("expected_from_testsettings.dsp", "fortesting.dsp") );

		parser_exit();
		config_exit();

	}

};


} // namespace srcdepsTest