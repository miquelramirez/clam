/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SDIFStorage__
#define __SDIFStorage__

#include "SDIFFrame.hxx"

namespace SDIF
{

	/** Storage is an abstract class, that is passed to File,
	* used to handle every read frame.
	* 
	*       The most common construction is to use a Collection that is derived
	*       from Storage, and pass it to File::Read() but it is possible
	*       to implement other ways of handling read frames.
	* @see Collection
	*/
	class Storage
	{
	public:
		typedef std::list<Frame*>::const_iterator FrameIterator;

		/** virtual function that where derived classes get the frames read by the
		* File. Note that the derived class is responsible for handling
		* the memory deallocation of the frames it gets passed.
		*/
		virtual void Add(Frame* pFrame) = 0;
		virtual FrameIterator Begin(void) const = 0;
		virtual FrameIterator End(void) const = 0;
	};

}
#endif

