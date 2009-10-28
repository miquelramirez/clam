/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
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
 *
 */

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>

#include "MainWindow.hxx"
#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

#ifndef I18N_PATH
#ifdef WIN32
#define I18N_PATH "../"
#else
#define I18N_PATH "/home/vokimon/CajitasDeArena/CLAM-Install/"
#endif
#endif

void usage(const std::string & program)
{
	std::cout 
		<< " Usage: " << program
		<< " <networkfile> [-b <backend>] [-p] [--help]\n"
		<< "Options:\n"
		<< " -b <backend>   The backend can be JACK, PortAudio or Auto.\n"
		<< " -p             Play the network inmediately using the network name for client name.\n"
		<< " --help         This help.\n"
		<< std::endl;
}

int main( int argc, char ** argv )
{
#ifdef USE_LADSPA
	RunTimeLadspaLibraryLoader ladspaLoader;
	ladspaLoader.Load();
	RunTimeFaustLibraryLoader faustLoader;
	faustLoader.Load();
#endif

	QApplication app( argc, argv );

	QString networkFile = "";
	QString backendName = "Auto";
	std::string clientName  = "CLAM network player";

	bool startInPlay=false;

	enum { none, backend } optionArgument = none;
	int argument=0;
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
			if (arg=="-p")
			{
				if (networkFile=="")
				{
					std::cerr<<"Can't start in play without define a network file"<<std::endl;
					return -1;
				}
				QString undottedName = networkFile;
				undottedName= undottedName.mid(undottedName.lastIndexOf("/")+1, -1);
				undottedName.truncate(undottedName.indexOf("."));
				clientName=undottedName.toLocal8Bit().constData();
				startInPlay=true;
				continue;
			}
			std::cerr << "Invalid option '" << arg << "'." << std::endl;
			usage(argv[0]);
			return -1;
		}
		switch (argument++)
		{
			case 0:
				networkFile = arg.c_str();
			break;
			default:
				std::cerr << "Too much arguments." << std::endl;
				usage(argv[0]);
				return -1;
		}
	}


	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_"+ locale);
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load(I18N_PATH "share/networkeditor/i18n/NetworkEditor_"+ locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("CLAM");
	QCoreApplication::setOrganizationDomain("clam.iua.upf.edu");
	QCoreApplication::setApplicationName("Network Editor");
	MainWindow w;
	w.show();

	w.setClientName(clientName);
	w.setBackend(backendName);
	if (networkFile!="")
	{
		w.load(networkFile);
		if (startInPlay) w.on_action_Play_triggered();
			
	}
		
	else w.clear();

	return app.exec();
}

