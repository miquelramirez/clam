#include "NetPeaksPlotController.hxx"
#include "PortMonitor.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPeaksPlotController::NetPeaksPlotController()
		    : mMonitor(0),
		      _linear(false),
		      _hasData(false),
		      _tooltip(""),
		      _renderingIsDone(false)
		{
		    SetPeaksColor(VMColor::Cyan(),VMColor::Red());
		    SetvRange(TData(-150.0),TData(0.0));
		    _renderer.SetVBounds(TData(0.0),TData(-150.0));
		    SetnSamples(22050);
		}

		NetPeaksPlotController::~NetPeaksPlotController()
		{
		}

		void NetPeaksPlotController::SetData(const SpectralPeakArray& peaks)
		{
		    if(!peaks.GetMagBuffer().Size()) return;
		    if(CanGetData())
		    {
			SetCanSendData(false);
			_magBuffer = peaks.GetMagBuffer();
			_freqBuffer = peaks.GetFreqBuffer();
			_linear = (peaks.GetScale() == EScale::eLinear);
			if(First()) Init();
			ProcessPeakData();
			SetCanSendData(true);
		    }
		}

		void NetPeaksPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetPeaksPlotController::SetPeaksColor(Color cline, Color cpoint)
		{
			_renderer.SetPeakColor(cline, cpoint);
		}

		void NetPeaksPlotController::Draw()
		{
			if (!mMonitor)
			{
			    if(CanSendData())
			    {
				SetCanGetData(false);
				_renderer.SetDataPtr(_magBuffer.GetPtr(), _freqBuffer.GetPtr(), _magBuffer.Size());
				SetCanGetData(true);
			    }
			    _renderer.Render();
			    NetPlotController::Draw();

			    _renderingIsDone=true;

			    return;
			}

			if(MonitorIsRunning())
			{
			    const CLAM::SpectralPeakArray & peaks = mMonitor->FreezeAndGetData();

			    // TODO: Because we have exclusive right for
			    // to the data we could remove some of this copies
			    _magBuffer = peaks.GetMagBuffer();
			    _freqBuffer = peaks.GetFreqBuffer();
			    _linear = (peaks.GetScale() == EScale::eLinear);
			    if(First() && _magBuffer.Size()) Init();
			    ProcessPeakData();
                            _renderer.SetDataPtr(_magBuffer.GetPtr(), _freqBuffer.GetPtr(), _magBuffer.Size());
			    _renderer.Render();
			    NetPlotController::Draw();

			    mMonitor->UnfreezeData();
			}
			else
			{
			    _renderer.Render();
			    NetPlotController::Draw();
			}

			_renderingIsDone=true;
		}

		void NetPeaksPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}

		void NetPeaksPlotController::ProcessPeakData()
		{
		    if(_linear)
		    {
			TSize nPeaks = _magBuffer.Size();
			for(int i = 0; i < nPeaks; i++)
			{
			    _magBuffer[i] = 20.0*log10(_magBuffer[i]);
			}
		    }
		}

	        void NetPeaksPlotController::Init()
		{
		    _hasData=true;
		    SetFirst(false);
		    FullView();
		}

	        void NetPeaksPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    NetPlotController::UpdatePoint(x,y);
		    _tooltip="";
		    if(_hasData)
		    {
			_tooltip = "frequency="+(_tooltip.setNum(x,'f',0))+"Hz "+"magnitude="+(_tooltip.setNum(y,'f',0))+"dB";  
		    }
		    if(_renderingIsDone)
		    {
			_renderingIsDone=false;
			emit toolTip(_tooltip);
		    }
		}
	}
}

// END


