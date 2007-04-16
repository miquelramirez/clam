#include "WaitMessage.hxx"
#include <cstring>

namespace CLAMGUI
{
	WaitMessage::WaitMessage(const char* title)
	{
		mTitle = new char[strlen(title)+1];
		strncpy(mTitle,title, strlen(title)+1 );
	}
	
	WaitMessage::~WaitMessage()
	{
		delete[] mTitle;
	}
}

