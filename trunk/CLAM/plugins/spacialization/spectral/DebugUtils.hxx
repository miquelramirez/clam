#ifndef spectral_DebugUtils_hxx
#define spectral_DebugUtils_hxx

#include <string>
#include <iosfwd>
#if defined(__linux__)
#include <execinfo.h>
#endif

#ifdef __GNUC__
#include <cxxabi.h> // For demangling symbols
#endif//__GNUC__


namespace spectral
{

	std::string demangle(const std::string & mangledName);
	void printBackTrace(std::ostream & os);

}

#endif//spectral_DebugUtils_hxx
