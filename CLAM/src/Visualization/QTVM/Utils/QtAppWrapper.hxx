#ifndef __QTAPPWRAPPER__
#define __QTAPPWRAPPER__

class QApplication;

namespace CLAM
{
	namespace VM
	{
		class QtAppWrapper
		{
			typedef QApplication* Wrapper; 
			
			public:
				static void Init();
				static void Quit();
				static int Run();
				
			private:
				static Wrapper _app;
				
				static bool IsInitiated();
				
		};
	}
}

#endif

