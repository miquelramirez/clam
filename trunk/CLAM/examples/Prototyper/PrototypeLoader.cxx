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
#include "NetPeaksPlot.hxx"


class NetworkPlayer
{
	CLAM::AudioManager _audioManager;
	CLAM::Network _network;
	CLAM::Thread _thread;
	bool _stoped;
public:
	NetworkPlayer(const std::string & networkFile)
		: _audioManager( 44100, 512 )
		, _thread(/*realtime*/true)
		, _stoped(true)

	{
		_network.AddFlowControl( new CLAM::PushFlowControl( /*frameSize*/ 512 ));
		CLAM::XmlStorage::Restore(_network,networkFile);
		_thread.SetThreadCode( makeMemberFunctor0( *this, NetworkPlayer, Do ) );
		Start();
	}
	void Start()
	{
		_stoped=false;
		_network.Start();
		_thread.Start();
	}
	void Do()
	{
		while (!_stoped)
		{
			_network.DoProcessings();
		}
	}
	void Stop()
	{
		_stoped=true;
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

class PrototypeLoader
{
	char * _networkFile;
	char * _interfaceFile;
	QWidget * _mainWidget;
	NetworkPlayer _player;
public:

};
void connectWidgetsWithControls(CLAM::Network & network, QWidget * qtPrototype)
{
	QObjectList * widgets = qtPrototype->queryList(0,"InControl::.*");
	for (QObjectListIt it(*widgets); it.current(); ++it)
	{
		QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
		std::string controlName(aWidget->name() + 11);
		for (std::string::size_type colonPosition = controlName.find("::", 0);
			colonPosition!=std::string::npos;
			colonPosition = controlName.find("::", colonPosition))
		{
			controlName.replace(colonPosition, 2, ".");
		}
		std::cout << "Control: " << controlName << std::endl;
		std::cout << controlName << std::endl;

		CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
		                  SLOT(sendControl(int)));
	}

}

void connectWidgetsWithMappedControls(CLAM::Network & network, QWidget * qtPrototype)
{
	QObjectList * widgets = qtPrototype->queryList(0,"InControlFloat::.*");
	for (QObjectListIt it(*widgets); it.current(); ++it)
	{
		QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
		std::string controlName(aWidget->name() + 16);
		for (std::string::size_type colonPosition = controlName.find("::", 0);
			colonPosition!=std::string::npos;
			colonPosition = controlName.find("::", colonPosition))
		{
			controlName.replace(colonPosition, 2, ".");
		}
		std::cout << "0:1 Mapped Control: " << controlName << std::endl;

		CLAM::InControl & receiver = network.GetInControlByCompleteName(controlName);
		QtSlot2Control * notifier = new QtSlot2Control(controlName.c_str());
		notifier->linkControl(receiver);
		notifier->connect(aWidget,SIGNAL(valueChanged(int)),
		                  SLOT(sendMappedControl(int)));
	}

}
void connectWidgetsWithPeaksPorts(CLAM::Network & network, QWidget * qtPrototype)
{
	QObjectList * widgets = qtPrototype->queryList("CLAM::VM::NetPeaksPlot",0);
	for (QObjectListIt it(*widgets); it.current(); ++it)
	{
		QWidget * aWidget = dynamic_cast<QWidget*>(it.current());
		std::string controlName(aWidget->name());
		std::cout << "Peak Port Monitor: " << controlName << std::endl;
	}

}
QWidget * loadPrototype(CLAM::Network & network, char* uiFile)
{
	QWidget * qtPrototype = (QWidget *) QWidgetFactory::create( uiFile );
	if (!qtPrototype) return 0;

	connectWidgetsWithControls(network,qtPrototype);
	connectWidgetsWithMappedControls(network,qtPrototype);

	return qtPrototype;

}



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

	NetworkPlayer player(networkFile);

	QApplication app( argc, argv );

	QWidget * qtPrototype = loadPrototype(player.Network(), uiFile);

	if (!qtPrototype) return -1;

	// Set up the dynamic dialog here
	app.setMainWidget( qtPrototype );

	qtPrototype->show();
	int result = app.exec();

	return result;
}



