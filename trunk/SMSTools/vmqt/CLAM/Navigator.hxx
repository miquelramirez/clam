#ifndef __NAVIGATOR__
#define __NAVEGATOR__

#include <qwidget.h>

class QPushButton;
class QLCDNumber;

namespace CLAM
{
	namespace VM
	{
		class Navigator : public QWidget
		{
			Q_OBJECT
		public:
			Navigator(QWidget* parent=0);
			~Navigator();

		signals:
			void valueChanged(int);

		public slots:
			void setValue(int);
			void setRange(int,int);

		private slots:
			void first();
			void prior();
			void next();
			void last();

		private:
			QPushButton* mFirst;
			QPushButton* mPrior;
			QPushButton* mNext;
			QPushButton* mLast;
			QLCDNumber*  mDisplay;

			int          mMinValue;
			int          mMaxValue;
			int          mCurrentValue;

			void BuildNavigator();

			bool IsValid(int value);
	   
		};
	}
}

#endif

