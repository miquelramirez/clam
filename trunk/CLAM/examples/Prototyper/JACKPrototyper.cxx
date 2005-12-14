
#ifndef USE_JACK
#error ERROR: trying to compile a JACK based example without having it enabled in CLAM
#endif

#include "JACKNetworkPlayer.hxx"
#include "PrototypeLoader.hxx"

#include "Utils.hxx"

int main( int argc, char *argv[] )
{
	if ( argc<2 || argc>5 )
	{
		std::cout << " Usage: " 
			<< argv[0] << " <networkfile> [ <uifile> inputPort outputPort ]" << std::endl;
		return -1;
	}

	std::string networkFile, uiFile;
	
	QApplication app( argc, argv );

	std::list<std::string> portlist;

	//Deal with Network&Interface files
	if ( argc==2 )
	{
		networkFile=argv[1];
		uiFile=GetUiFromXmlFile(networkFile);
	}
	else if ( argc>2 )
	{
		networkFile=argv[1];
		uiFile=argv[2];
	}

	//Deal with input and output port patterns
	if ( argc==4 ) //Case 1: only input
	{		
		portlist.push_back( std::string( argv[3]) );
		portlist.push_back( "NULL");
	}
	else if ( argc==5 ) //Case 2: input & output
	{
		portlist.push_back( argv[3] );
		portlist.push_back( std::string(argv[4]) );
	}
	else //Case 3: nothing specified
	{
		portlist.push_back( "NULL" );
		portlist.push_back( "NULL" );
	}

	if ( !FileExists(networkFile) || !FileExists(uiFile) )
		return -1;
	
	CLAM::PrototypeLoader loader( networkFile );

	loader.SetNetworkPlayer( *(new CLAM::JACKNetworkPlayer(networkFile, portlist)) );
	
	QWidget * prototype = loader.LoadPrototype( uiFile );
	if ( !prototype ) return -1;
	loader.ConnectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


	loader.Start();
	int result = app.exec();
	loader.Stop();

	return result;
}
