#include "XmlStorageErr.hxx"
#include <string>


namespace CLAM
{
	XmlStorageErr::XmlStorageErr(const std::string & errors)
	{
		_errors = errors;
	}
	const char * XmlStorageErr::what() const throw()
	{
		return _errors.c_str();
	}

}


