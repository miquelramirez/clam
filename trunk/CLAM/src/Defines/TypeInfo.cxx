#include <sstream>
#include <typeinfo>
#include "Assert.hxx"
#include "TypeInfo.hxx"
#include <cstring>

namespace CLAM
{

bool SameType(const std::type_info & type1, const std::type_info & type2)
{
	if (strcmp(type1.name(),type2.name())!=0) return false;
	if (type1!=type2)
	{
		std::ostringstream os;
		os 
			<< "Two instances of type " << type1.name() << ".\n"
			<< "Check that the type has a destructor defined in a cxx file or for different library versions.."
			<< std::flush;
		;
		CLAM_WARNING(true, os.str().c_str());
	}
	return true;
}

}

