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

#ifndef _ErrProcessingObj_
#define _ErrProcessingObj_

#include "Err.hxx"

namespace CLAM {

	class Processing;

	/** Derived class of the Err base class
	 * Handles errors in Processing Object Routines
	 */

	// WARNING: This class has been renamed for ErrProcessingObj
	// Keeping a define for backwards compatibility
	class ErrProcessingObj : public Err {

	public:

		ErrProcessingObj();

		ErrProcessingObj(const char* msg);

		ErrProcessingObj(const char* msg,const Processing *o);

		~ErrProcessingObj() throw() {} 

	};

}

#endif

