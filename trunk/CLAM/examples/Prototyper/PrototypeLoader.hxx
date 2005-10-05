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

#include <iostream>

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
	NetworkPlayer * mPlayer;
	std::list<CLAM::VM::NetPlot * > mPortMonitors;
public:
	PrototypeLoader(std::string networkFile);

	~PrototypeLoader();

	void SetNetworkPlayer( NetworkPlayer& player);
	
	QWidget * LoadPrototype(std::string uiFile);
	
	void ConnectWithNetwork();

public slots:
	void Start();
	
	void Stop();

private:
	void Substitute(std::string & subject, const char * pattern, const char * substitution);
	
	std::string GetNetworkNameFromWidgetName(const char * widgetName);

	void ConnectWidgetsWithControls(CLAM::Network & network, QWidget * prototype);

	void ConnectWidgetsWithMappedControls(CLAM::Network & network, QWidget * prototype);
	
	template < typename PlotClass >
	void ConnectWidgetsWithPorts(char* prefix, char* plotClassName);
};

} //end namespace CLAM

#endif
