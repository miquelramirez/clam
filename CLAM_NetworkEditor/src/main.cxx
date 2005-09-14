/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#if USE_OSCPACK
#include "OSCEnabledNetwork.hxx"
#else
#include "Network.hxx"
#endif

#include "NetworkController.hxx"
#include "PushFlowControl.hxx"
#include "BasicFlowControl.hxx"
#include <string>

#include "AudioManager.hxx"

#include "MIDIManager.hxx"

#include <qapplication.h>

#include <cmath>
#include <ctime>
#include "MainWindow.hxx"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

void ConfigureNetwork(CLAM::Network & net)
{	
	int frameSize = 1024; // was 512
	net.AddFlowControl( new CLAM::PushFlowControl( frameSize ));

}

int main( int argc, char **argv )
{
#ifdef Q_WS_X11
	XInitThreads();
#endif

	CLAM::AudioManager audioManager( 44100, 1024 ); //was 44100, 512

	CLAM::MIDIManager midiManager;
	
	srand(time(NULL)); // gui stuff

#if USE_OSCPACK
	CLAM::OSCEnabledNetwork net;
	net.SetName("CLAM OSCEnabledNetwork");
#else
	CLAM::Network net;
	net.SetName("CLAM Network");
#endif
	ConfigureNetwork(net);

	CLAMVM::NetworkController controller;
	controller.BindTo(net);

	QApplication app( argc, argv );
	app.setFont(QFont("Verdana", 9));
	NetworkGUI::MainWindow mw;
	
	mw.GetNetworkPresentation().AttachToNetworkController(controller);
	app.setMainWidget(&mw);
	mw.show();
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

	return app.exec();
}

