#ifndef __SEGMENTATIONMARKSPLOTCONTROLLER__
#define __SEGMENTATIONMARKSPLOTCONTROLLER__

#include <list>
#include "VLineArrayRenderer.hxx"
#include "PlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SegmentationMarksPlotController : public PlotController
		{
			typedef std::list<unsigned> Marks;

			public:
				SegmentationMarksPlotController();
				virtual ~SegmentationMarksPlotController();
			
				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

				virtual void Draw();
				virtual void SurfaceDimensions(int w,int h);

			protected:
				virtual void SetHBounds(const TData& left, const TData& right);
				virtual void SetVBounds(const TData& bottom, const TData& top);

			private:
				Marks _marks;
				VLineArrayRenderer _marksRenderer;
				bool _mustProcessMarks;

				void ProcessMarksData();
		};
	}
}

#endif

