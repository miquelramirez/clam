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

#include "ErrDynamicType.hxx"
#include <cstdio>


namespace CLAM {

	ErrDynamicType::ErrDynamicType()
	{
		mMsg = new char[250];
		snprintf(mMsg, 250, "\nDynamicType Error"); // Initialization of the error message
	}

	ErrDynamicType::ErrDynamicType(char* msg)
	{
		mMsg = new char[250];
		snprintf(mMsg, 250,"\nDynamicType Error: %s", msg); // Initialization of the error message
	}

	ErrDynamicType::ErrDynamicType(char* msg1, const char* msg2)
	{
		mMsg = new char[250];
		snprintf(mMsg, 250,"\nDynamicType Error: %s %s", msg1, msg2); // Initialization of the error message
	}

	ErrDynamicType::ErrDynamicType(char* msg1, const char* msg2, const char* msg3)
	{
		mMsg = new char[250];
		snprintf(mMsg, 250,"\nDynamicType Error: %s %s %s", msg1, msg2, msg3); // Initialization of the error message
	}

	ErrDynamicType::~ErrDynamicType() throw()
	{
	}

}
