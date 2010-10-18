#include "Python.h"

#include "NetworkUpgrader.hxx"
#include <CLAM/EmbeddedFile.hxx>
#include <CLAM/Assert.hxx>
#include <iostream>

CLAM_EMBEDDED_FILE(migrationScript,"migrationScript")
CLAM_EMBEDDED_FILE(clamrefactor,"../CLAM/scripts/clamrefactor.py")



static std::string pythonString(PyObject * object)
{
	PyObject * pythonString = PyObject_Str(object);
	std::string result = PyString_AsString(pythonString);
	Py_DECREF(pythonString);
	return result;
}
static std::string pythonClassName(PyObject * object)
{
	PyObject * pystring = PyObject_GetAttrString(object,"__name__");
	std::string result = PyString_AsString(pystring);
	Py_DECREF(pystring);
	return result;
}

class NetworkUpgrader::Impl
{
public:
	std::string _result;
	std::string _errorMessage;
	const char * run(const std::string & networkContents);
	const char * takePythonError();
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
	PyObject *pType=0;
	PyObject *pValue=0;
	PyObject *pTrace=0;
	PyErr_Fetch(&pType, &pValue, &pTrace);
	CLAM_ASSERT(pType,"pType es zero");
	CLAM_ASSERT(pValue,"pValue es zero");
	_errorMessage = 
		pythonClassName(pType) + ": " +
		pythonString(pValue);
//		(pTrace? pythonString(pTrace):"");

	std::cout << _errorMessage << std::endl;
	
	if (pTrace) Py_DECREF(pTrace);
	Py_DECREF(pValue);
	if (pType) Py_DECREF(pType);
	Py_Finalize();
	return 0;
}

const char * NetworkUpgrader::Impl::run(const std::string & filename)
{
	Py_Initialize();
	PyObject * pModule = PyImport_ImportModule("__main__");
	if (not pModule)
		return takePythonError();

	PyObject * pDict = PyModule_GetDict(pModule);
	// Hack to avoid __main__ exectuion
	PyDict_SetItemString(pDict, "__name__", Py_BuildValue("s","boo"));

	int error = PyRun_SimpleString(clamrefactor);
	if (error)
	{
		_errorMessage = "Error loading clamrefactor python script";
		return 0;
	}

	PyObject * pClassClamNetwork = PyDict_GetItemString(pDict, "ClamNetwork");
	if (!PyCallable_Check(pClassClamNetwork))
		return takePythonError();

	PyObject * pClamNetwork = Py_BuildValue( "(s)", filename.c_str() );
	PyObject * pInstanceClamNetwork = PyObject_CallObject(pClassClamNetwork, pClamNetwork); 
	Py_DECREF(pClamNetwork);
	if (pInstanceClamNetwork == NULL)
		return takePythonError();

	PyObject * pValue = PyObject_CallMethod(
		pInstanceClamNetwork, "runScript", "(s)", migrationScript );
	if (not pValue)
		return takePythonError();

	PyObject * pModule2 = PyImport_ImportModule("StringIO");
	if (pModule2 == NULL)
		return takePythonError();

	PyObject * pDict2 = PyModule_GetDict(pModule2);
	PyObject * pClassStringIO = PyDict_GetItemString(pDict2, "StringIO");
	PyObject * pStringIO = Py_BuildValue( "(s)", "" );
	PyObject * pInstanceStringIO = PyObject_CallObject(pClassStringIO, pStringIO);
	if (!PyCallable_Check(pClassStringIO))
		return takePythonError(); 
	PyObject_CallMethod(pInstanceClamNetwork, "dump", "(O)", pInstanceStringIO);
	pValue = PyObject_CallMethod(pInstanceStringIO, "getvalue", NULL);

	char * cstring;
	if (pValue == NULL)
		return takePythonError();

	PyArg_Parse(pValue, "s", &cstring);
	Py_DECREF(pModule);
	Py_DECREF(pModule2);
	Py_DECREF(pStringIO);
	Py_DECREF(pInstanceClamNetwork);
	Py_DECREF(pInstanceStringIO);
	Py_DECREF(pValue);
	Py_Finalize();
	_result = cstring;
	return _result.c_str();
}


