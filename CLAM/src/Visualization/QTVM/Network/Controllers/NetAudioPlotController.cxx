#include "NetAudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlotController::NetAudioPlotController() 
		    : mMonitor(0),
		      _hasData(false),
		      _tooltip(""),
		      _renderingIsDone(false)
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(-1.0),TData(1.0));
		}

		NetAudioPlotController::~NetAudioPlotController()
		{
		}

		void NetAudioPlotController::SetData(const Audio& audio)
		{
		    if(!audio.GetBuffer().Size()) return;
		    if(CanGetData())
		    {
			SetCanSendData(false);
			_cacheData=audio;
			if(First()) Init(_cacheData.GetBuffer().Size());
			SetCanSendData(true);
		    }
		}

		void NetAudioPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetAudioPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void NetAudioPlotController::Draw()
		{
			if (!mMonitor)
			{
			    if(CanSendData())
			    {
				SetCanGetData(false);
				_renderer.SetDataPtr(_cacheData.GetBuffer().GetPtr(),_cacheData.GetBuffer().Size(),NormalMode);
				SetCanGetData(true);
			    }
			    _renderer.Render();
			    NetPlotController::Draw();
			    
			    _renderingIsDone=true;
			    
			    return;
			}
		      
			if(MonitorIsRunning())
			{
			    const Audio& audio = mMonitor->FreezeAndGetData();
			    
			    // TODO: Because we have exclusive right for
			    // to the data we could remove some of this copies

			    TSize audioSize = audio.GetBuffer().Size();
			    if(First() && audioSize) Init(audioSize);
			    _renderer.SetDataPtr(audio.GetBuffer().GetPtr(),audioSize,NormalMode);

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

		void NetAudioPlotController::FullView()
		{
			_view.left = TData(0.0);
			_view.right = TData(GetnSamples());
			_view.top = GetvMax();
			_view.bottom = GetvMin();
			emit sendView(_view);
		}

	        void NetAudioPlotController::Init(const TSize& frameSize)
		{
		    _hasData=true;
		    SetnSamples(frameSize);
		    SetFirst(false);
		    FullView();
		}

	        void NetAudioPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    NetPlotController::UpdatePoint(x,y);
		    _tooltip="";
		    if(_hasData)
		    {
			_tooltip = "amplitude="+(_tooltip.setNum(y,'f',3));  
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

