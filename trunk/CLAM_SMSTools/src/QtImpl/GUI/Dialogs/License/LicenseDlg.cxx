#include <qtextbrowser.h>
#include "LicenseDlg.hxx"

namespace QtSMS
{
	LicenseDlg::LicenseDlg(QWidget* parent)
		: LicenseDlgBase(parent)
	{
		mDisplay->mimeSourceFactory()->setFilePath("doc/en/");
		mDisplay->setSource("license.html");
	}
		
	LicenseDlg::~LicenseDlg()
	{
	}
}

// END
