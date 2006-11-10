#include "QtWaitMessage.hxx"
#include <CLAM/QtWaitMessageDlg.hxx>

namespace CLAMGUI
{
	QtWaitMessage::QtWaitMessage(const char* title)
		: WaitMessage(title)
	{
		mWaitMessage = new CLAM::VM::QtWaitMessageDlg;
		mWaitMessage->Caption("SMS Tools 2");
		mWaitMessage->LabelText(title);
		mWaitMessage->Show();
	}

	QtWaitMessage::~QtWaitMessage()
	{
		delete mWaitMessage;
	}
}

// END

