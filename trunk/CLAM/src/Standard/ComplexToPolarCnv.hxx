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

/* Template class used to convert array of polars into arrays of complexs */

#ifndef _ComplexToPolarCnv_
#define _ComplexToPolarCnv_

#include "Complex.hxx"
#include "Polar.hxx"
#include "Array.hxx"
#include "Err.hxx"
#include "DataTypes.hxx"

namespace CLAM {

template <class T> class ComplexToPolarCnv
{
public:

/* Constructors and destructors */
	ComplexToPolarCnv(){};
	~ComplexToPolarCnv(){};

/* Converting routines */

	/* Complex To Polar */
	void ToPolar(const Array<ComplexTmpl<T> > &inputArray, 
		Array<PolarTmpl<T> > &outputArray) const
	{
		CLAM_ASSERT(outputArray.Size() == inputArray.Size(),
			"ComplexToPolarCnv::Convert: unable to convert buffers with different sizes");
		
		for (int i=0; i<inputArray.Size(); i++) {
			outputArray[i].SetMag(inputArray[i].Mag());
			outputArray[i].SetAng(inputArray[i].Ang()); 
		}
	}

	/* Polar To Complex */
	void ToComplex(const Array<PolarTmpl<T> > &inputArray, 
		Array<ComplexTmpl<T> > &outputArray) const
	{
		CLAM_ASSERT(outputArray.Size() == inputArray.Size(),
			"ComplexToPolarCnv::Convert: unable to convert buffers with different sizes");
		
		for (int i=0; i<inputArray.Size(); i++) {
			outputArray[i].SetReal(inputArray[i].Real());
			outputArray[i].SetImag(inputArray[i].Imag());
		}
	}
};

typedef ComplexToPolarCnv<TData> ComplexToPolarCnv_;

}

#endif

