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

#include "Err.hxx"
#include <cstdio>
#include <cstring>
#include <new>

namespace CLAM {

/* invoking the default constructor */
	Err::Err() throw()
	{
		mMsg = 0;
	}

/* invoking the overloaded constructor */
	Err::Err(const char* msg) throw()
	{
		mMsg = new(std::nothrow) char[strlen(msg)+1];
		if (!mMsg) return;
		strncpy(mMsg,msg,strlen(msg)+1);
	}

	Err::Err(const Err& orig) throw()
	{
		mMsg = new(std::nothrow) char[strlen(orig.what())+1];
		if (!mMsg) return;
		strncpy(mMsg,orig.what(),strlen(orig.what())+1);
	}

	Err::~Err() throw()
	{
		delete [] mMsg;
	};

/* specifying the header's member function */
	void Err::Print(void) const throw()
	{	
		fprintf(stderr,"CLAM Error: ");
		if (mMsg)
			fprintf(stderr,mMsg);
		else
			fprintf(stderr,"Unknown Error");
		fprintf(stderr,"\n");
	}

	void Err::Embed(const std::exception &e) throw()
	{
		static const char* separation = "\n Nested error: ";
		int len;
		char* msg;
		len = strlen(mMsg) + strlen(e.what()) + strlen(separation) + 1;
		msg = new(std::nothrow) char[len];
		if (!msg) return;
		strncpy(msg,mMsg,len);
		strncat(msg,separation,len);
		strncat(msg,e.what(),len);
		delete [] mMsg;
		mMsg = msg;
	}

}
