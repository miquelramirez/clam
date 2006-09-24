#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

#include "PrototypeLoader.hxx"
#include <CLAM/BlockingNetworkPlayer.hxx>
#include <CLAM/JACKNetworkPlayer.hxx>

#include "Utils.hxx"
#include <iostream>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>

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

	std::string networkFile;
	if (argc<2)
	{
		QString file = QFileDialog::getOpenFileName(0,
			"Choose a Network to run",
			QString::null,
			"CLAM Network files(*.clamnetwork)");
		if (file.isEmpty()) return -1;
		networkFile=file.toStdString();
	}
	else
	{
		networkFile=argv[1];
	}

	std::string uiFile = argc>2 ? argv[2] : GetUiFromXmlFile(networkFile);

	CLAM::NetworkPlayer * networkPlayer;
	CLAM::JACKNetworkPlayer * jackPlayer = new CLAM::JACKNetworkPlayer();
	if ( jackPlayer->IsConnectedToServer())
	{
		networkPlayer = jackPlayer;
	}
	else
	{
		delete jackPlayer;
		networkPlayer = new CLAM::BlockingNetworkPlayer();
	}

	CLAM::PrototypeLoader prototype;

	bool ok = prototype.LoadNetwork(networkFile);
	if (not ok) return -1;

	prototype.SetNetworkPlayer( *networkPlayer );
	
	QWidget * interface = prototype.LoadInterface( uiFile.c_str() );
	if (not interface) return -1;

	prototype.ConnectWithNetwork();

	prototype.Show();
	prototype.Start();
	int result = app.exec();
	prototype.Stop();

	return result;
}
