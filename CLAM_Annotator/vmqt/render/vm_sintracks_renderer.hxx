#ifndef __VMQT_SINTRACKS_RENDERER_H__
#define __VMQT_SINTRACKS_RENDERER_H__

#include <vector>
#include "Array.hxx"
#include "SpectralPeakArray.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksRenderer : public Renderer2D
		{
		public:
			SinTracksRenderer();
			~SinTracksRenderer();

			void set_data(const Array<SpectralPeakArray>& peakMtx);

		protected:
			void render();
			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);

		private:
			int    rd_glList;
			TSize  rd_peakMtx_size;
			GLView rd_local_view;

			std::vector< std::vector<SinTrackNode> > rd_tracks;
		};
	}
}

#endif


