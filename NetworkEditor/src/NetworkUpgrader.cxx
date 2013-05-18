#ifdef CLAM_USE_PYTHON
#include "Python.h"
#endif

#include "NetworkUpgrader.hxx"

#ifdef CLAM_USE_PYTHON

#include <CLAM/EmbeddedFile.hxx>
#include <CLAM/Assert.hxx>
#include <iostream>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
namespace py=boost::python;

CLAM_EMBEDDED_FILE(migrationScript, "migrationScript")
CLAM_EMBEDDED_FILE(clamrefactor, CLAM_PREFIX "/share/clam/clamrefactor.py")

class NetworkUpgrader::Impl
{
public:
	std::string _result;
	std::string _errorMessage;
	const char * run(const std::string & networkContents);
	const char * takePythonError(const std::string & context);
	const char * setError(const std::string & error)
	{
		_errorMessage = error;
		return 0;
	}
};


NetworkUpgrader::NetworkUpgrader()
	: _impl(new Impl)
{
}
NetworkUpgrader::~NetworkUpgrader()
{
	delete _impl;
}

const char * NetworkUpgrader::run(const std::string & filename)
{
	return _impl->run(filename);
}

const char * NetworkUpgrader::errorMessage() const
{
	return _impl->_errorMessage.c_str();
}


const char * NetworkUpgrader::Impl::takePythonError(const std::string & context="")
{
	PyObject * pType=0;
	PyObject * pValue=0;
	PyObject * pTrace=0;
	PyErr_Fetch(&pType, &pValue, &pTrace);

	_errorMessage = context;

	py::object type(py::handle<>(py::allow_null(pType)));
	_errorMessage += py::extract<std::string>(type.attr("__name__"));
	_errorMessage += ": ";

	_errorMessage += py::extract<std::string>(
		py::object(py::handle<>(py::allow_null(pValue))));

	if (pTrace)
	{
		_errorMessage += "\nTraceback:\n";
		py::object trace(py::handle<>(py::allow_null(pTrace)));
		py::object tb = py::import("traceback");
		py::str tracebackstring = py::str("").join(
				tb.attr("format_tb")(trace));
		_errorMessage += py::extract<std::string>(tracebackstring);
	}
	std::cerr << _errorMessage << std::endl;
	return 0;
}

const char * NetworkUpgrader::Impl::run(const std::string & filename)
{
	try
	{
		if (not Py_IsInitialized()) Py_Initialize();
		py::object _main = py::import("__main__");
		py::object _main_ns = _main.attr("__dict__");
		py::object builtin = py::import("__builtin__");
		// Simulate that we have a working command line
		py::exec("import sys" , _main_ns);
		py::exec("sys.argv=['clamrefactor.py']\n", _main_ns);
		// Not __main__ so it is run as it was a module, not a script
		_main.attr("__name__") = "NetworkUpgrader";
		// Run the embedded script
		_main.attr("__file__") = "(binary embeded) clamrefactor.py";
		py::exec(clamrefactor, _main_ns);

		py::object network = _main.attr("ClamNetwork")(filename);
		network.attr("runScript")(std::string(migrationScript));
		py::object stringio = py::import("StringIO").attr("StringIO")();
		network.attr("dump")(stringio);
		_result = py::extract<std::string>(stringio.attr("getvalue")());
		return _result.c_str();
	}
	catch (py::error_already_set & e)
	{
		return takePythonError();
	}
}

#else // CLAM_USE_PYTHON not defined

NetworkUpgrader::NetworkUpgrader()
	: _impl(0)
{
}
NetworkUpgrader::~NetworkUpgrader()
{
}

const char * NetworkUpgrader::run(const std::string & filename)
{
	return 0;
}

const char * NetworkUpgrader::errorMessage() const
{
	return "Python support is disabled. Python is required to upgrade networks on the fly.";
}

#endif

