/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __LPMODEL__
#define __LPMODEL__

#include "ProcessingData.hxx"
#include "Array.hxx"

namespace CLAM
{
	class Spectrum;

	class LPModel /*of the spectral characteristics of a given signal*/
		: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( LPModel, 5, ProcessingData );

		DYN_ATTRIBUTE( 0, public, DataArray,   FilterCoefficients );
		DYN_ATTRIBUTE( 1, public, DataArray,   ReflectionCoefficients );
		DYN_ATTRIBUTE( 2, public, TData,       AvgSqrFilterError );
		DYN_ATTRIBUTE( 3, public, TData,       SpectralRange );
		DYN_ATTRIBUTE( 4, protected, unsigned, Order );

	public:
		void ToSpectrum( Spectrum& spec ) const;
		void UpdateModelOrder( TSize order );

	protected:
		void DefaultInit();
	};
}


#endif // LPModel.hxx

