#ifndef __QTPROGRESS__
#define __QTPROGRESS__

#include "Progress.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtProgressDlg;
	}
}

namespace CLAMGUI
{
	class QtProgress : public Progress
	{
	public:
		QtProgress( const char* title, float from, float to );
		~QtProgress();

		void Update(float val);
	
	private:
		CLAM::VM::QtProgressDlg* mProgressDlg;
	};
}

#endif

