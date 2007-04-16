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

#ifndef __MATRIX__
#define __MATRIX__

#include <valarray>
#include "Slice_iter.hxx"
#include "CSlice_iter.hxx"

namespace CLAMGUI{

template < typename T >
class Matrix
{
	std::valarray<T>*    pData;
	unsigned int          mdim1;
	unsigned int          mdim2;

public:

	Matrix( unsigned int x, unsigned int y )
	{
		mdim1 = x;
		mdim2 = y;

		pData = new std::valarray<T>( x*y );
	}

	Matrix( const Matrix& m );

	Matrix& operator=( const Matrix& m );

	~Matrix()
	{
		delete pData;
	}

	unsigned int size() const
	{
		return mdim1*mdim2;
	}

	unsigned int dim1() const 
	{
		return mdim1;
	}

	unsigned int dim2() const
	{
		return mdim2;
	}

	inline T& operator()( unsigned int i, unsigned int j )
	{
		return row( i )[ j ];
	}

	inline T operator()( unsigned int i, unsigned int j ) const
	{
		return row(i)[j];
	}


	inline Slice_iter<T> row( unsigned int i )
	{
		return Slice_iter<T>( pData, std::slice( i*mdim2, mdim2, 1 ) );
	}

	inline CSlice_iter<T> row( unsigned int i ) const
	{
		return CSlice_iter<T>(  pData, std::slice( i*mdim2, mdim2, 1) );
	}

	inline Slice_iter<T> column( unsigned int j )
	{
		return Slice_iter<T>(  pData, std::slice( j, mdim1, mdim2 ) );
	}

	inline CSlice_iter<T> column( unsigned int j ) const
	{
		return CSlice_iter<T>(  pData, std::slice( j, mdim1, mdim2) );
	}

	Slice_iter<T> operator[]( unsigned int i )
	{
		return row( i );
	}

	CSlice_iter<T> operator[]( unsigned int i ) const
	{
		return row( i );
	}

	Matrix& operator*( T d )
	{
		(*pData) *= d;

		return *this;
	}

	inline std::valarray<T> operator*( const std::valarray<T>& v )
	{
		std::valarray<T> res( dim1() );

		for ( int i = 0; i < dim1(); i++ )
			res(i) = mul( row(i), v );

		return res;
	}



	std::valarray<T>& array() 
	{
		return *pData;
	}

private:

	inline T mul( const std::valarray<T>& v1, const std::valarray<T>& v2 )
	{
		T res = 0;

		for (int i = 0; i < v1.size(); i++)
			res+=v1[i]*v2[i];

		return res;
	}

};

};//Namespace CLAMGUI

#endif // Matrix.hxx

