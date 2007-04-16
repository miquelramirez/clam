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

#ifndef __COMMONOPS__
#define __COMMONOPS__

namespace CLAM
{

	template < typename DataType >
	class Multiply
	{
	public:
	
		static const DataType IdentityElement;
		
		template <typename DataType2>
		DataType operator()( DataType op1, DataType2 op2 )
		{
			return op1 * op2;
		}
	};

	template <typename DataType>
		const DataType Multiply<DataType>::IdentityElement = 1;
	
	template< typename DataType >
	class Add
	{
	public:

		static const DataType IdentityElement;

		template <typename DataType2>
		DataType operator() ( DataType op1, DataType2 op2 )
		{
			return op1 + op2;
		}
	};

	template< typename DataType >
		const DataType Add<DataType>::IdentityElement = 0;


}

#endif // CommonOps.hxx

