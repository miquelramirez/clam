#include "QtBinder.hxx"
#include <CLAM/Network.hxx>


CLAM::QtBinder::QtBinder()
{
	// auto registering
	binders().push_back(this);
}

CLAM::QtBinder::~QtBinder()
{
	// TODO: maybe unregister
}

CLAM::QtBinder::Binders & CLAM::QtBinder::binders()
{
	static Binders binders;
	return binders;
}

void CLAM::QtBinder::bindRecursively( QObject * userInterface, CLAM::Network & network, QStringList & errors)
{
	if (handles(userInterface))
		bind(userInterface, network, errors);
	foreach (QObject * child, userInterface->children())
	{
		bindRecursively(child, network, errors);
	}
}

void CLAM::QtBinder::bindAllBinders(QObject * userInterface, CLAM::Network & network, QStringList & errors)
{
	for (Binders::iterator it=binders().begin(); it!=binders().end(); it++)
		(*it)->bindRecursively(userInterface, network, errors);
}

CLAM::QtBinder * CLAM::QtBinder::findHandler(QObject * uiElement)
{
	for (Binders::iterator it=binders().begin(); it!=binders().end(); it++)
		if ((*it)->handles(uiElement)) return *it;
	return 0;
}

std::string CLAM::QtBinder::widget2NetworkName(const QString & prefix, const QString & widgetName)
{
	return widgetName
		.mid(prefix.length())
		.replace("___", " ")
		.replace("__", ".")
		.toStdString()
		;
}
bool CLAM::QtBinder::error(QStringList & errors, const QString & message)
{
	errors.append(message);
	return false;
}

bool CLAM::QtBinder::missingProcessing(const std::string & processingName, Network & network, QStringList & errors)
{
	if (network.HasProcessing(processingName))
		return false;
	error(errors, 
		QString("The interface asked to connect to the "
			"processing '%1' which is not in the network.")
			.arg(processingName.c_str()));
	return true;
}

bool CLAM::QtBinder::missingOutPort(const std::string & portName, Network & network, QStringList & errors)
{
	std::string processingName = network.GetProcessingIdentifier(portName);
	if (missingProcessing(processingName,network,errors)) return true;
	std::string shortPortName = network.GetConnectorIdentifier(portName);
	CLAM::Processing & processing = network.GetProcessing(processingName);
	if (processing.HasOutPort(shortPortName))
		return false; // no problem :-)
	QStringList ports;
	for (unsigned i=0; i<processing.GetNOutPorts(); i++)
		ports << processing.GetOutPort(i).GetName().c_str();
	QString available("Available: ");
	available += ports.join(", ")+"."; 
	if (ports.empty()) available="None available.";
	error(errors, 
		QString("The interface asked to connect to a port '%1' "
			"not available in the processing '%2'.%3")
			// TODO: Try with...
			.arg(shortPortName.c_str())
			.arg(processingName.c_str())
			.arg(available)
			);
	return true;
}

bool CLAM::QtBinder::missingInControl(const std::string & controlName, Network & network, QStringList & errors)
{
	std::string processingName = network.GetProcessingIdentifier(controlName);
	if (missingProcessing(processingName,network,errors)) return true;
	std::string shortControlName = network.GetConnectorIdentifier(controlName);
	CLAM::Processing & processing = network.GetProcessing(processingName);
	if (processing.HasInControl(shortControlName))
		return false; // no problem :-)
	QStringList controls;
	for (unsigned i=0; i<processing.GetNInControls(); i++)
		controls << processing.GetInControl(i).GetName().c_str();
	QString available("Available: ");
	available += controls.join(", ")+"."; 
	if (controls.empty()) available="None available.";
	error(errors, 
		QString("The interface asked to connect to a control '%1' "
			"not available in the processing '%2'. %3")
			// TODO: Try with...
			.arg(shortControlName.c_str())
			.arg(processingName.c_str())
			.arg(available)
			);
	return true;
}

#include "ControlSurfaceWidget.hxx"

namespace CLAM { class ControlSurfaceBinder; }

class CLAM::ControlSurfaceBinder : public QtBinder
{
public:
	bool handles(QObject * qobject)
	{
		return isSubClassOf<ControlSurfaceWidget>(qobject);
	}
	
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors)
	{
		QString xcontrol = uiElement->property("clam_xcontrol").toString();
		QString ycontrol = uiElement->property("clam_ycontrol").toString();
		if (xcontrol.isEmpty()) return error(errors,
			QString("clam_xcontrol property required to bind a ControlSurfaceWidget"));
		if (ycontrol.isEmpty()) return error(errors,
			QString("clam_ycontrol property required to bind a ControlSurfaceWidget"));

		if (missingInControl(xcontrol.toStdString(), network, errors)) return false;
		if (missingInControl(ycontrol.toStdString(), network, errors)) return false;
	}
};



