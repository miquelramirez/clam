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


#ifndef _PolarTmplDef_
#define _PolarTmplDef_

#include <iostream>
#include <sstream>

namespace CLAM
{
	// complex '+=' operator
	template <class T>
	const PolarTmpl<T>& PolarTmpl<T>::operator += (const PolarTmpl<T>& a)
	{
		T r1,i1,r2,i2,r3,i3;
		
		r1 = fabs(a.mMag) * CLAM_cos(a.mAng); 
		i1 = fabs(a.mMag) * CLAM_sin(a.mAng); 
		r2 = fabs(mMag) * CLAM_cos(mAng);
		i2 = fabs(mMag) * CLAM_sin(mAng);
		
		r3 = r1+r2;
		i3 = i1+i2;
		
		mMag = sqrt (r3*r3 + i3*i3);
		mAng = atan2 (i3,r3); 
	
		return *this;
	}

	//  complex '-=' operator 
	template <class T>
	const PolarTmpl<T>& PolarTmpl<T>::operator -= (const PolarTmpl<T>& a)
	{
		T r1,i1,r2,i2,r3,i3;

		r1 = fabs(a.mMag) * CLAM_cos(a.mAng); 
		i1 = fabs(a.mMag) * CLAM_sin(a.mAng); 
		r2 = fabs(mMag) * CLAM_cos(mAng);
		i2 = fabs(mMag) * CLAM_sin(mAng);
		
		r3 = r2-r1;
		i3 = i2-i1;
		
		mMag = CLAM_sqrt (r3*r3 + i3*i3);
		mAng = CLAM_atan2 (i3,r3); 
	
		return *this;
	}

	// polar '+' operator 
	template <class T>
	PolarTmpl<T> PolarTmpl<T>::operator + (const PolarTmpl<T>& b) const
	{
		T r1,i1,r2,i2,r3,i3;
		
		r1 = fabs(mMag) * CLAM_cos(mAng); 
		i1 = fabs(mMag) * CLAM_sin(mAng); 
		r2 = fabs(b.mMag) * CLAM_cos(b.mAng);
		i2 = fabs(b.mMag) * CLAM_sin(b.mAng);
		
		r3 = r1+r2;
		i3 = i1+i2;
		
		PolarTmpl<T> ret(CLAM_sqrt (r3*r3 + i3*i3),CLAM_atan2 (i3,r3)); 
		return ret;
	}

	 //  polar '-' operator 
	template <class T>
	PolarTmpl<T> PolarTmpl<T>::operator - (const PolarTmpl<T>& b) const
	{
		T r1,i1,r2,i2,r3,i3;
		
		r1 = fabs(mMag) * CLAM_cos(mAng); 
		i1 = fabs(mMag) * CLAM_sin(mAng); 
		r2 = fabs(b.mMag) * CLAM_cos(b.mAng);
		i2 = fabs(b.mMag) * CLAM_sin(b.mAng);
		
		r3 = r1-r2;
		i3 = i1-i2;
		
		PolarTmpl<T> ret(CLAM_sqrt (r3*r3 + i3*i3),CLAM_atan2 (i3,r3)); 
		return ret;
	}

	template <class T>
	inline std::istream& operator >> (std::istream & is,
			PolarTmpl<T> & a)
	{
		if (is.flags() & std::ios::skipws) {
			char c = '\0';
			do
				is.get(c);
			while (is && isspace(c));
			if (is)	is.putback(c);
		}
		char c = '\0';
		is >> c;
		if (c!='{') {
			if (is)	is.putback(c);
//			std::cerr << "A polar starting with '" << c << "'" << std::endl;
			return is;
		}
		T m;
		T p; 
		if (!(is >> m)) return is;
		if (!(is >> p)) return is;
		if (is.flags() & std::ios::skipws) {
			char c = '\0';
			do
				is.get(c);
			while (is && isspace(c));
			if (is) is.putback(c);
		}
		if (!is.get(c) || c!='}') return is;

		a.SetMag(m);
		a.SetAng(p);
		return is;
	}

	template <class T> 
	std::ostream& operator << (std::ostream& myStream, const PolarTmpl<T>& a)
	{
		return myStream 
			<< "{"
			<< a.Mag()
			<< " "
			<< a.Ang()
			<< "}";
	}

} // namespace CLAM


#endif // _PolarTmplDef_

