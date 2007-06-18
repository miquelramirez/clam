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

#include "TDFilterKernel.hxx"
#include <string>
#include <sstream>

namespace CLAM
{
	Enum::tEnumValue ETDFilterType::sEnumValues[] = {
		{ ETDFilterType::eFIR, "FIR" },
		{ ETDFilterType::eAutoregressive, "Autoregressive" },
		{ ETDFilterType::eARMA, "ARMA" },
		{ 0, NULL }
	};

	Enum::tValue ETDFilterType::sDefault = ETDFilterType::eAutoregressive ;

	void TDFilterKernel::DefaultInit()
	{
		UpdateData();
	}

	void TDFilterKernel::SetType( ETDFilterType &type )
	{
		switch( (int)type )
		{
		case ETDFilterType::eFIR:
			{
				mType = type;

				AddZerosPolynomial();
				AddFreqZeros();
				AddAmpZeros();
				AddZerosOrder();

				break;
			}
		case ETDFilterType::eAutoregressive:
			{
				mType = type;

				AddPolesPolynomial();
				AddFreqPoles();
				AddAmpPoles();
				AddPolesOrder();
				AddNPoles();

				break;
			}
		case ETDFilterType::eARMA:
			{
				mType = type;

				AddZerosPolynomial();
				AddFreqZeros();
				AddAmpZeros();
				AddZerosOrder();

				AddPolesPolynomial();
				AddFreqPoles();
				AddAmpPoles();
				AddPolesOrder();
				AddNPoles();

				break;
			}
		}

		UpdateData();
	}

	void TDFilterKernel::SetPolesSize( TSize s )
	{
		GetPolesPolynomial().SetSize( s + 1 );

		int ss = s / 2;
		GetFreqPoles().Resize(ss);
		GetFreqPoles().SetSize(ss);

		GetAmpPoles().Resize(ss);
		GetAmpPoles().SetSize(ss);

		SetPolesOrder( s + 1 );
	}

	void TDFilterKernel::SetZerosSize( TSize s )
	{
		GetZerosPolynomial().SetSize( s + 1 );

		GetFreqZeros().Resize(s);
		GetFreqZeros().SetSize(s);

		GetAmpZeros().Resize(s);
		GetAmpZeros().SetSize(s);

		SetZerosOrder( s + 1 );
	}

	void TDFilterKernel::CheckTypes(void) const
	{
		CLAM_BEGIN_CHECK

		if( HasZerosPolynomial() )
		{
			GetZerosPolynomial().CheckTypes();

			if( GetAmpZeros().Size() != GetZerosOrder() - 1 )
			{
				std::stringstream ss;
				ss << "TDFilterKernel::CheckTypes: Wrong size in array of zeros amplitudes.\n"
				   << "	Expected: " << GetZerosOrder() - 1 << ", used " << GetAmpZeros().Size();
				CLAM_ASSERT(0,ss.str().c_str());
			}
			if( GetFreqZeros().Size() != GetZerosOrder() - 1 )
			{
				std::stringstream ss;
				ss << "TDFilterKernel::CheckTypes: Wrong size in array of zeros frequencies.\n"
				   << "	Expected: " << GetZerosOrder() - 1 << ", used " << GetFreqZeros().Size();
				CLAM_ASSERT(0,ss.str().c_str());
			}
		}

		if( HasPolesPolynomial() )
		{
			GetPolesPolynomial().CheckTypes();

			if( GetAmpPoles().Size() != GetPolesOrder() - 1 )
			{
				std::stringstream ss;
				ss << "TDFilterKernel::CheckTypes: Wrong size in array of poles amplitudes.\n"
				   << "	Expected: " << GetPolesOrder() - 1 << ", used " << GetAmpPoles().Size();
				CLAM_ASSERT(0,ss.str().c_str());
			}
			if( GetFreqPoles().Size() != GetPolesOrder() - 1 )
			{
				std::stringstream ss;
				ss << "TDFilterKernel::CheckTypes: Wrong size in array of poles frequencies.\n"
				   << "	Expected: " << GetPolesOrder() - 1 << ", used " << GetFreqPoles().Size();
				CLAM_ASSERT(0,ss.str().c_str());
			}
		}

		CLAM_END_CHECK

	}
} //namespace
