#include "sipunwrap.hxx"
#include <boost/python.hpp>
#include <iostream>

#if SIP_API_MAJOR_NR <4
#error Requires at least sip>=4
#endif

const sipAPIDef *sip_api()
{
	static const sipAPIDef *sip_API = 0;
	if (sip_API) return sip_API;
#if defined(SIP_USE_PYCAPSULE)
	sip_API = (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
	/* Import the SIP module. */
	PyObject * sip_module = PyImport_ImportModule("sip");
	if (sip_module == NULL) return NULL;

	/* Get the module's dictionary. */
	PyObject * sip_module_dict = PyModule_GetDict(sip_module);

	/* Get the "_C_API" attribute. */
	PyObject * c_api = PyDict_GetItemString(sip_module_dict, "_C_API");
	if (c_api == NULL) return NULL;

	/* Sanity check that it is the right type. */
	if (!PyCObject_Check(c_api)) return NULL;

	/* Get the actual pointer from the object. */
	sip_API = (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
	return sip_API;
}

void * sipUnwrap(PyObject *obj_ptr)
{
	if (!PyObject_TypeCheck(obj_ptr, sip_api()->api_wrapper_type))
	{
		PyErr_SetString(PyExc_TypeError, "Not a wraped type");
		boost::python::throw_error_already_set();
	}

	// transfer ownership from python to C++
	sip_api()->api_transfer_to(obj_ptr, 0);
	// reinterpret to sipWrapper
	sipSimpleWrapper *wrapper = reinterpret_cast<sipSimpleWrapper*>(obj_ptr);
	#if (SIP_API_MAJOR_NR == 8 && SIP_API_MINOR_NR >= 1) || SIP_API_MAJOR_NR > 8
	return sip_api()->api_get_address(wrapper);
	#elif SIP_API_MAJOR_NR == 8
	return wrapper->data;
	#else
	return wrapper->u.cppPtr;
	#endif
}


PyObject * incref(PyObject * object)
{
	Py_INCREF(object);
	return object;
}

PyObject * sipWrap(QObject * object)
{
	if (!object) return incref(Py_None);
	std::cout << "Not null" << std::endl;

#if SIP_API_MAJOR_NR >=4
	const sipTypeDef *type = sip_api()->api_find_type("QObject");
#else     
	sipWrapperType *type = sip_api()->api_find_class("QObject");
#endif
	if (!type) return incref(Py_None);
	std::cout << "Type exists" << std::endl;

#if SIP_API_MAJOR_NR >=4
	PyObject *sip_obj = sip_api()->api_convert_from_type(object, type, 0);
#else
	PyObject *sip_obj = sip_api()->api_convert_from_instance(object, type, 0);
#endif
	if (!sip_obj) return incref(Py_None);
	std::cout << "Convertible" << std::endl;
	PyObject_Print(sip_obj, stdout, Py_PRINT_RAW);
	return incref(sip_obj);
}

