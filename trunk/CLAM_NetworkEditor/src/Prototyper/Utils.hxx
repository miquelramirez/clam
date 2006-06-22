#ifndef _PROTOTYPER_UTILS_HXX_
#define _PROTOTYPER_UTILS_HXX_

#include <fstream>
#include <string>

bool FileExists( const std::string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file )
	{
		std::cerr << "ERROR: opening file <" << filename << ">" << std::endl;
		return false;
	}
	file.close();
	return true;
}

std::string GetUiFromXmlFile(const std::string& xmlfile)
{
	std::string filename = xmlfile;
	filename.erase( filename.size()-12, 12 );
	filename+=std::string(".ui");
	return filename;
}

#endif
