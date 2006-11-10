#ifndef __QTWAITMESSAGE__
#define __QTWAITMESSAGE__

#include "WaitMessage.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtWaitMessageDlg;
	}
}

namespace CLAMGUI
{
	class QtWaitMessage : public WaitMessage
	{
	public:
		QtWaitMessage(const char* title);
		~QtWaitMessage();

	private:
		CLAM::VM::QtWaitMessageDlg* mWaitMessage;

	};
}

#endif

