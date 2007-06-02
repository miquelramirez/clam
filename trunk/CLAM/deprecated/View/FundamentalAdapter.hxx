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

#ifndef __FUNDAMENTALADAPTER__
#define __FUNDAMENTALADAPTER__

#include "DataTypes.hxx"
#include "ModelAdapter.hxx"
#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "Signalv2.hxx"

namespace CLAM
{
	class Segment;
}

namespace CLAMVM
{
	class FundamentalAdapter : public ModelAdapter
	{
	public:
		
		FundamentalAdapter();
		virtual ~FundamentalAdapter();

		virtual const char* GetClassName() const {
			return "FundamentalAdapter";
		}

		virtual bool Publish();

		virtual bool BindTo( const CLAM::Segment& adaptee );

		const CLAM::Segment* Adaptee() const	{
			return mAdaptedObject;
		}

		bool  HasCachedData() const {
			return mFundFreqTrajectory.Size() != 0;
		}

	protected: // methods
		
		bool ExtractTrajectory();

	public: // Signals and Slots
		
		SigSlot::Signalv2< const CLAM::Array<TimeFreqMagPoint>&, CLAM::TData  >  TrajectoryExtracted;
		SigSlot::Signalv2< CLAM::TTime , CLAM::TTime > TimeSpanChanged;

	protected:

		const CLAM::Segment*          mAdaptedObject;
		CLAM::Array<TimeFreqMagPoint> mFundFreqTrajectory;
	};
}

#endif // FundamentalAdapter.hxx

