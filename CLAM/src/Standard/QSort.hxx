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

/*Class that implements sorting functionality for a template array. Is based
on qsort algorithm*/

#ifndef _QSort_
#define _QSort_

#include "Array.hxx"//for template array functionality
#include "IndexArray.hxx"

#ifdef __mac_os
#include <stdlib.h>// for qsort

#else
#include <search.h>//for qsort

#endif

namespace CLAM {

template <class T> class QSort//class T must match that in array
{

public:

/*Constructor*/
	QSort(const Array<T>& array)
		:mArray(array)
	{
	}

	
/*Sort procedure, calls qsort function*/
	void Sort(void) {
		spCurrent = this;
		qsort((mArray.GetPtr()),mArray.Size(),sizeof(T),sCompare);
	}

/*Sort procedure, calls qsort function*/
	void SortIndexArray(IndexArray& indexArray) {
		spCurrent = this;
		mpIndexArray = &indexArray;
		qsort((mpIndexArray->GetPtr()),mpIndexArray->Size(),sizeof(int),
			sIndexCompare);
	}

private:
	Array<T>const & mArray;//pointer to member array
	IndexArray *mpIndexArray;
	
	/*Virtual compare procedure to use with qsort, 'cheating' to be able to define
	a static sCompare procedure that uses class T*/
	static QSort<T>* spCurrent;

	static int sCompare(const void *a,const void* b)
	{
		return spCurrent->Compare((const T&) *(T*) a,(const T&) *(T*) b);
	}

	static int sIndexCompare(const void *a,const void* b)
	{
		TIndex ia = *(TIndex*)a;
		TIndex ib = *(TIndex*)b;
		return spCurrent->Compare(
			((spCurrent->mArray))[ia],((spCurrent->mArray))[ib]
		);
	}

	virtual int Compare(const T& a,const T& b) { return (a<b) ? -1 : (a==b) ? 0 : 1; } 
};

/*for use with the sCompare routine*/
template <class T> QSort<T>* QSort<T>::spCurrent;

}

#endif
