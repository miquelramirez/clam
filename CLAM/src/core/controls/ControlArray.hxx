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

#ifndef _ControlArray_hxx_
#define _ControlArray_hxx_

#include "Assert.hxx"
#include <vector>
#include <list>
#include <string>
#include <sstream>

namespace CLAM
{

class Processing;

/**
 * This class wrapps a vector (array) of Controls and gives its Controls
 * automatic names at construction.
 */ 
template <class ControlT>
class ControlArray
{
	typedef std::vector<ControlT *> Controls;
	Controls mControls;

public:
	ControlArray() { }
	ControlArray(int size, const std::string &name, Processing* parent) {
		Resize(size,name,parent);
	}
	ControlArray(int size, const std::list<std::string> &names,
			Processing* parent)
	{
		Resize(size, names, parent);
	}
	template <typename ProcessingType, typename ValueType>
	ControlArray(int size, const std::string &name, ProcessingType * parent,
		void (ProcessingType::*method)(unsigned, ValueType) )
	{
		Resize(size,name,parent,method);
	}
	~ControlArray() { Clear(); }

	ControlT       &operator[](int i)        { return *mControls[i]; }
	const ControlT &operator[](int i) const  { return *mControls[i]; }

	void Resize(int size, const std::string &name, Processing* parent);
	void Resize(int size, const std::list<std::string>& names, Processing* parent);
	template <typename ProcessingType, typename ValueType>
	void Resize(int size, const std::string &name, ProcessingType * parent,
		void (ProcessingType::*method)(unsigned, ValueType) )
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
			mControls[i] = new ControlT(i, str.str(), parent, method);	
		}
	}

	void Append(int size, const std::string &names, Processing* parent);
	void Append(int size, const std::list<std::string>& names, Processing* parent);

	int Size() const { return mControls.size(); }

	void Clear() { Shrink(0); }

protected:
	void Shrink(int size);
};
 	
template <class ControlT>
void ControlArray<ControlT>::Resize(int size, const std::string &name,
		Processing* parent)
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
		mControls[i] = new ControlT(str.str(), parent);	
	}
	
}

template <class ControlT>
void ControlArray<ControlT>::Resize(int size,
		const std::list<std::string>& names, Processing* parent)
{
	int previousSize = mControls.size();
	if (size < previousSize) 
	{
		Shrink(size);
		return;
	}
	CLAM_ASSERT(size - previousSize <= int(names.size()),
		"ControlArray::Resize: error, not enough labels provided");
	mControls.resize(size);
	std::list<std::string>::const_iterator name = names.begin();
	for (int i = previousSize; i<size; i++, name++)
		mControls[i] = new ControlT(*name, parent);	
}

template <class ControlT>
void ControlArray<ControlT>::Append(int count, 
		const std::string &name, Processing* parent)
{
	Resize(Size() + count, name, parent);
}

template <class ControlT>
void ControlArray<ControlT>::Append(int count, 
		const std::list<std::string>& names, Processing* parent)
{
	Resize(Size() + count, names, parent);
}


template <class ControlT>
void ControlArray<ControlT>::Shrink(int size)
{
	int previousSize = mControls.size();
	if (size == previousSize) return;
	CLAM_ASSERT(size < previousSize,
		"ControlArray::Cannot Shrink a ControlArray to a larger size");
	for (int i = previousSize-1; i >= size; i--)
		delete mControls[i];	
	mControls.resize(size);
}

}; //namespace CLAM

#endif

