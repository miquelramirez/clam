/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>

#include "MainWindow.hxx"

#ifdef _MSVC_VER
#include <CLAM/InitAudioIO.hxx>
#include <CLAM/InitProcessing.hxx>
#endif

#ifndef I18N_PATH
#define I18N_PATH "../"
#endif

void usage(const std::string & program)
{
	std::cout 
		<< " Usage: " << program
		<< " [-b <backend>] [--help]\n"
		<< "Options:\n"
		<< " -b <backend>   The backend can be JACK, PortAudio or Auto.\n"
		<< " --help         This help.\n"
		<< std::endl;
}

int main( int argc, char ** argv )
{

#ifdef _MSVC_VER
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif
	
	QApplication app( argc, argv );

	QString backendName = "Auto";
	enum { none, backend } optionArgument = none;
	for (int i=1; i<argc; i++)
	{
		std::string arg = argv[i];
		if (optionArgument!=none)
		{
			backendName = arg.c_str();
			optionArgument=none;
			continue;
		}
		if (arg[0]=='-')
		{
			if (arg=="--help")
			{
				usage(argv[0]);
				return 0;
			}
			if (arg=="-b")
			{
				optionArgument=backend;
				continue;
			}
			std::cerr << "Invalid option '" << arg << "'." << std::endl;
			usage(argv[0]);
			return -1;
		}
	}

	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_"+ locale);
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load(I18N_PATH "share/chordata/i18n/Chordata_"+ locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("CLAM");
	QCoreApplication::setOrganizationDomain("clam.iua.upf.edu");
	QCoreApplication::setApplicationName("Chordata");
	MainWindow w;
	w.show();
	w.setBackend(backendName);

	return app.exec();
}
