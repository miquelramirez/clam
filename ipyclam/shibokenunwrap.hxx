#ifndef shibokenpunwrap_hxx
#define shibokenpunwrap_hxx

#include <Python.h>

class QObject;
void * shibokenUnwrap(PyObject *obj_ptr);
PyObject * shibokenWrap(QObject * object);

#endif//shibokenpunwrap_hxx
