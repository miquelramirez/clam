#include "Python.h"

#include "NetworkUpgrader.hxx"
#include <CLAM/EmbeddedFile.hxx>
#include <CLAM/Assert.hxx>
#include <iostream>

CLAM_EMBEDDED_FILE(migrationScript,"migrationScript")
CLAM_EMBEDDED_FILE(clamrefactor,"../CLAM/scripts/clamrefactor.py")


/**
	Scoped python initialization and finalization
*/
class PyContext
{
public:
	PyContext() { Py_Initialize(); }
	~PyContext() { Py_Finalize(); }
};

/**
	Wraps a PyObject borrowed reference (no need to Py_DECREF)
*/
typedef PyObject * PyBorrowed;

/**
	Wraps a PyObject* new reference which must be Py_DECREF.
*/
class PyReference
{
	typedef PyObject * Pointer;
	Pointer _obj;
public:
	PyReference(Pointer obj, bool skipCheck=false)
		: _obj(obj)
	{
	}
	~PyReference()
	{
		if (_obj) { Py_DECREF(_obj);}
	}
	operator bool()
	{
		return _obj!=0;
	}
	operator Pointer()
	{
		CLAM_ASSERT(_obj,"Accessing a null python object");
		return _obj;
	}
	PyObject & operator*()
	{
		CLAM_ASSERT(_obj,"Accessing a null python object");
		return *_obj;
	}
};

static std::string pythonString(PyObject * object)
{
	PyReference pythonString = PyObject_Str(object);
	return PyString_AsString(pythonString);
}

static std::string pythonClassName(PyObject * object)
{
	PyReference pystring = PyObject_GetAttrString(object,"__name__");
	return PyString_AsString(pystring);
}


class NetworkUpgrader::Impl
{
public:
	std::string _result;
	std::string _errorMessage;
	const char * run(const std::string & networkContents);
	const char * takePythonError();
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



const char * NetworkUpgrader::Impl::takePythonError()
{
	std::cout << "Taking error" << std::endl;
	PyObject *pType=0;
	PyObject *pValue=0;
	PyObject *pTrace=0;
	PyErr_Fetch(&pType, &pValue, &pTrace);
	CLAM_ASSERT(pType,"pType es zero");
	CLAM_ASSERT(pValue,"pValue es zero");
	PyReference type = pType;
	PyReference value = pValue;
//	PyReference trace = pTrace;
	_errorMessage = 
		pythonClassName(pType) + ": " +
		pythonString(pValue);
//		(pTrace? pythonString(pTrace):"");

	std::cerr << _errorMessage << std::endl;
	return 0;
}

const char * NetworkUpgrader::Impl::run(const std::string & filename)
{
	PyContext context;
	PyReference main = PyImport_ImportModule("__main__");
	if (not main)
		return takePythonError();

	PyBorrowed mainDict = PyModule_GetDict(main);
	// Hack to avoid __main__ exectuion
	{
		PyReference boo =  Py_BuildValue("s","boo");
		PyDict_SetItemString(mainDict, "__name__", boo);
	}

	int error = PyRun_SimpleString(clamrefactor);
	if (error)
		return setError("Error loading clamrefactor python script");

	PyBorrowed clamNetworkClass = PyDict_GetItemString(mainDict, "ClamNetwork");
	if (!clamNetworkClass)
		return setError("Bad clamrefactor script: No ClamNetwork class found");
	if (!PyCallable_Check(clamNetworkClass))
		return setError("Bad clamrefactor script: ClamNetwork is not callable");

	PyReference networkFilename = Py_BuildValue( "(s)", filename.c_str() );
	PyReference network = PyObject_CallObject(clamNetworkClass, networkFilename); 
	if (not network)
		return takePythonError();

	PyReference runScriptResult = PyObject_CallMethod(
		network, "runScript", "(s)", migrationScript );
	if (not runScriptResult)
		return takePythonError();

	PyReference stringIOModule = PyImport_ImportModule("StringIO");
	if (!stringIOModule)
		return setError("Unexpected error: No StringIO module found");
	PyBorrowed stringIODict = PyModule_GetDict(stringIOModule);
	PyBorrowed stringIOClass = PyDict_GetItemString(stringIODict, "StringIO");
	if (!stringIOClass)
		return setError("Unexpected error: No StringIO class found");
	if (!PyCallable_Check(stringIOClass))
		return setError("Unexpected error: StringIO is not callable");

	PyReference initStreamValue = Py_BuildValue( "(s)", "" );
	PyReference stream = PyObject_CallObject(stringIOClass, initStreamValue);
	PyReference(PyObject_CallMethod(network, "dump", "(O)", &*stream));
	PyReference result = PyObject_CallMethod(stream, "getvalue", NULL);
	_result = pythonString(result);

	return _result.c_str();
}


