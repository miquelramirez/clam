#include <qapplication.h>
#include <qwidgetfactory.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <iostream>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"
#include "QtSlot2Control.hxx"
#include "NetAudioPlot.hxx"
#include "NetPeaksPlot.hxx"
#include "NetSpectrumPlot.hxx"
#include "NetFundPlot.hxx"
#include "NetAudioBuffPlot.hxx"
#include "NetSpecgramPlot.hxx"
#include "NetFundTrackPlot.hxx"
#include "NetSinTracksPlot.hxx"

#include "PortMonitor.hxx"

////////////////Temporary includes
#include "SimpleOscillator.hxx"
#include "AudioOut.hxx"

class NetworkPlayer
{
  CLAM::AudioManager _audioManager;
  CLAM::Network _network;
  CLAM::Thread _thread;
  bool _stopped;

  CLAM::SimpleOscillator _oscil;
  CLAM::AudioOut _aout;

public:
  NetworkPlayer(const std::string & networkFile)
    : _audioManager( 44100, 512 )
    , _thread(/*realtime*/true)
    , _stopped(true)

  {
	CLAM::PushFlowControl * fc = new CLAM::PushFlowControl(512);
	_network.AddFlowControl( fc );
	CLAM::XmlStorage::Restore(_network,networkFile);
	fc->AddOutsiderGenerator( &_oscil );
	_thread.SetThreadCode( makeMemberFunctor0( *this, NetworkPlayer, Do ) );

    //    CONNECT PORTS
    CLAM::Processing & entrada=_network.GetProcessing("SMSAnalysisCore");
    CLAM::Processing & sortida=_network.GetProcessing("SMSSynthesis");

//    _network.AddProcessing("SimpleOscillator",&_oscil);
//    _network.ConnectPorts("SimpleOscillator.Audio Output","SMSAnalysisCore.Input Audio");
    
    ConnectPorts(_oscil,"Audio Output",entrada,"Input Audio");
    ConnectPorts(sortida,"OutputAudio",_aout,"Audio Input");
  }
  void Start()
  {
    _stopped=false;
    _network.Start();
    _thread.Start();

    _oscil.Start();
    _aout.Start();
  }
  void Do()
  {
    while (!_stopped)
      {
	if (false && _oscil.CanConsumeAndProduce())
	  {
	    std::cout <<"\nDO\n";
	    _oscil.Do();
	  }
	
	_network.DoProcessings();
      }
  }
  void Stop()
  {
    _aout.Stop();
    _oscil.Stop();

    _stopped=true;
    _thread.Stop();
    _network.Stop();
  }
  ~NetworkPlayer()
  {
    Stop();
  }
  CLAM::Network & Network()
  {
    return _network;
  }
};

static std::string getMonitorNumber()
{
  static unsigned number = 0;
  std::stringstream os;
  os << number++;
  return os.str();
}


class PrototypeLoader
{
  char * _networkFile;
  char * _interfaceFile;
  QWidget * _mainWidget;
  NetworkPlayer _player;
  std::list<CLAM::VM::NetPlot * > _portMonitors;
public:
  PrototypeLoader(char * networkFile)
    : _networkFile(networkFile)
    , _player(networkFile)
  {
		
  }
  QWidget * loadPrototype(char* uiFile)
  {
    _mainWidget = (QWidget *) QWidgetFactory::create( uiFile );
    return _mainWidget;
  }
  void connectWithNetwork()
  {
    CLAM::Network & network = _player.Network();
    QWidget * prototype = _mainWidget;
    connectWidgetsWithControls(network,prototype);
    connectWidgetsWithMappedControls(network,prototype);

    connectWidgetsWithPorts<CLAM::VM::NetAudioPlot>
      ("OutPort__.*", "CLAM::VM::NetAudioPlot");
    connectWidgetsWithPorts<CLAM::VM::NetSpectrumPlot>
      ("OutPort__.*", "CLAM::VM::NetSpectrumPlot");
    connectWidgetsWithPorts<CLAM::VM::NetPeaksPlot>
      ("OutPort__.*", "CLAM::VM::NetPeaksPlot");
    connectWidgetsWithPorts<CLAM::VM::NetFundPlot>
      ("OutPort__.*", "CLAM::VM::NetFundPlot");
    connectWidgetsWithPorts<CLAM::VM::NetAudioBuffPlot>
      ("OutPort__.*", "CLAM::VM::NetAudioBuffPlot");
    connectWidgetsWithPorts<CLAM::VM::NetSpecgramPlot>
      ("OutPort__.*", "CLAM::VM::NetSpecgramPlot");
    connectWidgetsWithPorts<CLAM::VM::NetFundTrackPlot>
      ("OutPort__.*", "CLAM::VM::NetFundTrackPlot");
    // TODO: Still not ported
    //		connectWidgetsWithPorts<CLAM::VM::NetSinTracksPlot>
    //	        	("OutPort__.*", "CLAM::VM::NetSinTracksPlot");
  }
public slots:
void Start()
  {
    _player.Start();
  }

private:
  void substitute(std::string & subject, const char * pattern, const char * substitution)
  {
    for (std::string::size_type position = subject.find(pattern, 0);
	 position!=std::string::npos;
	 position = subject.find(pattern, position))
      {
	subject.replace(position, strlen(pattern), substitution);
      }
  }
  std::string GetNetworkNameFromWidgetName(const char * widgetName)
  {
    std::string networkName(widgetName);
    substitute(networkName,"___", " ");
    substitute(networkName,"__", ".");
    return networkName;
  }

  void connectWidgetsWithControls(CLAM::Network & network, QWidget * prototype)
  {
    QObjectList * widgets = prototype->queryList(0,"InControl__.*");
    for (QObjectListIt it(*widgets); it.current(); ++it)
      {
	QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
	std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 11);

	std::cout << "* Control: " << controlName << std::endl;

	CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
	QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
	notifier->linkControl(receiver);
	notifier->connect(aWidget,SIGNAL(valueChanged(int)),
			  SLOT(sendControl(int)));
      }

  }

  void connectWidgetsWithMappedControls(CLAM::Network & network, QWidget * prototype)
  {
    QObjectList * widgets = prototype->queryList(0,"InControlFloat__.*");
    for (QObjectListIt it(*widgets); it.current(); ++it)
      {
	QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
	std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 16);
	std::cout << "* Mapped Control (100:1): " << controlName << std::endl;

	CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
	QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
	notifier->linkControl(receiver);
	notifier->connect(aWidget,SIGNAL(valueChanged(int)),
			  SLOT(sendMappedControl(int)));
      }
  }
  template < typename PlotClass >
  void connectWidgetsWithPorts(char* prefix, char* plotClassName)
  {
    CLAM::Network & network = _player.Network();
    QWidget * prototype = _mainWidget;
    QObjectList * widgets = prototype->queryList(plotClassName,prefix);
    for (QObjectListIt it(*widgets); it.current(); ++it)
      {
	QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
	std::string controlName=GetNetworkNameFromWidgetName(aWidget->name() + 9);
	std::cout << "* " << plotClassName << ": " << controlName << std::endl;

	typedef typename PlotClass::MonitorType MonitorType;
	MonitorType * portMonitor = new MonitorType;

	std::string monitorName = "PrototyperMonitor"+getMonitorNumber();
	network.AddProcessing(monitorName,portMonitor);

	network.ConnectPorts(controlName,monitorName+".Input");
	PlotClass * plot = (PlotClass*) aWidget;
	plot->SetMonitor(*portMonitor);
      }
  }


};



int main( int argc, char *argv[] )
{
  if (argc!=3)
    {
      std::cout << " Usage: " 
		<< argv[0] << " <networkfile> <uifile>" << std::endl;
      return -1;
    }

  char * networkFile = argv[1]; // "SpectralDelay.clam"
  char * uiFile = argv[2]; // "SpectralDelay.ui" 

  QApplication app( argc, argv );

  PrototypeLoader loader(networkFile);


  QWidget * prototype = loader.loadPrototype(uiFile);
  if (!prototype) return -1;
  loader.connectWithNetwork();

  // Set up the dynamic dialog here
  app.setMainWidget( prototype );

  prototype->show();


  loader.Start();
  int result = app.exec();

  return result;
}
