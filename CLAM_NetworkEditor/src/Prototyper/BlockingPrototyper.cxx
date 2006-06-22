#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

#include "PrototypeLoader.hxx"
#include "BlockingNetworkPlayer.hxx"

#include "Utils.hxx"
#include <iostream>
#include <qfiledialog.h>

int main( int argc, char *argv[] )
{
	if (argc>3)
	{
		std::cout << " Usage: " 
			<< argv[0] << " <networkfile> [ <uifile> ]" << std::endl;
		return -1;
	}

#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

	QApplication app( argc, argv );

	std::string networkFile, uiFile;
	if (argc<2)
	{
		QString file = QFileDialog::getOpenFileName( QString::null, "CLAM Network files(*.clamnetwork)", 0, 0, "Choose a Network to run");
		if (!file) return -1;
		networkFile=file.ascii();
	}
	else
	{
		networkFile=argv[1];
	}

	uiFile= argc>2 ? argv[2] : GetUiFromXmlFile(networkFile);

	if ( !FileExists(networkFile) || !FileExists(uiFile) )
		return -1;
	
	CLAM::PrototypeLoader loader( networkFile );
	loader.SetNetworkPlayer( *(new CLAM::BlockingNetworkPlayer(networkFile)) );
	
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
