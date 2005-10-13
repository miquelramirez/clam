#include <qlabel.h>
#include <qpixmap.h>
#include "AboutBox.hxx"

namespace QtSMS
{	
	AboutBox::AboutBox(QWidget* parent)
		: AboutBoxBase(parent)
	{
		mMtgXpmLbl->setPixmap(QPixmap("rc/mtg3.xpm"));
		mIuaXpmLbl->setPixmap(QPixmap("rc/iua2.xpm"));
	}
		
	AboutBox::~AboutBox()
	{
	}
}

// END
