/*
 *	main.cxx
 *	main function
 *
 *  Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <qapplication.h>
#include "V2MGUI.hxx"

int main(int argc,char** argv)
{
	QApplication app(argc,argv);
	V2MGUI* mw = new V2MGUI();
	mw->show();
	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END




