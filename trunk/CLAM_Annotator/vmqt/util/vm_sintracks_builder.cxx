#include "CLAM_Math.hxx"
#include "vm_sintracks_builder.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksBuilder::SinTracksBuilder()
			: tb_palette(0.0f)
		{
		}

		SinTracksBuilder::~SinTracksBuilder()
		{
		}

		const std::vector< std::vector<SinTrackNode> >& 
		SinTracksBuilder::get_tracks(const Array<SpectralPeakArray>& peakMtx)
		{
			tb_table.clear();
			tb_tracks.clear();
			for(TIndex i=0; i < peakMtx.Size(); i++)
			{
				add_nodes(peakMtx[i],(unsigned)i);
			}
			return tb_tracks;
		}

		void SinTracksBuilder::add_to_table(unsigned trackId, unsigned index)
		{
			tb_table[trackId]=index;
		}

		bool SinTracksBuilder::has_track_id(unsigned trackId)
		{
			return (tb_table.find(trackId) != tb_table.end());
		}

		unsigned SinTracksBuilder::get_index(unsigned trackId)
		{
			return tb_table[trackId];
		}

		void SinTracksBuilder::add_nodes(const SpectralPeakArray& peaks, unsigned timeIndex)
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
				int colorIndex = tb_palette.get(clamp_to_range(mag));
				tb_palette.get_rgb_from_index( colorIndex, node.color.r, node.color.g, node.color.b);
				unsigned trackId = int(peaks.GetIndex(i));
				if(has_track_id(trackId))
				{
					unsigned index = get_index(trackId);
					if(tb_tracks[index][tb_tracks[index].size()-1].timeIndex == node.timeIndex-1)
					{
						tb_tracks[index].push_back(node);
					}
					else
					{
						std::vector<SinTrackNode> newTrack;
						newTrack.push_back(node);
						tb_tracks.push_back(newTrack);
						add_to_table(trackId,tb_tracks.size()-1);
					}
				}
				else
				{
					std::vector<SinTrackNode> newTrack;
					newTrack.push_back(node);
					tb_tracks.push_back(newTrack);
					add_to_table(trackId,tb_tracks.size()-1);
				}
			}
		}

		float SinTracksBuilder::clamp_to_range(float value) 
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
