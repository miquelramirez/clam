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

#include "ErrProcessingObj.hxx"
#include "Processing.hxx"
#include <cstdio>

namespace CLAM {

	ErrProcessingObj::ErrProcessingObj()
	{
		mMsg = new (std::nothrow) char[1024];
		if (!mMsg) return;
		snprintf(mMsg, 1024, "Processing Error"); // Initialization of the error message
	}


	ErrProcessingObj::ErrProcessingObj(const char* msg,const Processing *o)
	{
		mMsg = new (std::nothrow) char[1024];
		if (!mMsg) return;
		if (!o)
			snprintf(mMsg,1024,"Processing Error: %s", msg); // Initialization of the error message
		else
//			snprintf(mMsg,1024,"Error in processing object %s: \n%s", o->GetFullName().c_str(),msg);
			snprintf(mMsg,1024,"Error in processing object %s: \n%s", o->GetClassName(),msg);
	}

}
