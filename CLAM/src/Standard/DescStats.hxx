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

/* STORAGE CLASS FOR STATISTICS COMPUTATIONS
FG
em

The processing class that actually proceeds to the computations is 
ComputeDescStats

The data members of this class are:
- the values of several statistics
- a pointer towards a data vector 
- an array of data sorted index (used for applications with sorted data)
- a sorting object
- an axiliary vector that stores the "centered" samples at a given power
- an axiliary vector that stores the samples at a given power
- a flags structure indicating which statistics are going to be computed
- a flags structure indicating which statistics have been computed
- a boolean indicating wether the data vector has changed

The user requests some stats to be computed by the use of flags.
These flags are first given in the construction, they can also be changed by the 
member function ChgeRequest.

Use the Get functions to read a value of a statistic.
To compute a set of statistics, set the request flags' array and 
use the ComputeDescStats class that uses the Set functions.

Friend classes defined for modifiying the pointer to data vector.

The mean is the 1st order moment (which is equal to the 1st order cumulant). 
The variance is the 2d order central moment (which is equal to the 2d order 
cumulant).The Skewness is implemented as the 3d order central moment (which 
is equal to the 3d order cumulant). The Kurtosis is implemented as the 4th 
order cumulant (which is equal to the 4th order central moment minus 3 times 
the squared variance).

Two friend classes can access to the members. 

*/

#ifndef _DescStats_
#define _DescStats_

#include "Err.hxx"
#include "Vector.hxx"
#include "QSort.hxx"
#include "Flags.hxx"
#include "Component.hxx"
#include "CLAM_Math.hxx"
#include <stdio.h>

namespace CLAM {

struct FlagsDescStats:Flags<FlagsDescStats>
{
	// Central tendency
	bool bStartOfFlags : 1; // obligatory for Flags derived 

	bool bMean : 1;
	bool bMedian : 1;
	bool bMode : 1;
	bool bTrimean : 1;
	// Dispersion
	bool bAvgDev : 1;
	bool bVar : 1;
	bool bStdDev : 1;
	bool bMin : 1;
	bool bMax : 1;
	bool bRange : 1;
	bool bSemIntRange : 1;
	// Shape
	bool bSkew : 1;
	bool bKurt : 1;
	//bool bFourthOrdCentMom : 1;

	//Sums (UnNormalized Moments)
	bool bFstOSum : 1;
	bool bSdOSum : 1;
	bool bTrdOSum : 1;
	bool bFthOSum : 1;

	bool bAbsFstOSum : 1;

	bool bEndOfFlags : 1; // obligatory for Flags derived 
};

template <class T> class DescStats;
template <class T> class JointStats;
template <class T> class ComputeJointStats;

#include "ComputeDescStats.hxx"
#include "JointStats.hxx"
#include "ComputeJointStats.hxx"
#include "Component.hxx"
#include "Storage.hxx"
#ifdef CLAM_USE_XML
#include "XMLStaticAdapter.hxx"
#endif//CLAM_USE_XML


template <class T> class DescStats : public Component
{

	friend class ComputeDescStats<T>;
	friend class JointStats<T>;
	friend class ComputeJointStats<T>;

public:

	// Constructors
	DescStats(Vector<T> *pDataVector, FlagsDescStats& 
				requestedStatistics)
		:mAuxVector1(pDataVector->GetDimension()),
		 mAuxVector2(pDataVector->GetDimension()),
		 mSortingObject(pDataVector->GetBuffer()),
		 mIndexes(pDataVector->GetDimension())
	{
		mRequestedStatsFlags = requestedStatistics;
		mComputedStatsFlags.Reset();
		mpDataVector = pDataVector;
		mbIsUpToDate = true;
		mSortingObject.SortIndexArray(mIndexes);
	}


	DescStats(Vector<T> *pDataVector)
		:mAuxVector1(pDataVector->GetDimension()),
		 mAuxVector2(pDataVector->GetDimension()),
		 mSortingObject(pDataVector->GetBuffer()),
		 mIndexes(pDataVector->GetDimension())
	{
		mRequestedStatsFlags.Reset();
		mComputedStatsFlags.Reset();
		mpDataVector = pDataVector;
		mbIsUpToDate = true;
		mSortingObject.SortIndexArray(mIndexes);
	}
	
	// Destructor 
	~DescStats() {}
	

	// Function for changing the request
	void ChgeRequest(FlagsDescStats& newRequestedStatsFlags) { mRequestedStatsFlags = newRequestedStatsFlags; }
	
	// This function must be used in the main program when changing the input data vector
	void DataVectorModified()	{ mbIsUpToDate = false; }	
	
	// Get functions
	double GetMean() const
	{
		if (!mComputedStatsFlags.bMean)
			throw Err("(in DescStats) statistic not computed at that point");
		return mMean;
	}

	double GetMedian() const
	{
		if (!mComputedStatsFlags.bMedian)
			throw Err("(in DescStats) statistic not computed at that point");
		return mMedian;
	}

	T GetMode() const
	{
		if (!mComputedStatsFlags.bMode)
			throw Err("(in DescStats) statistic not computed at that point");
		return mMode;
	}

	double GetTrimean() const
	{
		if (!mComputedStatsFlags.bTrimean)
			throw Err("(in DescStats) statistic not computed at that point");
		return mTrimean;
	}

	double GetVar() const
	{
		if (!mComputedStatsFlags.bVar)
			throw Err("(in DescStats) statistic not computed at that point");
		return mVar;
	}

	double GetStdDev() const
	{
		if (!mComputedStatsFlags.bStdDev)
			throw Err("(in DescStats) statistic not computed at that point");
		return mStdDev;
	}

	double GetAvgDev() const 
	{
		if (!mComputedStatsFlags.bAvgDev)
			throw Err("(in DescStats) statistic not computed at that point");
		return mAvgDev;
	}

	T GetMin() const
	{
		if (!mComputedStatsFlags.bMin)
			throw Err("(in DescStats) statistic not computed at that point");
		return mMin;
	}

	T GetMax() const 
	{
		if (!mComputedStatsFlags.bMax)
			throw Err("(in DescStats) statistic not computed at that point");
		return mMax;
	}

	T GetRange() const
	{
		if (!mComputedStatsFlags.bRange)
			throw Err("(in DescStats) statistic not computed at that point");
		return mRange;
	}

	double GetSemIntRange() const
	{
		if (!mComputedStatsFlags.bSemIntRange)
			throw Err("(in DescStats) statistic not computed at that point");
		return mSemIntRange;
	}

	double GetKurt() const
	{
		if (!mComputedStatsFlags.bKurt)
			throw Err("(in DescStats) statistic not computed at that point");
		return mKurt;
	}
	
	double GetSkew() const
	{
		if (!mComputedStatsFlags.bSkew)
			throw Err("(in DescStats) statistic not computed at that point");
		return mSkew;
	}

	double GetFstOSum() const 
	{
		if (!mComputedStatsFlags.bFstOSum)
			throw Err("(in DescStats) statistic not computed at that point");
		return mFstOSum;
	}

	double GetAbsFstOSum() const 
	{
		if (!mComputedStatsFlags.bAbsFstOSum)
			throw Err("(in DescStats) statistic not computed at that point");
		return mAbsFstOSum;
	}

	double GetSdOSum() const
	{
		if (!mComputedStatsFlags.bSdOSum)
			throw Err("(in DescStats) statistic not computed at that point");
		return mSdOSum;
	}

	double GetTrdOSum() const 
	{
		if (!mComputedStatsFlags.bTrdOSum)
			throw Err("(in DescStats) statistic not computed at that point");
		return mTrdOSum;
	}

	double GetFthOSum() const
	{
		if (!mComputedStatsFlags.bFthOSum)
			throw Err("(in DescStats) statistic not computed at that point");
		return mFthOSum;
	}

	/*const double GetFourthOrdCentMom() 
	{
		if (mComputedStatsFlags.bFourthOrdCentMom == 1)
			return mFourthOrdCentMom;
		else throw Err("(in DescStats) statistic not computed at that point");
	}*/

	FlagsDescStats GetRequestedFlags() const { return mRequestedStatsFlags;}
	FlagsDescStats GetComputedFlags() const { return mComputedStatsFlags; }

	void StoreOn (Storage & storer) { 
#ifdef CLAM_USE_XML
		if (mComputedStatsFlags.bMean) {
			XMLStaticAdapter wrapper(GetMean(),"Mean");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bMedian) {
			XMLStaticAdapter wrapper(GetMedian(),"Median");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bMode) {
			XMLStaticAdapter wrapper(GetMode(),"Mode");
			storer.Store(&wrapper);
			}

		if (mComputedStatsFlags.bTrimean) {
			XMLStaticAdapter wrapper(GetTrimean(),"Trimean");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bAvgDev) {
			XMLStaticAdapter wrapper(GetAvgDev(),"AvgDev");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bVar) {
			XMLStaticAdapter wrapper(GetVar(),"Var");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bStdDev) {
			XMLStaticAdapter wrapper(GetStdDev(),"StdDev");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bMin) {
			XMLStaticAdapter wrapper(GetMin(),"Min");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bMax) {
			XMLStaticAdapter wrapper(GetMax(),"Max");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bRange) {
			XMLStaticAdapter wrapper(GetRange(),"Range");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bSemIntRange) {
			XMLStaticAdapter wrapper(GetSemIntRange(),"SemIntRange");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bSkew) {
			XMLStaticAdapter wrapper(GetSkew(),"Skew");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bKurt) {
			XMLStaticAdapter wrapper(GetKurt(),"Kurt");
			storer.Store(&wrapper);
			}
		/*
		if (mComputedStatsFlags.bFourthOrdCentMom) {
			XMLStaticAdapter wrapper(GetFourthOrdCentMom(),"FourthOrdCentMom");
			storer.Store(&wrapper);
			}
		*/
		if (mComputedStatsFlags.bFstOSum) {
			XMLStaticAdapter wrapper(GetFstOSum(),"FstOSum");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bSdOSum) {
			XMLStaticAdapter wrapper(GetSdOSum(),"SdOSum");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bTrdOSum) {
			XMLStaticAdapter wrapper(GetTrdOSum(),"TrdOSum");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bFthOSum) {
			XMLStaticAdapter wrapper(GetFthOSum(),"FthOSum");
			storer.Store(&wrapper);
			}
		if (mComputedStatsFlags.bAbsFstOSum) {
			XMLStaticAdapter wrapper(GetAbsFstOSum(),"AbsFstOSum");
			storer.Store(&wrapper);
			}

#endif // CLAM_USE_XML
	}

protected:

// Setting functions
	void SetMean(double mean) {mMean = mean;}
	void SetMedian(double median) {mMedian = median;}
	void SetMode(T mode) {mMode = mode;}
	void SetTrimean(double trimean) {mTrimean = trimean;}
	void SetVar(double var) {mVar = var;}	
	void SetStdDev(double sdev) {mStdDev = sdev;}
	void SetAvgDev(double adev) {mAvgDev = adev;}
	void SetMin(T min) {mMin = min;}
	void SetMax(T max) {mMax = max;}
	void SetRange(T rge) {mRange = rge;}
	void SetSemIntRange(double sir) {mSemIntRange = sir;}
	void SetKurt(double kurt) {mKurt = kurt;}
	void SetSkew(double skew){mSkew = skew;}
	void SetFstOSum(double fstos){mFstOSum = fstos;}
	void SetAbsFstOSum(double afstos){mAbsFstOSum = afstos;}
	void SetSdOSum(double sdos){mSdOSum = sdos;}
	void SetTrdOSum(double trdos){mTrdOSum = trdos;}
	void SetFthOSum(double fthos){mFthOSum = fthos;}
	//void SetFourthOrdCentMom(double focm)	{mFourthOrdCentMom = focm;};	
	
	Vector<T> *mpDataVector;
	
	Vector<T> mAuxVector1; //centered samples at a given power
	Vector<T> mAuxVector2; //samples at a given power
	IndexArray mIndexes;
	QSort<T> mSortingObject;
	
	FlagsDescStats mRequestedStatsFlags;
	FlagsDescStats mComputedStatsFlags;
	
	bool mbIsUpToDate;
	
	double mMean;
	double mMedian;
	T mMode;
	double mTrimean;
	double mVar;
	double mStdDev;
	double mAvgDev;
	T mMin;
	T mMax;
	T mRange;
	double mSemIntRange;
	double mKurt;
	double mSkew;
	double mFstOSum;
	double mAbsFstOSum;
	double mSdOSum;
	double mTrdOSum;
	double mFthOSum;

 /* Not needed for the moment
		void SetVector(Vector<T> *pNewVector)	// only used by friend classes for initializing. em.
		{ 
		 mAuxVector = Vector<T> (pNewVector->GetDimension(), pNewVector->GetScale());
		 mSortingObject = QSort<T> (pNewVector->GetBuffer());
		 mIndexes = IndexArray (pNewVector->GetDimension());
		 mComputedStatsFlags.Reset();
		 mpDataVector = pNewVector;
		 mbIsUpToDate = true;
		 mSortingObject.SortIndexArray(mIndexes);
		 mRequestedStatsFlags.Reset(); // reset requested flags?
	 }
 */ 

// 	double mFourthOrdCentMom;
};

template <class T> inline EDataFormat DataFormat(DescStats<T>* &)
{	return eFmtDefault; }

}

#endif//_DescStats_
