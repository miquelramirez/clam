#include <QApplication>
#include "Assert.hxx"
#include "vm_qapp_wrapper.hxx"

namespace CLAM
{
	namespace VM
	{
		QAppWrapper::Wrapper QAppWrapper::vm_qapp = 0;

		void QAppWrapper::init()
		{
			if(initiated()) return;
			int argc = 1;
			vm_qapp = new QApplication(argc,0);
		}

		void QAppWrapper::quit()
		{
			CLAM_ASSERT(initiated(), 
						"QAppWrapper::quit: wrapper has not been initiated yet, please call QAppWrapper::init() first");
			vm_qapp->quit();
		}

		int QAppWrapper::run()
		{
			CLAM_ASSERT(initiated(), 
						"QAppWrapper::quit: wrapper has not been initiated yet, please call QAppWrapper::init() first");
			vm_qapp->quit();
			vm_qapp->connect(vm_qapp,SIGNAL(lastWindowClosed()),vm_qapp,SLOT(quit()));
			return vm_qapp->exec();
		}

		bool QAppWrapper::initiated()
		{
			return (vm_qapp != 0);
		}
	}
}

// END
