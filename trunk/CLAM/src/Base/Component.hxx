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

///////////////////////////////////////////////////////////////////////////////
// Class Component: Abstract class for all components in the 
///////////////////////////////////////////////////////////////////////////////
// Description: 
///////////////////////////////////////////////////////////////////////////////

#ifndef _Component_
#define _Component_

#include "Storage.hxx"

namespace CLAM {
/**
 * Abstract class for objects that can have subitems to be stored on a 
 * storage.
 */
class Component {
// Construction/Destruction
public:
	//* The required virtual destructor
	virtual ~Component() {};
// Redifinible methods
public:
	/** 
	 * Stores component's subitems on the given Storage
	 * @param store The given storage where the subitem will be stored
	 * @see Storage
	 */
	virtual void StoreOn (Storage & store)=0;
	/**
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be loaded from
	 * @see Storage
	 */
	virtual void LoadFrom (Storage & store)=0;

	// TODO: Documentation for copy functions
	// TODO: Copy functions must be pure virtual
	virtual Component* ShallowCopy() const { return 0; };
	virtual Component* DeepCopy() const { return 0; };

	/**
	 * Return the class name
	 * @todo This function must be defined as pure virtual when all the components will define it.
	 */
	virtual const char * GetClassName () {return 0;}
};

}

#endif//_Component_

