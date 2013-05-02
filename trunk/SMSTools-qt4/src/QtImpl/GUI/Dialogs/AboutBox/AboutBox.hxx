#ifndef __ABOUTBOX__
#define __ABOUTBOX__

#include "ui_AboutBox.hxx"

namespace QtSMS
{
	class AboutBox : public QDialog, public Ui::AboutBox
	{
	public:
		AboutBox(QWidget* parent=0);
		~AboutBox();
		void mouseReleaseEvent( QMouseEvent * e )
		{
			close();
		}
	};
}

#endif

