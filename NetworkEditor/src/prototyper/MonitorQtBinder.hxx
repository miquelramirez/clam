#ifndef CLAM_MonitorQtBinder_hxx
#define CLAM_MonitorQtBinder_hxx

#include "QtBinder.hxx"

namespace CLAM {
	template<typename MonitorWidget, typename MonitorProcessing>
	class MonitorQtBinder;
}

#include <CLAM/Network.hxx>

/**
	MonitorQtBinder is a template QtBinder subclass to bind MonitorWidget
	to the output ports of any processing in the network.
	It does so by
	- creating a MonitorProcessing in the network, 
	- connecting it to the out port, and,
	- setting the MonitorProcessing as data source for the MonitorWidget.

	If the MonitorWidget has a custom property named 'clamOutPort' 
	with value 'processingName.outPortName',
	this binder will try to connect it to the out port 'outPortName'
	of the processing named 'processingName'.

	An alternative (old) method is name setting the widget name
	OutPort__processingName__outControlName.
	Using this method requires you turn any space in the port or 
	processing names into three underline symbols (___).
	Right clicking on a port in the NetworkEditor gives you an option
	to copy the mangled name into the clipboard.
	Such mangling is not needed if you use the clamOutPort property.

	To add a new available monitor widget (ie. MyWidget) that uses
	a given kind of monitor processing (ie. MyProcessing) to get data from,
	just add the following code in a cxx file. (not in a header)
	@code
	static CLAM::MonitorQtBinder<MyWidget,MyProcessing> myBinder("MyWidget");
	@endcode
	@todo Links on how to define and use a Monitor processing

	@ingroup UiBinders
*/
template < typename MonitorWidget, typename MonitorProcessing>
class CLAM::MonitorQtBinder : public CLAM::QtBinder
{
	QString _prefix;
	const char * _portProperty;
	const char * _plotClassName;
public:
	MonitorQtBinder(const char* plotClassName)
		: _prefix("OutPort__")
		, _portProperty("clamOutPort")
		, _plotClassName(plotClassName)
	{}
	bool handles(QObject * uiElement)
	{
		if (not isSubClassOf<MonitorWidget>(uiElement)) return false;
		return true;
		if (uiElement->objectName().startsWith(_prefix)) return true;
		if (hasProperty(uiElement, _portProperty)) return true;
		return false;
	}
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors)
	{
		// Just to be sure, no guaranties the user has used handles before to check
		if (not isSubClassOf<MonitorWidget>(uiElement)) return error(errors,
			QString("MonitorQtBinder: Expected widget of class %1 and was %2.")
				.arg(QString(_plotClassName))
				.arg(uiElement->metaObject()->className())
				);

		std::string portName;
		if (uiElement->objectName().startsWith(_prefix))
			portName = widget2NetworkName(uiElement->objectName());
		else if (hasProperty(uiElement, _portProperty))
			portName = uiElement->property(_portProperty).toString().toStdString();
		else return error(errors,
			QString("MonitorQtBinder: Widget name '%1' should start by with '%2', "
				"or have the '%3' custom property defined")
				.arg(uiElement->objectName())
				.arg(_prefix)
				.arg(_portProperty)
				);

		std::cout << "* " << _plotClassName << " connected to port '" << portName << "'" << std::endl;

		if (missingOutPort(portName,network,errors)) return false;

		MonitorProcessing * monitorProcessing = new MonitorProcessing;
		std::string monitorName = network.GetUnusedName("BinderMonitor");
		network.AddProcessing(monitorName, monitorProcessing);
		if (not network.ConnectPorts(portName, monitorName+".Input"))
		{
			// TODO: Remove monitorProcessing. Not a memory leak but an 
			// unconnected port turns the network unplayable
			return error(errors,
				// TODO: tell the user the data types
				QString("MonitorQtBinder: Unable to connect a monitor of type '%1' to out port '%2'. "
					"Do they handle different types of data?")
					.arg(_plotClassName)
					.arg(portName.c_str())
					);
		}
		MonitorWidget * plot = (MonitorWidget*) uiElement;
		plot->setDataSource(*monitorProcessing);
		return true;
	}
protected:
	std::string widget2NetworkName(const QString & widgetName)
	{
		return widgetName
			.mid(_prefix.length())
			.replace("___", " ")
			.replace("__", ".")
			.toStdString()
			;
	}
	static void Substitute(std::string & subject, const char * pattern, const char * substitution)
	{
		for (std::string::size_type position = subject.find(pattern, 0);
			position!=std::string::npos;
			position = subject.find(pattern, position))
		{
			subject.replace(position, strlen(pattern), substitution);
		}
	}
};

#endif//CLAM_MonitorQtBinder_hxx

