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
#ifndef __TABFUNCT__
#define __TABFUNCT__

#include "DataTypes.hxx"
#include "Assert.hxx"
#include <vector>

using std::vector;

namespace CLAM
{
/**
 * Precalculated (tabulated) functor, for dealing with efficient version of 
 * expensive functions (with its associated cost in space and precision).
 * The original function is passed as the template argument, requiring from 
 * the template, the same operator interface as the one that TabFunct offers:
 * TData operator() (const TData).
 * 
 * The only constructor available takes three parameters : table size, lower bound
 * and upper bound.
 * 
 */
template <class OriginalFunction> class TabFunct : public OriginalFunction
{
private:
	TabFunct() {};
public:
	TData operator() (const TData arg) {

		CLAM_DEBUG_ASSERT(arg>=mLowerBound && arg<=mUpperBound, "Tablulated functor argument out of bound");
		
		int index = int((arg-mLowerBound) / mIncr);

		CLAM_DEBUG_ASSERT(index<=mTableSize-2, "Bad index calculation");

		TData x1 = mLowerBound+mIncr*index;
		TData yIncr = mTable[index+1]-mTable[index];

		return mTable[index] + ((arg-x1) * yIncr) * mInvIncr;
	}

// constructor
	TabFunct(const unsigned tableSize, const TData lowerBound, const TData upperBound) :
		mLowerBound(lowerBound),
		mUpperBound(upperBound),
		mTableSize(tableSize)

	{
		CLAM_ASSERT(tableSize>=2, "Tabulating a function with less than 2 points.");
		CLAM_ASSERT(lowerBound < upperBound, "No interval left to calculate values");

		CalculateTable();
	}

private:
	
	void CalculateTable()
	{
		mTable.reserve(mTableSize);
		
		mIncr = (mUpperBound - mLowerBound) / (mTableSize-1);
		mInvIncr = 1/mIncr;

		TData arg = mLowerBound;

		for (int i=0; i<=mTableSize-1; arg+=mIncr, i++)
			mTable[i] = OriginalFunction::operator() (arg);

	}
	// internal table
	vector<TData>      mTable;
	TData              mLowerBound;
	TData              mUpperBound;
	unsigned           mTableSize;
	TData              mIncr, mInvIncr;

};

} //namespace

#endif // TabFunct.hxx

