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
		class SinTracksRenderer : public CLAM::VM::Renderer2D
		{
		public:
			SinTracksRenderer();
			~SinTracksRenderer();

			void set_data(const CLAM::Array<CLAM::SpectralPeakArray>& peakMtx);

		protected:
			void render();
			void set_hbounds(double left, double right);
			void set_vbounds(double bottom, double top);

		private:
			int              rd_glList;
			CLAM::TSize      rd_peakMtx_size;
			CLAM::VM::GLView rd_local_view;

			std::vector< std::vector<CLAM::VM::SinTrackNode> > rd_tracks;

			int get_glList();
		};
	}
}

#endif


