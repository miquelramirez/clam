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


#ifndef _PolarTmplDec_
#define _PolarTmplDec_

#include <iosfwd>
#include "CLAM_Math.hxx"

namespace CLAM
{

	template <class T>
	class PolarTmpl
	{
	private:
		T mMag, mAng;

	public:
		PolarTmpl(T mag = 0.0,T ang = 0.0)// constructor
		{
			mMag = mag;
			mAng = ang;
		};

		const T Mag(void) const {return mMag;}; // accessor 
		const T Ang(void) const {return mAng;}; // accessor 

		void SetMag(const T& mag) { mMag = mag;}; // accesor
		void SetAng(const T& ang) { mAng = ang;}; // accesor

		// returns real part
		const T Real (void) const
		{
			return fabs(mMag) * CLAM_cos(mAng);
		}

		// returns imaginary part
		const T Imag (void) const
		{
			return fabs(mMag) * CLAM_sin(mAng); 
		}

		// friend function to handle cartesian coordinates
		friend PolarTmpl<T> ToComplex(const T& real, const T& imag)
		{
			return PolarTmpl<T> (CLAM_sqrt (real*real + imag*imag),CLAM_atan2 (imag,real));
		};

		// ------   member operators ... ------
		//  complex '=' operator  (float)
		const PolarTmpl<T>& operator = (const float mag)
		{
			mMag = mag;
			mAng = 0;
			return *this;
		}

		// complex '=' operator           
		const PolarTmpl<T>& operator = (const PolarTmpl<T>& a) 
		{
			mMag = a.mMag;
			mAng = a.mAng;
			return *this;

		}

		// complex '+=' operator
		const PolarTmpl<T>& operator += (const PolarTmpl<T>& a);

		//  complex '-=' operator 
		const PolarTmpl<T>& operator -= (const PolarTmpl<T>& a);

		// polar '-' operator 
		PolarTmpl<T> operator - (const PolarTmpl<T>& b) const;

		// polar '+' operator 
		PolarTmpl<T> operator + (const PolarTmpl<T>& b) const;

		// complex '*' operator
		PolarTmpl<T> operator * (const PolarTmpl<T>& b) const
		{
			PolarTmpl<T> ret(mMag * b.mMag,mAng + b.mAng);
			return ret;  
		}

		// complex '/' operator 
		PolarTmpl<T> operator / (const PolarTmpl<T>& b) const
		{
			PolarTmpl<T> ret(mMag / b.mMag,mAng - b.mAng);
			return ret;  
		}

		// complex '==' operator
		bool operator == (const PolarTmpl<T>& b) const
		{	
			if ((mMag == b.mMag)&&(mAng == b.mAng)) return true;
			else return false;
		}

		// polar '!=' operator
		bool operator != (const PolarTmpl<T>& b) const
		{	
			if ((mMag == b.mMag)&&(mAng == b.mAng)) return false;
			else return true;
		}

	};

	template <class T>
	std::istream& operator >> (std::istream & stream, PolarTmpl<T> & a);
	
	template <class T>
	std::ostream& operator << (std::ostream & stream, const PolarTmpl<T> & a);
	
} // namespace CLAM

#endif // _PolarTmplDec_

