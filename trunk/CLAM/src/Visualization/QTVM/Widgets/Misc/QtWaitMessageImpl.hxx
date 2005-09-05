#ifndef __QTWAITMESSAGEIMPL__
#define __QTWAITMESSAGEIMPL__

#include <string>
#include <qdialog.h>

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
		
		protected:
			void closeEvent(QCloseEvent* e);

		private:
			QLabel*           mLabel;
			WaitMsgIndicator* mIndicator;

			enum { REFRESH_INTERVAL=20 };
		};
	}
}

#endif

