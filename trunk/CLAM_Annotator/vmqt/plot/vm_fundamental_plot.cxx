#include "vm_plot2d.hxx"
#include "vm_data_array_renderer.hxx"
#include "vm_fundamental_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		FundamentalPlot::FundamentalPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			init_fundamental_plot();
		}

		FundamentalPlot::~FundamentalPlot()
		{
		}

		void FundamentalPlot::set_data(const Segment& segment, bool update)
		{
			if(!update)
			{
				set_xrange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				set_yrange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_huge_mode_enabled(false);
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data(get_fund_data(segment));
		}

		void FundamentalPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data_color(Color(0,0,255));
		}

		void FundamentalPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data_color(Color(0,255,0));
		}

		void FundamentalPlot::init_fundamental_plot()
		{
			wp_plot->add_renderer("fundamental", new DataArrayRenderer());
			wp_plot->send_to_back("fundamental");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}

		std::pair<int,int> FundamentalPlot::get_zoom_steps(TSize size, TData yrange)
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

		void FundamentalPlot::set_xrange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::set_xrange(xmin,xmax,scale);
		}

		void FundamentalPlot::set_yrange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::set_yrange(ymin,ymax,scale);
		}

		void FundamentalPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		DataArray FundamentalPlot::get_fund_data(const Segment& segment)
		{
			DataArray fund_data;
			int n_frames = segment.GetnFrames();
			fund_data.Resize(n_frames);
			fund_data.SetSize(n_frames);
			for(int i = 0; i < n_frames; i++)
			{
				fund_data[i] = segment.GetFrame(i).GetFundamental().GetFreq(0);
			}
			return fund_data;
		}
	}
}

// END
