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

#ifndef _SearchList_
#define _SearchList_

#include "OSDefines.hxx"
#include "List.hxx"

namespace CLAM {

template <class T> class SearchList
{
private:
	List<T>* mpList;
	Elem<T>* mpPrevElem;
	TIndex mPrevIndex;
public:
	SearchList()
	{
		mPrevIndex = 0;
		mpPrevElem = NULL;
	}
	SearchList(List<T>& list)
	{
		Set(list);
		mPrevIndex = 0;
		mpPrevElem = NULL;
	}
	void Set(List<T>& list)
	{
		mpList = &list;
	}
	const TIndex& Find(const T& value);
};

template <class T> 
	inline const TIndex& SearchList<T>::Find(const T& value)
{
	if (mpPrevElem==NULL) {
		mpPrevElem = (Elem<T>*) mpList->First();
		mPrevIndex = 0;
	}
	if (value>mpPrevElem->Value())
	{
		if (value>((Elem<T>*) mpList->Last())->Value()) {
			mpPrevElem = (Elem<T>*) mpList->Last();
			mPrevIndex = mpList->Size()-1;		
		} else {
			do {
				mpPrevElem = (Elem<T>*) mpPrevElem->Next();
				mPrevIndex++;
			} while (value>mpPrevElem->Value());		
		}
	}
	if (value<mpPrevElem->Value())
	{
		if (value<((Elem<T>*) mpList->First())->Value()) {
			mpPrevElem = NULL;
			mPrevIndex = -1;
		} else {
			do {
				mpPrevElem = (Elem<T>*) mpPrevElem->Prev();
				mPrevIndex--;
			} while (value<mpPrevElem->Value());			
		}
	}
	return mPrevIndex;
}

}

#endif

