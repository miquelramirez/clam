#include "SelPosPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SelPosPlotController::SelPosPlotController()
		{
		}

		SelPosPlotController::~SelPosPlotController()
		{
		}

		void SelPosPlotController::SetSelPos(const TData& value)
		{
			PlotController::SetSelPos(value);
			UpdateDial(GetSelPos());
		}

		void SelPosPlotController::Draw()
		{
			SegmentationMarksPlotController::Draw();
			_dial.Render();
		}

		void SelPosPlotController::SetDialColor(Color c)
		{
			_dial.SetColor(c);
		}
				
		void SelPosPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SegmentationMarksPlotController::SetHBounds(left,right);
			_dial.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void SelPosPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SegmentationMarksPlotController::SetVBounds(bottom,top);
			_dial.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void SelPosPlotController::UpdateDial(const TData& value)
		{
			_dial.Update(value);
		}

		TData SelPosPlotController::GetDialPos() const
		{
			return _dial.GetPos();
		}
	}
}

// END
