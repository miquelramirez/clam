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

///////////////////////////////////////////////////////////////////////////////
// Class Storable: Interface for objects to be stored on a Storage
///////////////////////////////////////////////////////////////////////////////
// Description: 
// Subclasses of this abstract class define interfaces needed to store objects
// on Storage subclasses.
// All common queries about storability must be defined here as virtual 
// functions
///////////////////////////////////////////////////////////////////////////////

#ifndef _Storable_
#define _Storable_

namespace CLAM {
/**
 * Interface for objects to be stored on a Storage.
 * <P>
 * Subclasses of this abstract class define interfaces needed to store 
 * objects on each Storage subclasses.
 * <P>
 * All common queries about storability must be defined here as virtual
 * functions.
 */
class Storable {
// Construction/Destruction
public:
	//* The required virtual destructor
	virtual ~Storable() {};
// Redifinible methods
public:
//	virtual bool isXMLable()=0;
//	virtual void beXMLable(bool amIXMLable=true)=0;
};

}

#endif//_Storable_

