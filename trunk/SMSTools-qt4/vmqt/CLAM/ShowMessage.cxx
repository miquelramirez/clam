#include <qmessagebox.h>
#include <CLAM/ShowMessage.hxx>

namespace CLAM
{
	namespace VM
	{
		ShowMessage::Messages ShowMessage::mMessages;

		void ShowMessage::NoMIDISupport()
		{
			if(!IsInitiated()) Init();
			if(mMessages[NOMIDI]) return;
			mMessages[NOMIDI] = true;
			QMessageBox message("MIDI support not available",
								"Seems that your system has not MIDI support. MIDI settings will be deactivated",
								QMessageBox::Warning,
								QMessageBox::Ok,
								QMessageBox::NoButton,
								QMessageBox::NoButton); 

				message.exec();
		}

		void ShowMessage::Init()
		{
			for(unsigned i=0; i < MESSAGES; i++) mMessages.push_back(false);
		}

		bool ShowMessage::IsInitiated()
		{
			return mMessages.size();
		}
	}
}

// END

