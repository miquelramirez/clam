/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __FUNCTORBASE__
#define __FUNCTORBASE__

//#ifndef __MWERKS__
//#include "cbl_config.h"
//#endif


#include <cstring>
#include <cstddef>

namespace CBL
{

class FunctorBase
{
	class A
	{
	};

	class B
	{
	};

	class AB : public A, public B
	{
	};


public:
	
	typedef void (AB::*_MemFunc)();
	
	FunctorBase()
		: callee( 0 ), func( 0 )
	{
	}
	
	FunctorBase( const void* c, const void* f, size_t sz )
	{
		if ( c ) // This means that the callback is a member function
		{
			callee = (void *)c;
			memcpy( memFunc, f, sz );
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




