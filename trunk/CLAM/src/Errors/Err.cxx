/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
		if ( mMsg )
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
		Embed(e.what());
	}

	void Err::Embed(const char* str) throw()
	{
		static const char* separation = "\n Nested error: ";
		size_t msgLen = strlen(mMsg);
		size_t sepLen = strlen(separation); 
		size_t strLen = strlen(str);
		size_t len;
		char* msg;
		len = msgLen + sepLen + strLen + 1;
		msg = new(std::nothrow) char[len];
		if (!msg) return;
		strcpy(msg,mMsg);
		strcpy(msg+msgLen,separation);
		strcpy(msg+msgLen+sepLen,str);
		delete [] mMsg;
		mMsg = msg;
	}

}

