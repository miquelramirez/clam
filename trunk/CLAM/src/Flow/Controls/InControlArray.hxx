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

#ifndef _InControlArray_hxx_
#define _InControlArray_hxx_

#include "InControl.hxx"
#include <vector>
#include <list>
#include <string>

namespace CLAM
{
/**
 * This class wrapps a vector (array) of InControls and gives its InControl automatic
 * names at construction
 */ 
class InControlArray
{
	typedef std::vector<InControl*> Controls;
	Controls mControls;
public:
//Constructors
	InControlArray();
	InControlArray(int size, const std::string &name, Processing* parent);
	InControlArray(int size, const std::list<std::string> &names, Processing* parent);
	

	~InControlArray();

	InControl       &operator[](int i)        { return *mControls[i]; }
	const InControl &operator[](int i) const  { return *mControls[i]; }

	void Resize(int size, const std::string &name, Processing* parent);
	void Resize(int size, const std::list<std::string>& names, Processing* parent);
				

	int Size() const {return mControls.size();}

protected:
	void Shrink(int size);
};



}; //namespace CLAM

#endif
