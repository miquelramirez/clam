#ifndef sipunwrap_hxx
#define sipunwrap_hxx

#include <sip.h>

class QObject;
void * sipUnwrap(PyObject *obj_ptr);
PyObject * sipWrap(QObject * object);

#endif//sipunwrap_hxx
