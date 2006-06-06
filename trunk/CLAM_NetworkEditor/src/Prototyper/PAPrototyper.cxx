#ifndef USE_PORTAUDIO
#error ERROR: trying to compile a PortAudio based example without having it enabled in CLAM
#endif

#include "PrototypeLoader.hxx"
#include "PANetworkPlayer.hxx"

#include "Utils.hxx"
#include <iostream>

int main( int argc, char *argv[] )
{
	if (argc<2 || argc>3)
	{
		std::cout << " Usage: " 
			<< argv[0] << " <networkfile> [ <uifile> ]" << std::endl;
		return -1;
	}

#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

	std::string networkFile, uiFile;
	
	networkFile=argv[1];

	uiFile= argc>2 ? argv[2] : GetUiFromXmlFile(networkFile);

	if ( !FileExists(networkFile) || !FileExists(uiFile) )
		return -1;
	
	QApplication app( argc, argv );

	CLAM::PrototypeLoader loader( networkFile );
	loader.SetNetworkPlayer( *(new CLAM::PANetworkPlayer(networkFile)) );
	
	QWidget * prototype = loader.LoadPrototype( uiFile );
	if (!prototype) return -1;
	loader.ConnectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


	loader.Start();
	int result = app.exec();
	loader.Stop();

	return result;
}
