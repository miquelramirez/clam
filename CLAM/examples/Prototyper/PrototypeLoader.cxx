#include <qapplication.h>
#include <qwidgetfactory.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <iostream>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"
#include "QtSlot2Control.hxx"
#include "NetAudioPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetFundPlot.hxx"
#include "AudioPlotProcessing.hxx"
#include "PeaksPlotProcessing.hxx"
#include "SpectrumPlotProcessing.hxx"
#include "FundPlotProcessing.hxx"


class NetworkPlayer
{
	CLAM::AudioManager _audioManager;
	CLAM::Network _network;
	CLAM::Thread _thread;
	bool _stoped;
public:
	NetworkPlayer(const std::string & networkFile)
		: _audioManager( 44100, 512 )
		, _thread(/*realtime*/true)
		, _stoped(true)

	{
		_network.AddFlowControl( new CLAM::PushFlowControl( /*frameSize*/ 512 ));
		CLAM::XmlStorage::Restore(_network,networkFile);
		_thread.SetThreadCode( makeMemberFunctor0( *this, NetworkPlayer, Do ) );
	}
	void Start()
	{
		_stoped=false;
		_network.Start();
		_thread.Start();
	}
	void Do()
	{
		while (!_stoped)
		{
			_network.DoProcessings();
		}
	}
	void Stop()
	{
		_stoped=true;
		_thread.Stop();
		_network.Stop();
	}
	~NetworkPlayer()
	{
		Stop();
	}
	CLAM::Network & Network()
	{
		return _network;
	}
};

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
	NetworkPlayer _player;
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

		connectWidgetsWithPorts<CLAM::VM::NetAudioPlot, CLAM::AudioPlotProcessing>
			("OutPort__.*", "CLAM::VM::NetAudioPlot", "Audio Input");
		connectWidgetsWithPorts<CLAM::VM::NetSpectrumPlot, CLAM::SpectrumPlotProcessing>
			("OutPort__.*", "CLAM::VM::NetSpectrumPlot", "Spectrum Input");
		connectWidgetsWithPorts<CLAM::VM::NetPeaksPlot, CLAM::PeaksPlotProcessing>
			("OutPort__.*", "CLAM::VM::NetPeaksPlot", "Peaks Input");
		connectWidgetsWithPorts<CLAM::VM::NetFundPlot, CLAM::FundPlotProcessing>
			("OutPort__.*", "CLAM::VM::NetFundPlot", "Fundamental Input");
	}
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

			std::cout << "Control: " << controlName << std::endl;

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
			std::cout << "0:1 Mapped Control: " << controlName << std::endl;

			CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
			QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
			notifier->linkControl(receiver);
			notifier->connect(aWidget,SIGNAL(valueChanged(int)),
					  SLOT(sendMappedControl(int)));
		}
	}
	template < typename PlotClass, typename MonitorClass >
	void connectWidgetsWithPorts(char* prefix, char* plotClassName, char* monitorControlName)
	{
		CLAM::Network & network = _player.Network();
		QWidget * prototype = _mainWidget;
		QObjectList * widgets = prototype->queryList(plotClassName,prefix);
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 9);
			std::cout << plotClassName << ": " << controlName << std::endl;

			MonitorClass * monitor = new MonitorClass;
			std::string monitorName = "PrototyperMonitor"+getMonitorNumber();
			network.AddProcessing(monitorName,monitor);
			network.ConnectPorts(controlName,monitorName+"."+monitorControlName);
			PlotClass * plot = (PlotClass*) aWidget;
			monitor->SetPlot(plot);
			plot->Show();
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



