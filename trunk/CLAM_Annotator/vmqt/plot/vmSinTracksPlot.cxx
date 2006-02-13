#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmSinTracksRenderer.hxx"
#include "vmSinTracksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksPlot::SinTracksPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			InitSinTracksPlot();
		}

		SinTracksPlot::~SinTracksPlot()
		{
		}


		void SinTracksPlot::SetData(const Segment& segment, bool update)
		{
			if(!update)
			{
				SetXRange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				SetYRange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<SinTracksRenderer*>(mPlot->GetRenderer("sintracks"))->SetData(GetPeakMatrix(segment));
		}

		void SinTracksPlot::InitSinTracksPlot()
		{
			mPlot->AddRenderer("sintracks", new SinTracksRenderer());
			mPlot->SendToBack("sintracks");
			mPlot->BringToFront("locator");
			mXRuler->SetStep(0.01);
			mYRuler->SetStep(5.0);
			mYRuler->ShowIntegers();
			backgroundWhite();
		}
		   
		void SinTracksPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::SetXRange(xmin,xmax,scale);
		}

		void SinTracksPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::SetYRange(ymin,ymax,scale);
		}

		void SinTracksPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			SegmentationPlot::SetZoomSteps(hsteps,vsteps);
		}

		std::pair<int,int> SinTracksPlot::GetZoomSteps(TSize size, TData yrange)
		{
			double n = 5.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;
			}
			
			n = 40.0;
			int yratio = 0;
			while(n < yrange)
			{
				n *= 2.0;
				yratio++;
			}
			return std::make_pair(--xratio,--yratio);
		}

		Array<SpectralPeakArray> SinTracksPlot::GetPeakMatrix(const Segment& in)
		{
			Array<SpectralPeakArray> peak_mtx;
			int n_frames = in.GetnFrames();
			peak_mtx.Resize(n_frames);
			peak_mtx.SetSize(n_frames);
			for(int i = 0; i < n_frames; i++)
			{
				peak_mtx[i] = in.GetFrame(i).GetSpectralPeakArray();
			}
			return peak_mtx;
		}
	}
}

// END
