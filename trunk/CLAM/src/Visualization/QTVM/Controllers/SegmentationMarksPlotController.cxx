#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentationMarksPlotController::SegmentationMarksPlotController()
		{
			_mustProcessMarks=false;
		}

		SegmentationMarksPlotController::~SegmentationMarksPlotController()
		{
		}
			
		void SegmentationMarksPlotController::SetMarks(std::list<unsigned>& marks)
		{
			_marks = marks;
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetMarksColor(Color c)
		{
			_marksRenderer.SetColor(c);
		}

		void SegmentationMarksPlotController::Draw()
		{
			if(_marks.size() > 0)
			{
				if(_mustProcessMarks) ProcessMarksData();
				_marksRenderer.Render();
			}
			
		}

		void SegmentationMarksPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetHBounds(const TData& left, const TData& right)
		{
			PlotController::SetHBounds(left,right);
			_marksRenderer.SetHBounds(GetLeftBound(),GetRightBound());
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetVBounds(const TData& bottom, const TData& top)
		{
			PlotController::SetVBounds(bottom,top);
			_marksRenderer.SetVBounds(GetBottomBound(),GetTopBound());
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::ProcessMarksData()
		{
			unsigned left = unsigned(GetLeftBound());
			unsigned right = unsigned(GetRightBound());

			Array<unsigned> processedMarks;

			Marks::iterator it = _marks.begin();
			for(;it != _marks.end();it++)
			{
				if((*it) > right) break;
				if((*it) >= left) processedMarks.AddElem((*it));
			}

			_marksRenderer.SetData(processedMarks);
			_mustProcessMarks=false;
		}
	}
}

// END

