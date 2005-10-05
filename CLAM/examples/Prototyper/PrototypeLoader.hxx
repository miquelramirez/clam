#ifndef _PROTOTYPE_LOADER_HXX_
#define _PROTOTYPE_LOADER_HXX_

#include <qapplication.h>
#include <qwidgetfactory.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <iostream>
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "QtSlot2Control.hxx"
#include "NetAudioPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetFundPlot.hxx"
#include "NetAudioBuffPlot.hxx"
#include "NetSpecgramPlot.hxx"
#include "NetFundTrackPlot.hxx"
#include "NetSinTracksPlot.hxx"

#include "NetworkPlayer.hxx"
#include "PortMonitor.hxx"

#include "AudioManager.hxx"
#include "Network.hxx"

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
	NetworkPlayer * mPlayer;
	std::list<CLAM::VM::NetPlot * > mPortMonitors;
public:
	PrototypeLoader(char * networkFile)
		: mNetworkFile(networkFile)
	{
		mPlayer=NULL;	
	}

	~PrototypeLoader()
	{
		delete mPlayer;
		
		delete mMainWidget;

		// mPortMonitors isn't used yet, but nevermind
		for (std::list<CLAM::VM::NetPlot*>::iterator it=mPortMonitors.begin(); it!=mPortMonitors.end(); it++)
			delete *it;
	}

	void SetNetworkPlayer( NetworkPlayer& player)
	{
		mPlayer=&player;
	}
	
	QWidget * loadPrototype(char* uiFile)
	{
		mMainWidget = (QWidget *) QWidgetFactory::create( uiFile );
		return mMainWidget;
	}
	void connectWithNetwork()
	{
		CLAM_ASSERT( mPlayer!=NULL , "PrototypeLoader::ConnectWithNetwork() : no NetworkPlayer assigned. Do it with SetNetworkPlayer(..)");
		
		CLAM::Network & network = mPlayer->GetNetwork();
		QWidget * prototype = mMainWidget;
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
		//connectWidgetsWithPorts<CLAM::VM::NetSinTracksPlot>
		//	("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
	}
public slots:
	void Start()
	{
		CLAM_ASSERT( mPlayer!=NULL , "PrototypeLoader::ConnectWithNetwork() : no NetworkPlayer assigned. Do it with SetNetworkPlayer(..)");
		mPlayer->Start();
	}
	void Stop()
	{
		mPlayer->Stop();
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
};

} //end namespace CLAM

#endif
