#include "DebugUtils.hxx"
#include <cstdlib>
#include <ostream>

namespace spectral {
	std::string demangleStackFrame(const std::string & mangledName);
}

std::string spectral::demangle(const std::string & mangledName)
{
	/// Demangles a typeid name (just works with gcc)
	std::string result = mangledName;
	#ifdef __GNUC__
	int demangleError = 0;
	char * demangled = abi::__cxa_demangle(mangledName.c_str(),0,0,&demangleError);
	if (!demangleError && demangled)
		result = demangled;
	if (demangled) free(demangled);
	#endif//__GNUC__
	return result;
}

std::string spectral::demangleStackFrame(const std::string & mangledName)
{
	#ifdef __GNUC__
		size_t parenthesisPos = mangledName.find_first_of('(');
		if (parenthesisPos == std::string::npos) return mangledName;
		size_t endPos = mangledName.find_first_of("+", parenthesisPos);
		if (endPos == std::string::npos) endPos = mangledName.find_first_of(")", parenthesisPos);
		std::string head = mangledName.substr(0,parenthesisPos+1);
		std::string tail = mangledName.substr(endPos);
		std::string mangled = mangledName.substr(parenthesisPos+1, endPos-parenthesisPos-1);
		return head + demangle(mangled) + tail;
	#endif//__GNUC__
	return mangledName;
}

void spectral::printBackTrace(std::ostream & os)
{
#if defined(__linux__)

	void *bt_array[100];	// 100 should be enough ?!?
	char **bt_strings;
	int num_entries;

	if ((num_entries = backtrace(bt_array, 100)) < 0) {
		os << " Unable to generate a backtrace" << std::endl;
		return;
	}

	if ((bt_strings = backtrace_symbols(bt_array, num_entries)) == NULL) {
		os << " Unable to adquire symbols names for the backtrace" << std::endl;
		return;
	}

	os << "\n Backtrace:\n" << std::endl;
	for (int i = 0; i < num_entries; i++) {
		os << "[" << i << "] " <<  demangleStackFrame(bt_strings[i]) << std::endl;
	}
	free(bt_strings);
#else
	os << " Unable to adquire symbols names for the backtrace" << std::endl;
#endif
}

