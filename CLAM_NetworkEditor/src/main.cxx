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
#include "BlockingNetworkPlayer.hxx"
#include "JACKNetworkPlayer.hxx"
#include "PushFlowControl.hxx"
#include "BasicFlowControl.hxx"
#include <string>

#include <qapplication.h>

#include <cmath>
#include <ctime>
#include "MainWindow.hxx"

#include "MIDIManager.hxx"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

void ConfigureNetwork(CLAM::Network & net)
{	
	int frameSize = 512;
	net.AddFlowControl( new CLAM::PushFlowControl( frameSize ));
}

int main( int argc, char **argv )
{
#ifdef Q_WS_X11
	XInitThreads();
#endif

	CLAM::MIDIManager midiManager;
	srand(time(NULL)); // gui stuff


#if USE_OSCPACK
	CLAM::OSCEnabledNetwork* net=new CLAM::OSCEnabledNetwork();
	net->SetName("CLAM OSCEnabledNetwork");
#else
	CLAM::Network* net=new CLAM::Network();
	net->SetName("CLAM Network");
#endif
	ConfigureNetwork(*net);

	CLAMVM::NetworkController* controller=new CLAMVM::NetworkController();
	CLAM::NetworkPlayer *player=new CLAM::JACKNetworkPlayer();

	controller->SetNetworkPlayer(*player);
	controller->BindTo(*net);

	/*std::string opt;
	
	//TODO add a real parameter checking. Now it is rubbish.
	switch(argc)
	{
		//No parameter
		case 1:
			player=new CLAM::BlockingNetworkPlayer();
			break;
			
		//Network file specified
		case 2:
			opt=argv[1];
			if (opt==std::string("--help"))
			{
				std::cout << "\nUsage: " << argv[0] << " [ <NetworkFile> ] [ -c jack | portaudio ]\n"<< std::endl;
				return 0;
			}
			controller->LoadNetworkFrom(argv[1]);
			break;
			
		//Connect-to specified
		case 3:
			opt=argv[2];
			if ( opt==std::string("jack"))
			{
				//player=new CLAM::JACKNetworkPlayer();
				std::cout << "\nJACK Option still not available" << std::endl;
			}
			else if ( opt==std::string("portaudio"))
			{
				//player=new CLAM::PANetworkPlayer();
				std::cout << "\nPA Option still not available" << std::endl;
				return 0;
			}
			break;
			
		//Network and connect-to specified
		case 4:	
			controller->LoadNetworkFrom(argv[1]);
		
			opt=argv[3];
			if ( opt==std::string("jack"))
			{
				//player=new CLAM::JACKNetworkPlayer();
				std::cout << "\nJACK Option still not available" << std::endl;
			}
			else if ( opt==std::string("portaudio"))
			{
				//player=new CLAM::PANetworkPlayer();
				std::cout << "\nPA Option still not available" << std::endl;
				return 0;
			}
			break;
			
		default:
			std::cout << "\nUsage: " << argv[0] << " [ <NetworkFile> ] [ -c jack | portaudio ]\n"<< std::endl;
			return 0;
			break;
	}*/
	
	QApplication app( argc, argv );
	app.setFont(QFont("Verdana", 9));
	NetworkGUI::MainWindow mw;
	
	mw.GetNetworkPresentation().AttachToNetworkController(*controller);
	app.setMainWidget(&mw);
	mw.show();
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

	return app.exec();
}

