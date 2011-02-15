#include <Python.h>


#include <CLAM/Network.hxx>
#include <CLAM/XMLStorage.hxx>

CLAM::Network network;
static PyObject * ClamError;
static PyObject * clam_error(const std::string & msg)
{
	PyErr_SetString(ClamError, msg.c_str());
	return NULL;
}

static PyObject * clam_load(PyObject * self, PyObject * args)
{
	const char * filename;
	if (!PyArg_ParseTuple(args, "s", &filename))
		return NULL;
	printf("Loading '%s'...\n", filename);
	try
	{
		CLAM::XmlStorage::Restore(network, filename);
	}
	catch (CLAM::Err & e)
	{
		return clam_error(e.what());
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * clam_xml(PyObject * self, PyObject * args)
{
	std::ostringstream os;
	CLAM::XmlStorage::Dump(network, "Network", os);
	return Py_BuildValue("s", os.str().c_str());
}

static PyObject * clam_dump(PyObject * self, PyObject * args)
{
	char * filename = 0;
	if (!PyArg_ParseTuple(args, "|s", &filename))
		return NULL;
	try
	{
		if (!filename)
			CLAM::XmlStorage::Dump(network, "Network", std::cout);
		else
			CLAM::XmlStorage::Dump(network, "Network", filename);
	}
	catch (std::exception & e)
	{
		return clam_error(e.what());
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * clam_connect_ports(PyObject * self, PyObject * args)
{
	char * source, * target;
	if (!PyArg_ParseTuple(args, "ss", &source, &target))
		return NULL;
	try
	{
		CLAM::OutPortBase & out = network.GetOutPortByCompleteName(source);
		CLAM::InPortBase & in = network.GetInPortByCompleteName(target);
		if (not out.IsConnectableTo(in))
		{
			return clam_error("Unable to connect ports");
		}
		network.ConnectPorts(source, target);
	}
	catch (std::exception & e)
	{
		return clam_error(e.what());
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * clam_disconnect_ports(PyObject * self, PyObject * args)
{
	char * source, * target;
	if (!PyArg_ParseTuple(args, "ss", &source, &target))
		return NULL;
	try
	{
		if (not network.DisconnectPorts(source, target))
			return clam_error("Unable to disconnect ports");
	}
	catch (std::exception & e)
	{
		return clam_error(e.what());
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * clam_add_processing(PyObject * self, PyObject * args)
{
	char * type;
	char * name = 0;
	if (!PyArg_ParseTuple(args, "s|s", &type, &name))
		return NULL;
	std::string actualName;
	try 
	{
		if (name)
		{
			actualName = name;
			network.AddProcessing(name, type);
		}
		else
		{
			actualName = network.AddProcessing(type);
		}
	}
	catch (std::exception & e)
	{
		return clam_error(e.what());
	}
	return Py_BuildValue("s",actualName.c_str());
}


static PyObject * clam_inports(PyObject * self, PyObject * args)
{
	char * processingName;
	if (!PyArg_ParseTuple(args, "s", &processingName))
		return NULL;
	if (!network.HasProcessing(processingName))
		return clam_error("No such processing");

	CLAM::Processing & processing = network.GetProcessing(processingName);
	PyObject * connectionList = PyList_New(0);
	for (unsigned i = 0; i<processing.GetNInPorts(); i++)
		PyList_Append(connectionList,
			Py_BuildValue("s", processing.GetInPort(i).GetName().c_str()));
	return connectionList;
}

static PyObject * clam_outports(PyObject * self, PyObject * args)
{
	char * processingName;
	if (!PyArg_ParseTuple(args, "s", &processingName))
		return NULL;
	if (!network.HasProcessing(processingName))
		return clam_error("No such processing");

	CLAM::Processing & processing = network.GetProcessing(processingName);
	PyObject * connectionList = PyList_New(0);
	for (unsigned i = 0; i<processing.GetNOutPorts(); i++)
		PyList_Append(connectionList,
			Py_BuildValue("s", processing.GetOutPort(i).GetName().c_str()));
	return connectionList;
}




extern "C" void initipyclam()
{
	static PyMethodDef ModuleMethods[] = {
		{"load",    clam_load, METH_VARARGS},
		{"xml",     clam_xml, METH_VARARGS},
		{"dump",     clam_dump, METH_VARARGS},
		{"add",     clam_add_processing, METH_VARARGS},
		{"connect_ports",     clam_connect_ports, METH_VARARGS},
		{"disconnect_ports",     clam_disconnect_ports, METH_VARARGS},
		{"inports",     clam_inports, METH_VARARGS},
		{"outports",     clam_outports, METH_VARARGS},
		{NULL,      NULL}        /* Sentinel */
	};

	ClamError = PyErr_NewException("ipyclam.Error", NULL, NULL);
	puts("initipyclam");
	Py_InitModule("ipyclam", ModuleMethods);
}



