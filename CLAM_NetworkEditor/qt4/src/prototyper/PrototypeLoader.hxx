#ifndef _PROTOTYPE_LOADER_HXX_
#define _PROTOTYPE_LOADER_HXX_

#include <QtGui/QDialog>
#include <CLAM/NetworkPlayer.hxx>
#include <CLAM/PortMonitor.hxx>

#include <CLAM/AudioManager.hxx>
#include <CLAM/Network.hxx>

#include <iostream>
#include <list>
#include <string>

namespace CLAM
{

class PrototypeLoader : public QObject
{
	Q_OBJECT
private:
	std::string _networkFile;
	Network _network;
	NetworkPlayer * _player;
	QWidget * _interface;
//	std::list<CLAM::VM::NetPlot * > mPortMonitors; //QT4PORT
public:
	PrototypeLoader();

	~PrototypeLoader();

	bool LoadNetwork(std::string networkFile);
	void SetNetworkPlayer( const std::list<std::string> & backends );
	void SetNetworkPlayer( NetworkPlayer& player);
	QWidget * LoadInterface(QString uiFile);
	
	void ConnectWithNetwork();
	void Show();

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
