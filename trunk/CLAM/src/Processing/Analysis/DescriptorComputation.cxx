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

#include "DescriptorComputation.hxx"
#include "Descriptor.hxx"

namespace CLAM
{
	DescriptorComputation::DescriptorComputation()
	{
		Configure( DescriptorComputationConfig() );
	}

	DescriptorComputation::DescriptorComputation( const DescriptorComputationConfig& cfg )
	{
		Configure( cfg );
	}

	DescriptorComputation::~DescriptorComputation()
	{
	}

	bool DescriptorComputation::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mConfig, cfg );
		return true;
	}

	const char* DescriptorComputation::GetClassName() const
	{
		return "DescriptorComputation";
	}

	bool DescriptorComputation::Do( void )
	{
		/* not implemented */
		return false;
	}

	bool DescriptorComputation::Do( Descriptor& input )
	{
		input.Compute();
		return true;
	}

	bool DescriptorComputation::Do( DescriptorAbs& input )
	{
		input.Compute();
		return true;
	}


}

