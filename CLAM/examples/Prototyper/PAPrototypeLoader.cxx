#include <qapplication.h>
#include <qwidgetfactory.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <iostream>
#include <string>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "QtSlot2Control.hxx"
#include "NetAudioPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetFundPlot.hxx"
#include "NetAudioBuffPlot.hxx"
#include "NetSpecgramPlot.hxx"
#include "NetFundTrackPlot.hxx"
#include "NetSinTracksPlot.hxx"
#include "Text.hxx"
#include "List.hxx"

#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include "PortMonitor.hxx"

/////////////Temporary includes
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"

#include "BasicFlowControl.hxx"

#include <portaudio.h>

int portaudio_process (const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);
void portaudio_shutdown (void *arg);

typedef std::vector<CLAM::ExternGenerator*> PAOutPortList;
typedef std::vector<CLAM::ExternSink*> PAInPortList;

class PANetworkPlayer
{
	CLAM::Network _network;	

	int _cbuffersize, _cframerate;

	PAOutPortList _receiverlist;
	PAInPortList _senderlist;
	
	//PA CODE : declare client (stream)
	PaStream * pa_stream;

public:
	PANetworkPlayer(const std::string & networkFile)
	{
		_cbuffersize=512;
		_cframerate=44100;
		
		CLAM::PushFlowControl * fc = new CLAM::PushFlowControl(_cbuffersize);
		_network.AddFlowControl( fc );

		CLAM::XmlStorage::Restore(_network,networkFile);
		
		//PA CODE
		//init client
		ControlIfPortAudioError( Pa_Initialize() );
		
		//registra callback
		
		//registra funcio shutdown
		
		CreatePorts(_network);
	}
	
	void CreatePorts(const CLAM::Network& net)
	{
		bool receiverwarning, senderwarning;
		receiverwarning=senderwarning=false;
		
		//Get them from the Network and add it to local list		
		for (CLAM::Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
		{
			if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
			{
				//Using PortAudio we only accept 2 channels max
				if (_receiverlist.size()==2)
				{
					if (!receiverwarning)
					{
						std::cout <<"WARNING: more than two ExternGenerators detected, using the first ones"<<std::endl;
						receiverwarning=true;
					}
					continue;
				}
				
				//Get Processing address
				_receiverlist.push_back( (CLAM::ExternGenerator*)it->second );
			}
			else if (std::string("ExternSink")==std::string(it->second->GetClassName()))
			{
				//Using PortAudio we only accept 2 channels max
				if (_receiverlist.size()==2)
				{
					if (!senderwarning)
					{
						std::cout <<"WARNING: more than two ExternSinks detected, using the first ones"<<std::endl;
						senderwarning=true;
					}
	
					continue;
				}

				//Get Processing address
				_senderlist.push_back( (CLAM::ExternSink*)it->second );
			}
		}

		//Create configuration for input&output and then register the stream
		PaStreamParameters inputParameters, outputParameters, *inParams, *outParams;

		inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
		inputParameters.channelCount = 2;       /* stereo output */
		inputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples*/
		inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowOutputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = paFloat32 | paNonInterleaved ; /* 32 bit floating point output, having non-interleaved samples */
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		if (_receiverlist.size()==0) inParams=NULL;
		if (_senderlist.size()==0) outParams=NULL;
		
		ControlIfPortAudioError(
			Pa_OpenStream(
				&pa_stream,
				inParams,
				outParams,
				double(_cframerate),
				_cbuffersize,			
				paClipOff,      /* we won't output out of range samples so don't bother clipping them */
				portaudio_process,
				this )
			);

	}

	void DoInPorts(CLAM::TData** input, unsigned long nframes)
	{
		for (PAOutPortList::iterator it=_receiverlist.begin(); it!=_receiverlist.end(); it++)
		{
			//Retrieve PA buffer location
			//Tell the ExternGenerator to put PA's buffer info into CLAM
	
		}

	}
	
	void DoOutPorts(CLAM::TData** output, unsigned long nframes)
	{
		for (PAInPortList::iterator it=_senderlist.begin(); it!=_senderlist.end(); it++)
		{
			//Retrieve PA buffer location
			//Tell the ExternGenerator to put CLAM's buffer info PA
		}

	}

	void Do(const void *inputBuffers, void *outputBuffers,
                            unsigned long framesPerBuffer)
	{
		DoInPorts( (CLAM::TData**) inputBuffers, framesPerBuffer);
		
		//for (int stepcount=0; stepcount < (int(nframes)/int(_cbuffersize)); stepcount++)
		_network.Do();

		DoOutPorts( (CLAM::TData**) outputBuffers, framesPerBuffer);
	}
	
	void Start()
	{
		_network.Start();

		//PA CODE (the init order of network, ... should be decided) : activate
		Pa_StartStream( pa_stream );
	}
	
	void Stop()
	{
		//PA CODE (the init order of network, ... should be decided) : deactivate
		Pa_StopStream( pa_stream );
		
		_network.Stop();
	}

	inline void ControlIfPortAudioError(int result)
	{
		//If everything ok, return now
		if( result == paNoError ) return;

		//If there has been an error, inform and quit!
		std::cerr <<"PortAudio Error #"<<result<<": "<< Pa_GetErrorText( result )<<std::endl;
		exit(result);
	}
	
	~PANetworkPlayer()
	{
		Stop();

		//Close stream and terminate
		ControlIfPortAudioError( Pa_CloseStream( pa_stream ) );
		Pa_Terminate();
	}
	
	CLAM::Network & Network()
	{
		return _network;
	}
};

//PA CODE
int portaudio_process (const void *inputBuffers, void *outputBuffers,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
	PANetworkPlayer* player=(PANetworkPlayer*)userData;
	player->Do(inputBuffers, outputBuffers, framesPerBuffer);

	std::cout <<"CALLBACK\n";
	
	return 0;
}

static std::string getMonitorNumber()
{
	static unsigned number = 0;
	std::stringstream os;
	os << number++;
	return os.str();
}


class PrototypeLoader
{
	char * _networkFile;
	char * _interfaceFile;
	QWidget * _mainWidget;
	PANetworkPlayer _player;
	std::list<CLAM::VM::NetPlot * > _portMonitors;
public:
	PrototypeLoader(char * networkFile)
		: _networkFile(networkFile)
		, _player(networkFile)
	{
		
	}
	QWidget * loadPrototype(char* uiFile)
	{
		_mainWidget = (QWidget *) QWidgetFactory::create( uiFile );
		return _mainWidget;
	}
	void connectWithNetwork()
	{
		CLAM::Network & network = _player.Network();
		QWidget * prototype = _mainWidget;
		connectWidgetsWithControls(network,prototype);
		connectWidgetsWithMappedControls(network,prototype);

		connectWidgetsWithPorts<CLAM::VM::NetAudioPlot>
			("OutPort__.*", "CLAM::VM::NetAudioPlot");
		connectWidgetsWithPorts<CLAM::VM::NetSpectrumPlot>
			("OutPort__.*", "CLAM::VM::NetSpectrumPlot");
		connectWidgetsWithPorts<CLAM::VM::NetPeaksPlot>
			("OutPort__.*", "CLAM::VM::NetPeaksPlot");
		connectWidgetsWithPorts<CLAM::VM::NetFundPlot>
			("OutPort__.*", "CLAM::VM::NetFundPlot");
		connectWidgetsWithPorts<CLAM::VM::NetAudioBuffPlot>
			("OutPort__.*", "CLAM::VM::NetAudioBuffPlot");
		connectWidgetsWithPorts<CLAM::VM::NetSpecgramPlot>
			("OutPort__.*", "CLAM::VM::NetSpecgramPlot");
		connectWidgetsWithPorts<CLAM::VM::NetFundTrackPlot>
			("OutPort__.*", "CLAM::VM::NetFundTrackPlot");
		// TODO: Still not ported
		//		connectWidgetsWithPorts<CLAM::VM::NetSinTracksPlot>
		//						("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
	}
	
public slots:
	void Start()
	{
		_player.Start();
	}

private:
	void substitute(std::string & subject, const char * pattern, const char * substitution)
	{
		for (std::string::size_type position = subject.find(pattern, 0);
			position!=std::string::npos;
			position = subject.find(pattern, position))
		{
			subject.replace(position, strlen(pattern), substitution);
		}
	}
	
	std::string GetNetworkNameFromWidgetName(const char * widgetName)
	{
		std::string networkName(widgetName);
		substitute(networkName,"___", " ");
		substitute(networkName,"__", ".");
		return networkName;
	}

	void connectWidgetsWithControls(CLAM::Network & network, QWidget * prototype)
	{
		QObjectList * widgets = prototype->queryList(0,"InControl__.*");
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 11);

			std::cout << "* Control: " << controlName << std::endl;

			CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
			QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
			notifier->linkControl(receiver);
			notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				SLOT(sendControl(int)));
		}
	}

	void connectWidgetsWithMappedControls(CLAM::Network & network, QWidget * prototype)
	{
		QObjectList * widgets = prototype->queryList(0,"InControlFloat__.*");
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 16);
			std::cout << "* Mapped Control (100:1): " << controlName << std::endl;

			CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
			QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
			notifier->linkControl(receiver);
			notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				SLOT(sendMappedControl(int)));
		}
	}
	
	template < typename PlotClass >
	void connectWidgetsWithPorts(char* prefix, char* plotClassName)
	{
		CLAM::Network & network = _player.Network();
		QWidget * prototype = _mainWidget;
		QObjectList * widgets = prototype->queryList(plotClassName,prefix);
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 9);
			std::cout << "* " << plotClassName << ": " << controlName << std::endl;
	
			typedef typename PlotClass::MonitorType MonitorType;
			MonitorType * portMonitor = new MonitorType;
	
			std::string monitorName = "PrototyperMonitor"+getMonitorNumber();
			network.AddProcessing(monitorName,portMonitor);
	
			network.ConnectPorts(controlName,monitorName+".Input");
			PlotClass * plot = (PlotClass*) aWidget;
			plot->SetMonitor(*portMonitor);
		}
	}
};

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

	PrototypeLoader loader(networkFile);


	QWidget * prototype = loader.loadPrototype(uiFile);
	if (!prototype) return -1;
	loader.connectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


	loader.Start();
	int result = app.exec();

	return result;
}
