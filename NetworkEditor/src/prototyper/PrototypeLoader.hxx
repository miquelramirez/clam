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
	std::string _backendName;
	Network _network;
	NetworkPlayer * _player;
	QWidget * _interface;
//	std::list<CLAM::VM::NetPlot * > mPortMonitors; //QT4PORT
public:
	PrototypeLoader();

	~PrototypeLoader();

	bool LoadNetwork(std::string networkFile);
	bool ChooseBackend( std::list<std::string> backends, const std::string & name);
	QWidget * LoadInterface(QString uiFile);
	
	void ConnectWithNetwork();
	void Show();

public slots:
	void Start();
	void Stop();
	void OpenAudioFile();

private:
	void Substitute(std::string & subject, const char * pattern, const char * substitution);
	
	std::string GetNetworkNameFromWidgetName(const char * widgetName);

	void ConnectWidgetsWithIntegerControls();
	void ConnectWidgetsWithMappedControls();
	void ConnectWidgetsUsingControlBounds();
	void ConnectWidgetsWithAudioFileReaders();
	void ConnectWidgetsWithBooleanControls();
	
	template < typename PlotClass, typename MonitorType >
	void ConnectWidgetsWithPorts(char* prefix, char* plotClassName);

	bool ReportMissingProcessing(const std::string & processingName);
	bool ReportMissingInControl(const std::string & fullControlName);
	bool ReportMissingOutPort(const std::string & fullControlName);

	void UpdatePlayStatus();
};

} //end namespace CLAM

#endif

