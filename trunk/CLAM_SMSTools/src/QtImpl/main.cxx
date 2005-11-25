#include <qapplication.h>
//// to add a splash windows later ////
//#include <qsplashscreen.h          //
///////////////////////////////////////
#include "QtSMSTools.hxx"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

int main( int argc, char ** argv )
{
#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

#ifdef Q_WS_X11
	XInitThreads();
#endif

	QApplication app( argc, argv );

///////////////////////////////////
//	                         //
//   TODO: add a splash window   //
//                               // 
///////////////////////////////////

	QtSMS::QtSMSTools* w = new QtSMS::QtSMSTools;
	app.setMainWidget(w);
	w->showMaximized();
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END

