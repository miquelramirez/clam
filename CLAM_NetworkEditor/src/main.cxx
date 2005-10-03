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
#include "PANetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "BasicFlowControl.hxx"

#include <qapplication.h>

#include <cmath>
#include <ctime>
#include "MainWindow.hxx"

#include "MIDIManager.hxx"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#include <fstream>
#include <iostream>
#include <string>

using std::string;

void ConfigureNetwork(CLAM::Network & net)
{	
	int frameSize = 512;
	net.AddFlowControl( new CLAM::PushFlowControl( frameSize ));
}

void PrintUsageAndExit( const string& extramessage)
{
	std::cout << extramessage << std::endl;
	std::cout << "  Usage: ./NetworkEditor [ <NetworkFile> --driver,-d alsa | jack | portaudio ]"<< std::endl;
	exit(0);
}

CLAM::NetworkPlayer* CreateNetworkPlayerFromName(const string name)
{
	if ( name == string("alsa"))
		return new CLAM::BlockingNetworkPlayer();
	else if ( name == string("jack") )
		return new CLAM::JACKNetworkPlayer();
	else if ( name == string("portaudio") )
		return new CLAM::PANetworkPlayer();
	else PrintUsageAndExit("Incorrect driver!");

	return NULL;
}

bool IsAskingForDriver(char* arg)
{
	return ( string(arg) == string("-d") || string(arg) == string("--driver") );
}

bool IsAskingForHelp(char* arg)
{
	return ( string(arg) == string("-h") || string(arg) == string("--help") );
}

CLAM::NetworkPlayer* ProcessParameters(int argc, char **argv, string& xmlfile)
{	
	switch (argc)
	{
		//No parameter
		case 1:
			return CreateNetworkPlayerFromName("alsa");
			break;
			
		//Network file specified
		case 2:
			if ( IsAskingForHelp( argv[1] ) )
				PrintUsageAndExit("NetworkEditor help");
			
			xmlfile=string(argv[1]);
			
			return CreateNetworkPlayerFromName("alsa");
			break;
			
		//Connect-to specified
		case 3:
			if ( !IsAskingForDriver( argv[1] ) )	
				PrintUsageAndExit("ERROR: Unknown command");
			
			return CreateNetworkPlayerFromName( string(argv[2]) );
			break;
			
		//Network and connect-to specified
		case 4:	
			xmlfile=string(argv[1]);
	
			if ( !IsAskingForDriver( argv[2] ) )
				PrintUsageAndExit("ERROR: Unknown command");
	
			return CreateNetworkPlayerFromName( string(argv[3]) );
			break;
			
		default:
			PrintUsageAndExit("ERROR: incorrect number of arguments");
			break;
	}

	return NULL;

}
bool FileExists( const string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file )
	{
		std::cerr << "ERROR: opening file <" << filename << ">" << std::endl;
		return false;
	}
	file.close();
	return true;
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
	CLAM::NetworkPlayer *player;
	
	string xmlfile;
	
	player=ProcessParameters( argc, argv, xmlfile );
	
	controller->SetNetworkPlayer( *player );
	controller->BindTo( *net );

	QApplication app( argc, argv );
	app.setFont( QFont("Verdana", 9) );
	NetworkGUI::MainWindow mw;
	
	mw.GetNetworkPresentation().AttachToNetworkController(*controller);


	if ( !xmlfile.empty() && FileExists( xmlfile) )
		controller->LoadNetworkFrom( xmlfile );
	
	app.setMainWidget( &mw );
	mw.show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

	return app.exec();
}

