#include "PrototypeLoader.hxx"
#include "QtSlot2Control.hxx"
#include <QtUiTools/QUiLoader>
#include <QtDesigner/QFormBuilder>
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/PushFlowControl.hxx>
#include <CLAM/BlockingNetworkPlayer.hxx>
#include <fstream>
#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
#endif
#include <QtGui/QWidget>

#include "Oscilloscope.hxx"
#include "Vumeter.hxx"
#include "SpectrumView.hxx"
#include "PeakView.hxx"
#include "Tonnetz.hxx"
#include "KeySpace.hxx"
#include "PolarChromaPeaks.hxx"
#include "ChordRanking.hxx"
//#include "NetAudioPlot.hxx" // QT4PORT
//#include "NetPeaksPlot.hxx" // QT4PORT
//#include "NetSpectrumPlot.hxx" // QT4PORT
//#include "NetFundPlot.hxx" // QT4PORT
//#include "NetAudioBuffPlot.hxx" // QT4PORT
//#include "NetSpecgramPlot.hxx" // QT4PORT
//#include "NetFundTrackPlot.hxx" // QT4PORT
//#include "NetSinTracksPlot.hxx" // QT4PORT

static std::string getMonitorNumber()
{
	static unsigned number = 0;
	std::stringstream os;
	os << number++;
	return os.str();
}

inline bool FileExists( const std::string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file ) return false;
	return true;
}

namespace CLAM
{
	
PrototypeLoader::PrototypeLoader()
	: _player(0)
	, _interface(0)
{
	int frameSize = 2048;
	_network.AddFlowControl(new CLAM::PushFlowControl(frameSize));
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
		_networkFile=file.toStdString();
	}
	QString errorMessage;
	try
   	{
		CLAM::XMLStorage::Restore(_network, _networkFile);
		return true;
	}
	catch (CLAM::XmlStorageErr & e)
	{
		errorMessage = FileExists(_networkFile) ?
			tr("<p>An occurred while loading the network file %1.<p>"
				"<p><b>%2</b></p>").arg(_networkFile.c_str()).arg(e.what()) :
			tr("<p>Network file '%1' not found.</p>").arg(_networkFile.c_str());
	}
	QMessageBox::critical(0, tr("Error loading the network"), errorMessage);
	_network.Clear();
	return false;
}

PrototypeLoader::~PrototypeLoader()
{
	if (_player) delete _player;
}
void PrototypeLoader::Show()
{
	_interface->show();
}


CLAM::NetworkPlayer * tryNetworkPlayer(const std::string & backend)
{	
#ifdef USE_JACK		
	if (backend=="jack")
	{
		CLAM::JACKNetworkPlayer * jackPlayer = new CLAM::JACKNetworkPlayer();
		if ( jackPlayer->IsConnectedToServer()) return jackPlayer;
		delete jackPlayer;
		return 0;
	}
#endif
	if (backend=="alsa")
	{
		return new CLAM::BlockingNetworkPlayer();
	}
	return 0;
}

bool PrototypeLoader::ChooseBackend( std::list<std::string> backends )
{
	// If no backends specified, do the standard backend waterfall
	if (backends.empty())
	{
		backends.push_back("jack");
		backends.push_back("alsa");
	}
	for (std::list<std::string>::iterator it = backends.begin();
			it!=backends.end();
			it++)
	{
		_player = tryNetworkPlayer(*it);
		if ( !_player)
		{
			std::cerr << "Backend '" << *it << "' unavailable." << std::endl;
			continue;
		}
		_backendName = *it;
		std::cout << "Backend '" << *it << "' selected." << std::endl;
		_player->SetNetwork(_network);
		return true;
	}
	QMessageBox::critical(0,
		tr("Error chosing a backend"),
		tr("No audio backend was available."));
	return false;
}

static QWidget * DoLoadInterface(const QString & uiFile)
{
	QFile file(uiFile);
	file.open(QFile::ReadOnly);
//	QFormBuilder loader; // TODO: Change this to a QUiLoader
	QUiLoader loader;
	loader.addPluginPath("/user/share/NetworkEditor/qtplugins"); //TODO Make that an option
	QStringList paths = loader.pluginPaths();
	for (QStringList::iterator it = paths.begin(); it!=paths.end(); it++)
	{
		std::cout << "Looking for plugins at path: " << it->toStdString() << std::endl;
	}
	QWidget * userInterface = loader.load(&file, 0 );
	if (userInterface)
	{
		// TODO: Do this just when no icon set.
		userInterface->setWindowIcon(QIcon(":/icons/images/NetworkEditor-icon.png"));
	}
	file.close();
	return userInterface;
}

QWidget * PrototypeLoader::LoadInterface(QString uiFile)
{
	if (_interface) delete _interface;
	if (uiFile.isEmpty())
	{
		QString networkExtension = ".clamnetwork";
		uiFile = _networkFile.c_str();
		if (uiFile.endsWith(networkExtension))
			uiFile.chop(networkExtension.length());
		uiFile += ".ui";
	}
	_interface = DoLoadInterface(uiFile);
	if (_interface) return _interface;

	QString error = FileExists(uiFile.toStdString()) ?
		tr("Interface file '%1' had errors."):
		tr("Interface file '%1' not found.") ;
	QMessageBox::warning(0,
		tr("Error loading the interface"),
		tr("<p><b>%1</b></p>"
			"<p>Using a default interface.</p>").arg(error.arg(uiFile)));

	_interface = DoLoadInterface(":/interfaces/prototyper/DefaultInterface.ui");
	CLAM_ASSERT(_interface, "Failed to open the default interface");
	return _interface;
}

void PrototypeLoader::ConnectWithNetwork()
{
	CLAM_ASSERT( _player, "Connecting interface without having chosen a backend");
	
	ConnectWidgetsWithControls();
	ConnectWidgetsWithMappedControls();

	ConnectWidgetsWithPorts<Oscilloscope,OscilloscopeMonitor>
		("OutPort__.*", "Oscilloscope");
	ConnectWidgetsWithPorts<Vumeter,VumeterMonitor>
		("OutPort__.*", "Vumeter");
	ConnectWidgetsWithPorts<SpectrumView,SpectrumViewMonitor>
		("OutPort__.*", "SpectrumView");
	ConnectWidgetsWithPorts<PeakView,PeakViewMonitor>
		("OutPort__.*", "PeakView");
	ConnectWidgetsWithPorts<CLAM::VM::Tonnetz,TonnetzMonitor>
		("OutPort__.*", "CLAM::VM::Tonnetz");
	ConnectWidgetsWithPorts<CLAM::VM::KeySpace,KeySpaceMonitor>
		("OutPort__.*", "CLAM::VM::KeySpace");
	ConnectWidgetsWithPorts<PolarChromaPeaks,PolarChromaPeaksMonitor>
		("OutPort__.*", "PolarChromaPeaks");
	ConnectWidgetsWithPorts<CLAM::VM::ChordRanking,ChordRankingMonitor>
		("OutPort__.*", "CLAM::VM::ChordRanking");
/*
	// QT4PORT
	ConnectWidgetsWithPorts<CLAM::VM::NetAudioPlot>
		("OutPort__.*", "CLAM::VM::NetAudioPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetSpectrumPlot>
		("OutPort__.*", "CLAM::VM::NetSpectrumPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetPeaksPlot>
		("OutPort__.*", "CLAM::VM::NetPeaksPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetFundPlot>
		("OutPort__.*", "CLAM::VM::NetFundPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetAudioBuffPlot>
		("OutPort__.*", "CLAM::VM::NetAudioBuffPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetSpecgramPlot>
		("OutPort__.*", "CLAM::VM::NetSpecgramPlot");
	ConnectWidgetsWithPorts<CLAM::VM::NetFundTrackPlot>
		("OutPort__.*", "CLAM::VM::NetFundTrackPlot");
	// TODO: Still not ported
	//ConnectWidgetsWithPorts<CLAM::VM::NetSinTracksPlot>
	//	("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
*/
	QObject * playButton = _interface->findChild<QObject*>("PlayButton");
	if (playButton) connect(playButton, SIGNAL(clicked()), this, SLOT(Start()));  

	QObject * stopButton = _interface->findChild<QObject*>("StopButton");
	if (stopButton) connect(stopButton, SIGNAL(clicked()), this, SLOT(Stop()));

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
		QMessageBox::critical(0, tr("Unable to play the network"), 
				tr("<p><b>Audio output unavailable or busy.</b></p>"));
		return;
	}
	
	if (  _network.IsEmpty() )
	{
		QMessageBox::critical(0, tr("Unable to play the network"), 
				tr("<p><b>A network without processings is not playable.</b></p>"));
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		QMessageBox::critical(0, tr("Unable to play the network"), 
				tr("<p><b>Not all the processings are properly configured.</b></p>"
				));
		return;
	}
	// TODO: Activate this once it works
	if ( false && _network.HasUnconnectedInPorts() )
	{
		QMessageBox::critical(0, tr("Unable to play the network"), 
				tr("<p><b>The network has some in ports which are not connected.</b></p>"
				"<p>All in ports must be feeded in order to play the network</p>"
				));
		return;
	}
	if ( !_player->IsCallbackBased() && !_network.HasSyncSource() )
	{
		QMessageBox::critical(0, tr("Unable to play the network"), 
			tr("<p>The network needs an AudioIn or AudioOut in order to be playable.</p>"));
		return;
	}
	std::cout << "Starting..." << std::endl;
	_player->Start();
	UpdatePlayStatus();
}

void PrototypeLoader::Stop()
{
	std::cout << "Stopping..." << std::endl;
	_player->Stop();
	UpdatePlayStatus();
}

void PrototypeLoader::UpdatePlayStatus()
{
	QLabel * playbackIndicator = _interface->findChild<QLabel*>("PlaybackIndicator");
	if (playbackIndicator)
	{
		if ( !_player->IsStopped())
			playbackIndicator->setText("<p style='color:green'>Playing...</p>");
		else
			playbackIndicator->setText("<p style='color:red'>Stopped</p>");
	}
}

void PrototypeLoader::Substitute(std::string & subject, const char * pattern, const char * substitution)
{
	for (std::string::size_type position = subject.find(pattern, 0);
		position!=std::string::npos;
		position = subject.find(pattern, position))
	{
		subject.replace(position, strlen(pattern), substitution);
	}
}

std::string PrototypeLoader::GetNetworkNameFromWidgetName(const char * widgetName)
{
	std::string networkName(widgetName);
	Substitute(networkName,"___", " ");
	Substitute(networkName,"__", ".");
	return networkName;
}

void PrototypeLoader::ConnectWidgetsWithControls()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("InControl__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string controlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(11).toAscii());
		std::cout << "* Control: " << controlName << std::endl;

		// TODO: It may not be present
		CLAM::InControl & receiver = _network.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendControl(int)));
	}
}

void PrototypeLoader::ConnectWidgetsWithMappedControls()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("InControlFloat__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string controlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(16).toAscii());
		std::cout << "* Mapped Control (100:1): " << controlName << std::endl;

		// TODO: It may not be present
		CLAM::InControl & receiver = _network.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendMappedControl(int)));
	}
}

template < typename PlotClass, typename MonitorType>
void PrototypeLoader::ConnectWidgetsWithPorts(char* prefix, char* plotClassName)
{
	std::cout << "Looking for " << plotClassName << " widgets..." << std::endl;
	// QT4PORT
//	if (!QWidgetFactory::supportsWidget(plotClassName))
//		qWarning(tr("No support for widgets %1. Maybe the CLAM qt plugins has not been loaded").arg(plotClassName));
//	QList<PlotClass*> widgets = _interface->findChildren<PlotClass*>(QRegExp(prefix));
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp(prefix));
	for (typename QList<QWidget*>::Iterator it=widgets.begin();
			it!=widgets.end();
		   	it++)
	{
		QWidget * aWidget = *it;
		if (aWidget->metaObject()->className() != std::string(plotClassName)) continue;
		std::string portName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(9).toAscii());
		std::cout << "* " << plotClassName << " connected to port " << portName << std::endl;

		MonitorType * portMonitor = new MonitorType;

		std::string monitorName = "PrototyperMonitor"+getMonitorNumber();
		_network.AddProcessing(monitorName,portMonitor);

		// TODO: It may not be present
		_network.ConnectPorts(portName,monitorName+".Input");
		PlotClass * plot = (PlotClass*) aWidget;
		plot->setDataSource(*portMonitor);
	}
}
	
} //end namespace CLAM
