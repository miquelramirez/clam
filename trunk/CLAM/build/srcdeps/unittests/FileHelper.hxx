#ifndef _FileHelper_hxx_
#define _FileHelper_hxx_


namespace srcdepsTest
{

class FileHelper
{

	std::string mTestPath;
	std::string mCurrentFileName;
public:
	FileHelper( const std::string& path) : mTestPath( path )
	{}

	//! updates and returns the c_str of the mCurrentFileName. This member string contains all the path
	const char* helper_filename( const std::string nameWithoutPath )
	{
		mCurrentFileName = mTestPath + nameWithoutPath;
		return mCurrentFileName.c_str();
	}
	const std::string& TestPath() 
	{
		return mTestPath;
	}
};


} // namespace

#endif