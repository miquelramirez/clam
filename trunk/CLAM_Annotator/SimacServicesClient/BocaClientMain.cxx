#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include "BocaClientGui.hxx"

#ifndef I18N_PATH
#ifdef WIN32
#define I18N_PATH "../"
#else
#define I18N_PATH "/home/vokimon/CajitasDeArena/CLAM-Install/"
#endif
#endif


int main( int argc, char ** argv )
{

	QApplication app( argc, argv );

	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_"+ locale);
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load(I18N_PATH "share/annotator/i18n/Annotator_"+ locale);
	app.installTranslator(&translator);

	const char * taskFile = (argc>1)? argv[1] : 0;
	const char * projectFile = (argc>2)? argv[2] : 0;
	const char * projectDir = (argc>3)? argv[3] : 0;

	QCoreApplication::setOrganizationName("CLAM");
	QCoreApplication::setOrganizationDomain("clam.iua.upf.edu");
	QCoreApplication::setApplicationName("Boca Client");
	BocaClientGui w(taskFile, projectFile, projectDir);
	w.show();
	return app.exec();
}

