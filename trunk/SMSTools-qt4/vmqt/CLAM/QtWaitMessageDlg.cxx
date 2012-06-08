#include <CLAM/QtWaitMessageDlg.hxx>
#include <CLAM/QtWaitMessageImpl.hxx>

namespace CLAM
{
	namespace VM
	{
		QtWaitMessageDlg::QtWaitMessageDlg()
		{
			mWaitMessage = new QtWaitMessageImpl;
			mWaitMessage->setGeometry(mWaitMessage->x(),mWaitMessage->y(),250,60);
			mWaitMessage->setModal(true);
		}

		QtWaitMessageDlg::~QtWaitMessageDlg()
		{
			mWaitMessage->hide();
			delete mWaitMessage;
		}

		void QtWaitMessageDlg::Caption(const std::string& caption)
		{
			mWaitMessage->setCaption(caption.c_str());
		}

		void QtWaitMessageDlg::LabelText(const std::string& label)
		{
			mWaitMessage->LabelText(label);
		}

		void QtWaitMessageDlg::Show()
		{
			mWaitMessage->show();
		}
	}

}

// END

