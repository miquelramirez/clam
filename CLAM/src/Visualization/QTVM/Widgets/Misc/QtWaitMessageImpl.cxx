#include <qapplication.h>
#include <qeventloop.h>
#include <qlayout.h>
#include <qlabel.h>
#include "xtime.hxx"
#include "WaitMsgIndicator.hxx"
#include "QtWaitMessageImpl.hxx"

namespace CLAM
{
	namespace VM
	{
		QtWaitMessageImpl::QtWaitMessageImpl(QWidget * parent)
			: QDialog(parent)
			, mIsActive(true)
		{
			mLabel = new QLabel(this);
			mIndicator = new WaitMsgIndicator(this);
			mIndicator->setFixedHeight(25);

			QBoxLayout* layout = new QVBoxLayout(this);
			layout->setMargin(15);
			layout->setSpacing(10);

			layout->addWidget(mLabel);
			layout->addWidget(mIndicator);

			mThread.SetThreadCode(makeMemberFunctor0((*this),QtWaitMessageImpl,thread_code));
			mThread.Start();
		}

		QtWaitMessageImpl::~QtWaitMessageImpl()
		{
			if(mThread.IsRunning())
			{
				mIsActive = false;
				mThread.Stop();
			}
		}

		void QtWaitMessageImpl::LabelText(const std::string& label)
		{
			mLabel->setText(label.c_str());
		}

		void QtWaitMessageImpl::thread_code()
		{
			unsigned t0,t1;
			t0 = t1 =  GetTime();
			while(mIsActive)
			{
				t1 = GetTime();
				if(t1-t0 > REFRESH_TIME)
				{
					mIndicator->refresh();
					qApp->eventLoop()->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
					t0 = t1 = GetTime();
				}
			}
		}

		unsigned QtWaitMessageImpl::GetTime()
		{
			unsigned m1,m2;
			xtime _t;
			xtime_get(&_t,TIME_UTC);
			m1 = _t.sec*MILLISECONDS_PER_SECOND;
			m2 = _t.nsec/NANOSECONDS_PER_MILLISECOND;
			return m1+m2;
		}
	}
}

// END

