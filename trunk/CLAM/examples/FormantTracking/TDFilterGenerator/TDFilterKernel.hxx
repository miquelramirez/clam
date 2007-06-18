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

#ifndef _TDFilterKernel_
#define _TDFilterKernel_

#include "Enum.hxx"
#include "Polynomial.hxx"

namespace CLAM
{
	class ETDFilterType: public Enum
	{
	public:

		static tEnumValue sEnumValues[];
		static tValue sDefault;
		ETDFilterType() : Enum( sEnumValues, sDefault ) {}
		ETDFilterType( tValue v ) : Enum( sEnumValues, v ) {}
		ETDFilterType( std::string s ) : Enum( sEnumValues, s ) {}

		typedef enum {
			eAutoregressive,
			eARMA,
			eFIR,
		};

		virtual Component* Species() const
		{
			return ( Component* ) new ETDFilterType( eAutoregressive );
		};
	};

	class TDFilterKernel: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (TDFilterKernel, 9, ProcessingData);
	private:
		/** The filter poles */
		DYN_ATTRIBUTE (0, private, Polynomial, PolesPolynomial);
		/** The filter zeros */
		DYN_ATTRIBUTE (1, private, Polynomial, ZerosPolynomial);
	public:
		/** The frequencies of the filter zeros **/
		DYN_ATTRIBUTE (2, public, DataArray, FreqZeros);
		/** The frequencies of the filter poles **/
		DYN_ATTRIBUTE (3, public, DataArray, FreqPoles);
		/** The amplitudes of the filter zeros **/
		DYN_ATTRIBUTE (4, public, DataArray, AmpZeros);
		/** The amplitudes of the filter poles **/
		DYN_ATTRIBUTE (5, public, DataArray, AmpPoles);

		DYN_ATTRIBUTE (6, public, TSize, PolesOrder);

		DYN_ATTRIBUTE (7, public, TSize, ZerosOrder);

		DYN_ATTRIBUTE (8, public, TSize, NPoles);

	protected:

		void DefaultInit(); // To be used from the dynamic-type-generated constructor.
		
	private:

		ETDFilterType	mType;

	public:

		void CheckTypes(void) const;

		void SetType( ETDFilterType &type ) ;

		inline const ETDFilterType &GetType(void) const { return mType; }

		void SetPolesSize( TSize s );

		void SetZerosSize( TSize s );

		inline DataArray &B() const { return GetZerosPolynomial().GetCoefficients(); }

		inline void B( const DataArray& B ) { GetZerosPolynomial().SetCoefficients( B ); }

		inline DataArray &A() const { return GetPolesPolynomial().GetCoefficients(); }

		inline void A( const DataArray& A ) { GetPolesPolynomial().SetCoefficients( A ); }

		inline Array<Complex> &Zeros() const { return GetZerosPolynomial().GetRoots(); }

		inline void Zeros( const Array<Complex> &zeros ) { GetZerosPolynomial().SetRoots( zeros ); }

		inline Array<Complex> &Poles() const { return GetPolesPolynomial().GetRoots(); }

		inline void Poles( const Array<Complex> &poles ) { GetPolesPolynomial().SetRoots( poles ); }

	};
} //nampespace

#endif //TDFilterKernel.hxx

