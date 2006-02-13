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

