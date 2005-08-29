#include <qapplication.h>
//// to add a splash windows later ////
//#include <qsplashscreen.h          //
///////////////////////////////////////
#include "QtSMSTools.hxx"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif


int main( int argc, char ** argv )
{

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

