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

#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include "PortMonitor.hxx"

#include "BasicFlowControl.hxx"

#include "PANetworkPlayer.hxx"
#include <portaudio.h>

namespace CLAM
{

static std::string getMonitorNumber()
{
	static unsigned number = 0;
	std::stringstream os;
	os << number++;
	return os.str();
}


class PrototypeLoader
{
	char * mNetworkFile;
	QWidget * mMainWidget;
	PANetworkPlayer mPlayer;
	std::list<VM::NetPlot * > mPortMonitors;
public:
	PrototypeLoader(char * networkFile)
		: mNetworkFile(networkFile)
		, mPlayer(networkFile)
	{
		
	}
	QWidget * loadPrototype(char* uiFile)
	{
		mMainWidget = (QWidget *) QWidgetFactory::create( uiFile );
		return mMainWidget;
	}
	void connectWithNetwork()
	{
		Network & network = mPlayer.GetNetwork();
		QWidget * prototype = mMainWidget;
		connectWidgetsWithControls(network,prototype);
		connectWidgetsWithMappedControls(network,prototype);

		connectWidgetsWithPorts<VM::NetAudioPlot>
			("OutPort__.*", "CLAM::VM::NetAudioPlot");
		connectWidgetsWithPorts<VM::NetSpectrumPlot>
			("OutPort__.*", "CLAM::VM::NetSpectrumPlot");
		connectWidgetsWithPorts<VM::NetPeaksPlot>
			("OutPort__.*", "CLAM::VM::NetPeaksPlot");
		connectWidgetsWithPorts<VM::NetFundPlot>
			("OutPort__.*", "CLAM::VM::NetFundPlot");
		connectWidgetsWithPorts<VM::NetAudioBuffPlot>
			("OutPort__.*", "CLAM::VM::NetAudioBuffPlot");
		connectWidgetsWithPorts<VM::NetSpecgramPlot>
			("OutPort__.*", "CLAM::VM::NetSpecgramPlot");
		connectWidgetsWithPorts<VM::NetFundTrackPlot>
			("OutPort__.*", "CLAM::VM::NetFundTrackPlot");
		// TODO: Still not ported
		//		connectWidgetsWithPorts<VM::NetSinTracksPlot>
		//						("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
	}
	
public slots:
	void Start()
	{
		mPlayer.Start();
	}
	void Stop()
	{
		mPlayer.Stop();
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

	void connectWidgetsWithControls(Network & network, QWidget * prototype)
	{
		QObjectList * widgets = prototype->queryList(0,"InControl__.*");
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 11);

			std::cout << "* Control: " << controlName << std::endl;

			InControl & receiver = network.GetInControlByCompleteName(controlName);
			QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
			notifier->linkControl(receiver);
			notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				SLOT(sendControl(int)));
		}
	}

	void connectWidgetsWithMappedControls(Network & network, QWidget * prototype)
	{
		QObjectList * widgets = prototype->queryList(0,"InControlFloat__.*");
		for (QObjectListIt it(*widgets); it.current(); ++it)
		{
			QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
			std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 16);
			std::cout << "* Mapped Control (100:1): " << controlName << std::endl;

			InControl & receiver = network.GetInControlByCompleteName(controlName);
			QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
			notifier->linkControl(receiver);
			notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				SLOT(sendMappedControl(int)));
		}
	}
	
	template < typename PlotClass >
	void connectWidgetsWithPorts(char* prefix, char* plotClassName)
	{
		Network & network = mPlayer.GetNetwork();
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
};

} //end namespace CLAM

#include <fstream>
#include <iostream>

using std::string;

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

	std::ifstream file( argv[1] );

	if( !file ) {
		std::cerr << "ERROR: opening file <" << argv[1] << ">" << std::endl;
		return -1;
	}
	file.close();
	file.open( argv[2] );
	
	if( !file ) {
		std::cerr << "Error opening file <" << argv[2] << ">" << std::endl;
		return -1;
	}
	file.close();
	
	QApplication app( argc, argv );

	CLAM::PrototypeLoader loader(networkFile);

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
