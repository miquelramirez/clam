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

/*
This template class describes an object that controls 
a varying-length sequence of elements. 

The initialization is either the reading of an existing sequence of 
elements (a Template Array) or the creation of an empty queue; in both 
cases a maximum size is defined.

The use of the step is twofold: The user can follow the efficient memory 
allocation philosophy implemented in the Array's step, i.e. he can set a 
step greater than 1 and keep on using the Add, Read and Remove functions that 
process elements individually. 
The other way to use the step is to set a step equal to the number of elements 
the user wishes to be processed simultaneously, and use the overloaded 
functions.

One can also precise  the processing mode (fifo or lifo, others could be 
added e.g. random...).
*/

#ifndef _Queue_
#define _Queue_

#include "Array.hxx"
#include "Err.hxx"
#include <stdio.h>

namespace CLAM {

enum Emode//processing mode of the sequence of elements
{
	fifo,
	lifo
};

template <class T> class Queue
{
public:
	enum {defaultMaxSize=100};
/* Constructors */
//An empty queue is created to be processed by the member functions	
	Queue(int sizemax = defaultMaxSize, Emode mode = fifo)
		:mQueueBuffer(0, 1) 
	{
		if (sizemax > 0)
		{
			mSizeMax = sizemax; 
			mMode = mode;
		}
		else throw Err("(in Queue) senseless size of queue");
	}
//Same thing but the step is defined by the user
	Queue(int step, int sizemax = defaultMaxSize, Emode mode = fifo)
		:mQueueBuffer(0,step) 
	{
		if ((sizemax < 0) || (step < 0))
			throw Err("(in Queue) senseless construction of queue");
		else
		{
			mSizeMax = sizemax; 
			mMode = mode;
		}
	}
//A queue is created copying an existing sequence of elements, the step
// is already defined in the array
	Queue(Array<T>& src, int sizemax = defaultMaxSize, 
		Emode mode = fifo):mQueueBuffer(src.Size())
	{
		if (sizemax > 0)
		{
			mQueueBuffer = src; 
			mSizeMax = mQueueBuffer.Size()+sizemax; 
			mMode = mode;
		}
		else throw Err("(in Queue) senseless size of queue");
	}

/* Destructor */
	~Queue() {}

/* Data members */
protected:
	Array<T> mQueueBuffer;
	int mSizeMax; //maximum size allowed for the queue
	Emode mMode;

public:
/* member functions */

	int UsedElem(void) const 
	{
		return mQueueBuffer.Size(); 
	}

	int AvlbleElem(void) const 
	{
		return mSizeMax-mQueueBuffer.Size();
	}

	void Empty()
	{
		mQueueBuffer.Resize(0);
		mQueueBuffer.SetSize(0);
	}
	
//Add at the end of the buffer in both mode
	void Add(const T& x)
	{
		if (mQueueBuffer.Size()+1 > mSizeMax)
			throw Err("(in Queue::Add) Not enough available space in queue, change the maximum size");
		else mQueueBuffer.AddElem(x);
	}
//The user must define an element of type T as input, yielded back as output
//with the value read.
	void Read(T& elem) 
	{
			if (mQueueBuffer.Size() <= 0 )
				throw Err("(in Queue::Read) Queue already empty");
			else 
			{
			//Read at the end of the buffer if lifo
			//Read at the beginning if fifo
				if (mMode == lifo)
				{
					elem = mQueueBuffer[mQueueBuffer.Size()-1];
					mQueueBuffer.DeleteElem(mQueueBuffer.Size()-1);
				}
				else
				{
					elem = mQueueBuffer[0];					
					mQueueBuffer.DeleteElem(0);
				}
			}
	}
//Remove at the end of the buffer if lifo
//Remove at the beginning if fifo
	void Remove()
	{
		if (mQueueBuffer.Size() <= 0)
			throw Err("(in Queue::Remove) Queue already empty");
		else 
		{
			if (mMode == lifo)
				mQueueBuffer.DeleteElem(mQueueBuffer.Size()-1);
			else mQueueBuffer.DeleteElem(0);
		}
	}


/* Overloaded functions for step != 1*/

//The user should change the step before using this overloaded function
	void Add(const Array<T>& addArray)
	{
		if (addArray.Size() != mQueueBuffer.Step())
			throw Err("the size of the array must be equal to the step");
		else
		{
			if (mQueueBuffer.Size()+mQueueBuffer.Step() > mSizeMax)
				throw Err("(in Queue::Add) Not Enough available space in queue, change the maximum size or the adding step");
			else
			{
				for (int i=0;i<=addArray.Size()-1; i++)
					mQueueBuffer.AddElem(addArray[i]);
			}
		}
	}
//The user must create an array of T for the function to fill
	void Read (Array<T>& readArray)
	{
		if (readArray.Size() != mQueueBuffer.Step())
			throw Err("(in Queue::Read) the size of the array to fill must be equal to the step");
		else
		{
			if (mQueueBuffer.Step() > mQueueBuffer.Size())
				throw Err("(in Queue::Read) the step is bigger than the size of the queue");
			else
			{
				if (mMode == lifo)
				 	for (int i=0; i<=readArray.Size()-1; i++)
					{
						readArray[i] = mQueueBuffer[mQueueBuffer.Size()
									-readArray.Size()+i];
						mQueueBuffer.DeleteElem(mQueueBuffer.Size()
							-readArray.Size()+i);
					}
				else //fifo
				{
					for (int i=0; i<=readArray.Size()-1; i++)
					{
						readArray[i] = mQueueBuffer[0];
						mQueueBuffer.DeleteElem(0);
					}
				}
			}
		}
	}
	void Remove(int notused)
	{
		if (mQueueBuffer.Size() <= 0)
			throw Err("(in Queue::Remove) Queue already empty");
		else 
		{
			for (int i=1; i<=mQueueBuffer.Step(); i++)
			{
				if (mMode == lifo)
					mQueueBuffer.DeleteElem(mQueueBuffer.Size()-1);
				else mQueueBuffer.DeleteElem(0);
			}
		}
	}

	void ChgeStep(int newStep)
	{
		if (newStep+mQueueBuffer.Size() > mSizeMax)
			throw Err("(in Queue::ChgeStep) step too big regarding sizemax");
		else
			mQueueBuffer.SetStep(newStep);
	}
};

}

#endif

/* 
Add a function to change the mode?
*/
