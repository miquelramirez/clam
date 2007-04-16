#include "Progress.hxx"
#include <cstring>

namespace CLAMGUI
{
	Progress::Progress(const char* title,float from,float to)
	{
		
		mTitle = new char[strlen(title)+1];
		strncpy( mTitle, title, strlen(title)+1 );
		mFrom = from;
		mTo = to;
	}
	
	Progress::~Progress()
	{
		delete[] mTitle;
	}


}

