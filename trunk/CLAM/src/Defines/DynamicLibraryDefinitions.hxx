#ifndef __DynamicLibraryDefinitions_hxx__
#define __DynamicLibraryDefinitions_hxx__

// Defines needed to build the windows dll
#ifdef WIN32
#	ifdef _USRDLL
#		define CLAM_DLL_EXPORT _declspec(dllexport)
#	else
#		define CLAM_DLL_EXPORT _declspec(dllimport)
#	endif
#else
#	define CLAM_DLL_EXPORT
#endif

#endif // __DynamicLibraryDefinitions_hxx__

