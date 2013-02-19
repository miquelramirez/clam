#include <boost/python.hpp>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include "sipunwrap.hxx"
#include "shibokenunwrap.hxx"
#include <QtUiTools/QUiLoader>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <iostream>

namespace py = boost::python;

QWidget * loadUiGeneric(const std::string & uifilename)
{
	QString uiFile = uifilename.c_str();
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
	file.close();
	if (userInterface)
	{
		if (userInterface->windowIcon().isNull())
			userInterface->setWindowIcon(QIcon(":/icons/images/Prototyper-icon.svg"));
	}
	return userInterface;
}


PyObject * loadUiPySide(const std::string & uifilename)
{
	QWidget * userInterface = loadUiGeneric(uifilename);
	PyObject * object = shibokenWrap(userInterface);
	return object;
}

PyObject * loadUi(const std::string & uifilename)
{
	QWidget * userInterface = loadUiGeneric(uifilename);
	PyObject * object = sipWrap(userInterface);
	return object;
}

QWidget * createWidgetGeneric(const std::string & className)
{
	QUiLoader loader;
	QString qClassName = className.c_str();
	if (not loader.availableWidgets().contains(qClassName))
	{
		// TODO: Turn this into an exception
		foreach (const QString & availableClassName, loader.availableWidgets())
			std::cout << availableClassName.toStdString() << std::endl;
	}
	return loader.createWidget(qClassName);
}

PyObject * createWidgetPySide(const std::string & className)
{
	QWidget * widget = createWidgetGeneric(className);
	return shibokenWrap(widget);
}

PyObject * createWidget(const std::string & className)
{
	QWidget * widget = createWidgetGeneric(className);
	return sipWrap(widget);
}

py::list availableWidgets()
{
	py::list result;
	QUiLoader loader;
	foreach (const QString & availableClassName, loader.availableWidgets())
		result.append(availableClassName.toStdString());
	return result;
}

BOOST_PYTHON_MODULE(PySide)
{
	// Keep 'using namespace' in the inner scope
	using namespace boost::python;

	def("loadUi",
		loadUiPySide,
		"Returns a QWidget filled with the given qt ui (PySide version)."
		);
	def("createWidget",
		createWidgetPySide,
		"Returns QWidget from a Qt plugin of the named class using Qt factories (PySide version)."
		);
	def("availableWidgets",
		availableWidgets,
		"Returns the names of the widget classes that can be instantiated by createWidget."
		);
}
