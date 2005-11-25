#ifndef __SEGMENTRENDERER__
#define __SEGMENTRENDERER__

#include "Renderer.hxx"
#include "Segmentation.hxx"

namespace CLAM
{
	namespace VM
	{
		class SegmentRenderer : public Renderer
		{
		public:
			SegmentRenderer();
			~SegmentRenderer();

			void SetData(const Segmentation& s);
			void Render();

			void SetMargin(double margin);
		
		private:
			const Segmentation * mSegmentation;
			double               mMargin;
			
			enum { NORMAL=0, CURRENT=1, SELECTED=2, NLINEWIDTH=2, CLINEWIDTH=4 };

			void Rect(double left, double right, double top, double bottom);
			bool IsVisible(double left, double right, double lBound, double rBound);
			void DrawSegment(double left, double right, double top, double bottom, int type=NORMAL);
		};
	}
}

#endif
