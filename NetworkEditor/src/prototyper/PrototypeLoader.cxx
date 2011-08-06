#include "PrototypeLoader.hxx"
#include "QtSlot2Control.hxx"
#include <QtUiTools/QUiLoader>
#include <QtDesigner/QFormBuilder>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
#endif
#ifdef USE_PORTAUDIO
#include <CLAM/PANetworkPlayer.hxx>
#endif
#include <QtGui/QWidget>
#include <QtCore/QVariant>
#include <QtCore/QTimer>


#include "QtBinder.hxx"

namespace
{

static bool FileExists( const std::string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file ) return false;
	return true;
}

}

namespace CLAM
{


PrototypeLoader::PrototypeLoader()
	: _player(0)
	, _interface(0)
	, _useGui(true)
	, _playButton(0)
	, _pauseButton(0)
	, _stopButton(0)
{
	periodicPlaybackStatusUpdate();
}

void PrototypeLoader::reportWarning(const QString & title, const QString & message)
{
	if (_useGui)
	{
		QMessageBox::warning(_interface, title, message);
		return;
	}
	std::cout << "ERROR! " << title.toLocal8Bit().data() << std::endl;
	std::cout << message.toLocal8Bit().data() << std::endl;
}

void PrototypeLoader::reportError(const QString & title, const QString & message)
{
	if (_useGui)
	{
		QMessageBox::critical(_interface, title, message);
		return;
	}
	std::cout << "WARNING! " << title.toLocal8Bit().data() << std::endl;
	std::cout << message.toLocal8Bit().data() << std::endl;
}

bool PrototypeLoader::LoadNetwork(std::string networkFile)
{
	_networkFile = networkFile;
	if (_networkFile=="")
	{
		QString file = QFileDialog::getOpenFileName(0,
			"Choose a Network to run",
			QString::null,
			"CLAM Network files (*.clamnetwork)");
		if (file.isEmpty()) return false;
		_networkFile=file.toLocal8Bit().constData();
	}
	QString errorMessage;
	try
   	{
		CLAM::XMLStorage::Restore(_network, _networkFile);
		_network.SetPlayer(_player);
		return true;
	}
	catch (CLAM::XmlStorageErr & e)
	{
		errorMessage = FileExists(_networkFile) ?
			tr("<p>An occurred while loading the network file %1.<p>"
				"<p><b>%2</b></p>").arg(_networkFile.c_str()).arg(e.what()) :
			tr("<p>Network file '%1' not found.</p>").arg(_networkFile.c_str());
	}
	reportError(
		tr("Error loading the network"),
		errorMessage);
	_network.Clear();
	return false;
}

PrototypeLoader::~PrototypeLoader()
{
}
void PrototypeLoader::Show()
{
	_interface->show();
}


CLAM::NetworkPlayer * tryNetworkPlayer(const std::string & backend, const std::string & name)
{	
#ifdef USE_JACK		
	if (backend=="jack")
	{
		CLAM::JACKNetworkPlayer * player = new CLAM::JACKNetworkPlayer(name);
		if ( player->IsWorking()) return player;
		delete player;
		return 0;
	}
#endif
#ifdef USE_PORTAUDIO
	if (backend=="portaudio")
	{
		return new CLAM::PANetworkPlayer;
	}
#endif
	return 0;
}

bool PrototypeLoader::ChooseBackend( std::list<std::string> backends, const std::string & name)
{
	// If no backends specified, do the standard backend waterfall
	if (backends.empty())
	{
		backends.push_back("jack");
		backends.push_back("portaudio");
		backends.push_back("alsa");
	}
	for (std::list<std::string>::iterator it = backends.begin();
			it!=backends.end();
			it++)
	{
		_player = tryNetworkPlayer(*it, name);
		if ( !_player)
		{
			std::cerr << "Backend '" << *it << "' unavailable." << std::endl;
			continue;
		}
		_backendName = *it;
		std::cout << "Backend '" << *it << "' selected." << std::endl;
		return true;
	}
	reportError(
		tr("Error chosing a backend"),
		tr("No audio backend was available."));
	return false;
}

static QWidget * DoLoadInterface(const QString & uiFile)
{
	QFile file(uiFile);
	file.open(QFile::ReadOnly);
	QUiLoader loader;
	loader.addPluginPath("/user/share/NetworkEditor/qtplugins"); //TODO Make that an option
	QDir dir(QApplication::applicationDirPath());
	loader.addPluginPath( QString(dir.absolutePath())+"/../plugins" ); //TODO do only for mac?

	QStringList paths = loader.pluginPaths();
	for (QStringList::iterator it = paths.begin(); it!=paths.end(); it++)
	{
		std::cout << "Looking for plugins at path: " << it->toLocal8Bit().constData() << std::endl;
	}
	QWidget * userInterface = loader.load(&file, 0 );
	if (userInterface)
	{
		if (userInterface->windowIcon().isNull())
			userInterface->setWindowIcon(QIcon(":/icons/images/Prototyper-icon.svg"));
	}
	file.close();
	return userInterface;
}

QWidget * PrototypeLoader::LoadInterface(QString uiFile)
{
	if (_interface) delete _interface;
	if (uiFile.isEmpty())
	{
		uiFile = QString::fromLocal8Bit(_networkFile.c_str());
		int pos = uiFile.lastIndexOf(".");
		uiFile.truncate(pos); // if not found nothing happens
		uiFile += ".ui";
	}
	_interface = DoLoadInterface(uiFile);
	if (_interface) return _interface;

	QString error = FileExists(uiFile.toLocal8Bit().constData()) ?
		tr("Interface file '%1' had errors."):
		tr("Interface file '%1' not found.") ;
	reportWarning(
		tr("Error loading the interface"),
		tr("<p><b>%1</b></p>"
			"<p>Using a default interface.</p>").arg(error.arg(uiFile)));

	_interface = DoLoadInterface(":/interfaces/prototyper/DefaultInterface.ui");
	CLAM_ASSERT(_interface, "Failed to open the default interface");
	return _interface;
}

void PrototypeLoader::ConnectUiWithNetwork()
{
	CLAM_ASSERT( _player, "Connecting interface without having chosen a backend");
	
	QStringList errors;
	QtBinder::bindAllBinders(_interface, _network, errors);
	if (not errors.empty())
		reportError( QString("Interface Binding Errors"), errors.join("\n"));
		

	_playButton = _interface->findChild<QPushButton*>("PlayButton");
	_pauseButton = _interface->findChild<QPushButton*>("PauseButton");
	_stopButton = _interface->findChild<QPushButton*>("StopButton");
	if (_playButton) connect(_playButton, SIGNAL(clicked()), this, SLOT(Start()));  
	if (_pauseButton) connect(_pauseButton, SIGNAL(clicked()), this, SLOT(Pause()));  
	if (_stopButton) connect(_stopButton, SIGNAL(clicked()), this, SLOT(Stop()));

	QLabel * backendIndicator = _interface->findChild<QLabel*>("BackendIndicator");
	if (backendIndicator)
	{
		QString backendIcon = ":/icons/images/";
		backendIcon+=_backendName.c_str();
		backendIcon+="logo-mini.png";
		backendIndicator->setPixmap(backendIcon);
		backendIndicator->setToolTip(
			tr("<p>Using '%1' backend</p><p><img src='%2'/></p>")
			.arg(_backendName.c_str())
			.arg(backendIcon));
	}
}


void PrototypeLoader::Start()
{
	UpdatePlayStatus();
	if (  !_player )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>Audio output unavailable or busy.</b></p>"));
		return;
	}
	
	if (  _network.IsEmpty() )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>A network without processings is not playable.</b></p>"));
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>Not all the processings are properly configured.</b></p><pre>%1</pre>"
			).arg(_network.GetConfigurationErrors().c_str()));
		return;
	}
	// TODO: Activate this once it works
	if ( false && _network.HasUnconnectedInPorts() )
	{
		reportError(
			tr("Unable to play the network"), 
			tr("<p><b>The network has some in ports which are not connected.</b></p>"
			"<p>All in ports must be feeded in order to play the network</p>"
			));
		return;
	}
	std::cout << "Starting..." << std::endl;
	_network.Start();
	UpdatePlayStatus();
}

void PrototypeLoader::Stop()
{
	std::cout << "Stopping..." << std::endl;
	_network.Stop();
	UpdatePlayStatus();
}

void PrototypeLoader::Pause()
{
	std::cout << "Pausing..." << std::endl;
	_network.Pause();
	UpdatePlayStatus();
}

void PrototypeLoader::periodicPlaybackStatusUpdate()
{
	UpdatePlayStatus();
	QTimer::singleShot(500, this, SLOT(periodicPlaybackStatusUpdate()));
}

void PrototypeLoader::UpdatePlayStatus()
{
	QLabel * playbackIndicator = _interface->findChild<QLabel*>("PlaybackIndicator");
	if (playbackIndicator)
	{
		if ( _network.IsPlaying())
			playbackIndicator->setText(tr("<p style='color:green'>Playing...</p>"));
		else if (_network.IsPaused())
			playbackIndicator->setText(tr("<p style='color:orange;text-decoration:blink'>Paused</p>"));
		else
			playbackIndicator->setText(tr("<p style='color:red'>Stopped</p>"));
	}
	if (_playButton) _playButton->setEnabled(not _network.IsPlaying());
	if (_pauseButton) _pauseButton->setEnabled(_network.IsPlaying());
	if (_stopButton) _stopButton->setEnabled(not _network.IsStopped());
}


} //end namespace CLAM

