#ifndef _PROTOTYPE_LOADER_HXX_
#define _PROTOTYPE_LOADER_HXX_

#include <QtGui/QWidget>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/PortMonitor.hxx>

#include <CLAM/AudioManager.hxx>
#include <CLAM/Network.hxx>

#include <iostream>

namespace CLAM
{

class PrototypeLoader : public QWidget
{
	Q_OBJECT
private:
	std::string mNetworkFile;
	NetworkPlayer * mPlayer;
	Network mNetwork;
//	std::list<CLAM::VM::NetPlot * > mPortMonitors; //QT4PORT
public:
	PrototypeLoader(std::string networkFile);

	~PrototypeLoader();

	void SetNetworkPlayer( NetworkPlayer& player);
	
	bool LoadPrototype(const QString & uiFile);
	
	void ConnectWithNetwork();

public slots:
	void Start();
	
	void Stop();

private:
	void Substitute(std::string & subject, const char * pattern, const char * substitution);
	
	std::string GetNetworkNameFromWidgetName(const char * widgetName);

	void ConnectWidgetsWithControls();

	void ConnectWidgetsWithMappedControls();
	
	template < typename PlotClass >
	void ConnectWidgetsWithPorts(char* prefix, char* plotClassName);
};

} //end namespace CLAM

#endif
