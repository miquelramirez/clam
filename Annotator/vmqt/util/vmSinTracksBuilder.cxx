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

#include "CLAM_Math.hxx"
#include "vmSinTracksBuilder.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksBuilder::SinTracksBuilder()
			: mPalette(0.0f)
		{
		}

		SinTracksBuilder::~SinTracksBuilder()
		{
		}

		const std::vector< std::vector<SinTrackNode> >& 
		SinTracksBuilder::GetTracks(const Array<SpectralPeakArray>& peakMtx)
		{
			mTable.clear();
			mTracks.clear();
			for(TIndex i=0; i < peakMtx.Size(); i++)
			{
				AddNodes(peakMtx[i],(unsigned)i);
			}
			return mTracks;
		}

		void SinTracksBuilder::AddToTable(unsigned trackId, unsigned index)
		{
			mTable[trackId]=index;
		}

		bool SinTracksBuilder::HasTrackId(unsigned trackId)
		{
			return (mTable.find(trackId) != mTable.end());
		}

		unsigned SinTracksBuilder::GetIndex(unsigned trackId)
		{
			return mTable[trackId];
		}

		void SinTracksBuilder::AddNodes(const SpectralPeakArray& peaks, unsigned timeIndex)
		{
			bool linear = (peaks.GetScale() == CLAM::EScale::eLinear);
			for(TIndex i=0; i < peaks.GetMagBuffer().Size(); i++)
			{
				if(peaks.GetIndex(i) < 0) continue;
				SinTrackNode node;
				node.freq = double(peaks.GetFreqBuffer()[i]);
				node.timeIndex = timeIndex;
				float mag = float(peaks.GetMagBuffer()[i]);
				if(linear) mag = float(20.0*log10(mag));
				int colorIndex = mPalette.Get(ClampToRange(mag));
				mPalette.GetRGBFromIndex( colorIndex, node.color.r, node.color.g, node.color.b);
				unsigned trackId = int(peaks.GetIndex(i));
				if(HasTrackId(trackId))
				{
					unsigned index = GetIndex(trackId);
					if(mTracks[index][mTracks[index].size()-1].timeIndex == node.timeIndex-1)
					{
						mTracks[index].push_back(node);
					}
					else
					{
						std::vector<SinTrackNode> newTrack;
						newTrack.push_back(node);
						mTracks.push_back(newTrack);
						AddToTable(trackId,mTracks.size()-1);
					}
				}
				else
				{
					std::vector<SinTrackNode> newTrack;
					newTrack.push_back(node);
					mTracks.push_back(newTrack);
					AddToTable(trackId,mTracks.size()-1);
				}
			}
		}

		float SinTracksBuilder::ClampToRange(float value) 
		{
			if(value >= 0) return 1.0f; // 0 = max
			if(value <= -100.0) return 0.0f; // -100 = min
			value += 100.0f;
			value /= 100.0f;
			return value;
		}
	}
}

// END

