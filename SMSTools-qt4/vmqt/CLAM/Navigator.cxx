#include <qlayout.h>
#include <qpushbutton.h>
#include <qlcdnumber.h>
#include <qtooltip.h>
#include <CLAM/Message.hxx>
#include <CLAM/Navigator.hxx>

namespace CLAM
{
	namespace VM
	{
		Navigator::Navigator(QWidget* parent)
			: QWidget(parent)
			, mMinValue(0)
			, mMaxValue(0)
			, mCurrentValue(0)
		{
			BuildNavigator();
		}
		
		Navigator::~Navigator()
		{
		}

		void Navigator::setValue(int value)
		{
			if(!IsValid(value)) return;
			mCurrentValue = value;
			mDisplay->display(mCurrentValue);
		}

		void Navigator::setRange(int min,int max)
		{
			if(min >= max)
			{
				CLAM::VM::Message(QMessageBox::Warning,"Navigator","setRange: min value must be minor than max value");
			}
			else
			{
				mMinValue = min;
				mMaxValue = max;
				mCurrentValue = mMinValue;
				mDisplay->display(mCurrentValue);
			}
		}

		void Navigator::first()
		{
			if(!IsValid(mMinValue)) return;
			setValue(mMinValue);
			emit valueChanged(mMinValue);
		}
		
		void Navigator::prior()
		{
			if(!IsValid(mCurrentValue-1)) return;
			setValue(mCurrentValue-1);
			emit valueChanged(mCurrentValue);
		}

		void Navigator::next()
		{
			if(!IsValid(mCurrentValue+1)) return;
			setValue(mCurrentValue+1);
			emit valueChanged(mCurrentValue);
		}

		void Navigator::last()
		{
			if(!IsValid(mMaxValue)) return;
			setValue(mMaxValue);
			emit valueChanged(mMaxValue);
		}

		void Navigator::BuildNavigator()
		{
			mFirst = new QPushButton(this);
			mFirst->setFixedSize(25,25);
			mFirst->setText("<<");
			QToolTip::add(mFirst,"First");
			mPrior = new QPushButton(this);
			mPrior->setFixedSize(25,25);
			mPrior->setAutoRepeat(true);
			mPrior->setText("<");
			QToolTip::add(mPrior,"Prior");
			mNext = new QPushButton(this);
			mNext->setFixedSize(25,25);
			mNext->setAutoRepeat(true);
			mNext->setText(">");
			QToolTip::add(mNext,"Next");
			mLast = new QPushButton(this);
			mLast->setFixedSize(25,25);
			mLast->setText(">>");
			QToolTip::add(mLast,"Last");
			mDisplay = new QLCDNumber(this);
			mDisplay->setFixedHeight(25);
			mDisplay->setMode(QLCDNumber::Dec);
			mDisplay->setSegmentStyle(QLCDNumber::Flat);
			mDisplay->display(mCurrentValue);

			QBoxLayout* layout = new QHBoxLayout(this);
			layout->addWidget(mFirst);
			layout->addWidget(mPrior);
			layout->addWidget(mDisplay);
			layout->addWidget(mNext);
			layout->addWidget(mLast);
			
			connect(mFirst,SIGNAL(clicked()),SLOT(first()));
			connect(mPrior,SIGNAL(clicked()),SLOT(prior()));
			connect(mNext,SIGNAL(clicked()),SLOT(next()));
			connect(mLast,SIGNAL(clicked()),SLOT(last()));
		}

		bool Navigator::IsValid(int value)
		{
			return (value != mCurrentValue && value >= mMinValue && value <= mMaxValue);
		}

	}
}

// END

