#include <cppunit/extensions/HelperMacros.h>
#include "FileHelper.hxx"
#include <iostream>

#include <stdio.h>
#include <sstream>
#include <fstream>

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

class DspParserTest : public CppUnit::TestFixture, public FileHelper
{
	CPPUNIT_TEST_SUITE( DspParserTest );

	CPPUNIT_TEST( testhelperCopyFiles );
	CPPUNIT_TEST( test_dsp_parse_passing_an_empty_dsp );

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
	//! helper file for comparing two text files
	bool AreFilesEqual( const std::string& firstFile, const std::string secondFile )
	{

		std::ostringstream diagnostic;
		bool foundDifferents = false;

		const int maxbuff = 4096;
		char buff1[maxbuff];
		char buff2[maxbuff];
		
		std::ifstream file1( helper_filename(firstFile.c_str()) );
		std::ifstream file2( helper_filename(secondFile.c_str()) );

		if( file1.fail() )
		{
			foundDifferents = true;
			diagnostic << "file " << firstFile << " doesn't exist\n";
		}
		if( file2.fail() )
		{
			foundDifferents = true;
			diagnostic << "file "<< secondFile <<" doesn't exist\n";
		}

		// mirar longituts
		file1.seekg(0, std::ios::end);
		file2.seekg(0, std::ios::end);

		if( file1.tellg() != file2.tellg() )
		{
			foundDifferents = true;
			diagnostic << "lenght of file " << firstFile << " : is " << file1.tellg() << std::endl;
			diagnostic << "lenght of file " << secondFile << " : is " << file2.tellg() << std::endl;
		}
		else
		{
			file1.seekg(0, std::ios::beg);
			file2.seekg(0, std::ios::beg);
		}

		int line=1;
		while ( !foundDifferents && !file1.eof() )
		{

			file1.getline(buff1, maxbuff);
			file2.getline(buff2, maxbuff);


			std::string firststr(buff1);
			std::string secondstr(buff2);

			if ( firststr != secondstr )
			{
				foundDifferents = true;
				diagnostic << "Found two different lines in line "<< line <<std::endl;
				diagnostic << "First line:\t" << firststr << std::endl;
				diagnostic << "Second line:\t" << secondstr << std::endl;
			}
			++line;
		}
	
		file1.close();
		file2.close();

		if (foundDifferents) std::cout << diagnostic.str();
		return !foundDifferents;
	}

	void CopyFile( const std::string& input, const std::string& output)
	{
		std::ifstream is( helper_filename(input.c_str()) );
		std::ofstream os( helper_filename(output.c_str()) );
		CPPUNIT_ASSERT_MESSAGE("in CopyFiles(..) input file doesn't exist", !is.fail());

		char c;
		is.get(c);
		while ( !is.eof() )
		{
			os.put(c);
			is.get(c);
		}
		is.close();
		os.close();
	}

	void testhelperCopyFiles()
	{
		CopyFile( "foo", "copia_foo");
		CPPUNIT_ASSERT( AreFilesEqual("foo", "copia_foo") );

	}

	void test_dsp_parse_passing_an_empty_dsp()
	{
		CopyFile("empty.dsp", "fortesting.dsp");
			
		gendepend = 0;
		recursesrcs = 1;
		
		/*
		config_init();
		listhash_add_item_str(config,"OS_WINDOWS","1");
		listhash_add_item_str(config,"OS_LINUX","0");
		listhash_add_item_str(config,"OS_MACOSX","0");

		//config_parse( helper_filename("fortesting_settings.cfg");
		parser_init();

		config_check();
		*/
		config_init();
		listhash_add_item_str(config, "PROGRAM", "fortesting");
		parser_init();
		list_add_str_once( guessed_sources, helper_filename("a.cxx") );
		list_add_str_once( guessed_sources, helper_filename("d.cxx") );
		list_add_str_once( guessed_sources, helper_filename("e.cxx") );
		list_add_str_once( guessed_sources, helper_filename("j.cxx") );
		includepaths_add_rec( helper_filename("subdir") );
		
		int cnt = 0;
		item* i = guessed_sources->first;
		while (i)
		{
			//fprintf(stderr,"%s %d %d\n",i->str,cnt,list_size(guessed_sources));
			parser_run(i->str);
			i = i->next;
			cnt++;
		}

		dsp_parse( helper_filename("fortesting.dsp") );

		parser_exit();
		config_exit();

		CPPUNIT_ASSERT( AreFilesEqual("expected.dsp", "fortesting.dsp") );
	}

};


} // namespace srcdepsTest