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


#ifndef _BPFTmplDec_
#define _BPFTmplDec_

#include <iosfwd>

#include "PointTmpl.hxx" // for point functionality
#include "Err.hxx"//for Exception handling
#include "Array.hxx" // array functionality
#include "SearchArray.hxx" //array searches
#include "Enum.hxx"
#include "Component.hxx"
#include "GlobalEnums.hxx"

namespace CLAM
{
	class EInterpolation;

	template <class TX,class TY> class BPFTmpl:public Component
	{
		static const TData Infinity;

	public:
/**
* Default constructor: takes interpolation to be linear by default
*/
		BPFTmpl();

/**
* Constructor from a given interpolation type. 
* @param Interpolation Type: EInterpolation (linear, polynomical, spline...)
*/
		BPFTmpl(const EInterpolation& eInterpolation);

/**
* Constructor from a given interpolation type and an initial size. The initial size is used
* to allocate the member mArray. Is the one to use for efficiency whenever a maximum size of
* the BPF is known beforehand.
* @param size Allocation size for the member array of points.
* @param eInterpolation Kind of interpolation: (linear, polynomical, spline...)
*/
		BPFTmpl(TSize size);

		BPFTmpl(TSize size,
			const EInterpolation& eInterpolation);

/**
* Copy Constructor.
* @param orig BPF to be copied
*/
		BPFTmpl(const BPFTmpl<TX,TY>& orig);

/**
* Special initializer. All points up to the allocated size are initialized with the X value
* being the actual index in the array.
*/
		void Init();

/**
* Returns the class name. This implementation returns a NULL pointer
*/
		const char * GetClassName() const {return "BPF";}

/**
* Allocated Size of the member array of points
* @return allocated size of the array
* @see Size
*/
		TSize AllocatedSize(void) const {return mArray.AllocatedSize();}

/**
* Size of the member array of points, points with a higher index may be allocated but are
* not used
* @return size of the array
* @see AllocatedSize
*/
		TSize Size(void) const {return mArray.Size();}

/**
* Sets the size of the member array of points, points with a higher index may be 
* allocated but are not used. Actual allocated size is left untouched
* @param newSize new logical size
* @see Resize
* @see CLAM::Array::SetSize
*/
		void SetSize (TSize newSize){mArray.SetSize(newSize);}

/**
* Sets the allocated size of the member array of points, points with a higher index may be 
* allocated but are not used. Actually does a new memory allocation
* @param newSize new allocated size
* @see SetSize
* @see CLAM::Array::Resize
*/
		void Resize (TSize newSize){mArray.Resize(newSize);}

/**
* Inserts a point in the correct position. Note that points in the array are always sorted
* according to their X value. Must therefore perform a previous serch.
* @param point the point to be inserted
* @see CLAM::SearchArray 
*/
		TIndex Insert(const PointTmpl<TX,TY> &point);

/**
* Inserts a point made of an X and a Y value in the correct position. Note that 
* points in the array are always sorted according to their X value. Must therefore 
* perform a previous serch.
* @param x X value
* @param y Y value
* @see Insert
*/
		TIndex Insert(const TX &x,const TX &y);

/**
* Deletes the point found at the given index
* @param index Index of the point to delete
*/
		void DeleteIndex(TIndex index);

/**
* Deletes the point with a closest X value to the one given
* @param x X value of the point to delete
*/
		void DeleteThroughXValue(const TX& x);

/**
* Deletes the points between indices given
* @param leftIndex left index
* @param rightIndex right index
*/
		void DeleteBetweenIndex(TIndex leftIndex,TIndex rightIndex);
	
/**
* Deletes the points having an X value in between the ones given
* @param leftX left X value
* @param rightX right X value
*/
		void DeleteBetweenXValues(const TX& leftX,const TX& rightX);

/** 
* Setting Y value of a point
* @param index The index where the point is found
* @param y The new value for the Y
*/
		void SetValue(TIndex index,const TY &y)
		{
			mArray[index].SetY(y);
			mIsSplineUpdated=false;
		}

		void SetXValue(TIndex index,const TX &x){
			mArray[index].SetX(x);
			mIsSplineUpdated=false;
		}

/** 
* Setting a different interpolation type. Auxiliary member arrays mc and md are resized if
* necessary.
* @param new interpolation type
*/
		void SetIntpType(const EInterpolation& eInterpolation);
		
/** 
* Setting Step for resizing the array when needed
* @param step Allocation step value
* @see CLAM::Array::mStep
*/
		void SetStep(TSize step){mArray.SetStep(step);}
/** 
* Querying for step used for resizing the array when needed
* @return Allocation step size
* @see CLAM::Array::mStep
*/
		TSize GetStep() {return mArray.GetStep();}

/** 
* Getting the value of the point located at a given position
* @param index
* @see GetValue
*/
		TY GetValueFromIndex(TIndex index) const {
			return mArray[index].GetY();
		}

/** 
* Getting the value of a point from the value of its X component. Performs the kind
* of interpolation set at that moment
* @param x X value
* @see GetValueFromIndex
* @see meInterpolation
*/
		TY GetValue(const TX& x)	const {
			return GetValue(x,meInterpolation);
		}

/** 
* Getting the value of a point from the value of its X component. Performs the kind
* of interpolation passed as parameter
* @param x X value
* @param eInterpolation interpolation type 
* @see GetValueFromIndex
*/
		TY GetValue (const TX& x,const EInterpolation& eInterpolation) const;

/** 
* Get X value from a given Index
* @param index
* @return X value of the point located at the given index
*/	
		const TX& GetXValue(TIndex index) const 
		{
			return mArray[index].GetX();
		}

/** 
* Get point position in BPf from X value
* @param x X value
* @return index position
*/
		TIndex GetPosition(const TX& x) const;


/**
* Fills the interpolation points index buffer with the n closest points to
* the selected point, trying to keep n/2 points to the right and n/2 to the left
* whenever possible, being n the interpolation order plus one.
* <p>
* When the point are too near the BPF edges, selected points are adjusted to this edge.
* When n is an even value, only n/2-1 points are selected at the left side of the selected index.
* @param foundIndex The index of the central point.
*/
		void GetnClosest(TIndex foundIndex) const;

/**
*  = operator, TODO: check!!!!
*/
		BPFTmpl<TX, TY>& operator=(const BPFTmpl<TX, TY> &originalBPF);

/**
* Updates Spline table creating it from scratch
*/
		void UpdateSplineTable();

/**
* Changes the BPF border conditions
*/
		void SetLeftDerivative(TData val);
		void UnsetLeftDerivative(void);
		void SetRightDerivative(TData val);
		void UnsetRightDerivative(void);
	
/**
* Inspector for getting the Interpolation type
*/
		const EInterpolation	&GetInterpolation() const {
			return meInterpolation; 
		}

/** 
* Accesor to the member array of points
*/
		Array<PointTmpl<TX,TY> > &GetPointArray() {
			return mArray;
		}
		
protected:
	/*Member variables*/
	/**
	* Interpolation Type
	* @see GlobalEnums.hxx
	*/
		EInterpolation meInterpolation;
	/**
	 * Current number of points, does not need to be the same as size
	 */
		TSize mnPoints;
	/**
	* Actual array of points
	* @see CLAM::Array
	*/
		Array<PointTmpl<TX,TY> > mArray;
	/**
	* Implements search functionality based on locate and hunt
	* @see CLAM::SearchArray
	*/
		SearchArray<PointTmpl<TX,TY> > mSearch;//search functionality for array
	/**
	* mutable member: just used for optimizing access speed
	* holds the closest points for the last get or set operation
	*/
		mutable Array<TIndex> mClosestPoints;
	/**
	* mutable member: just used for optimizing access speed
	* holds auxiliary arrays for polynomic interpolation
	*/
		mutable Array<TY> mc,md;
	/**
	* auxiliary member: just used for optimizing access speed
	* holds the order of the current interpolation (only makes sense if it is polynomic)
	*/
		int mOrder; //interpolation order for polynomic interp.
	/**
	* mutable member: just used for optimizing access speed
	* holds the last index used in a search 
	*/
		mutable TIndex mLastIndex; 
	/**
	* mutable member: just used for optimizing access speed
	* holds the last X value returned from a search
	*/
		mutable TX mLastX; 

	/**
	* Member variable for spline interpolation
	* holds the actual spline table
	*/
		Array<TY> mSplineTable;
	/**
	* Member variable for spline interpolation
	* keps track of whether spline table is updated
	*/
		bool mIsSplineUpdated;

	/**
	 * Member variable for spline interpolation.
	 * stores the value of the first derivative at the left point.
	 * If equal or greater to 1e30, zero second derivative is used
	 * as left boundary condition.
	 */

		TData mLeftDerivative;

	/**
	 * Member variable for spline interpolation.
	 * stores the value of the first derivative at the right point.
	 * If equal or greater to 1e30, zero second derivative is used
	 * as right boundary condition.
	 */

		TData mRightDerivative;
		
/**
* Member protected method
* Performs the actual polynomial interpolation algorithm
*/
	TY BPFPolInt(const TX& x,const Array<TIndex>& closestPointsIndex,
		TData &errorEstimate) const;
	void CreateSplineTable();
	TY BPFSplineInt(const TX& x) const;
	void StoreOn(Storage & storage) const;
	void LoadFrom(Storage & storage);

	};

} // namespace CLAM

#endif // _BPFTmplDec_

