#ifndef __FUNCTORBASE__
#define __FUNCTORBASE__

#ifndef __MWERKS__
#include "cbl_config.h"
#endif


#include <cstring>
#include <cstddef>

namespace CBL
{

class FunctorBase
{
	
public:
	
	typedef void (FunctorBase::*_MemFunc)();
	
	FunctorBase()
		: callee( 0 ), func( 0 )
	{
	}
	
	FunctorBase( const void* c, const void* f, _CSTD::size_t sz )
	{
		if ( c ) // This means that the callback is a member function
		{
			callee = (void *)c;
			_CSTD::memcpy( memFunc, f, sz );
		}
		else // This means that the callback is to a non-member function
		{
			func = f;
		}
	}
	
	// for evaluation conditions
	
	operator bool() const 
	{
		return func||callee;
	}
	
	class DummyInit
	{
	};
	
	void* callee;
	
	union
	{
		const void *func;
		char memFunc[sizeof(_MemFunc)];
	};
	
	virtual ~FunctorBase()
	{
	}
};

}

#endif // FunctorBase.hxx




