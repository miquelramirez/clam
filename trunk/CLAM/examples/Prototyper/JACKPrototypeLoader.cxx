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

#include "BasicFlowControl.hxx"

#include "JACKNetworkPlayer.hxx"


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
	std::string mNetworkFile;
	QWidget * mMainWidget;
	JACKNetworkPlayer mPlayer;
	std::list<VM::NetPlot * > mPortMonitors;
public:
	PrototypeLoader(const std::string& networkFile, const std::list<std::string> portlist)
		: mNetworkFile(networkFile)
		, mPlayer(networkFile, portlist)
	{
		
	}
	QWidget * loadPrototype(const char* uiFile)
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

int main( int argc, char *argv[] )
{
	if ( argc<2 || argc>5 )
	{
		std::cout << " Usage: " 
			<< argv[0] << " <networkfile> [ <uifile> inputPort outputPort ]" << std::endl;
		return -1;
	}

	std::string networkFile, uiFile;
	
	QApplication app( argc, argv );

	std::list<std::string> portlist;

	//Deal with Network&Interface files
	if ( argc==2 )
	{
		networkFile=argv[1];
		std::string filename = networkFile;
		filename.erase( filename.size()-4, 4 );
		filename+=std::string(".ui");
		uiFile=filename;
	}
	else if ( argc>2 )
	{
		networkFile=argv[1];
		uiFile=argv[2];
	}

	//Deal with input and output port patterns
	if ( argc==4 ) //Case 1: only input
	{		
		portlist.push_back( std::string( argv[3]) );
		portlist.push_back( "NULL");
	}
	else if ( argc==5 ) //Case 2: input & output
	{
		portlist.push_back( argv[3] );
		portlist.push_back( std::string(argv[4]) );
	}
	else //Case 3: nothing specified
	{
		portlist.push_back( "NULL" );
		portlist.push_back( "NULL" );
	}

	CLAM::PrototypeLoader loader( networkFile , portlist);

	QWidget * prototype = loader.loadPrototype( uiFile.c_str() );
	if ( !prototype ) return -1;
	loader.connectWithNetwork();

	// Set up the dynamic dialog here
	app.setMainWidget( prototype );

	prototype->show();


	loader.Start();
	int result = app.exec();
	loader.Stop();

	return result;
}
