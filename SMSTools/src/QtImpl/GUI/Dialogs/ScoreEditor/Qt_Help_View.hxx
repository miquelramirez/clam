#ifndef __QTHELPVIEW__
#define __QTHELPVIEW__

#include <qtextedit.h>

namespace QtSMS
{
	class Qt_Help_View : public QTextEdit
	{
	public:
		Qt_Help_View(QWidget* parent=0);
		~Qt_Help_View();

		void SetText(const QString& text);
		
	protected:
		void keyPressEvent(QKeyEvent* e);
	};
}

#endif

