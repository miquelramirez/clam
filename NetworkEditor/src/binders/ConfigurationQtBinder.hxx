#ifndef CLAM_ConfigurationQtBinder_hxx
#define CLAM_ConfigurationQtBinder_hxx

#include "QtBinder.hxx"
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <CLAM/Network.hxx>
#include "Configurator.hxx"

namespace CLAM {
	class ConfigurationQtBinder;
}


class ConfigurationLauncher : public QObject
{
Q_OBJECT
private:
	CLAM::Network & _network;
	std::string _processingName;
public:
	ConfigurationLauncher(CLAM::Network & network, const std::string & processingName)
		: _network(network)
		, _processingName(processingName)
	{
	}
	~ConfigurationLauncher()
	{
		std::cout << "ConfigurationLauncher for " << _processingName << " deleted" << std::endl;
	}
public slots:
	void launch();
};


/**
	ConfigurationQtBinder is a QtBinder subclass to bind QActions
	so that they open a configurator dialog for a given processing
	in the network.
	It does so by
	- creating a ConfigurationLauncher
	- connect the action to the ConfigurationLauncher::launch slot
	- when the launch slot is called the processing is retreived
	  and a config dialog is shown

	If the QAction has a custom property named'clamConfiguration'
	with value 'processingName', this binder will try to connect
	the action to launch a configuration editor for the processing
	named 'processingName'.

	An alternative (old) method is name setting the widget name
	to 'actionConfigure__processingName'.
	Using this method requires you turn any space in the
	processing names into three underline symbols (___).
	Such mangling is not needed if you use the clamConfigure
	property.

	@ingroup UiBinders
*/
class CLAM::ConfigurationQtBinder : public CLAM::QtBinder
{
	QString _prefix;
	QString _actionPrefix;
	const char * _portProperty;
public:

	ConfigurationQtBinder()
		: _prefix("configure__")
		, _actionPrefix("actionConfigure__")
		, _portProperty("clamConfiguration")
	{}
	bool handles(QObject * uiElement);
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors);
};

#endif//CLAM_ConfigurationQtBinder_hxx

