#include "QtWrapper.hxx"
#include "QtAppSignaler.hxx"
#include <qapplication.h>

using namespace CLAMGUI;




QtWrapper::~QtWrapper()
{
	delete mSignaler;
	mqApp->quit();
}

bool QtWrapper::IsClosing() const
{
	return mSignaler->is_finalizing;
}

void QtWrapper::Tick() const
{
	// Why sixty milliseconds? Because Windoze launches WM_PAINT 
	// approximately 30 times/sec, so this way it is more likely
	// to have consistently updated the screen...
	// X11 users: don't worry, it won't harm you by any means :)
	mqApp->processEvents(60);
}

void QtWrapper::Run() const
{
	mqApp->exec();
}



QtWrapper::QtWrapper()
	: mqApp(  NULL )
{
	int argc = 1;
	mqApp = new QApplication( argc, NULL );
	mSignaler = new QtAppSignaler( mqApp );

}
