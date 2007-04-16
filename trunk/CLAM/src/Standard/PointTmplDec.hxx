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


#ifndef _PointTmplDec_
#define _PointTmplDec_

#include <iosfwd>
#include "CLAM_Math.hxx"

namespace CLAM
{

	template <typename TX = TData, typename TY = TX> class PointTmpl
	{
	public:
	/*Constructors*/
		PointTmpl() : mX(0), mY(0)
		{};

		PointTmpl(TX xValue,TY yValue)
		{
			mX=xValue;
			mY=yValue;
		}

	/*Getting Values*/
		const TX& GetX() const{return mX;}
		const TY& GetY() const{return mY;}

	/*Setting Values*/
		void SetX(TX xValue){mX=xValue;}
		void SetY(TY yValue){mY=yValue;}

	/*Member operators*/
		const PointTmpl<TX,TY>& operator=(const PointTmpl<TX,TY>& newPoint)
		{
			mX=newPoint.mX;
			mY=newPoint.mY;
			return *this;
		}
		const PointTmpl<TX,TY>& operator+=(const PointTmpl<TX,TY>& newPoint)
		{
			mX+=newPoint.mX;
			mY+=newPoint.mY;
			return *this;
		}
		const PointTmpl<TX,TY>& operator-=(const PointTmpl<TX,TY>& newPoint)
		{
			mX-=newPoint.mX;
			mY-=newPoint.mY;
			return *this;
		}
		bool operator<(const PointTmpl<TX,TY>& newPoint)const{return mX<newPoint.GetX();}
		bool operator<=(const PointTmpl<TX,TY>& newPoint)const{return mX<=newPoint.GetX();}
		bool operator>(const PointTmpl<TX,TY>& newPoint)const{return mX>newPoint.GetX();}
		bool operator>=(const PointTmpl<TX,TY>& newPoint)const{return mX>=newPoint.GetX();}
		bool operator!=(const PointTmpl<TX,TY>& newPoint)const{return (mX!=newPoint.GetX());}
		bool operator==(const PointTmpl<TX,TY>& newPoint)const{return (mX==newPoint.GetX());}

	/*member funtion, euclidian distance*/
		double Distance(const PointTmpl<TX,TY>& newPoint)const
		{
			return CLAM_pow((TData)CLAM_pow((TData)(mX-newPoint.mX),(TData)2)+CLAM_pow((TData)(mY-newPoint.mY),(TData)2),(TData)0.5);
		}

	/*non-member operators*/
		PointTmpl<TX,TY> operator-(const PointTmpl<TX,TY>& otherPoint)
		{
			PointTmpl<TX,TY> result(mX-otherPoint.mX,mY-otherPoint.mY);
			return result;
		}
		PointTmpl<TX,TY> operator+(const PointTmpl<TX,TY>& otherPoint)
		{
			PointTmpl<TX,TY> result(mX+otherPoint.mX,mY+otherPoint.mY);
			return result;
		}

	protected:

	/*member variables, x and y values*/
		TX mX;
		TY mY;
	};

	template <class TX,class TY>
	std::istream& operator >> (std::istream & stream, PointTmpl<TX,TY> & a);
	
	template <class TX,class TY>
	std::ostream& operator << (std::ostream & stream, const PointTmpl<TX,TY> & a);
	
} // namespace CLAM

#endif // _PointTmplDec_

