#ifndef __ERRSYSTEM__
#define __ERRSYSTEM__

#include "Err.hxx"

namespace CLAM
{

	class LockError : public Err
	{
	public:
		LockError( const char* msg = 0 );
	};

	class ThreadResourceError : public Err
	{
	public:
		ThreadResourceError( const char* msg = 0);
	};
}

#endif // ErrSystem.hxx
