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

// DynamicNote.hxx.
// Just a test class for DynamicTypes.
// written by Pau Arumí - May 2001
//////////////////////////////////////////////////////////////////////

#if !defined _DynamicNote_
#define _DynamicNote_


#include "DynamicType.hxx"
#include "DynamicADSR.hxx"  // DynamicNote have one attribute of that type.
#include "Array.hxx"


using namespace CLAM;

class CTest : public DynamicType
{
public:
	REG_NUM_ATTR (CTest, 1);
	//	REGISTER_PTR (0, pArrayTest, DataArray*);
}
;

class DynamicNote : public CLAM::DynamicType
{
public:
	REG_NUM_ATTR (DynamicNote, 7);

	REGISTER (0, NSines, int);
	REGISTER (1, Pitch, float);
	REGISTER (2, A, int);
	REGISTER_PTR (3, pInt, int*);
	REGISTER_PTR (4, pADSR, DynamicADSR*);
	REGISTER_PTR (5, pCTest, CTest*);
	REGISTER_PTR (6, Wave, Array<TData>*);
};


#endif

