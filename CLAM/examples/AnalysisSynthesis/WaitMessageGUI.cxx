#include "WaitMessageGUI.hxx"
#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif

#ifdef WIN32
#include "CLAM_windows.h"
#endif

#include <iostream>
namespace CLAMGUI
{
	WaitMessageGUI::WaitMessageGUI(const char* title)
		:WaitMessage(title)
	{
		mWaitMsg = new Fl_WaitMessage;
		mWaitMsg->mLabel->label( title );
		mWaitMsg->mWindow->show();
	}
	
	WaitMessageGUI::~WaitMessageGUI()
	{
		mWaitMsg->mWindow->hide();
		delete mWaitMsg;
	}	

}
