#ifndef __WAITMESSAGEGUI__
#define __WAITMESSAGEGUI__

#include "Fl_WaitMessage.hxx"
#include "WaitMessage.hxx"
#include <pthread.h>

namespace CLAMGUI
{

class WaitMessageGUI
	: public WaitMessage
{
public:

	
	
	WaitMessageGUI(const char* title);

	~WaitMessageGUI();

private:
	Fl_WaitMessage* mWaitMsg;

};

}

#endif // WaitMessageGUI.hxx
