#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPlotController::NetPlotController()
		{
		        _nSamples = 0;
			_vmin = TData(0.0);
			_vmax = TData(1.0);
			_monitorIsRunning=false;
			
			InitView();
			SetFirst(true);
			SetCanGetData(true);
			SetCanSendData(false);

			mStartSlot.Wrap(this,&NetPlotController::ConcreteStartMonitor);
			mStopSlot.Wrap(this,&NetPlotController::ConcreteStopMonitor);
		}
	
		NetPlotController::~NetPlotController()
		{
		}
	        
	    void NetPlotController::SetnSamples(const TSize& nSamples)
	    {
			_nSamples = nSamples;
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
	        
	        void NetPlotController::ConcreteStartMonitor()
		{
		    _monitorIsRunning = true;
		}

	        void NetPlotController::ConcreteStopMonitor()
		{
		    _monitorIsRunning=false;
		    SetFirst(true);
		}

	        bool NetPlotController::MonitorIsRunning()
		{
		    return _monitorIsRunning;
		}

	        void NetPlotController::SetFirst(bool first)
		{
		    _first=first;
		}

	        bool NetPlotController::First()
		{
		    return _first;
		}

	        void NetPlotController::SetCanGetData(bool canget)
		{
		    _canGetData=canget;
		}

	        void NetPlotController::SetCanSendData(bool cansend)
		{
		    _canSendData=cansend;
		}

		bool NetPlotController::CanGetData()
		{
		    return _canGetData;
		}

		bool NetPlotController::CanSendData()
		{
		    return _canSendData;
		}
	    
	}
}

// END


