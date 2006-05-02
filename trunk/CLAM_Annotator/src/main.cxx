#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>

#include "Annotator.hxx"

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

	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_"+ locale);
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load("share/annotator/i18n/Annotator_"+ locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("CLAM");
	QCoreApplication::setOrganizationDomain("clam.iua.upf.edu");
	QCoreApplication::setApplicationName("Music Annotator");
	Annotator w(argc>1?argv[1]:"");
	w.show();
	return app.exec();
}
