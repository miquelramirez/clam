#ifndef __HEAPDBG__
#define __HEAPDBG__

#include "Assert.hxx"
#ifdef WIN32
#include <crtdbg.h>
#endif

#ifdef WIN32
#define CLAM_CHECK_HEAP( msg )\
CLAM_ASSERT( _CrtCheckMemory(), msg )
#else
#define CLAM_CHECK_HEAP( msg )
#endif

#endif // HeapDbg.hxx
