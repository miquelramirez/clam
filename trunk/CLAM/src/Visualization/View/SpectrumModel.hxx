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

#ifndef __SPECTRUMMODEL__
#define __SPECTRUMMODEL__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv2;
	using CLAM::TData;
	using CLAM::DataArray;

	class SpectrumModel
	{
	public:

		SpectrumModel( );
				
		virtual ~SpectrumModel();

		// signals
		Signalv1< const DataArray& >        MagnitudePublished;
		Signalv1< const DataArray& >        PhasePublished;
		Signalv1< TData >                   SpectralRangePublished;
		Signalv2< const DataArray&, TData > ObjectPublished;

	};

}

#endif // SpectrumModel.hxx

