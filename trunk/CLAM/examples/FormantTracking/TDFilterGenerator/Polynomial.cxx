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

#include "Polynomial.hxx"
#include <string>
#include <sstream>

namespace CLAM
{

	void Polynomial::DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}

	void Polynomial::SetSize( int s )
	{
		int oldSize;
		oldSize = GetSize();

		if( s == oldSize )	return;

		CLAM_ASSERT( s>=0, "Polynomial::SetSize(): Negative size specified" );
		if( HasCoefficients() )
		{
			GetCoefficients().Resize( s );
			GetCoefficients().SetSize( s );
		}

		if( HasRoots() )
		{
			GetRoots().Resize( s );
			GetRoots().SetSize( s );
		}
	}

	void Polynomial::CheckTypes(void) const
	{

		CLAM_BEGIN_CHECK
		
		if (GetCoefficients().Size()!= GetRoots().Size() ) { 
			std::stringstream ss;
			ss << "Polynomial::CalculateRoots: Wrong size in coefficients and roots buffers\n"
			   << "  Expected the same size" << ", used: \n" 
			   << GetCoefficients().Size() << " for coefficients buffer\n"
			   << "and " << GetRoots().Size() << " for complex roots buffer\n";
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if ( GetCoefficients().Size() == 0 )
			CLAM_ASSERT(0,"Polynomial CalculateRoots: Complex attribute required for Array object.");

		CLAM_END_CHECK
	}


	void Polynomial::CalculateRoots( void )
	{
		unsigned char flag = 1;
		double maxerr;
		DataArray	u, v, conv;

		CheckTypes();

		int degree = GetSize()-1;

		u.Resize( GetSize() );
		u.SetSize( GetSize() );

		v.Resize( GetSize() );
		v.SetSize( GetSize() );

		conv.Resize( GetSize() );
		conv.SetSize( GetSize() );

/*		Array<Complex> pred,p;

		p.Resize( GetSize() );
		p.SetSize( GetSize() );
		
		pred.Resize( GetSize() );
		pred.SetSize( GetSize() );
	
		p = GetCoefficients();		
		
		err = null( p.GetPtr() ,pred.GetPtr(),&degree,
			GetRoots().GetPtr(),&maxerr,(unsigned char)flag);
*/
		
		FindPolynomialRoots( GetCoefficients().GetPtr(),
							 u.GetPtr(), v.GetPtr(), conv.GetPtr(),
							 degree, 16, 4 );

		for(int i = 0; i < GetRoots().Size(); i++)
		{
			GetRoots()[i] = Complex( u[i], v[i] );
			std::cout << GetRoots()[i] << std::endl;
		}

	}
} //namespace
