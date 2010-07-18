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

class QPushButton;

namespace CLAM
{

class PrototypeBinder;

class PrototypeLoader : public QObject
{
	Q_OBJECT
private:
	std::string _networkFile;
	std::string _backendName;
	Network _network;
	NetworkPlayer * _player;
	QWidget * _interface;
	bool _useGui;
	QPushButton * _playButton;
	QPushButton * _pauseButton;
	QPushButton * _stopButton;

public:
	PrototypeLoader();
	~PrototypeLoader();
	void UseGUI(bool useIt) { _useGui=useIt; }
	bool LoadNetwork(std::string networkFile);
	bool ChooseBackend( std::list<std::string> backends, const std::string & name);
	QWidget * LoadInterface(QString uiFile);
	
	void ConnectUiWithNetwork();
	void Show();
	typedef std::list<PrototypeBinder*> Binders;
	static Binders & binders();

public slots:
	void Start();
	void Pause();
	void Stop();
	void OpenAudioFile();

private:
	std::string GetNetworkNameFromWidgetName(const char * widgetName);

	void ConnectWidgetsWithIntegerControls();
	void ConnectWidgetsWithMappedControls();
	void ConnectWidgetsUsingControlBounds();
	void ConnectWidgetsWithAudioFileReaders();
	void ConnectWidgetsWithBooleanControls();
	
	bool reportIfMissingProcessing(const std::string & processingName);
	bool reportIfMissingInControl(const std::string & fullControlName);
	bool reportIfMissingOutPort(const std::string & fullControlName);
	void reportError(const QString & title, const QString & message);
	void reportWarning(const QString & title, const QString & message);

	void UpdatePlayStatus();
private slots:
	void periodicPlaybackStatusUpdate();
};

class ControlSource;
class ControlSourceSender : public QObject
{
	Q_OBJECT
	ControlSource * _source;
public:
	ControlSourceSender(ControlSource * processing)
		: _source(processing)
	{
	}
public slots:
	void send(int value);
};


} //end namespace CLAM

#endif

