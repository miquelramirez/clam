#include "PrototypeLoader.hxx"
#include "QtSlot2Control.hxx"
//#include <QtUiTools/QUiLoader>
#include <QtDesigner/QFormBuilder>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <CLAM/XMLStorage.hxx>
#include <CLAM/PushFlowControl.hxx>

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

namespace CLAM
{
	
PrototypeLoader::PrototypeLoader(std::string networkFile)
	: mNetworkFile(networkFile)
	, mPlayer(NULL)
{
	int frameSize = 2048;
	mNetwork.AddFlowControl(new CLAM::PushFlowControl(frameSize));
	try
   	{
		CLAM::XMLStorage::Restore(mNetwork, mNetworkFile);
	}
	catch (CLAM::XmlStorageErr & e)
	{
		QMessageBox::critical(this, tr("Error loading the network"),
			tr("<p>An occurred while loading the network file.<p>"
				"<p><b>%1</b></p>").arg(e.what()));
		mNetwork.Clear();
	}
}

PrototypeLoader::~PrototypeLoader()
{
	if (mPlayer) delete mPlayer;
	
	// QT4PORT
//	for (std::list<CLAM::VM::NetPlot*>::iterator it=mPortMonitors.begin(); it!=mPortMonitors.end(); it++)
//		delete *it;
}

void PrototypeLoader::SetNetworkPlayer( NetworkPlayer& player)
{
	mPlayer=&player;
	mPlayer->SetNetwork(mNetwork);
}

bool PrototypeLoader::LoadPrototype(const QString & uiFile)
{
	QFile file(uiFile);
	file.open(QFile::ReadOnly);
	QFormBuilder loader; // TODO: Change this to a QUiLoader
	QWidget * widget = loader.load(&file, this );
	file.close();
	if (not widget) return false;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(widget);
	setLayout(layout);
	return true;
}

void PrototypeLoader::ConnectWithNetwork()
{
	CLAM_ASSERT( mPlayer!=NULL , "PrototypeLoader::ConnectWithNetwork() : no NetworkPlayer assigned. Do it with SetNetworkPlayer(..)");
	
	ConnectWidgetsWithControls();
	ConnectWidgetsWithMappedControls();
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
	QObject * playButton = findChild<QObject*>("PlayButton");
	if (playButton) connect(playButton, SIGNAL(clicked()), this, SLOT(Start()));  

	QObject * stopButton = findChild<QObject*>("StopButton");
	if (stopButton) connect(stopButton, SIGNAL(clicked()), this, SLOT(Stop()));  
}

void PrototypeLoader::Start()
{
	if ( not mPlayer )
	{
		QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p><b>Audio output unavailable or busy.</b></p>"));
		return;
	}
	
	if (  mNetwork.IsEmpty() )
	{
		QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p><b>A network without processings is not playable.</b></p>"));
		return;
	}
	// TODO: Activate this once it works
	if ( false and mNetwork.HasUnconnectedInPorts() )
	{
		QMessageBox::critical(this, tr("Unable to play the network"), 
				tr("<p><b>The network has some in ports which are not connected.</b></p>"
				"<p>All in ports must be feeded in order to play the network</p>"
				));
		return;
	}
	if (not mPlayer->IsCallbackBased() and not mNetwork.HasSyncSource() )
	{
		QMessageBox::critical(this, tr("Unable to play the network"), 
			tr("<p>The network needs an AudioIn or AudioOut in order to be playable.</p>"));
		return;
	}
	std::cout << "starting" << std::endl;
	mPlayer->Start();
	std::cout << "started" << std::endl;
}

void PrototypeLoader::Stop()
{
	mPlayer->Stop();
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
	QList<QWidget*> widgets = findChildren<QWidget*>(QRegExp("InControl__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string controlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(11).toAscii());
		std::cout << "* Control: " << controlName << std::endl;

		// TODO: It may not be present
		CLAM::InControl & receiver = mNetwork.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendControl(int)));
	}
}

void PrototypeLoader::ConnectWidgetsWithMappedControls()
{
	QList<QWidget*> widgets = findChildren<QWidget*>(QRegExp("InControlFloat__.*"));
	for (QList<QWidget*>::Iterator it=widgets.begin(); it!=widgets.end(); it++)
	{
		QWidget * aWidget = *it;
		std::string controlName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(16).toAscii());
		std::cout << "* Mapped Control (100:1): " << controlName << std::endl;

		// TODO: It may not be present
		CLAM::InControl & receiver = mNetwork.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
				  SLOT(sendMappedControl(int)));
	}
}

template < typename PlotClass >
void PrototypeLoader::ConnectWidgetsWithPorts(char* prefix, char* plotClassName)
{
	// QT4PORT
//	if (!QWidgetFactory::supportsWidget(plotClassName))
//		qWarning(tr("No support for widgets %1. Maybe the CLAM qt plugins has not been loaded").arg(plotClassName));
	QList<PlotClass*> widgets = findChildren<PlotClass*>(QRegExp(prefix));
	for (typename QList<PlotClass*>::Iterator it=widgets.begin();
			it!=widgets.end();
		   	it++)
	{
		PlotClass * aWidget = *it;
		std::string portName=GetNetworkNameFromWidgetName(aWidget->objectName().mid(9).toAscii());
		std::cout << "* " << plotClassName << ": " << portName << std::endl;

		typedef typename PlotClass::MonitorType MonitorType;
		MonitorType * portMonitor = new MonitorType;

		std::string monitorName = "PrototyperMonitor"+getMonitorNumber();
		mNetwork.AddProcessing(monitorName,portMonitor);

		// TODO: It may not be present
		mNetwork.ConnectPorts(portName,monitorName+".Input");
		PlotClass * plot = (PlotClass*) aWidget;
		plot->SetMonitor(*portMonitor);
	}
}
	
} //end namespace CLAM
