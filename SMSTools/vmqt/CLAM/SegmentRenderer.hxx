#ifndef __SEGMENTRENDERER__
#define __SEGMENTRENDERER__

#include <vector>
#include "Renderer.hxx"
#include "Segmentation.hxx"

namespace CLAM
{
	namespace VM
	{
		enum BoundType { LEFT_BOUND=0, RIGHT_BOUND=1, BOTTOM_BOUND=2, TOP_BOUND=3 };

		class SegmentRenderer : public Renderer
		{
		public:
			SegmentRenderer();
			~SegmentRenderer();

			void SetData(const Segmentation& s);
			void Render();

			void SetMargin(double margin);
			void SetVHighlighted(int index, BoundType type=LEFT_BOUND);
		
		private:
			const Segmentation * mSegmentation;
			std::vector<Color>   mColors; 
			double               mMargin;
			int                  mHighlighted;
			BoundType            mBoundType;
			
			enum { NORMAL=0, 
				   CURRENT=1, 
				   SELECTED=2, 
				   HIGHLIGHTED=3,
				   STIPPLED=4, 
				   NLINEWIDTH=2, 
				   CLINEWIDTH=2, 
				   FACTOR=4, 
				   PATTERN=0x5555 };

			void Colorize();
			void PlainRect(double left, double right, double top, double bottom);
			void StippledRect(double left, double right, double top, double bottom);
			bool IsVisible(double left, double right, double lBound, double rBound);
			void DrawSegment(double left, double right, double top, double bottom, bool isCurrent, bool isSelected);
			void DrawVHighlighted(double top,  double bottom);
		};
	}
}

#endif

