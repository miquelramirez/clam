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

#ifndef __SINETRACKSMODEL__
#define __SINETRACKSMODEL__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv2;
	using CLAM::Array;
	using CLAM::TTime;
	using CLAM::TData;


	class SinTracksModel
	{
	public:
				
		SinTracksModel( );
				
		virtual ~SinTracksModel();


		// Signals
		Signalv2< SineTrackList& , int > TrackListPublished;
		Signalv1< TData >                SpectralRangePublished;
		Signalv2< TTime, TTime >         DurationPublished;

	};

}

#endif // SinTracksModel.hxx

