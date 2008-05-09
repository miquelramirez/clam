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
#include "BocaClientGui.hxx"
#include <cstdlib>

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

