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

#include "OutControlArray.hxx"

namespace CLAM{
	

OutControlArray::OutControlArray(int size,
								 const std::string &name,
								 Processing *wtp)
{
	Resize(size,name,wtp);
}

OutControlArray::OutControlArray()
{
	mControls.resize(0);
}

OutControlArray::OutControlArray(
		int size, 
		const std::list<std::string>& names,
		Processing *parent)
{
	Resize(size, names, parent);
}

OutControlArray::~OutControlArray()
{
	Shrink(0);
}


void OutControlArray::Resize(int size, const std::string &name, Processing* parent)
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
		mControls[i] = new OutControl(str.str(), parent);	
	}
}

void OutControlArray::Resize(int size, const std::list<std::string>& names, Processing* parent)
{
	int previousSize = int ( mControls.size() );
	if(size < previousSize) 
	{
		Shrink(size);
		return;
	}
	CLAM_ASSERT( size-previousSize <= int(names.size()), "OutControlArray::Resize: error, not enough labels provided");
	mControls.resize( size );
	std::list<std::string>::const_iterator name = names.begin();
	for (int i = previousSize; i<size; i++, name++) {
		mControls[i] = new OutControl(*name, parent);	
	}
}

void OutControlArray::Shrink(int size)
{
	int previousSize = mControls.size();
	if (size==previousSize) return;
	CLAM_ASSERT(size < previousSize, "OutControlArray::Cannot Shrink a Control Array to a larger size");
	for (int i = previousSize-1; i >= size; i--) {
		delete mControls[i];	
	}
	mControls.resize(size);
}

	
	
	
};
