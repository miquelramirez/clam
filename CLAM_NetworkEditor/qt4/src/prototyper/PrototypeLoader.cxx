#include "PrototypeLoader.hxx"

namespace CLAM
{
	
PrototypeLoader::PrototypeLoader(std::string networkFile)
	: mNetworkFile(networkFile)
{
	mPlayer=NULL;	
}

PrototypeLoader::~PrototypeLoader()
{
	delete mPlayer;
	
	delete mMainWidget;

	// mPortMonitors isn't used yet, but nevermind
	for (std::list<CLAM::VM::NetPlot*>::iterator it=mPortMonitors.begin(); it!=mPortMonitors.end(); it++)
		delete *it;
}

void PrototypeLoader::SetNetworkPlayer( NetworkPlayer& player)
{
	mPlayer=&player;
}

QWidget * PrototypeLoader::LoadPrototype(std::string uiFile)
{
	mMainWidget = (QWidget *) QWidgetFactory::create( uiFile.c_str() );
	return mMainWidget;
}

void PrototypeLoader::ConnectWithNetwork()
{
	CLAM_ASSERT( mPlayer!=NULL , "PrototypeLoader::ConnectWithNetwork() : no NetworkPlayer assigned. Do it with SetNetworkPlayer(..)");
	
	CLAM::Network & network = mPlayer->GetNetwork();
	QWidget * prototype = mMainWidget;
	ConnectWidgetsWithControls(network,prototype);
	ConnectWidgetsWithMappedControls(network,prototype);

	ConnectWidgetsWithPorts<CLAM::VM::NetAudioPlot>
		("OutPort__.*", "CLAM::VM::NetAudioPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetSpectrumPlot>
		("OutPort__.*", "CLAM::VM::NetSpectrumPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetPeaksPlot>
		("OutPort__.*", "CLAM::VM::NetPeaksPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetFundPlot>
		("OutPort__.*", "CLAM::VM::NetFundPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetAudioBuffPlot>
		("OutPort__.*", "CLAM::VM::NetAudioBuffPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetSpecgramPlot>
		("OutPort__.*", "CLAM::VM::NetSpecgramPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetFundTrackPlot>
		("OutPort__.*", "CLAM::VM::NetFundTrackPlot");
	// TODO: Still not ported
	//ConnectWidgetsWithPorts<CLAM::VM::NetSinTracksPlot>
	//	("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
	QObject * playButton = mMainWidget->child("PlayButton");
	connect(playButton, SIGNAL(clicked()), this, SLOT(Start()));  
	QObject * stopButton = mMainWidget->child("StopButton");
	connect(stopButton, SIGNAL(clicked()), this, SLOT(Stop()));  
}

void PrototypeLoader::Start()
{
	CLAM_ASSERT( mPlayer!=NULL , "PrototypeLoader::ConnectWithNetwork() : no NetworkPlayer assigned. Do it with SetNetworkPlayer(..)");
	std::cout << "starting" << std::endl;
	mPlayer->Start();
	std::cout << "started" << std::endl;
}

void PrototypeLoader::Stop()
{
	mPlayer->Stop();
}

void PrototypeLoader::Substitute(std::string & subject, const char * pattern, const char * substitution)
{
	for (std::string::size_type position = subject.find(pattern, 0);
		position!=std::string::npos;
		position = subject.find(pattern, position))
	{
		subject.replace(position, strlen(pattern), substitution);
	}
}

std::string PrototypeLoader::GetNetworkNameFromWidgetName(const char * widgetName)
{
	std::string networkName(widgetName);
	Substitute(networkName,"___", " ");
	Substitute(networkName,"__", ".");
	return networkName;
}

void PrototypeLoader::ConnectWidgetsWithControls(CLAM::Network & network, QWidget * prototype)
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

void PrototypeLoader::ConnectWidgetsWithMappedControls(CLAM::Network & network, QWidget * prototype)
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
void PrototypeLoader::ConnectWidgetsWithPorts(char* prefix, char* plotClassName)
{
	if (!QWidgetFactory::supportsWidget(plotClassName))
		qWarning(tr("No support for widgets %1. Maybe the CLAM qt plugins has not been loaded").arg(plotClassName));
	CLAM::Network & network = mPlayer->GetNetwork();
	QWidget * prototype = mMainWidget;
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
	
} //end namespace CLAM
