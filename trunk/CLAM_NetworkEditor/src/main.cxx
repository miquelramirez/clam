


#include "Network.hxx"
#include "NetworkController.hxx"
#include "PushFlowControl.hxx"
#include <string>

#include "AudioManager.hxx"

#include <qapplication.h>

#include <cmath>
#include <time.h>
#include "MainWindow.hxx"

CLAM::AudioManager audioManager( 44100, 512 );

void ConfigureNetwork(CLAM::Network & net)
{	
	int frameSize = 512;
	net.AddFlowControl( new CLAM::PushFlowControl( frameSize ));

}

int main( int argc, char **argv )
{

	srand(time(NULL)); // gui stuff

	CLAM::Network net;
	net.SetName("CLAM Network");
	ConfigureNetwork(net);

	CLAMVM::NetworkController controller;
	controller.BindTo(net);

	QApplication app( argc, argv );
	app.setFont(QFont("Verdana", 9));
	NetworkGUI::MainWindow mw;
	
	mw.GetNetworkPresentation().AttachTo(controller);
	controller.Publish();
	app.setMainWidget(&mw);
	mw.show();
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
	return app.exec();
}

