
#include "Proto.hxx"
#include "BlockingNetworkPlayer.hxx"

int main( int argc, char *argv[] )
{
	if (argc!=3)
	{
		std::cout << " Usage: " 
			<< argv[0] << " <networkfile> <uifile>" << std::endl;
		return -1;
	}

	char * networkFile = argv[1]; // "SpectralDelay.clam"
	char * uiFile = argv[2]; // "SpectralDelay.ui" 

	QApplication app( argc, argv );

	CLAM::PrototypeLoader loader(networkFile);
	loader.SetNetworkPlayer( *(new CLAM::BlockingNetworkPlayer(networkFile)) );
	
	QWidget * prototype = loader.loadPrototype(uiFile);
	if (!prototype) return -1;
	loader.connectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


	loader.Start();
	int result = app.exec();
	loader.Stop();
	
	return result;
}
