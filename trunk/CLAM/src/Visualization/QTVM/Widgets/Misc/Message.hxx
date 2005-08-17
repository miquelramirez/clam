#ifndef __MESSAGE__
#define __MESSAGE__

#include <string>
#include <qmessagebox.h>

namespace CLAM
{
	namespace VM
	{
		static void Message(QMessageBox::Icon icon,
							const std::string& caption, 
							const std::string& message)
		{
			QMessageBox message_box(caption.c_str(),
									message.c_str(),
									icon,
									QMessageBox::Ok,
									QMessageBox::NoButton,
									QMessageBox::NoButton); 

			message_box.exec();
		}
	}
}
#endif

