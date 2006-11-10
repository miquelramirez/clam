#ifndef __QTSMSGENDERSELECTOR__
#define __QTSMSGENDERSELECTOR__

#include <qwidget.h>

namespace QtSMS
{
	class Qt_SMS_Gender_Selector : public QWidget
	{
		Q_OBJECT
	public:
		Qt_SMS_Gender_Selector(QWidget* parent=0);
		~Qt_SMS_Gender_Selector();

	signals:
		void genderChanged(int);

	};
}

#endif

