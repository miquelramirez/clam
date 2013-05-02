#include "StdOutWaitMessage.hxx"
#include <iostream>

namespace CLAMGUI
{

	StdOutWaitMessage::StdOutWaitMessage(const char* title)
	:WaitMessage(title)
	{
		std::cout << mTitle << "\n";
	}
	StdOutWaitMessage::~StdOutWaitMessage()
	{
		std::cout << "Done\n";
	}


}

