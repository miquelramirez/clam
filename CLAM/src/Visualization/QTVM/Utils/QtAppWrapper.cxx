#include <qapplication.h>
#include "QtAppWrapper.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAppWrapper::Wrapper QtAppWrapper::_app;

		void QtAppWrapper::Init()
		{
			if(!IsInitiated())
			{
				int argc=1;
				_app = new QApplication(argc,NULL);
			}
		}

		void QtAppWrapper::Quit()
		{
			_app->quit();
		}

		int QtAppWrapper::Run()
		{
			_app->connect(_app,SIGNAL(lastWindowClosed()),_app,SLOT(quit()));
			return _app->exec();
		}

		bool QtAppWrapper::IsInitiated()
		{
			return _app;
		}

	}
}

// END

