#ifndef __QTWAITMESSAGEIMPL__
#define __QTWAITMESSAGEIMPL__

#include <string>
#include <qdialog.h>
#include "Thread.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class WaitMsgIndicator;

		class QtWaitMessageImpl : public QDialog
		{
		public:
			QtWaitMessageImpl(QWidget * parent = 0);
			~QtWaitMessageImpl();

			void LabelText(const std::string& label);

		private:
			QLabel*           mLabel;
			WaitMsgIndicator* mIndicator;
			Thread            mThread;
			bool              mIsActive;

			enum { REFRESH_TIME=20 };

			void thread_code();
			unsigned GetTime();
		
		};
	}
}

#endif

