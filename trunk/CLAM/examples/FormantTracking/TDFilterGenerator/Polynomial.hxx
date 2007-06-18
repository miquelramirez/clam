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

#ifndef _Polynomial_
#define _Polynomial_

#include "ProcessingData.hxx"
#include "Complex.hxx"
#include "Polar.hxx"
#include "Array.hxx"
#include "PolynomialRootsFinder.hxx"


namespace CLAM
{

	class Polynomial: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Polynomial, 2, ProcessingData);

		/** The coefficients buffer */
		DYN_ATTRIBUTE (0, public, DataArray, Coefficients);
		/** The roots buffer in complex notation */
		DYN_ATTRIBUTE (1, public, Array< Complex >, Roots);
	protected:

		void DefaultInit(); // To be used from the dynamic-type-generated constructor.

	public:

		/** @returns the number of polynomial coefficients */
		int GetSize() const { return GetCoefficients().Size(); }

		/** Resizes the array coefficients and roots buffers to a new size */
		void SetSize( int s );

		unsigned char CalculateRoots( void );

		void CheckTypes(void) const;

	};

} // namespace

#endif // Polynomial.hxx

