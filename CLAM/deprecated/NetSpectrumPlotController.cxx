#include "SpecTypeFlags.hxx"
#include "NetSpectrumPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlotController::NetSpectrumPlotController()
		    : mMonitor(0)
			, mSpectralRange(TData(0.0))
			, mHasData(false)
		    , mRenderingIsDone(false)
			, mTooltip("")
		{
		    SetDataColor(VMColor::Green());
		    SetvRange(TData(-150.0),TData(0.0));
		}

		NetSpectrumPlotController::~NetSpectrumPlotController()
		{
		}

		void NetSpectrumPlotController::SetData(const Spectrum& spec)
		{
		    if(!spec.GetMagBuffer().Size()) return;
		    if(CanGetData())
		    {
				SetCanSendData(false);
				mSpec = spec;
				if(First()) Init(mSpec.GetMagBuffer().Size());
				AdaptSpectralData();
				SetCanSendData(true);
		    }
		}

		void NetSpectrumPlotController::SetMonitor(MonitorType & monitor)
		{
			mMonitor = & monitor;
			mMonitor->AttachStartSlot(mStartSlot);
			mMonitor->AttachStopSlot(mStopSlot);
		}

		void NetSpectrumPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void NetSpectrumPlotController::Draw()
		{
		    if(!mMonitor)
		    {
				if(CanSendData())
				{
					SetCanGetData(false);
					mRenderer.SetDataPtr(mSpec.GetMagBuffer().GetPtr(), mSpec.GetMagBuffer().Size());
					SetCanGetData(true);
				}
				mRenderer.Render();
				NetPlotController::Draw();
				mRenderingIsDone=true;
				return;
		    }

		    if(MonitorIsRunning())
		    {
				const Spectrum & spectrum = mMonitor->FreezeAndGetData();

				// TODO: Because we have exclusive right for
				// to the data we could remove some of this copies

				mSpec = spectrum;
				TSize specSize = mSpec.GetMagBuffer().Size();
				if(First() && specSize) Init(specSize);
				AdaptSpectralData();
				mRenderer.SetDataPtr(mSpec.GetMagBuffer().GetPtr(), mSpec.GetMagBuffer().Size());
				mRenderer.Render();
				NetPlotController::Draw();
				mMonitor->UnfreezeData();
		    }
		    else
		    {
				mRenderer.Render();
				NetPlotController::Draw();
		    }
		    mRenderingIsDone=true;
		}

		void NetSpectrumPlotController::FullView()	
		{
			mView.left = TData(0.0);
			mView.right = TData(GetnSamples());
			mView.top = GetvMax();
			mView.bottom = GetvMin();
			emit sendView(mView);
		}

		void NetSpectrumPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			mSpec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				SpecTypeFlags newFlags;
				mSpec.SetTypeSynchronize(newFlags);
			}
			// convert to dB
			mSpec.ToDB();
		}

		void NetSpectrumPlotController::Init(const TSize& size)
		{
		    mHasData=true;
		    mSpectralRange=mSpec.GetSpectralRange();
		    SetnSamples(size);
		    SetFirst(false);
		    FullView();
		}

		void NetSpectrumPlotController::UpdatePoint(const TData& x, const TData& y)
		{
		    TData xvalue=x;
		    NetPlotController::UpdatePoint(x,y);
		    mTooltip="";
		    if(mHasData)
		    {
				xvalue*=mSpectralRange;
				xvalue/=TData(GetnSamples());
				mTooltip = "frequency="+(mTooltip.setNum(xvalue,'f',0))+"Hz "+"magnitude="+(mTooltip.setNum(y,'f',0))+"dB";  
		    }
		    if(mRenderingIsDone)
		    {
				mRenderingIsDone=false;
				emit toolTip(mTooltip);
		    }
		}
	}
}

// END

