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

////////JACK CODE
#include <jack/jack.h>

int jack_process (jack_nframes_t nframes, void *arg);
void jack_shutdown (void *arg);

typedef struct
{
	jack_port_t* jackOutPort;
	CLAM::ExternGenerator* clamReceiver;
} JACKOutPortCouple;

typedef struct
{
	jack_port_t* jackInPort;
	CLAM::ExternSink* clamSender;
} JACKInPortCouple;

typedef std::vector<JACKOutPortCouple> JACKOutPortList;
typedef std::vector<JACKInPortCouple> JACKInPortList;

class JACKNetworkPlayer
{
	CLAM::Network _network;	

	int _jsamplerate, _jbuffersize, _cbuffersize;

	JACKOutPortList _receiverlist;
	JACKInPortList _senderlist;
	
	//JACK CODE
	jack_client_t *client;

public:
	JACKNetworkPlayer(const std::string & networkFile)
	{
		_cbuffersize=512;
		
		CLAM::PushFlowControl * fc = new CLAM::PushFlowControl(_cbuffersize);
		_network.AddFlowControl( fc );

		CLAM::XmlStorage::Restore(_network,networkFile);
		
		//JACK CODE
		//init client
		if ((client = jack_client_new ("CLAM client")) == 0)
		{
			fprintf (stderr, "JACK ERROR: server not running?\n");
			exit(1);
		}
		
		//registra callback
		jack_set_process_callback (client, jack_process, this);
		
		//registra funcio shutdown
		jack_on_shutdown (client, jack_shutdown, this);
		
		_jsamplerate=(int)jack_get_sample_rate (client);
		_jbuffersize=(int)jack_get_buffer_size (client);

		CreateInputPorts(_network);
		CreateOutputPorts(_network);
	}
	
	void CreateInputPorts(const CLAM::Network& net)
	{
		JACKOutPortCouple pair;
		
		//Get them from the Network and add it to local list		
		for (CLAM::Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
		{
			if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
			{
				//Get Processing address
				pair.clamReceiver=(CLAM::ExternGenerator*)it->second;
				pair.clamReceiver->SetFrameAndHopSize(_jbuffersize);

				//Register port on the JACK server
				pair.jackOutPort=jack_port_register (client,
					it->first.c_str(),
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

				//Add the pair (jack port, clam jack receiver) to the list
				_receiverlist.push_back(pair);
			}
		}
	}
	
	void CreateOutputPorts(const CLAM::Network& net)
	{
		JACKInPortCouple pair;
		
		//Get them from the Network and add it to local list		
		for (CLAM::Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
		{
			if (std::string("ExternSink")==std::string(it->second->GetClassName()))
			{
				//Get Processing address
				pair.clamSender=(CLAM::ExternSink*)it->second;
				pair.clamSender->SetFrameAndHopSize(_jbuffersize);

				//Register port on the JACK server
				pair.jackInPort=jack_port_register (client,
					it->first.c_str(),
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

				//Add the pair (jack port, clam jack receiver) to the list
				_senderlist.push_back(pair);
			}
		}

	}

	void DoInPorts(const jack_nframes_t nframes)
	{
		for (JACKOutPortList::iterator it=_receiverlist.begin(); it!=_receiverlist.end(); it++)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackInBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackOutPort, nframes);
			//Tell the ExternGenerator to put JACK's buffer info into CLAM
			(*it).clamReceiver->Do( (CLAM::TData*)jackInBuffer, nframes );
	
		}

	}
	
	void DoOutPorts(const jack_nframes_t nframes)
	{
		for (JACKInPortList::iterator it=_senderlist.begin(); it!=_senderlist.end(); it++)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackOutBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackInPort, nframes);
			//Tell the ExternGenerator to put CLAM's buffer info JACK
			(*it).clamSender->Do( (CLAM::TData*)jackOutBuffer, nframes);	
		}

	}

	void Do(const jack_nframes_t nframes)
	{
		DoInPorts(nframes);
		
		for (int stepcount=0; stepcount < (int(nframes)/int(_cbuffersize)); stepcount++)
			_network.Do();

		DoOutPorts(nframes);
	}
	
	void Start()
	{
		_network.Start();

		//JACK CODE (the init order of network, ... should be decided)
		if (jack_activate (client)) {
			fprintf (stderr, "JACK ERROR: cannot activate client");
			exit(1);
		}
	}
	
	void Stop()
	{
		//JACK CODE (the init order of network, ... should be decided)
		if (jack_deactivate (client))
		{
			fprintf (stderr, "cannot activate client");
			exit(1);
		}

		_network.Stop();
	}
	
	~JACKNetworkPlayer()
	{
		Stop();
		
		//JACK CODE
		jack_client_close (client);
	}
	
	CLAM::Network & Network()
	{
		return _network;
	}
};

//JACK CODE
int jack_process (jack_nframes_t nframes, void *arg)
{
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	player->Do(nframes);

	return 0;
}

void jack_shutdown (void *arg)
{
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	delete player;
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
	JACKNetworkPlayer _player;
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
