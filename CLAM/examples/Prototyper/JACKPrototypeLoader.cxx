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

#include "PortMonitor.hxx"

/////////////Temporary includes
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"

#include "BasicFlowControl.hxx"

////////JACK CODE
#include <jack/jack.h>

int jack_process (jack_nframes_t nframes, void *arg);
void jack_shutdown (void *arg);

namespace CLAM
{
	
class ExternalPortConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE( ExternalPortConfig, 3, ProcessingConfig);	
	DYN_ATTRIBUTE( 0, public, std::string, Name);
	DYN_ATTRIBUTE( 1, public, int, ExternalBufferSize);
	DYN_ATTRIBUTE( 2, public, int, ExternalSamplingRate);
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("Unnamed External Port");
		SetExternalBufferSize(1024);
		SetExternalSamplingRate(44100);
	}
};
	
class ExternalOutPort : public Processing
{
public:
	ExternalOutPort()
		: mOut("AudioOut", this)
	{
	}
	
	ExternalOutPort(const ExternalPortConfig &conf)
		: mOut("AudioOut", this)
	{
		Configure(conf);
	}
	
	void CopyExternalBuffer( TData* buff, int nframes )
	{
		if (!mOut.CanProduce())
		{
	//		std::cout << "_outport cant produce" << std::endl;
			return;
		}

		CLAM::Audio& so=mOut.GetAudio();
		//JACK's datatype for audio is the same as ours, range (-1,1) in a float
		std::copy(buff, buff+nframes, so.GetBuffer().GetPtr());
		mOut.Produce();
	}
	bool Do() {return true;}
	const ProcessingConfig& GetConfig() const {return mConf;}
	const char* GetClassName() const { return "ExternalOutPort";}
	bool ConcreteConfigure(const ProcessingConfig& conf)
	{
		CopyAsConcreteConfig(mConf,conf);

		mOut.SetSize( mConf.GetExternalBufferSize() );
		mOut.SetHop( mConf.GetExternalBufferSize() );
		mOut.SetSampleRate( mConf.GetExternalSamplingRate() );
		
		return true;
	}
private:
	ExternalPortConfig mConf;
	AudioOutPort mOut;		
};

class ExternalInPort : public Processing
{
public:
	ExternalInPort()
		: mIn("AudioIn", this)
	{
	}
	
	ExternalInPort(const ExternalPortConfig &conf)
		: mIn("AudioIn", this)
	{
		Configure(conf);
	}
	
	void CopyToExternalBuffer( TData* buff, int nframes )
	{
		if (!mIn.CanConsume())
		{
	//		std::cout << "_outport cant produce" << std::endl;
			return;
		}

		const CLAM::Audio& so=mIn.GetAudio();
		//JACK's datatype for audio is the same as ours, range (-1,1) in a float
		std::copy(so.GetBuffer().GetPtr(), so.GetBuffer().GetPtr()+nframes, buff);
		mIn.Consume();
	}
	bool Do() {return true;}
	const ProcessingConfig& GetConfig() const {return mConf;}
	const char* GetClassName() const { return "ExternalInPort";}
	bool ConcreteConfigure(const ProcessingConfig& conf)
	{
		CopyAsConcreteConfig(mConf,conf);

		mIn.SetSize( mConf.GetExternalBufferSize() );
		mIn.SetHop( mConf.GetExternalBufferSize() );
		mIn.SetSampleRate( mConf.GetExternalSamplingRate() );
		
		return true;
	}
private:
	ExternalPortConfig mConf;
	AudioInPort mIn;		
};
}

class JACKNetworkPlayer
{
	CLAM::Network _network;


	CLAM::ExternalInPort* _networkDestination;
	CLAM::ExternalOutPort* _networkSource;
	int _jsamplerate, _jbuffersize;

	//JACK CODE
	jack_port_t *input_port;
	jack_port_t *output_port;
	jack_client_t *client;

public:
	JACKNetworkPlayer(const std::string & networkFile)
	{
		CLAM::PushFlowControl * fc = new CLAM::PushFlowControl(512);
		_network.AddFlowControl( fc );

		CLAM::XmlStorage::Restore(_network,networkFile);
		
		//CONNECT PORTS
		//CLAM::Processing & entrada=_network.GetProcessing("SMSAnalysisCore");
		//CLAM::Processing & entrada=_network.GetProcessing("AudioAdder");
		//CLAM::Processing & sortida=_network.GetProcessing("SMSSynthesis");
		//CLAM::Processing & sortida=_network.GetProcessing("AudioAdder");

		_networkSource=new CLAM::ExternalOutPort();
		_networkDestination=new CLAM::ExternalInPort();

		//TODO  remove from Network 
		//fc->ForceGenerator( &entrada );

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

		//EP: REVISA METODE, A LA DOC HI SURT PERO AL src NO
		//ConnectPorts( _outport, entrada, "Input Audio");

		CLAM::ExternalPortConfig conf;
		conf.SetName("CLAM_Prototype_Input");
		conf.SetExternalBufferSize(_jbuffersize);
		conf.SetExternalSamplingRate(_jsamplerate);
		
		_networkSource->Configure(conf);

		conf.SetName("CLAM_Prototype_Output");
		_networkDestination->Configure(conf);
		
		_network.AddProcessing("ExternalOutPort",_networkSource);
		_network.ConnectPorts("ExternalOutPort.AudioOut","SMSAnalysisCore.Input Audio");
		_network.AddProcessing("ExternalInPort",_networkDestination);
		_network.ConnectPorts("SMSSynthesis.OutputAudio","ExternalInPort.AudioIn");
		
		ManageInputPorts();
		ManageOutputPorts();
	}
	
	void ManageInputPorts(void)
	{
		//Get them from the Network
		//Connect them to local outports
		//And then register into the JACK server

		//Register the port
		input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	}
	
	void ManageOutputPorts(void)
	{
		//Get them from the Network
		//Connect them to local inports
		//Then register these into the JACK server

		//Register the port
		output_port = jack_port_register (client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	}
	
	void Do(const jack_nframes_t nframes)
	{
		jack_default_audio_sample_t *jackInBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer (input_port, nframes);
		
		_networkSource->CopyExternalBuffer( (CLAM::TData*)jackInBuffer, nframes );

		_network.DoProcessingsLoop();
		_network.DoProcessingsLoop();

		jack_default_audio_sample_t *jackOutBuffer = 
			(jack_default_audio_sample_t *) jack_port_get_buffer (output_port, nframes);

		_networkDestination->CopyToExternalBuffer(( CLAM::TData*)jackOutBuffer, nframes);
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
