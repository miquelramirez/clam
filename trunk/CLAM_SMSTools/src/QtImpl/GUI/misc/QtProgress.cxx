#include <CLAM/QtProgressDlg.hxx>
#include "QtProgress.hxx"

namespace CLAMGUI
{
	QtProgress::QtProgress( const char* title, float from, float to )
		: Progress(title,from,to)
	{	   
		mProgressDlg = new CLAM::VM::QtProgressDlg;
		mProgressDlg->Caption("SMS Tools 2");
		mProgressDlg->LabelText(title);
		mProgressDlg->TotalSteps(int(to));
		mProgressDlg->Show();
	}

	QtProgress::~QtProgress()
	{
		delete mProgressDlg;
	}

	void QtProgress::Update(float val)
	{
		mProgressDlg->Update(int(val));
	}
}

// END

