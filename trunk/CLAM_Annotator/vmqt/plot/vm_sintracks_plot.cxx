#include "vm_plot2d.hxx"
#include "vm_sintracks_renderer.hxx"
#include "vm_sintracks_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		SinTracksPlot::SinTracksPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			init_sintracks_plot();
		}

		SinTracksPlot::~SinTracksPlot()
		{
		}


		void SinTracksPlot::set_data(const Segment& segment, bool update)
		{
			if(!update)
			{
				set_xrange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				set_yrange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<SinTracksRenderer*>(wp_plot->get_renderer("sintracks"))->set_data(get_peak_matrix(segment));
		}

		void SinTracksPlot::init_sintracks_plot()
		{
			wp_plot->add_renderer("sintracks", new SinTracksRenderer());
			wp_plot->send_to_back("sintracks");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}
		   
		void SinTracksPlot::set_xrange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::set_xrange(xmin,xmax,scale);
		}

		void SinTracksPlot::set_yrange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::set_yrange(ymin,ymax,scale);
		}

		void SinTracksPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		std::pair<int,int> SinTracksPlot::get_zoom_steps(TSize size, TData yrange)
		{
			double n = 5.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;;
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

		Array<SpectralPeakArray> SinTracksPlot::get_peak_matrix(const Segment& in)
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
