/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef EquidistantPointsFunction_hxx
#define EquidistantPointsFunction_hxx

#include <string>
#include <CLAM/DataTypes.hxx>

namespace CLAM
{
	class EquidistantPointsFunction
	{
	public:
		typedef std::string Units;
	public:
		EquidistantPointsFunction()
			: mBoundMin(false)
			, mBoundMax(false)
			, mMinValue(0)
			, mMaxValue(0)
		{
		}
		void setValueUnits(const Units & units)
		{
			mUnits = units;
		}
		void setDivision(CLAM::TData firstPosition, CLAM::TData intervalueGap, const Units & units)
		{
			mFirstPointPosition=firstPosition;
			mInterPointInterval=intervalueGap;
			mXUnits = units;
		}
		TData firstPointPosition() const { return mFirstPointPosition; }
		TData lastPointPosition() const { return mFirstPointPosition+mSize*mInterPointInterval; }
		TData gap() const { return mInterPointInterval; }

		void setValues(const CLAM::TData * data, unsigned size, const Units & units)
		{
			mValues = data;
			mSize = size;
			mBpf.Resize(size);
			mBpf.SetSize(size);
			for(int i=0; i<size ;i++)
			{
				mBpf.SetXValue(i, mFirstPointPosition+i*mInterPointInterval);
				mBpf.SetValue(i,mValues[i]);
			}
		}
		void limitMaxValue(CLAM::TData max)
		{
			mBoundMax=true;
			mMaxValue=max;
		}
		void limitMinValue(CLAM::TData min)
		{
			mBoundMin=true;
			mMinValue=min;
		}
		CLAM::TData getMax() const
		{
			if (mBoundMax) return mMaxValue;
			CLAM::TData max = 0;
			for (unsigned int i=0; i<mSize; i++)
			{
				if (mValues[i]<max) continue;
				max=mValues[i];
			}
			return max;
		}
		CLAM::TData getMin() const
		{
			if (mBoundMin) return mMinValue;
			CLAM::TData min = 0;
			for (unsigned int i=0; i<mSize; i++)
			{
				if (mValues[i]>min) continue;
				min=mValues[i];
			}
			return min;
		}
		CLAM::BPF & GetBPF()
		{
			return mBpf;
		}
		const CLAM::BPF & GetBPF() const
		{
			return mBpf;
		}
		unsigned nearestPoint(double xValue) const
		{
			return .5 + (xValue-mFirstPointPosition) / mInterPointInterval;
		}

	private:
		const CLAM::TData * mValues; //< Array containing the values
		unsigned mSize; //< The number of values
		CLAM::BPF mBpf; //< Transcribed BPF

		// Cadidates to become a FrameDivision object
		CLAM::TData mFirstPointPosition;
		CLAM::TData mInterPointInterval;
		Units mXUnits;
		
		// Candidates to become a MagnitudeType object
		bool mBoundMin;
		bool mBoundMax;
		CLAM::TData mMinValue;
		CLAM::TData mMaxValue;
		CLAM::TData mDefaultValue;
		Units mUnits;
	};
}




#endif // EquidistantPointsFunction_hxx

