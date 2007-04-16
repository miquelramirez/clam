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

#include "SMSSineFilter.hxx"
#include "Factory.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{


bool SMSSineFilter::Do(const SpectralPeakArray& in, SpectralPeakArray& out) // TODO make this method take a WindowGenerator object as a parameter and use it as a way to modify the filters
{
	out = in; // TODO big cludge for streaming refactoring
	
	DataArray& iMagArray = in.GetMagBuffer();
	DataArray& oMagArray = out.GetMagBuffer();
	TSize nPeaks = in.GetnPeaks();

	BPF& filter = mConfig.GetBPF();
	
	for( int i=0; i<nPeaks; i++ )
	{
		oMagArray[i] = iMagArray[i] + filter.GetValueFromIndex(i);	
	}

	return true;
}

	namespace detail
	{
		static ProcessingFactory::Registrator<SMSSineFilter> regtSMSSineFilter( "SMSSineFilter" );
	}

}

