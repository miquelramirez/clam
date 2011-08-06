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

public slots:
	void Start();
	void Pause();
	void Stop();

private:
	void reportError(const QString & title, const QString & message);
	void reportWarning(const QString & title, const QString & message);

	void UpdatePlayStatus();
private slots:
	void periodicPlaybackStatusUpdate();
};


} //end namespace CLAM

#endif

