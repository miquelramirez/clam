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

#ifndef _TestDynamicTypeBranches_
#define _TestDynamicTypeBranches_

#include "DynamicType.hxx"
#include "Array.hxx"

using namespace CLAM;

class DynamicADSR : public DynamicType
{
public:
	REG_NUM_ATTR (DynamicADSR, 4);

	
	REGISTER (0,	AttackTime, float);
	REGISTER (1,	AttackLevel,	float);
	REGISTER (2,	DecayTime,		float);
	REGISTER (3,	SustainLevel,	float);
	
};

class DynamicRegion : public CLAM::DynamicType
{
public:
	REG_NUM_ATTR (DynamicRegion, 5);

	
	REGISTER (0,	BeginTime, float);
	REGISTER (1,	EndTime,	float);
	REGISTER (2,	BeginAmplitude,		float);
	REGISTER (3,	EndAmplitude,	float);
	REGISTER (4,	Vibrato,	int);
	
};


using namespace CLAM;

class TestInterface : public DynamicType
{
public:
	TestInterface(const int n) : DynamicType(n) {};

	TestInterface(const TestInterface& prototype, bool shareData=false, bool deep=true) : 
		DynamicType(prototype, shareData, deep){};

//	virtual int GetNSines() const = 0;
//	virtual void SetNSines(int) = 0;



};
class Test : public TestInterface
{
public:
	REG_NUM_ATTR_USING_INTERFACE (Test, 1, TestInterface);
	REGISTER (0, NSines, int);
	
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
	REGISTER_PTR (5, pCTest, Test*);
	REGISTER_PTR (6, Wave, Array<TData>*);
};

template<class T> class TempTest : public CLAM::DynamicType
{
public:
	REG_NUM_ATTR(TempTest, 2);
	REGISTER (0, A, T);
	REGISTER (1, B, T*);

};

#endif
