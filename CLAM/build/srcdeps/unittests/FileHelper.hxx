#ifndef _FileHelper_hxx_
#define _FileHelper_hxx_

#include <string>


namespace srcdepsTest
{

class FileHelper
{

	std::string mTestPath;
	mutable std::string mCurrentFileName;
public:
	FileHelper( const std::string& path) : mTestPath( path )
	{}

	//! updates and returns the c_str of the mCurrentFileName. This member string contains all the path
	const char* helper_filename( const std::string nameWithoutPath ) const;
	
	const std::string& TestPath();
	
	void CopyFile( const std::string& input, const std::string& output) const;

	//! helper file for comparing two text files
	bool AreFilesEqual( const std::string& firstFile, const std::string secondFile ) const;


};


} // namespace

#endif