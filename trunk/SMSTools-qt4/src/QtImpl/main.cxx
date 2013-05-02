#include <qapplication.h>
//// to add a splash windows later ////
//#include <qsplashscreen.h          //
///////////////////////////////////////
#include "QtSMSTools.hxx"

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

	QApplication app( argc, argv );

///////////////////////////////////
//                               //
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

