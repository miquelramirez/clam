#ifndef __PROGRESSGUI__
#define __PROGRESSGUI__

#include "Fl_Progress.hxx"
#include "Progress.hxx"
#include <pthread.h>

namespace CLAMGUI
{

class ProgressGUI
	: public Progress
{
public:

	
	
	void Update(float val)
	{
		mProgressBar->mValue = val;
	}

	ProgressGUI( const char* title, float from, float to );

	~ProgressGUI();

private:

	Fl_Progress* mProgressBar;
};

}
#endif // ProgressGUI.hxx
