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

#ifndef __PLOTEXAMPLESHELPER__
#define __PLOTEXAMPLESHELPER__

#include "DataTypes.hxx"

namespace CLAM
{
	template < typename T > class Array;
	class Spectrum;
}

extern void generateLowPassFilter( CLAM::Spectrum& spectrum );

extern void generateBandPassFilter( CLAM::Spectrum& spectrum );

extern void generateHighPassFilter( CLAM::Spectrum& spectrum );

extern void measureRandRandomness( CLAM::Array< CLAM::TData >& measure );

extern void generateSignalSpectrum( CLAM::Spectrum& spectrum );

extern void filter( CLAM::Spectrum& signal,
		    CLAM::Spectrum& filterResponse,
		    CLAM::Spectrum& filteredSignal );

#endif // PlotExamplesHelper.hxx
