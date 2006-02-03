#ifndef __VMQT_SINTRACKS_BUILDER_H__
#define __VMQT_SINTRACKS_BUILDER_H__

#include <map>
#include <vector>
#include "SpectralPeakArray.hxx"
#include "vm_data_types.hxx"
#include "vm_palette.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksBuilder 
		{
		public:
			SinTracksBuilder();
			~SinTracksBuilder();

			const std::vector< std::vector<SinTrackNode> >& get_tracks(const Array<SpectralPeakArray>& peakMtx);

		private:
			std::map<unsigned,unsigned>              tb_table;
			std::vector< std::vector<SinTrackNode> > tb_tracks;
			Palette                                  tb_palette;

			void add_to_table(unsigned trackId, unsigned index);
			bool has_track_id(unsigned trackId);
			unsigned get_index(unsigned trackId);

			void add_nodes(const SpectralPeakArray& peaks, unsigned timeIndex);

			float clamp_to_range(float value);
		};
	}
}

#endif

