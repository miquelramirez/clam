#include <qprogressdialog.h>
#include <CLAM/QtProgressDlg.hxx>

namespace CLAM
{
	namespace VM
	{
		QtProgressDlg::QtProgressDlg()
		{		
			mProgressDlg = new QProgressDialog(0);
			mProgressDlg->setCancelButton(0);
			mProgressDlg->setGeometry(mProgressDlg->x(),mProgressDlg->y(),250,60);
			mProgressDlg->setModal(true);
		}

		QtProgressDlg::~QtProgressDlg()
		{
			mProgressDlg->hide();
			delete mProgressDlg;
		}

		void QtProgressDlg::Caption(const std::string& caption)
		{
			mProgressDlg->setCaption(caption.c_str());
		}

		void QtProgressDlg::LabelText(const std::string& label)
		{
			mProgressDlg->setLabelText(label.c_str());
		}

		void QtProgressDlg::TotalSteps(int steps)
		{
			mProgressDlg->setTotalSteps(steps);
		}

		void QtProgressDlg::Update(int val)
		{
			mProgressDlg->setProgress(val);
		}

		void QtProgressDlg::Show()
		{
			mProgressDlg->show();
		}
	}
}

// END

