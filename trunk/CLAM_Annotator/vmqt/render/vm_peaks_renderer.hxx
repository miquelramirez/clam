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

			void set_peaks(const CLAM::Array<CLAM::VM::Peak>& peaks);
			void set_peaks_color(const CLAM::VM::Color& cpoint, const CLAM::VM::Color& cline);

		protected:
			void render();
			void mouse_move_event(double x, double y);

		private:
			CLAM::Array<CLAM::VM::Peak> rd_cached_peaks;

			CLAM::VM::Color rd_cpoint;
			CLAM::VM::Color rd_cline;

			enum { LINE_WIDTH=1, POINT_SIZE=3, TOLERANCE=3 };

			int pick_peak(double x, double y);
		};
	};
}

#endif
