/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_SINTRACKS_BUILDER_H__
#define __VMQT_SINTRACKS_BUILDER_H__

#include <map>
#include <vector>
#include "SpectralPeakArray.hxx"
#include "vmDataTypes.hxx"
#include "vmPalette.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksBuilder 
		{
		public:
			SinTracksBuilder();
			~SinTracksBuilder();

			const std::vector< std::vector<SinTrackNode> >& GetTracks(const Array<SpectralPeakArray>& peakMtx);

		private:
			std::map<unsigned,unsigned>              mTable;
			std::vector< std::vector<SinTrackNode> > mTracks;
			Palette                                  mPalette;

			void AddToTable(unsigned trackId, unsigned index);
			bool HasTrackId(unsigned trackId);
			unsigned GetIndex(unsigned trackId);

			void AddNodes(const SpectralPeakArray& peaks, unsigned timeIndex);

			float ClampToRange(float value);
		};
	}
}

#endif

