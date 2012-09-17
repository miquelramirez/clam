#include "shibokenunwrap.hxx"
#include <shiboken/basewrapper.h>
#include <shiboken/typeresolver.h>
#include <shiboken/conversions.h>
#include <iostream>
#include <QtCore/QObject>

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


