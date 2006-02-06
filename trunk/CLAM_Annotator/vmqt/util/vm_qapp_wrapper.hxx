#ifndef __VMQT_QAPP_WRAPPER_H__
#define __VMQT_QAPP_WRAPPER_H__

class QApplication;

namespace CLAM
{
	namespace VM
	{
		class QAppWrapper
		{
			typedef QApplication* Wrapper;
		public:
			static void init();
			static void quit();
			static int run();

		private:
			static Wrapper vm_qapp;
			static bool initiated();
		};
	}
}

#endif

