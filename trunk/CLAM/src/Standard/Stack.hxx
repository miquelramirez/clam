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

/* Simple stack class with push and pull funcionality.
  Template
  The stack is defined as an Array of T */

 
#ifndef _Stack_
#define _Stack_

#include "Array.hxx"
#include "Err.hxx"

namespace CLAM {
template <class T> class Stack
{

public:

// CONSTRUCTORS

	/* Constructor empty Stack */
	Stack()
		:mStackBuffer(0) // 0 length buffer
	{ 
		top = -1;
	}
		
	/* Constructor empty Stack with size = initialsize */
	Stack(int initialsize)
		:mStackBuffer(0)
	{
		top = -1;
		SetSize(initialsize);
	}

// OPERATIONS
	
	/* Push an element */
	void Push (T n)
	{
		if( (top+1) >= size)
			throw "Full Stack";
		else {
			top++ ;
			mStackBuffer[top]= n;
		}
	}
	
	/* Pop an element */
	T Pop()
	{
		if ( top < 0)
			throw "Empty Stack";
		else {
			top--;
			return mStackBuffer[top+1];
		}
	}

	/* Empty Stack */
	void Empty()
	{
		top = -1;
		mStackBuffer.Resize(0);
		mStackBuffer.SetSize(0);
	}

	/* Get stack size */
	int GetSize () const {return size;}

	/* Set size to newsize */
	void SetSize (int newsize)
	{
		size = newsize;
		mStackBuffer.Resize(newsize);
		mStackBuffer.SetSize(newsize);
	}

	/* Is the stack empty? */
	bool IsEmpty () const { return (!(top+1)); }
	
	/* Return the top element of the stack without popping it */
	T& Top() const { return (mStackBuffer[top]); }

	/* Return de available size on the stack */
	int FreeSize() const { return (size - (top+1)); }

	friend EDataFormat DataFormat( Stack<T>&) { return eFmtDefault; }

protected:
	
	// Buffer
	Array<T> mStackBuffer; 
 
	// Top of the stack (-1 if empty)
	int top;

	// Maximun size
	int size;
};

}

#endif
