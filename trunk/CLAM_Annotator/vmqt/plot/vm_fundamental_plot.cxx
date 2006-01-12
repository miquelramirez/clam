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

		void FundamentalPlot::set_data(const CLAM::Segment& segment, bool update)
		{
			wp_fund_data = get_fund_data(segment);
			if(!update)
			{
				set_xrange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				set_yrange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(wp_fund_data.Size(),segment.GetSamplingRate()/2.0);
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_huge_mode_enabled(false);
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data(wp_fund_data);
		}

		void FundamentalPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data_color(CLAM::VM::Color(0,0,255));
		}

		void FundamentalPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("fundamental"))->set_data_color(CLAM::VM::Color(0,255,0));
		}

		void FundamentalPlot::init_fundamental_plot()
		{
			wp_plot->add_renderer("fundamental", new CLAM::VM::DataArrayRenderer());
			wp_plot->send_to_back("fundamental");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}

		std::pair<int,int> FundamentalPlot::get_zoom_steps(CLAM::TSize size, CLAM::TData yrange)
		{
			double n = 15.0;
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

		void FundamentalPlot::set_xrange(double xmin, double xmax)
		{
			SegmentationPlot::set_xrange(xmin,xmax);
		}

		void FundamentalPlot::set_yrange(double ymin, double ymax)
		{
			SegmentationPlot::set_yrange(ymin,ymax);
		}

		void FundamentalPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		CLAM::DataArray FundamentalPlot::get_fund_data(const CLAM::Segment& segment)
		{
			CLAM::DataArray data;
			int n_frames = segment.GetnFrames();
			data.Resize(n_frames);
			data.SetSize(n_frames);
			for(int i = 0; i < n_frames; i++)
			{
				data[i] = segment.GetFrame(i).GetFundamental().GetFreq(0);
			}
			return data;
		}
	}
}

// END
