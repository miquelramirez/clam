#ifndef __STDOUTWAITMESSAGE__
#define __STDOUTWAITMESSAGE__

#include "WaitMessage.hxx"

namespace CLAMGUI
{

class StdOutWaitMessage
	: public WaitMessage
{
	StdOutWaitMessage( const char* title );
	
	~StdOutWaitMessage();

};	

}

#endif //StdOutWaitMessage.hxx
