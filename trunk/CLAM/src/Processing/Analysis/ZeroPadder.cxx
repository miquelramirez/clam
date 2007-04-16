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

#include "ZeroPadder.hxx"

namespace CLAM
{
	ZeroPadder::ZeroPadder()
		: mInput( "Input samples", this ),
		  mOutput( "Zero-padded samples", this )
	{
	}

	ZeroPadder::~ZeroPadder()
	{
	}

	bool ZeroPadder::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig( mConfig, c );

		return true;
	}

	bool ZeroPadder::ConcreteStart()
	{
		return true;
	}

	bool ZeroPadder::ConcreteStop()
	{
		return true;
	}

	bool ZeroPadder::Do( )
	{
		CLAM_ASSERT( AbleToExecute(), 
			     "This processing is not Ready to Do() anything" );
		
		CLAM::DataArray& src = mInput.GetAudio().GetBuffer();
		CLAM::DataArray& dst = mOutput.GetAudio().GetBuffer();

		int j = 0;
		int i = 0;

		for ( i = 0; i < src.Size(); i++, j++ )
		{
			dst[j] = src[i];
		}

		for ( ; j < dst.Size(); j++ )
		{
			dst[j] = TData(0.0);
		}

		return true;
	}
	
	
}

