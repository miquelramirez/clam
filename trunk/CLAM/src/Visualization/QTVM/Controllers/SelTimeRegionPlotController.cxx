#include <qstring.h>
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SelTimeRegionPlotController::SelTimeRegionPlotController()
		{
			_sampleRate = TData(44100.0);
			_duration = TData(0.0);
			SetKeyShiftPressed(false);
		}
		
		SelTimeRegionPlotController::~SelTimeRegionPlotController()
		{
		}

		void SelTimeRegionPlotController::SetSelPos(const TData& value)
		{
			if(_keyShiftPressed)
			{
				if(value > GetBeginRegion() && value < GetEndRegion())
				{
					if(value > GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/TData(2.0)))
					{
						SetEndRegion(value);
					}
					else
					{
						SetBeginRegion(value);
					}
					UpdateDial(value);
				}
				else
				{
					if(value > GetEndRegion())
					{
						SetEndRegion(value);
					}
					if(value < GetBeginRegion())
					{
						SetBeginRegion(value);
					}
					UpdateDial(value);
				}
				PlotController::SetSelPos(GetBeginRegion()+((GetEndRegion()-GetBeginRegion())/TData(2.0)));
			}
			else
			{
				SelPosPlotController::SetSelPos(value);
				SetBeginRegion(GetSelPos());
				SetEndRegion(GetSelPos());
			}
			emit selectedRegion(GetRegionTime());
		}
		
		void SelTimeRegionPlotController::Draw()
		{
			SelPosPlotController::Draw();
			_rMarker.Render();
		}

		void SelTimeRegionPlotController::SetRegionColor(Color c)
		{
			_rMarker.SetColor(c);
		}
		
		void SelTimeRegionPlotController::SetKeyShiftPressed(bool pr)
		{
			_keyShiftPressed = pr;
		}

		void SelTimeRegionPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			_rMarker.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void SelTimeRegionPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			_rMarker.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void SelTimeRegionPlotController::SetBeginRegion(const TData& value)
		{
			_rMarker.SetBegin(value);
		}

		void SelTimeRegionPlotController::SetEndRegion(const TData& value)
		{
			_rMarker.SetEnd(value);
		}

		TData SelTimeRegionPlotController::GetBeginRegion() const
		{
			return _rMarker.GetBegin();
		}

		TData SelTimeRegionPlotController::GetEndRegion() const
		{
			return _rMarker.GetEnd();
		}

		MediaTime SelTimeRegionPlotController::GetRegionTime() const
		{
			MediaTime time;
			time.SetBegin(GetBeginRegion()/_sampleRate);
			time.SetEnd(GetEndRegion()/_sampleRate);
			if(time.GetBegin() < time.GetEnd())
			{
				time.AddDuration();
				time.UpdateData();
				time.SetDuration(time.GetEnd()-time.GetBegin());
			}
			else
			{
				time.SetEnd(_duration);
			}
			return time;
		}
	}
}

// END
