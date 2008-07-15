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

#include "Turnaround.hxx"

#ifdef _MSVC_VER
#include <CLAM/InitAudioIO.hxx>
#include <CLAM/InitProcessing.hxx>
#endif

#ifndef I18N_PATH
#define I18N_PATH "../"
#endif

int main( int argc, char ** argv )
{

#ifdef _MSVC_VER
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif
	
	QApplication app( argc, argv );

	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_"+ locale);
	app.installTranslator(&qtTranslator);

	QTranslator translator;
	translator.load(I18N_PATH "share/turnaround/i18n/Turnaround_"+ locale);
	app.installTranslator(&translator);

	QCoreApplication::setOrganizationName("CLAM");
	QCoreApplication::setOrganizationDomain("clam.iua.upf.edu");
	QCoreApplication::setApplicationName("Turnaround");
	Turnaround w(argc>1?argv[1]:"");
	w.show();
	return app.exec();
}
