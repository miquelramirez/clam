/***************************************************************************
 *                        main.cpp  -  rondebosse
 *
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "rondebosse.h"

//Cabeceras de Rappid
#include "Rappid.hxx"

static const char *description =
	I18N_NOOP("RondeBosse");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

#define VERSION ""

int main(int argc, char *argv[])
{
	RappidConfig cfg;

	cfg.parse_command_line(argc,argv);

	Rappid app(cfg);

	papp = &app;

	KAboutData aboutData( "rondebosse", I18N_NOOP("RondeBosse"),
    VERSION, description, KAboutData::License_GPL,
    "(c) 2002, MUSIC TECHNOLOGY GROUP (MTG)", 0, 0, "");
    aboutData.addAuthor("MUSIC TECHNOLOGY GROUP (MTG)",0, "");
    KCmdLineArgs::init( 0, argv, &aboutData );
    KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

    KApplication a;
    RondeBosse *rondebosse = new RondeBosse();
    a.setMainWidget(rondebosse);
    rondebosse->show();

    rondebosse->UpdateSliderValues();

    return a.exec();
}
	
