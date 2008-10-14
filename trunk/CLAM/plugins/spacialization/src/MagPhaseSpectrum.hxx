/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef MagPhaseSpectrum_hxx
#define MagPhaseSpectrum_hxx

#include <CLAM/DataTypes.hxx>
#include <vector>

namespace CLAM
{

/**
 Light-weight spectrum representation using two vectors of magnitudes and phase.
 Linear scale is used for the magnitude.
 The phase is expressed in radiants.

 @ingroup NewSpectralProcessing.
 @see ComplexSpectrum, CLAM::Spectrum
*/
struct MagPhaseSpectrum 
{
	std::vector<TData> magnitudes;
	std::vector<TData> phases;
	TData spectralRange;
};

} // namespace CLAM

#endif // MagPhaseSpectrum_hxx
