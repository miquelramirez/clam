#include "FileHelper.hxx"

#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

namespace srcdepsTest
{

//! updates and returns the c_str of the mCurrentFileName. This member string contains all the path
const char* FileHelper::helper_filename( const std::string nameWithoutPath ) const
{
	mCurrentFileName = mTestPath + nameWithoutPath;
	return mCurrentFileName.c_str();
}

const std::string& FileHelper::TestPath() 
{
	return mTestPath;
}

void FileHelper::CopyFile( const std::string& input, const std::string& output) const
{
	std::ifstream is( helper_filename(input.c_str()) );
	std::ofstream os( helper_filename(output.c_str()) );
	std::ostringstream errorMessage;
	if (is.fail())
	{
		errorMessage << "in CopyFiles(in,out) input file: '" << input << "' doesn't exist";
		throw std::exception(errorMessage.str().c_str());
	}

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

//! helper file for comparing two text files
bool FileHelper::AreFilesEqual( const std::string& firstFile, const std::string secondFile ) const
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



} // namespace
