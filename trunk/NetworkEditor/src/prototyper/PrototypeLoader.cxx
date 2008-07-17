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
#include <QtCore/QList>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <fstream>
#include <CLAM/BlockingNetworkPlayer.hxx>
#ifdef USE_JACK
#include <CLAM/JACKNetworkPlayer.hxx>
#endif
#ifdef USE_PORTAUDIO
#include <CLAM/PANetworkPlayer.hxx>
#endif
#include <QtGui/QWidget>
#include <QtCore/QVariant>

// Designer widgets
#include "Oscilloscope.hxx"
#include "OscilloscopeMonitor.hxx"
#include "Vumeter.hxx"
#include "VumeterMonitor.hxx"
#include "SpectrumView.hxx"
#include "SpectrumViewMonitor.hxx"
#include "PeakView.hxx"
#include "PeakViewMonitor.hxx"
#include "Tonnetz.hxx"
#include "TonnetzMonitor.hxx"
#include "KeySpace.hxx"
#include "KeySpaceMonitor.hxx"
#include "Spectrogram.hxx"
#include "SpectrogramMonitor.hxx"
#include "PolarChromaPeaks.hxx"
#include "PolarChromaPeaksMonitor.hxx"
#include "ChordRanking.hxx"
#include "ChordRankingMonitor.hxx"
#include "LPModelView.hxx"
#include "LPModelViewMonitor.hxx"
#include "MelSpectrumView.hxx"
#include "MelSpectrumViewMonitor.hxx"
#include "MelCepstrumView.hxx"
#include "MelCepstrumViewMonitor.hxx"
#include "ProgressControl.hxx"
#include "ProgressControlWidget.hxx"

inline bool FileExists( const std::string filename )
{
	//Check for existence of XML Network file
	std::ifstream file( filename.c_str() );
	if( !file ) return false;
	return true;
}

namespace CLAM
{

class PrototypeBinder
{
public:
	PrototypeBinder()
	{
		PrototypeLoader::binders().push_back(this);
	}
	virtual ~PrototypeBinder() {}
	virtual void bindWidgets(Network & network, QWidget * userInterface) =0;
protected:
	std::string GetNetworkNameFromWidgetName(const char * widgetName)
	{
		std::string networkName(widgetName);
		Substitute(networkName,"___", " ");
		Substitute(networkName,"__", ".");
		return networkName;
	}
	void Substitute(std::string & subject, const char * pattern, const char * substitution)
	{
		for (std::string::size_type position = subject.find(pattern, 0);
			position!=std::string::npos;
			position = subject.find(pattern, position))
		{
			subject.replace(position, strlen(pattern), substitution);
		}
	}
	bool ReportMissingProcessing(const std::string & processingName, Network & network, QWidget * userInterface)
	{
		if (network.HasProcessing(processingName))
			return false;
		QMessageBox::warning(userInterface,
			QString("Error connecting controls"),
			QString("The interface asked to connect to the processing '%1' which is not in the network.")
				.arg(processingName.c_str()));
		return true;
	}
	bool ReportMissingOutPort(const std::string & portName, Network & network, QWidget * userInterface)
	{
		std::string processingName = network.GetProcessingIdentifier(portName);
		if (ReportMissingProcessing(processingName,network,userInterface)) return true;
		std::string shortPortName = network.GetConnectorIdentifier(portName);
		if (network.GetProcessing(processingName).HasOutPort(shortPortName))
			return false; // no problem :-)
		QMessageBox::warning(userInterface,
			QString("Error connecting controls"),
			QString("The interface asked to connect to a port '%1' not available in the processing '%2'.") // TODO: Try with...
				.arg(shortPortName.c_str())
				.arg(processingName.c_str()
				));
		return true;
	}
	bool ReportMissingInControl(const std::string & controlName, Network & network, QWidget * userInterface)
	{
		std::string processingName = network.GetProcessingIdentifier(controlName);
		if (ReportMissingProcessing(processingName,network,userInterface)) return true;
		std::string shortControlName = network.GetConnectorIdentifier(controlName);
		if (network.GetProcessing(processingName).HasInControl(shortControlName))
			return false; // no problem :-)
		QMessageBox::warning(userInterface,
			QString("Error connecting controls"),
			QString("The interface asked to connect to a control '%1' not available in the processing '%2'.") // TODO: Try with...
				.arg(shortControlName.c_str())
				.arg(processingName.c_str()
				));
		return true;
	}
};

template < typename PlotClass, typename MonitorType>
class MonitorBinder : public PrototypeBinder
{
	const char * _prefix;
	const char * _plotClassName;
public:
	MonitorBinder(const char* prefix, const char* plotClassName)
		: _prefix(prefix)
		, _plotClassName(plotClassName)
	{}
	virtual void bindWidgets(Network & network, QWidget * userInterface)
	{
		std::cout << "Looking for " << _plotClassName << " widgets..." << std::endl;
		QList<QWidget*> widgets = userInterface->findChildren<QWidget*>(QRegExp(_prefix));
		for (typename QList<QWidget*>::Iterator it=widgets.begin();
				it!=widgets.end();
				it++)
		{
			QWidget * aWidget = *it;
			if (aWidget->metaObject()->className() != std::string(_plotClassName)) continue;
			std::string portName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(9).toAscii());
			std::cout << "* " << _plotClassName << " connected to port " << portName << std::endl;

			if (ReportMissingOutPort(portName,network,userInterface)) continue;

			MonitorType * portMonitor = new MonitorType;
			std::string monitorName = network.GetUnusedName("PrototyperMonitor");
			network.AddProcessing(monitorName, portMonitor);
			network.ConnectPorts(portName, monitorName+".Input");
			PlotClass * plot = (PlotClass*) aWidget;
			plot->setDataSource(*portMonitor);
		}
	}
};

template <typename PlotClass, typename MonitorType>
class ProgressControlBinder : public PrototypeBinder
{
	const char * _prefix;
	const char * _plotClassName;
public:
	ProgressControlBinder(const char* prefix, const char* plotClassName)
		: _prefix(prefix)
		, _plotClassName(plotClassName)
	{}
	
	virtual void bindWidgets(Network & network, QWidget * userInterface)
	{
		std::cout << "Looking for " << _plotClassName << " widgets..." << std::endl;
		QList<QWidget*> widgets = userInterface->findChildren<QWidget*>(QRegExp(_prefix));
		for (typename QList<QWidget*>::Iterator it=widgets.begin();
			it!=widgets.end();
			it++)
		{
			QWidget * aWidget = *it;
			if (aWidget->metaObject()->className() != std::string(_plotClassName)) continue;
			
			MonitorType * portMonitor = new MonitorType;
			std::string monitorName = network.GetUnusedName("PrototyperProgressControl");
			network.AddProcessing(monitorName, portMonitor);
			((PlotClass*) aWidget)->SetProcessing(portMonitor);
		}
	}	
};

static MonitorBinder<Oscilloscope,OscilloscopeMonitor> oscilloscopeBinder
	("OutPort__.*", "Oscilloscope");
static MonitorBinder<Vumeter,VumeterMonitor> vumeterBinder
	("OutPort__.*", "Vumeter");
static MonitorBinder<SpectrumView,SpectrumViewMonitor> spectrumViewBinder
	("OutPort__.*", "SpectrumView");
static MonitorBinder<PeakView,PeakViewMonitor> peakMonitorBinder
	("OutPort__.*", "PeakView");
static MonitorBinder<CLAM::VM::Tonnetz,TonnetzMonitor> tonnetzBinder
	("OutPort__.*", "CLAM::VM::Tonnetz");
static MonitorBinder<CLAM::VM::KeySpace,KeySpaceMonitor> keyspaceBinder
	("OutPort__.*", "CLAM::VM::KeySpace");
static MonitorBinder<CLAM::VM::Spectrogram,SpectrogramMonitor> spectrogramBinder
	("OutPort__.*", "CLAM::VM::Spectrogram");
static MonitorBinder<PolarChromaPeaks,PolarChromaPeaksMonitor> polarChromaPeaksBinder
	("OutPort__.*", "PolarChromaPeaks");
static MonitorBinder<CLAM::VM::ChordRanking,ChordRankingMonitor> chordRankingBinder
	("OutPort__.*", "CLAM::VM::ChordRanking");
static MonitorBinder<CLAM::VM::LPModelView,LPModelViewMonitor> lpModelBinder
	("OutPort__.*", "CLAM::VM::LPModelView");
static MonitorBinder<CLAM::VM::MelCepstrumView,MelCepstrumViewMonitor> melCepstrumBinder
	("OutPort__.*", "CLAM::VM::MelCepstrumView");
static MonitorBinder<CLAM::VM::MelSpectrumView,MelSpectrumViewMonitor> melSpectrumBinder
	("OutPort__.*", "CLAM::VM::MelSpectrumView");

static ProgressControlBinder<ProgressControlWidget, ProgressControl> progressControlBinder
	("ProgressControl__.*", "ProgressControlWidget");


class ConfigurationBinder : public PrototypeBinder
{
public:
	virtual void bindWidgets(Network & network, QWidget * userInterface)
	{
		std::cout << "Looking for configuration actions..." << std::endl;
		static QRegExp pattern("Config__(.*)");
		QList<QAction*> actions = userInterface->findChildren<QAction*>(pattern);
		for (QList<QAction*>::iterator it=actions.begin(); it!=actions.end(); it++)
		{
			std::cout << "Action: " << (*it)->objectName().toLocal8Bit().constData() << std::endl;
			if (not pattern.exactMatch((*it)->objectName())) continue;
			std::string processing = GetNetworkNameFromWidgetName(pattern.cap(1).toStdString().c_str());
			if (ReportMissingProcessing(processing,network,userInterface)) continue;
	//		QObject::connect(*it, SIGNAL(triggered()), this, SLOT(lauchDialog()));
		}
	}
};
static ConfigurationBinder configurationBinder;


PrototypeLoader::PrototypeLoader()
	: _player(0)
	, _interface(0)
{
}

PrototypeLoader::Binders & PrototypeLoader::binders()
{
	static Binders theBinders;
	return theBinders;
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
	QMessageBox::critical(_interface,
			tr("Error loading the network"), errorMessage);
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
	if (backend=="alsa")
	{
		return new CLAM::BlockingNetworkPlayer;
	}
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
	QMessageBox::critical(_interface,
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
		uiFile = QString.fromLocal8Bit(_networkFile.c_str());
		int pos = uiFile.lastIndexOf(".");
		uiFile.truncate(pos); // if not found nothing happens
		uiFile += ".ui";
	}
	_interface = DoLoadInterface(uiFile);
	if (_interface) return _interface;

	QString error = FileExists(uiFile.toLocal8Bit().constData()) ?
		tr("Interface file '%1' had errors."):
		tr("Interface file '%1' not found.") ;
	QMessageBox::warning(_interface,
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
	
	ConnectWidgetsWithIntegerControls();
	ConnectWidgetsWithMappedControls();
	ConnectWidgetsUsingControlBounds();
	ConnectWidgetsWithBooleanControls();
	ConnectWidgetsWithAudioFileReaders();

	for (Binders::iterator it=binders().begin(); it!=binders().end(); it++)
		(*it)->bindWidgets(_network, _interface);

	ConnectWidgetsWithProgressControls();

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

void PrototypeLoader::OpenAudioFile()
{
	QObject * loadButton = sender();
	std::string processingName = loadButton->objectName().mid(12).toStdString();
	std::cout << "Loading audio for " << processingName << std::endl;
	CLAM::Processing & processing = _network.GetProcessing(processingName);
	CLAM::MonoAudioFileReaderConfig config =
		dynamic_cast<const CLAM::MonoAudioFileReaderConfig &> (processing.GetConfig());
	QString filename = 
		QFileDialog::getOpenFileName(_interface, 
			tr("Choose an audio file"),
			QString::fromLocal8Bit(config.GetSourceFile().c_str()),
			tr("Audio files (*.wav *.ogg *.mp3)")
			);
	if (filename.isEmpty()) return;
	config.SetSourceFile(filename.toLocal8Bit().constData());
	Stop();
	processing.Configure(config);
	Start();
}


void PrototypeLoader::Start()
{
	UpdatePlayStatus();
	if (  !_player )
	{
		QMessageBox::critical(_interface,
				tr("Unable to play the network"), 
				tr("<p><b>Audio output unavailable or busy.</b></p>"));
		return;
	}
	
	if (  _network.IsEmpty() )
	{
		QMessageBox::critical(_interface,
				tr("Unable to play the network"), 
				tr("<p><b>A network without processings is not playable.</b></p>"));
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		QMessageBox::critical(_interface,
				tr("Unable to play the network"), 
				tr("<p><b>Not all the processings are properly configured.</b></p><pre>%1</pre>"
				).arg(_network.GetConfigurationErrors().c_str()));
		return;
	}
	// TODO: Activate this once it works
	if ( false && _network.HasUnconnectedInPorts() )
	{
		QMessageBox::critical(_interface,
				tr("Unable to play the network"), 
				tr("<p><b>The network has some in ports which are not connected.</b></p>"
				"<p>All in ports must be feeded in order to play the network</p>"
				));
		return;
	}
	if ( !_player->IsCallbackBased() && !_network.HasSyncSource() )
	{
		QMessageBox::critical(_interface,
			tr("Unable to play the network"), 
			tr("<p>The network needs an AudioIn or AudioOut in order to be playable.</p>"));
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

void PrototypeLoader::UpdatePlayStatus()
{
	QLabel * playbackIndicator = _interface->findChild<QLabel*>("PlaybackIndicator");
	if (playbackIndicator)
	{
		if ( !_network.IsStopped())
			playbackIndicator->setText(tr("<p style='color:green'>Playing...</p>"));
		else
			playbackIndicator->setText(tr("<p style='color:red'>Stopped</p>"));
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

bool PrototypeLoader::ReportMissingProcessing(const std::string & processingName)
{
	if (_network.HasProcessing(processingName))
		return false;
	QMessageBox::warning(_interface,
		tr("Error connecting controls"),
		tr("The interface asked to connect to the processing '%1' which is not in the network.")
			.arg(processingName.c_str()));
	return true;
}
bool PrototypeLoader::ReportMissingOutPort(const std::string & portName)
{
	std::string processingName = _network.GetProcessingIdentifier(portName);
	if (ReportMissingProcessing(processingName)) return true;
	std::string shortPortName = _network.GetConnectorIdentifier(portName);
	if (_network.GetProcessing(processingName).HasOutPort(shortPortName))
		return false; // no problem :-)
	QMessageBox::warning(_interface,
		tr("Error connecting controls"),
		tr("The interface asked to connect to a port '%1' not available in the processing '%2'.") // TODO: Try with...
			.arg(shortPortName.c_str())
			.arg(processingName.c_str()
			));
	return true;
}
bool PrototypeLoader::ReportMissingInControl(const std::string & controlName)
{
	std::string processingName = _network.GetProcessingIdentifier(controlName);
	if (ReportMissingProcessing(processingName)) return true;
	std::string shortControlName = _network.GetConnectorIdentifier(controlName);
	if (_network.GetProcessing(processingName).HasInControl(shortControlName))
		return false; // no problem :-)
	QMessageBox::warning(_interface,
		tr("Error connecting controls"),
		tr("The interface asked to connect to a control '%1' not available in the processing '%2'.") // TODO: Try with...
			.arg(shortControlName.c_str())
			.arg(processingName.c_str()
			));
	return true;
}

void PrototypeLoader::ConnectWidgetsWithIntegerControls()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("InControlInteger__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string controlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(18).toAscii());
		std::cout << "* Control: " << controlName << std::endl;

		if (ReportMissingInControl(controlName)) continue;
		CLAM::InControl & receiver = _network.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str()); // TODO: Memory leak here
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
		std::string fullControlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(16).toAscii());
		std::cout << "* Mapped Control (100:1): " << fullControlName << std::endl;

		if (ReportMissingInControl(fullControlName)) continue;
		CLAM::InControl & receiver = _network.GetInControlByCompleteName(fullControlName);
		QtSlot2Control * notifier = new QtSlot2Control(fullControlName.c_str()); // TODO: Memory leak here
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendMappedControl(int)));
	}
}

void PrototypeLoader::ConnectWidgetsUsingControlBounds()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("InControl__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string fullControlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(11).toAscii());
		std::cout << "* Widget using control bounds (map: 100:1->bounds): " << fullControlName << std::endl;

		if (ReportMissingInControl(fullControlName)) continue;

		if (aWidget->metaObject()->indexOfProperty("minimum") >= 0)
			aWidget->setProperty("minimum", QVariant(0));
		if (aWidget->metaObject()->indexOfProperty("maximum") >= 0)
			aWidget->setProperty("maximum", QVariant(200));
		if (aWidget->metaObject()->indexOfProperty("maximum") >= 0)
			aWidget->setProperty("singleStep", QVariant(1));
		if (aWidget->metaObject()->indexOfProperty("pageStep") >= 0)
			aWidget->setProperty("pageStep", QVariant(5));
		if (aWidget->metaObject()->indexOfProperty("value") >= 0)
			aWidget->setProperty("value", QVariant(100));

		CLAM::InControl & receiver = _network.GetInControlByCompleteName(fullControlName);
		QtSlot2Control * notifier = new QtSlot2Control(
					fullControlName.c_str(), 
					receiver.LowerBound(),
					receiver.UpperBound()
				); // TODO: Memory leak here
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendMappedControl(int)));
	}
}

void PrototypeLoader::ConnectWidgetsWithBooleanControls()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("InControlBool__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string fullControlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(15).toAscii());
		std::cout << "* Bool Control (100:1): " << fullControlName << std::endl;

		if (ReportMissingInControl(fullControlName)) continue;
		CLAM::InControl & receiver = _network.GetInControlByCompleteName(fullControlName);
		QtSlot2Control * notifier = new QtSlot2Control(fullControlName.c_str()); // TODO: Memory leak here
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(toggled(bool)),
				  SLOT(sendBooleanControl(bool)));
	}
}

void PrototypeLoader::ConnectWidgetsWithAudioFileReaders()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("AudioFile__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin();
			it!=widgets.end();
		   	it++)
	{
		QWidget * loadButton = *it;
		std::string processingName = loadButton->objectName().mid(12).toStdString();
		std::cout << "* Load Audio File Button connected to Audio file reader '" << processingName << "'" << std::endl;
		if (ReportMissingProcessing(processingName)) continue;
		connect(loadButton, SIGNAL(clicked()), this, SLOT(OpenAudioFile()));
	}
}

void PrototypeLoader::ConnectWidgetsWithProgressControls()
{
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp("ProgressControl__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string fullControlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(17).toAscii());
		std::cout << "* Progress Control: " << fullControlName << std::endl;

		CLAM::Processing * sender = ((ProgressControlWidget *) aWidget)->GetProcessing();
		CLAM::Processing & receiver = _network.GetProcessing(fullControlName);
		ConnectControls(*sender, "Progress Jump", receiver, "Current Time Position (%)");
		ConnectControls(receiver, "Current Time Position", *sender, "Progress Update");
	}
}

template < typename PlotClass, typename MonitorType>
void PrototypeLoader::ConnectWidgetsWithPorts(char* prefix, char* plotClassName)
{
	std::cout << "Looking for " << plotClassName << " widgets..." << std::endl;
	QList<QWidget*> widgets = _interface->findChildren<QWidget*>(QRegExp(prefix));
	for (typename QList<QWidget*>::Iterator it=widgets.begin();
			it!=widgets.end();
		   	it++)
	{
		QWidget * aWidget = *it;
		if (aWidget->metaObject()->className() != std::string(plotClassName)) continue;
		std::string portName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(9).toAscii());
		std::cout << "* " << plotClassName << " connected to port " << portName << std::endl;

		if (ReportMissingOutPort(portName)) continue;

		MonitorType * portMonitor = new MonitorType;
		std::string monitorName = _network.GetUnusedName("PrototyperMonitor");
		_network.AddProcessing(monitorName, portMonitor);
		_network.ConnectPorts(portName, monitorName+".Input");
		PlotClass * plot = (PlotClass*) aWidget;
		plot->setDataSource(*portMonitor);
	}
}
	
} //end namespace CLAM

