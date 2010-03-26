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


#include "Polar.hxx"
#include <iostream>
#include <sstream>

namespace CLAM
{
	// complex '+=' operator
	const Polar& Polar::operator += (const Polar& a)
	{
		TData r1 = fabs(a.mMag) * CLAM_cos(a.mAng); 
		TData i1 = fabs(a.mMag) * CLAM_sin(a.mAng); 
		TData r2 = fabs(mMag) * CLAM_cos(mAng);
		TData i2 = fabs(mMag) * CLAM_sin(mAng);
		
		TData r3 = r1+r2;
		TData i3 = i1+i2;
		
		mMag = sqrt (r3*r3 + i3*i3);
		mAng = atan2 (i3,r3); 
	
		return *this;
	}

	//  complex '-=' operator 
	const Polar& Polar::operator -= (const Polar& a)
	{
		TData r1 = fabs(a.mMag) * CLAM_cos(a.mAng); 
		TData i1 = fabs(a.mMag) * CLAM_sin(a.mAng); 
		TData r2 = fabs(mMag) * CLAM_cos(mAng);
		TData i2 = fabs(mMag) * CLAM_sin(mAng);
		
		TData r3 = r2-r1;
		TData i3 = i2-i1;
		
		mMag = CLAM_sqrt (r3*r3 + i3*i3);
		mAng = CLAM_atan2 (i3,r3); 
	
		return *this;
	}

	// polar '+' operator 
	Polar Polar::operator + (const Polar& b) const
	{
		TData r1 = fabs(mMag) * CLAM_cos(mAng); 
		TData i1 = fabs(mMag) * CLAM_sin(mAng); 
		TData r2 = fabs(b.mMag) * CLAM_cos(b.mAng);
		TData i2 = fabs(b.mMag) * CLAM_sin(b.mAng);
		
		TData r3 = r1+r2;
		TData i3 = i1+i2;
		
		Polar ret(CLAM_sqrt (r3*r3 + i3*i3),CLAM_atan2 (i3,r3)); 
		return ret;
	}

	 //  polar '-' operator 
	Polar Polar::operator - (const Polar& b) const
	{
		TData r1 = fabs(mMag) * CLAM_cos(mAng); 
		TData i1 = fabs(mMag) * CLAM_sin(mAng); 
		TData r2 = fabs(b.mMag) * CLAM_cos(b.mAng);
		TData i2 = fabs(b.mMag) * CLAM_sin(b.mAng);
		
		TData r3 = r1-r2;
		TData i3 = i1-i2;
		
		Polar ret(CLAM_sqrt (r3*r3 + i3*i3),CLAM_atan2 (i3,r3)); 
		return ret;
	}

	std::istream& operator >> (std::istream & is,
			Polar & a)
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
		TData m;
		TData p; 
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

	std::ostream& operator << (std::ostream& myStream, const Polar& a)
	{
		return myStream 
			<< "{"
			<< a.Mag()
			<< " "
			<< a.Ang()
			<< "}";
	}

} // namespace CLAM


