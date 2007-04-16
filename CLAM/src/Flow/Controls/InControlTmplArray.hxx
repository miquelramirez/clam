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

#ifndef _InControlTmplArray_hxx_
#define _InControlTmplArray_hxx_

#include "InControl.hxx"
#include <vector>

namespace CLAM
{
/**
 * \todo document this class
 */ 
template <class TProcessing>
class InControlTmplArray
{
	typedef InControlTmpl<TProcessing> TInControl;
	typedef typename TInControl::TPtrMemberFuncId TPtrMemberFuncId;
	typedef std::vector<TInControl*> Controls;
	Controls mControls;

public:
	InControlTmplArray(int size, const std::string &name, TProcessing* parent,	TPtrMemberFuncId f);
	/** Overloaded constructor in which a list of names is passed for each of the
	 * controls*/
	InControlTmplArray(int size, const std::list<std::string>& names, TProcessing* parent, TPtrMemberFuncId f);
	/* This overload could be substituted by giving the size parameter a default value but this would	
	 * not be backward compatible*/
	InControlTmplArray();

	~InControlTmplArray();

	TInControl& operator[](int i) { return *mControls[i]; }
	const TInControl& operator[](int i) const { return *mControls[i]; }

	void Resize(int size, const std::string& name, TProcessing* parent, TPtrMemberFuncId f);
	void Resize(int size, const std::list<std::string>& names, TProcessing* parent, TPtrMemberFuncId f);

	int Size() const {return mControls.size();}

protected:
	void Shrink(int size);

};

//------------------------------------------------------------------------------
// Implementation
template <class TProcessing>
InControlTmplArray<TProcessing>::InControlTmplArray(
		int size, 
		const std::string &name,
		TProcessing *parent, 
		TPtrMemberFuncId f)
{
	CLAM_ASSERT(parent, "InControlTmplArray must be published. Check ctr processing* parameter");
	Resize(size,name,parent,f);
}

template <class TProcessing>
InControlTmplArray<TProcessing>::InControlTmplArray(
		int size, 
		const std::list<std::string>& names,
		TProcessing *parent, 
		TPtrMemberFuncId f)
{
	CLAM_ASSERT(parent, "InControlTmplArray must be published. Check ctr processing* parameter");
	Resize(size, names, parent,f);
}

template <class TProcessing>
InControlTmplArray<TProcessing>::InControlTmplArray()
{
	mControls.resize(0);
}

template <class TProcessing>
void InControlTmplArray<TProcessing>::Resize(int size, const std::string& name, TProcessing* parent, TPtrMemberFuncId f)
{
	int previousSize = mControls.size();
	if(size < previousSize) 
	{
		Shrink(size);
		return;
	}
	
	mControls.resize(size);
	for (int i = previousSize; i<size; i++) {
		std::stringstream str;
		str << name << "_" << i;
		mControls[i] = new TInControl(i, str.str(), parent, f);	
	}
}

template <class TProcessing>
void InControlTmplArray<TProcessing>::Resize(int size, const std::list<std::string>& names , TProcessing* parent, TPtrMemberFuncId f)
{
	int previousSize = mControls.size();
	if(size < previousSize) 
	{
		Shrink(size);
		return;
	}
	
	CLAM_ASSERT(size-previousSize <= names.size(), "InControlTmplArray::Resize not enoough labels are given");
	mControls.resize(size);
	std::list<std::string>::iterator name = names.begin();
	for (int i = previousSize; i<size; i++) {
		mControls[i] = new TInControl(i, *name, parent, f);	
	}
}

template <class TProcessing>
void InControlTmplArray<TProcessing>::Shrink(int size)
{
	int previousSize = mControls.size();
	CLAM_ASSERT(size < previousSize, "InControlArray::Cannot Shrink a Control Array to a larger size");
	for (int i = previousSize-1; i >= size; i--) {
		delete mControls[i];	
	}
	mControls.resize(size);
}


template <class TProcessing>
InControlTmplArray<TProcessing>::~InControlTmplArray()
{
	Shrink(0);
}

} //namespace CLAM

#endif

