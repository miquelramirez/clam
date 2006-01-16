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


		void SinTracksPlot::set_data(const CLAM::Segment& segment, bool update)
		{
			build_peak_matrix(segment);
			if(!update)
			{
				set_xrange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				set_yrange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(wp_peak_matrix.Size(),segment.GetSamplingRate()/2.0);
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<CLAM::VM::SinTracksRenderer*>(wp_plot->get_renderer("sintracks"))->set_data(wp_peak_matrix);
		}

		void SinTracksPlot::init_sintracks_plot()
		{
			wp_plot->add_renderer("sintracks", new CLAM::VM::SinTracksRenderer());
			wp_plot->send_to_back("sintracks");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}
		   
		void SinTracksPlot::set_xrange(double xmin, double xmax)
		{
			SegmentationPlot::set_xrange(xmin,xmax);
		}

		void SinTracksPlot::set_yrange(double ymin, double ymax)
		{
			SegmentationPlot::set_yrange(ymin,ymax);
		}

		void SinTracksPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		void SinTracksPlot::build_peak_matrix(const CLAM::Segment& in)
		{
			int n_frames = in.GetnFrames();
			wp_peak_matrix.Resize(n_frames);
			wp_peak_matrix.SetSize(n_frames);
			for(int i = 0; i < n_frames; i++)
			{
				wp_peak_matrix[i] = in.GetFrame(i).GetSpectralPeakArray();
			}
		}

		std::pair<int,int> SinTracksPlot::get_zoom_steps(CLAM::TSize size, CLAM::TData yrange)
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
	}
}

// END
