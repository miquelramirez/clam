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


#ifndef _BPFTmplDef_
#define _BPFTmplDef_

#include "GlobalEnums.hxx"

namespace CLAM
{
	

	template <typename TX, typename TY> const TData BPFTmpl<TX,TY>::Infinity = TData(1e30);

/**
* Default constructor: takes interpolation to be linear by default
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>::BPFTmpl<TX,TY>() : 
		mArray(0),
		mSearch(mArray),
		mClosestPoints(10),
		mc(2),
		md(2),
		mOrder(1),
		mLastIndex(-1),
		mLastX(0),
		mSplineTable(0),
		mIsSplineUpdated(false),
		mLeftDerivative((TData)Infinity),
		mRightDerivative((TData)Infinity)
	{
		mClosestPoints.SetSize(mClosestPoints.AllocatedSize());
		mc.SetSize(2);
		md.SetSize(2);
		meInterpolation=EInterpolation::eLinear;
	}

/**
* Constructor from a given interpolation type. 
* @argument Interpolation Type: EInterpolation (linear, polynomical, spline...)
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>::BPFTmpl<TX,TY>(const EInterpolation& eInterpolation) : 
		mArray(0),
		mSearch(mArray),
		mClosestPoints(10),
		mc(0),
		md(0),
		mLastIndex(-1),
		mLastX(0),
		mSplineTable(0),
		mIsSplineUpdated(false),
		mLeftDerivative(Infinity),
		mRightDerivative(Infinity)
	{
		mClosestPoints.SetSize(mClosestPoints.AllocatedSize());
		SetIntpType(eInterpolation);
	}


/**
* Constructor from a given interpolation type and an initial size. The initial size is used
* to allocate the member mArray. Is the one to use for efficiency whenever a maximum size of
* the BPF is known beforehand.
* @argument size: Allocation size for the member array of points.
* @argument Interpolation Type: EInterpolation (linear, polynomical, spline...)
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>::BPFTmpl<TX,TY>(TSize size) : 
		mArray(size),
		mSearch(mArray),
		mClosestPoints(10),
		mc(0),
		md(0),
		mLastIndex(-1),
		mLastX(0),
		mSplineTable(0),
		mIsSplineUpdated(false),
		mLeftDerivative(Infinity),
		mRightDerivative(Infinity)
	{
		mClosestPoints.SetSize(mClosestPoints.AllocatedSize());
		SetIntpType(EInterpolation::eLinear);
		SetSize(size);
	}

/**
* Constructor from a given interpolation type and an initial size. The initial size is used
* to allocate the member mArray. Is the one to use for efficiency whenever a maximum size of
* the BPF is known beforehand.
* @argument size: Allocation size for the member array of points.
* @argument Interpolation Type: EInterpolation (linear, polynomical, spline...)
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>::BPFTmpl<TX,TY>(TSize size,const EInterpolation& eInterpolation) : 
		mArray(size),
		mSearch(mArray),
		mClosestPoints(10),
		mc(0),
		md(0),
		mLastIndex(-1),
		mLastX(0),
		mSplineTable(0),
		mIsSplineUpdated(false),
		mLeftDerivative(Infinity),
		mRightDerivative(Infinity)
	{
		mClosestPoints.SetSize(mClosestPoints.AllocatedSize());
		SetIntpType(eInterpolation);
		SetSize(size);
	}

/**
* Copy Constructor.
* @argument originalBPF
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>::BPFTmpl<TX,TY>(const BPFTmpl<TX,TY>& orig)	:
		meInterpolation(orig.meInterpolation),
		mArray(orig.mArray),
		mClosestPoints(orig.mClosestPoints.AllocatedSize()),
		mc(orig.mc.AllocatedSize()),
		md(orig.md.AllocatedSize()),
		mOrder(orig.mOrder),
		mLastIndex(-1),
		mLastX(0),
		mSplineTable(orig.mSplineTable),
		mIsSplineUpdated(orig.mIsSplineUpdated),
		mLeftDerivative(orig.mLeftDerivative),
		mRightDerivative(orig.mRightDerivative)
	{
		mClosestPoints.SetSize(orig.mClosestPoints.Size());
		mc.SetSize(orig.mc.Size());
		md.SetSize(orig.md.Size());
		mSearch.Set(mArray);
		mnPoints=orig.mnPoints;

	}

/**
* Special initializer. All points up to the allocated size are initialized with the X value
* being the actual index in the array.
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::Init()
	{
		int i;
		mArray.SetSize(AllocatedSize());
		for(i=0;i<AllocatedSize();i++)
		{
			mArray[i].SetX((TX)i);
			SetValue(i,0);
		}
	}


/**
* Inserts a point in the correct position. Note that points in the array are always sorted
* according to their X value. Must therefore perform a previous serch.
* @param : point to insert
* @see : CLAM::SearchArray 
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::Insert(const PointTmpl<TX,TY> &point)
	{
		if(mArray.Size()==0||point.GetX()>mArray[Size()-1].GetX())
			mArray.AddElem(point);
		else
		{
			TIndex closestIndex=mSearch.Find(point);
			if(closestIndex==-1)
			{
				if(point.GetX()>GetXValue(Size()-1)) mArray.AddElem(point);
				else mArray.InsertElem(0,point);
			}
			else{
				if(GetXValue(closestIndex)==point.GetX())
					SetValue(closestIndex,point.GetY());
				else
					mArray.InsertElem(closestIndex+1,point);
			}
		}
		mIsSplineUpdated=false;
	}

/**
* Inserts a point made of an X and a Y value in the correct position. Note that 
* points in the array are always sorted according to their X value. Must therefore 
* perform a previous serch.
* @param : X value
* @param : Y value
* @see : Insert
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::Insert(const TX &x,const TX &y)
	{
		PointTmpl<TX,TY> tmpPoint(x,y);	
		Insert(tmpPoint);
	}
	
/**
* Deletes the point found at the given index
* @param : index of the point to delete
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::DeleteIndex(TIndex index){
		mArray.DeleteElem(index);
		mIsSplineUpdated=false;
	}
/**

* Deletes the point with a closest X value to the one given
* @param : X value of the point to delete
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::DeleteThroughXValue(const TX& x)
	{
		mArray.DeleteElem(GetPosition(x));
		mIsSplineUpdated=false;
	}
	
/**
* Deletes the points between indices given
* @param : left index
* @param : right index
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::DeleteBetweenIndex(TIndex leftIndex,TIndex rightIndex)
	{
		int i;
		for(i=leftIndex;i<=rightIndex;i++)
		{
			DeleteIndex(leftIndex);
		}
		mIsSplineUpdated=false;
	}
/**
* Deletes the points having an X value in between the ones given
* @param : left X value
* @param : right X value
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::DeleteBetweenXValues(const TX& leftX,const TX& rightX)
	{
		TIndex leftIndex=GetPosition(leftX);
		TIndex rightIndex=GetPosition(rightX);
		DeleteBetweenIndex(leftIndex,rightIndex);
		mIsSplineUpdated=false;
	}

/** 
* Setting a different interpolation type. Auxiliary member arrays mc and md are resized if
* necessary.
* @param : new interpolation type
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::SetIntpType(const EInterpolation& eInterpolation)
	{
		meInterpolation=eInterpolation;
		switch(eInterpolation)
		{
			case(EInterpolation::eLinear)://linear interpolation between two closest points
			{
				mOrder=1;
				mc.Resize(2);
				mc.SetSize(2);
				md.Resize(2);
				md.SetSize(2);
				break;
			}
			case(EInterpolation::ePolynomial2)://parabolic interpolation
			{
				mOrder=2;
				mc.Resize(3);
				mc.SetSize(3);
				md.Resize(3);
				md.SetSize(3);
				break;
			}
			case(EInterpolation::ePolynomial3)://3rd order polynomial interpolation
			{
				mOrder=3;
				mc.Resize(4);
				mc.SetSize(4);
				md.Resize(4);
				md.SetSize(4);
				break;
			}
			case(EInterpolation::ePolynomial4)://4th order polynomial interpolation
			{
				mOrder=4;
				mc.Resize(5);
				mc.SetSize(5);
				md.Resize(5);
				md.SetSize(5);
				break;
			}
			case(EInterpolation::ePolynomial5)://5th order polynomial interpolation
			{
				mOrder=5;
				mc.Resize(6);
				mc.SetSize(6);
				md.Resize(6);
				md.SetSize(6);
				break;
			}
			case(EInterpolation::ePolynomialn):/*nth order polynomial interpolation where n is number
				of points in the BPF-1. Must be less than 10*/
			{
				if(Size()>10) throw Err("BPF::SetIntpType:Cannot ser more than 10th order interpolation");
				mOrder=Size()-1;
				mc.Resize(mOrder+1);
				mc.SetSize(mOrder+1);
				md.Resize(mOrder+1);
				md.SetSize(mOrder+1);
				break;
			}
		}
	}

	
/** 
* Getting the value of a point from the value of its X component. Performs the kind
* of interpolation passed as parameter
* @param : X value
* @param : interpolation type 
* @see : GetValueFromIndex
*/
	template <class TX,class TY>
	TY BPFTmpl<TX,TY>::GetValue(const TX& x,const EInterpolation& eInterpolation) const	/*Gets value
		according to the interpolation type set*/
	{
		/*First we should check if the the x value belongs to the BPF itself
		and there is no need to interpolate*/
		if(x<mLastX)  mLastIndex=0;
		PointTmpl<TX,TY> tmpPoint(x,0.0);
		TIndex i=mSearch.Find(tmpPoint,mLastIndex);
		if(i==-1)
		{
			if(GetXValue(0)>x) return GetValueFromIndex(0);
			else return GetValueFromIndex(Size()-1);
		}
		mLastIndex=i;
		mLastX=x;
		if(GetXValue(mLastIndex)==x) return GetValueFromIndex(mLastIndex);
		else
		{
			TY result;
			switch(eInterpolation)
			{
				case(EInterpolation::eStep)://returns previous point value
				{
					GetnClosest(mLastIndex);
					if(GetXValue(mClosestPoints[0])<=x)
						result=GetValueFromIndex(mClosestPoints[0]);
					else
						result=GetValueFromIndex(mClosestPoints[0]+1);
					break;
				}
				case(EInterpolation::eRound)://return closest point value
				{
					GetnClosest(mLastIndex);
					result=GetValueFromIndex(mClosestPoints[0]);
					break;
				}
				case(EInterpolation::eLinear)://linear interpolation between two closest points
				{
					TData error=0;
					if(GetXValue(mLastIndex)<=x)
					{
						/*if(mLastIndex==Size()-1){ 
							result=GetValueFromIndex(Size()-1);
							break;
						}
						else{*/
							mClosestPoints[0]=mLastIndex;
							mClosestPoints[1]=mLastIndex+1;
						//}
					}
					else
					{
						/*if(mLastIndex==0){ 
							result=GetValueFromIndex(0);
							break;}
						else{*/
						mClosestPoints[0]=mLastIndex-1;
						mClosestPoints[1]=mLastIndex;
						//}
					}
					result=BPFPolInt(x,mClosestPoints,error);
					break;
				}
				case(EInterpolation::eSpline)://3rd order spline interpolation
				{
					 if(!mIsSplineUpdated) 
						throw Err("BPF::Spline table not updated");
					result=BPFSplineInt(x);//get actual value
					break;
				}
				case(EInterpolation::ePolynomial2)://parabolic interpolation
				{
					TData error=0;
					GetnClosest(mLastIndex);
					result=BPFPolInt(x,mClosestPoints,error);
					break;
				}
				case(EInterpolation::ePolynomial3)://3rd order polynomial interpolation
				{
					TData error=0;
					GetnClosest(mLastIndex);
					result=BPFPolInt(x,mClosestPoints,error);
					break;
				}
				case(EInterpolation::ePolynomial4)://4th order polynomial interpolation
				{
					TData error=0;
					GetnClosest(mLastIndex);
					result=BPFPolInt(x,mClosestPoints,error);
					break;
				}
				case(EInterpolation::ePolynomial5)://5th order polynomial interpolation
				{
					TData error=0;
					GetnClosest(mLastIndex);
					result=BPFPolInt(x,mClosestPoints,error);
					break;
				}
				case(EInterpolation::ePolynomialn):/*nth order polynomial interpolation where n is number
					of points in the BPF-1*/
				{
					TIndex i;
					Array<TIndex> indexArray(mArray.Size());
					TData error=0;
					for(i=0;i<mArray.Size();i++)
					{
						indexArray[i]=i;
					}
					result=BPFPolInt(x,indexArray,error);
					break;
				}
				default:
				{
					CLAM_ASSERT(false, "Invalid BPF interpolation method.");
					result = 0;
					break;
				}
			}
			return result;
		}
	}
	
/**
* Fills the interpolation points index buffer with the n closest points to
* the selected point, trying to keep n/2 points to the right and n/2 to the left
* whenever possible, being n the interpolation order plus one.
* <p>
* When the point are too near the BPF edges, selected points are adjusted to this edge.
* When n is an even value, only n/2-1 points are selected at the left side of the selected index.
* @param foundIndex The index of the central point.
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::GetnClosest(TIndex foundIndex) const
	{
		CLAM_ASSERT(Size() >= mOrder+1, "BPF size must be at least the interpolation order plus one");
		int first = foundIndex-mOrder/2;
		// Fix the first index when it goes too
		if (first<0) first=0;
		// At least mOrder+1 fordward
		int safelimit = Size()-(mOrder+1);
		if (first>safelimit) first = safelimit;

		int i;
		for(i=0;i<mOrder+1;i++) {
			mClosestPoints[i]=first+i;
		}

		/*
		TIndex oP=GetPosition(x);
		int N = mArray.Size();
		TIndex i, j;

		TData elemToTake = (n-1.0)/2.0;

		int toTakeToTheLeft = (int)elemToTake;
		int	toTakeToTheRight = (int)ceil(elemToTake);

		int dL = (oP) - toTakeToTheLeft;
		int dR = (N-oP) - toTakeToTheRight;

		if (dL>= 0 && dR >= 0)
		{
			i = oP - toTakeToTheLeft;
			j = oP + toTakeToTheRight;

			if (j == N)
			{
				j = N-1;
				i--;
			}
		}
		else if (dL<0 && dR >= 0)
		{
			i = 0;
			j = oP + toTakeToTheRight + (-1)*dL;	
		}
		else if (dL>=0 && dR < 0)
		{
			j = N-1;
			i = oP - toTakeToTheLeft - (-1)*dR - 1 ;	

		}
		else if (dL<0 && dR < 0)
		{
			throw Err("Not enough Points to interpolate");
		} 

		for (int k=i; k<=j; k++)
		{
			mClosestPoints[k-i] = k;
		}*/

	}

/** 
* Get point position in BPf from X value
* @param : X value
* @return : index position
*/
	template <class TX,class TY>
	TIndex BPFTmpl<TX,TY>::GetPosition(const TX& x) const
	{
		PointTmpl<TX,TY> tmpPoint(x,0);	
		return mSearch.Find(tmpPoint);
	}
	
/**
*  = operator, TODO: check!!!!
*/
	template <class TX,class TY>
	BPFTmpl<TX,TY>& BPFTmpl<TX,TY>::operator=(const BPFTmpl<TX,TY> &originalBPF)
	{
		meInterpolation = originalBPF.meInterpolation;
		mArray	= originalBPF.mArray;
		mSearch.Set(mArray);
		mSplineTable = originalBPF.mSplineTable;
		mIsSplineUpdated = originalBPF.mIsSplineUpdated;
		mnPoints=originalBPF.mnPoints;
		mOrder=originalBPF.mOrder;
		return *this;
	} 
	
/**
* Updates Spline table creating it from scratch
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::UpdateSplineTable()
	{
		if(!mIsSplineUpdated) 
			CreateSplineTable(); // Create spline table if not updated
		mIsSplineUpdated=true;
	}

/**
* Changes the BPF border conditions
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::SetLeftDerivative(TData val)
	{
		mLeftDerivative = val;
		mIsSplineUpdated=false;
	}
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::UnsetLeftDerivative(void)
	{
		if (mLeftDerivative < Infinity) {
			mLeftDerivative = Infinity;
			mIsSplineUpdated=false;
		}
	}
/**
* Changes the BPF border conditions
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::SetRightDerivative(TData val)
	{
		mRightDerivative = val;
		mIsSplineUpdated=false;
	}
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::UnsetRightDerivative(void)
	{
		if (mLeftDerivative < Infinity) {
			mRightDerivative = Infinity;
			mIsSplineUpdated=false;
		}
	}

/**
* Member protected method
* Performs the actual polynomial interpolation algorithm
*/
	template <class TX,class TY>
	TY BPFTmpl<TX,TY>::BPFPolInt
		(const TX& x,const Array<TIndex>& closestPointsIndex, TData &errorEstimate)
		const
	{
		int i,m,iClosest=1;
		TY y;
		TX den,dif,dift,ho,hp,w;
					
		dif=Abs(x-GetXValue(closestPointsIndex[0]));
		
		for(i=1;i<=mOrder+1;i++)
		{
			if((dift=Abs(x-GetXValue(closestPointsIndex[i-1])))<dif)
			{
				iClosest=i;
				dif=dift;
			}

			md[i-1]=mc[i-1]=GetValueFromIndex(closestPointsIndex[i-1]);
			
		}
	
		y=GetValueFromIndex(closestPointsIndex[iClosest-1]);
		iClosest--;
		for(m=1;m<mOrder+1;m++)
		{
			for(i=1;i<=mOrder+1-m;i++)
			{
				ho=GetXValue(closestPointsIndex[i-1])-x;
				hp=GetXValue(closestPointsIndex[i+m-1])-x;
				w=mc[i]-md[i-1];
				if((den=ho-hp)==0) //Error
				{
					throw Err("Error interpolating BPF");
				}
				den=w/den;
				md[i-1]=hp*den;
				mc[i-1]=ho*den;
			}
			if(2*iClosest<(mOrder+1-m)) 
			{
				errorEstimate=mc[iClosest];
			}
			else
			{
				errorEstimate=md[iClosest-1];
				iClosest--;
			}
			y+=errorEstimate;
		}

		return y;
	}
/**
* Member protected method
* creates spline table from current points in BPF
*/
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::CreateSplineTable()
	{
		int i,k;
		TY p,qn,un,sig;
		int n=mArray.Size();

		CLAM_ASSERT(n > 2,"BPF size too small for spline");

		Array<TY> u(n);
		u.SetSize(n);
		mSplineTable.Resize(n);
		mSplineTable.SetSize(n);

		if (mLeftDerivative >= Infinity)
			mSplineTable[0]=u[0]=0.0; // For a 'natural' spline
		else {
			mSplineTable[0] = -0.5;
			u[0]= (TData(3.0) / (GetXValue(1)-GetXValue(0)) ) * 
			      ( (GetValueFromIndex(1) - GetValueFromIndex(0)) /
			                (GetXValue(1)         - GetXValue(0))
			                        - mLeftDerivative );
		}

		for(i=2;i<=n-1;i++)
		{
			sig=(GetXValue(i-1)-GetXValue(i-2))/(GetXValue(i)-GetXValue(i-2));
			p=sig*mSplineTable[i-2]+2;
			mSplineTable[i-1]=(sig-1)/p;
			u[i-1]=(GetValueFromIndex(i)-GetValueFromIndex(i-1))/(GetXValue(i)-GetXValue(i-1))-
				(GetValueFromIndex(i-1)-GetValueFromIndex(i-2))/(GetXValue(i-1)-GetXValue(i-2));
			u[i-1]=(6*u[i-1]/(GetXValue(i)-GetXValue(i-2))-sig*u[i-2])/p;
		}
		if (mRightDerivative >= Infinity) {
			qn=un=0.0; //For a 'natural' spline
		}
		else {
			qn = 0.5;
			un = (TData(3.0)/(GetXValue(n-1)-GetXValue(n-2))) *
			  ( mRightDerivative - 
			      ( GetValueFromIndex(n-1) - GetValueFromIndex(n-2)) /
			      ( GetXValue(n-1)         - GetXValue(n-2)));
		}
		mSplineTable[n-1]=((un-qn*u[n-2])/(qn*mSplineTable[n-2]+1));
		for(k=n-1;k>=1;k--)
		{
			mSplineTable[k-1]=mSplineTable[k-1]*mSplineTable[k]+u[k-1];
		}
	}
/**
* Member protected method
* Performs the actual 3rd order spline interpolation algorithm
*/
	template <class TX,class TY>
	TY BPFTmpl<TX,TY>::BPFSplineInt(const TX& x) const
	{
		int klo,khi,k;
		TX h,b,a;

		klo=1;
		khi=mArray.Size();
		while(khi-klo>1)
		{
			k=(khi+klo)>>1;
			if(GetXValue(k-1)>x) khi=k;
			else klo=k;
		}
		h=GetXValue(khi-1)-GetXValue(klo-1);
		if(h==0.0)
		{
			throw Err("Error interpolating Spline");
		}
		a=(GetXValue(khi-1)-x)/h;
		b=(x-GetXValue(klo-1))/h;
		return (a*GetValueFromIndex(klo-1)+b*GetValueFromIndex(khi-1)+((a*a*a-a)*
			mSplineTable[klo-1]+(b*b*b-b)*mSplineTable[khi-1])*(h*h)/6);
	}

	template <class TX,class TY>
	void BPFTmpl<TX,TY>::StoreOn(Storage & storage)
	{
#		ifdef CLAM_USE_XML	 
		// This condition is not needed because storing an XML adapter
		// onto a non XML storage has no effect but it enhances performance.
		if (dynamic_cast < XMLStorage* > (&storage))
		{
			XMLComponentAdapter adapterInt(meInterpolation,"Interpolation",true);
			storage.Store(&adapterInt);
			XMLComponentAdapter adapter(mArray, "Points", true);
			storage.Store(&adapter);
		}
#		endif//CLAM_USE_XML
	}
	template <class TX,class TY>
	void BPFTmpl<TX,TY>::LoadFrom(Storage & storage)
	{
#		ifdef CLAM_USE_XML	 
		// This condition is not needed because storing an XML adapter
		// onto a non XML storage has no effect but it enhances performance.
		if (dynamic_cast < XMLStorage* > (&storage))
		{
			XMLComponentAdapter adapterInt(meInterpolation,"Interpolation",true);
			storage.Load(&adapterInt);
			XMLComponentAdapter adapter(mArray, "Points", true);
			storage.Load(&adapter);
		}
#		endif//CLAM_USE_XML
	}

} // namespace CLAM


#endif // _BPFTmplDef_
