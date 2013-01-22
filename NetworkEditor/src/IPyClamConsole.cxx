#include "IPyClamConsole.hxx"
#include <QtGui/QLabel>

#ifndef CLAM_USE_PYTHON
QWidget * GetIPyClamConsole(CLAM::Network & network)
{
	return 0; // Python disabled, no console
}
#else

#include <boost/python.hpp>
#include <QtGui/QHBoxLayout>
#include <shiboken/basewrapper.h>
#include <shiboken/typeresolver.h>
#include <shiboken/conversions.h>

namespace py=boost::python;
static void * error(const std::string & msg)
{
	std::cerr << msg << std::endl;
	return 0;
}

void * shibokenUnwrap(PyObject * pyobject)
{
	if (not Shiboken::Object::checkType(pyobject))
		return error("Not a shiboken object");

	SbkObject * sbkobject = (SbkObject *) pyobject;

	PyTypeObject * type = Shiboken::SbkType<QObject>();

	void * cppobject = Shiboken::Object::cppPointer(sbkobject, type);
	if (not cppobject)
		return error("Not a QObject");

	return cppobject;
}

PyObject * shibokenWrap(QObject * qobject)
{
	return Shiboken::createWrapper(qobject, /*python owns*/ false);
}

QWidget * GetIPyClamConsole(CLAM::Network & network)
{
	try
	{
		Py_Initialize();
		// Dummy __main__ namespace, to run execs
		py::object _main = py::import("__main__");
		py::object _main_ns = _main.attr("__dict__");
		// Adding working dir to the Python search path
		py::exec("import sys; sys.path.append('.')" , _main_ns, _main_ns);
		// Simulate that we have a working command line (expected by IPython)
		py::exec("sys.argv=['ipyclam']\n", _main_ns, _main_ns);
		// Build an ipyclam network having the CLAM network as backend
		py::object ipyclamModule = py::import("ipyclam");
		py::object proxy = py::object(py::ptr(&network)); // The proxy backend
		py::object net = ipyclamModule.attr("Network")(proxy); // The ipyclam network
		// Creating the IPython based console widget

		py::object consoleModule = py::import("ipyclam.ipyclam_qtconsole");
		py::object console = consoleModule.attr("IPythonConsoleQtWidget")();

		// Injecting the network into the namespace
		console.attr("namespace_inject")("net", net);

		// Unwrapping the PySide based qt console to use it as a abstract QWidget
		QWidget * consoleWidget = (QWidget*) shibokenUnwrap(console.ptr());
		return consoleWidget;
	}
	catch (py::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
		return 0;
	}

}



#endif//CLAM_USE_PYTHON
