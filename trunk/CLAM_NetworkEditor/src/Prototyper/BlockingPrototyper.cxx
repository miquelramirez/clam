#include "PrototypeLoader.hxx"
#include "BlockingNetworkPlayer.hxx"

#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif


#include "Utils.hxx"

#include <string>

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
	
	if (argc==2)
		uiFile=GetUiFromXmlFile(networkFile);
	else if (argc==3)
		uiFile = argv[2]; 
	
	if ( !FileExists(networkFile) || !FileExists(uiFile) )
		return -1;
	
	QApplication app( argc, argv );

	QApplication::addLibraryPath(app.applicationDirPath()+"/plugins");

	CLAM::PrototypeLoader loader(networkFile);
	loader.SetNetworkPlayer( *(new CLAM::BlockingNetworkPlayer(networkFile)) );
	
	QWidget * prototype = loader.LoadPrototype(uiFile);
	if (!prototype) return -1;
	loader.ConnectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


//	loader.Start();
	int result = app.exec();
	loader.Stop();
	
	return result;
}
