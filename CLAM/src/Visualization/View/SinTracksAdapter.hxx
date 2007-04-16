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

#ifndef __SINTRACKSADAPTER__
#define __SINTRACKSADAPTER__

#include "SpectralPeakArrayAdapter.hxx"
#include "SinTracksModel.hxx"
#include "SineTracksDef.hxx"
#include "SinTrackListBuilder.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	template <typename T> class Array;
	class Segment;
	class SpectralPeakArray;
};


namespace CLAMVM
{
	using CLAM::Segment;
	using CLAM::ProcessingData;
	using CLAM::Array;

	class SinTracksAdapter 
		: public SpectralPeakArrayAdapter, public SinTracksModel
	{
		// attributes
	private:
		const Segment*                  mObserved;
		const CLAM::Array< CLAM::SpectralPeakArray >* mMatrixObserved;
		CLAM::TData                     mMatrixSampleRate;
		SineTrackList                   mExtractedList;
		SinTrackBuilder                 mTrackBuilder;

		// implementation details
	protected:
				
		virtual void TransmitPeakArrays();

		// class interface
	public:

		SinTracksAdapter();

		virtual ~SinTracksAdapter();

		virtual const char* GetClassName() const
		{
			return "SinTracksAdapter";
		}


		virtual bool Publish();
			
		virtual bool BindTo( const ProcessingData& procDataObj );

		virtual bool BindTo( const CLAM::Array< CLAM::SpectralPeakArray >& peakMatrix, CLAM::TData srate );
	};

}
#endif // SinTrackView.hxx

