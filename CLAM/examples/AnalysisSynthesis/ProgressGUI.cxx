#include "ProgressGUI.hxx"
#include <pthread.h>
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif
#ifdef WIN32
#include "CLAM_windows.h"
#endif


namespace CLAMGUI
{

ProgressGUI::ProgressGUI(const char* title,float from,float to)
	:Progress(title,from,to)
{
	mProgressBar = new Fl_Progress;
	mProgressBar->mLabel->label( title );
	mProgressBar->mSlider->range( from, to );
	mProgressBar->mWindow->show();
}

ProgressGUI::~ProgressGUI()
{
	mProgressBar->mWindow->hide();
	delete mProgressBar;
}	

}
