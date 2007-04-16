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

#ifndef _Err_
#define _Err_

#include <exception>

namespace CLAM {

/**
 * Base class for the exception handling classes
 * <p>
 * Exception handling allows code that has encountered a condition it
 * cannot cope with to return to other code, avoiding crashes by
 * switching to the writing of error messages, managed by tbis base
 * class.
 * <p>
 * If the call invokes the overloaded constructor, then messages can
 * be defined either directly within the code invoking Err or in the
 * derived class (error regarding typically formats, memory or file
 * openings).
 * <p>
 * If the call invokes the default constructor, then the message is
 * set to "Unknown error"
 * <p>
 * One member function : Print
 * One data member : mMsg 
 */
	class Err : public std::exception
	{
	protected:
		char* mMsg;
	public:
		Err() throw();
		Err(const char* msg) throw();
		Err(const Err&) throw();

		virtual ~Err() throw();

		void Print(void) const throw();

		virtual const char* what() const throw() 
		{
			return mMsg?mMsg:"Unknown Error"; 
		}

		void Embed(const std::exception &) throw();
		void Embed(const char* str) throw();

	};

}

#endif

