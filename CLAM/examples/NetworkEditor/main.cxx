


#include "Network.hxx"
#include "NetworkController.hxx"
#include "PushFlowControl.hxx"
#include <string>

#include "Oscillator.hxx"
#include "AudioFile.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioMixer.hxx"
#include "AudioMultiplier.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "Qt_NetworkPresentation.hxx"
#include <qapplication.h>

#include <cmath>
#include <time.h>
#include "Factory.hxx"
#include "MainWindow.hxx"


CLAM::AudioManager audioManager( 44100, 512 );

void ConfigureNetwork(CLAM::Network & net)

{	
	int frameSize = 512;
	std::string fileInName = "testin.wav";
	std::string fileOutName = "testout.wav";

	CLAM::AudioFileConfig fileInCfg;
	fileInCfg.SetChannels(1);
	fileInCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileInCfg.SetFrameSize( frameSize );
	fileInCfg.SetFilename( fileInName );
	fileInCfg.SetKeepFrameSizes(true);		

	CLAM::OscillatorConfig generatorCfg;
	generatorCfg.SetFrequency(440.0);
	generatorCfg.SetAmplitude(0.5);

	CLAM::OscillatorConfig modulatorCfg;
	modulatorCfg.SetFrequency(20.0);
	modulatorCfg.SetAmplitude(0.5);

	CLAM::AudioMixerConfig mixerCfg;
	mixerCfg.SetFrameSize(frameSize);	

	CLAM::AudioFileConfig fileOutCfg;
	fileOutCfg.SetChannels(1);
	fileOutCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileOutCfg.SetFrameSize( frameSize );
	fileOutCfg.SetFilename( fileOutName );
	fileOutCfg.SetKeepFrameSizes(true);
	
//	CLAM::AudioIOConfig audioOutCfg;
//	audioOutCfg.SetFrameSize( frameSize );

	net.AddFlowControl( new CLAM::PushFlowControl( frameSize ));

	net.AddProcessing( "audio-out", new CLAM::AudioOut );
//	net.AddProcessing( "file-in", new CLAM::AudioFileIn(fileInCfg));
//	net.AddProcessing( "oscillator-modulator", new CLAM::Oscillator( modulatorCfg) );
//	net.AddProcessing( "multiplier", new CLAM::AudioMultiplier );
	net.AddProcessing( "oscillator-generator", new CLAM::Oscillator(generatorCfg) );
//	net.AddProcessing( "mixer", new CLAM::AudioMixer<3>(mixerCfg) );
	net.AddProcessing( "file-out", new CLAM::AudioFileOut(fileOutCfg));
	
//	net.ConnectPorts( "file-in.Output", "file-out.Input" );

//	net.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
//	net.ConnectPorts( "oscillator-modulator.Audio Output", 
//			  "multiplier.Second Audio Input" );
//	net.ConnectPorts( "multiplier.Audio Output" , "mixer.Input Audio_0" );
//	net.ConnectPorts( "oscillator-generator.Audio Output", "mixer.Input Audio_1" );
//	net.ConnectPorts( "mixer.Output Audio", "file-out.Input" );

	net.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
	net.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
}

int main( int argc, char **argv )
{

	srand(time(NULL)); // gui stuff

	CLAM::Network net;
	net.SetName("Network with audio mixer");
	ConfigureNetwork(net);
/*
	CLAMVM::NetworkController controller;
	controller.BindTo(net);

	QApplication app( argc, argv );
	app.setFont(QFont("Verdana", 9));
	NetworkGUI::MainWindow *mw = new NetworkGUI::MainWindow;

	mw->GetNetworkPresentation().AttachTo(controller);
	controller.Publish();

	mw->show();
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
	return app.exec();
*/

	net.Start();
	for (int i=0; i<400; i++)
	{
		net.DoProcessings();
	}
	net.Stop();
	std::cout << "stop and start" << std::endl;
	net.Start();
	for (int i=0; i<400; i++)
	{
		net.DoProcessings();
	}
	net.Stop();
	return 0;


}

