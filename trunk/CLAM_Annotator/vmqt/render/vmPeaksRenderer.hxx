#ifndef __VMQT_PEAKS_RENDERER_H__
#define __VMQT_PEAKS_RENDERER_H__

#include "Array.hxx"
#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class PeaksRenderer : public Renderer2D
		{
		public:
			PeaksRenderer();
			~PeaksRenderer();

			void SetPeaks(const Array<Peak>& peaks);
			void SetPeaksColor(const Color& cpoint, const Color& cline);

		protected:
			void Render();
			void MouseMoveEvent(double x, double y);

		private:
			Array<Peak> mCachedPeaks;

			Color mCpoint;
			Color mCline;

			enum { LINE_WIDTH=1, POINT_SIZE=3, TOLERANCE=3 };

			int PickPeak(double x, double y);
		};
	};
}

#endif
