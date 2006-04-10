#include <QtGui/QApplication>
#include "Assert.hxx"
#include "vmQAppWrapper.hxx"

namespace CLAM
{
	namespace VM
	{
		QAppWrapper::Wrapper QAppWrapper::mQApp = 0;

		void QAppWrapper::Init()
		{
			if(Initiated()) return;
			int argc = 1;
			mQApp = new QApplication(argc,0);
		}

		void QAppWrapper::Quit()
		{
			CLAM_ASSERT(Initiated(), 
						"QAppWrapper::Quit: wrapper has not been initiated yet, please call QAppWrapper::Init() first");
			mQApp->quit();
		}

		int QAppWrapper::Run()
		{
			CLAM_ASSERT(Initiated(), 
						"QAppWrapper::Run: wrapper has not been initiated yet, please call QAppWrapper::Init() first");
			mQApp->connect(mQApp,SIGNAL(lastWindowClosed()),mQApp,SLOT(quit()));
			return mQApp->exec();
		}

		bool QAppWrapper::Initiated()
		{
			return (mQApp != 0);
		}
	}
}

// END
