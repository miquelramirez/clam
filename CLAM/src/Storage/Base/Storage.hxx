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

//////////////////////////////////////////////////////////////////////
// Class Storage: The abstract class for storages.
// Description: 
// An storage is a place where you can save and/or retrieve objects 
// state. Subclases of Storage will implement the way the storage
// and the loading is done.
//////////////////////////////////////////////////////////////////////

#ifndef _Storage_
#define _Storage_

#include "Storable.hxx"
#include "Err.hxx"

namespace CLAM {


/**
 * @todo This class must subclass from somewhere
 * @todo This class must have semantic modifiers
 * @todo This class must be moved to some other file
 */
class ErrNotFound : public Err {
};

/**
 * The abstract class for storages.
 * An storage is a place where you can save and/or retrieve objects
 * state. Subclasses of Storage will implement the way the storage
 * and loading is done.
 */
class Storage {
// Construction/Destruction
public:
	//* The required virtual destructor
	virtual ~Storage() {};
// Operators for the Storage interface
public:
	/**
	 * Stores an storable object on the storage.
	 * <P>
	 * Concrete storages must implement this function as they
	 * need.
	 * <P>
	 * Once executed the function, no references to the actual
	 * storable or its subitems will be kept on the storage
	 * so the object itself can be deleted.
	 * 
	 * @param A pointer to the Storage object. No references
	 * will be keept on the storage.
	 */
	virtual void Store(const Storable & object)=0;
	/**
	 * Loads an storable object from the storage.
	 * <P>
	 * Concrete storages must implement this function as they
	 * need.
	 * <P>
	 * Once executed the function, no references to the actual
	 * storable or its subitems will be kept on the storage
	 * so the object itself can be deleted.
	 * 
	 * @param A pointer to the Storage object. No references
	 * will be keept on the storage.
	 * @returns false when the desired object is not present
	 * in the storage.
	 */
	virtual bool Load (Storable & object)=0;
};
}

#endif//_Storage_

