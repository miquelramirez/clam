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
			static void Init();
			static void Quit();
			static int Run();

		private:
			static Wrapper mQApp;
			static bool Initiated();
		};
	}
}

#endif

