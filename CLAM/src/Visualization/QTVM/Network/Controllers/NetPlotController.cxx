#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPlotController::NetPlotController()
		{
		    _nSamples = 1; 
			_vmin = TData(0.0);
			_vmax = TData(1.0);
			
			InitView();
		}
	
		NetPlotController::~NetPlotController()
		{
		}

		void NetPlotController::SetnSamples(const TSize& n)
		{
			_nSamples = n;
		}

		TSize NetPlotController::GetnSamples() const
		{
			return _nSamples;
		}

		void NetPlotController::SetvRange(const TData& vmin, const TData& vmax)
		{
			_vmin = vmin;
			_vmax = vmax;
		}

		TData NetPlotController::GetvMin() const
		{
			return _vmin;
		}

		TData NetPlotController::GetvMax() const
		{
			return _vmax;
		}

		void NetPlotController::InitView()
		{
			_view.left = 0.0f;
			_view.right = 1.0f;
			_view.bottom = 0.0f;
			_view.top = 1.0f;
		}
	}
}

// END


