#ifndef __VMQT_PEAKS_RENDERER_H__
#define __VMQT_PEAKS_RENDERER_H__

#include "Array.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class PeaksRenderer : public Renderer2D
		{
		public:
			PeaksRenderer();
			~PeaksRenderer();

			void set_peaks(const Array<Peak>& peaks);
			void set_peaks_color(const Color& cpoint, const Color& cline);

		protected:
			void render();
			void mouse_move_event(double x, double y);

		private:
			Array<Peak> rd_cached_peaks;

			Color rd_cpoint;
			Color rd_cline;

			enum { LINE_WIDTH=1, POINT_SIZE=3, TOLERANCE=3 };

			int pick_peak(double x, double y);
		};
	};
}

#endif
