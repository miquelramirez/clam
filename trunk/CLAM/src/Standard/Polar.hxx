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


#ifndef CLAM_Polar_hxx
#define CLAM_Polar_hxx

#include <iosfwd>
#include "CLAM_Math.hxx"
#include "TypeInfo.hxx"

namespace CLAM
{

	class Polar
	{
		typedef TData T;
	private:
		T mMag, mAng;

	public:
		Polar(T mag = 0.0,T ang = 0.0)// constructor
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
		friend Polar ToComplex(const T& real, const T& imag)
		{
			return Polar (CLAM_sqrt (real*real + imag*imag),CLAM_atan2 (imag,real));
		};

		// ------   member operators ... ------
		//  complex '=' operator  (float)
		const Polar& operator = (const float mag)
		{
			mMag = mag;
			mAng = 0;
			return *this;
		}

		// complex '=' operator           
		const Polar& operator = (const Polar& a) 
		{
			mMag = a.mMag;
			mAng = a.mAng;
			return *this;

		}

		// complex '+=' operator
		const Polar& operator += (const Polar& a);

		//  complex '-=' operator 
		const Polar& operator -= (const Polar& a);

		// polar '-' operator 
		Polar operator - (const Polar& b) const;

		// polar '+' operator 
		Polar operator + (const Polar& b) const;

		// complex '*' operator
		Polar operator * (const Polar& b) const
		{
			Polar ret(mMag * b.mMag,mAng + b.mAng);
			return ret;  
		}

		// complex '/' operator 
		Polar operator / (const Polar& b) const
		{
			Polar ret(mMag / b.mMag,mAng - b.mAng);
			return ret;  
		}

		// complex '==' operator
		bool operator == (const Polar& b) const
		{	
			if ((mMag == b.mMag)&&(mAng == b.mAng)) return true;
			else return false;
		}

		// polar '!=' operator
		bool operator != (const Polar& b) const
		{	
			if ((mMag == b.mMag)&&(mAng == b.mAng)) return false;
			else return true;
		}

	};

	std::istream& operator >> (std::istream & stream, Polar & a);
	std::ostream& operator << (std::ostream & stream, const Polar & a);
	
	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Polar);

} // namespace CLAM

#endif // CLAM_Polar_hxx

