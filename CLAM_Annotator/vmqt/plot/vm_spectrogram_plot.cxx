#include <QGridLayout>
#include "vm_plot2d.hxx"
#include "vm_sonogram_builder.hxx"
#include "vm_specgram_panel.hxx"
#include "vm_spectrogram_renderer.hxx"
#include "vm_spectrogram_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrogramPlot::SpectrogramPlot(QWidget* parent)
			: SegmentationPlot(parent)
			, wp_sonogram_colormap(COLOR_SONOGRAM)
			, wp_color_scale_width(50)
		{
			init_spectrogram_plot();
		}

		SpectrogramPlot::~SpectrogramPlot()
		{
		}

		void SpectrogramPlot::set_data(const CLAM::Segment& segment, bool update)
		{
			if(!update)
			{
				set_xrange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				set_yrange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<CLAM::VM::SpectrogramRenderer*>(wp_plot->get_renderer("spectrogram"))->set_data(get_spec_matrix(segment));
		}

		void SpectrogramPlot::colorSpecgram()
		{
			CLAM::VM::SonogramBuilder b;
			wp_panel->set_color_scale(b.get_color_scale(wp_color_scale_width));
			wp_sonogram_colormap = COLOR_SONOGRAM;
			static_cast<CLAM::VM::SpectrogramRenderer*>(wp_plot->get_renderer("spectrogram"))->colorSonogram();
		}

		void SpectrogramPlot::blackWhiteSpecgram()
		{
			CLAM::VM::SonogramBuilder b;
			wp_panel->set_color_scale(b.get_gray_scale(wp_color_scale_width));
			wp_sonogram_colormap = BW_SONOGRAM;
			static_cast<CLAM::VM::SpectrogramRenderer*>(wp_plot->get_renderer("spectrogram"))->blackWhiteSonogram();
		}

		void SpectrogramPlot::updateColorScale(int width)
		{
			wp_color_scale_width = width;
			CLAM::VM::SonogramBuilder b;
			switch(wp_sonogram_colormap)
			{
				case COLOR_SONOGRAM:
					wp_panel->set_color_scale(b.get_color_scale(wp_color_scale_width));
					break;
				case BW_SONOGRAM:
					wp_panel->set_color_scale(b.get_gray_scale(wp_color_scale_width));
					break;
				default:
					break;
			}
		}

		void SpectrogramPlot::init_spectrogram_plot()
		{
			wp_plot->add_renderer("spectrogram", new CLAM::VM::SpectrogramRenderer());
			wp_plot->send_to_back("spectrogram");
			wp_plot->bring_to_front("locator");

			wp_panel = new CLAM::VM::SpecgramPanel(this);
			wp_layout->addWidget(wp_panel,3,0,1,3);

			colorSpecgram();
			backgroundWhite();

			connect(wp_panel,SIGNAL(colorScaleWidthChanged(int)),this,SLOT(updateColorScale(int)));
			connect(static_cast<CLAM::VM::SpectrogramRenderer*>(wp_plot->get_renderer("spectrogram")),
					SIGNAL(fixedLabels(QString,QString)),
					wp_panel,
					SLOT(setFixedLabels(QString,QString)));
			connect(static_cast<CLAM::VM::SpectrogramRenderer*>(wp_plot->get_renderer("spectrogram")),
					SIGNAL(labels(QString,QString,QString,QString)),
					wp_panel,
					SLOT(updateLabels(QString,QString,QString,QString)));
			
		}
		   
		void SpectrogramPlot::set_xrange(double xmin, double xmax)
		{
			SegmentationPlot::set_xrange(xmin,xmax);
		}

		void SpectrogramPlot::set_yrange(double ymin, double ymax)
		{
			SegmentationPlot::set_yrange(ymin,ymax);
		}

		void SpectrogramPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		std::pair<int,int> SpectrogramPlot::get_zoom_steps(CLAM::TSize size, CLAM::TData yrange)
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

		CLAM::Array<CLAM::Spectrum> SpectrogramPlot::get_spec_matrix(const CLAM::Segment& in)
		{
			CLAM::Array<CLAM::Spectrum> spec_mtx;
			spec_mtx.Resize(in.GetnFrames());
			spec_mtx.SetSize(in.GetnFrames());
			for(CLAM::TIndex i=0; i < in.GetnFrames(); i++)
			{
				spec_mtx[i] = in.GetFrame(i).GetSinusoidalAnalSpectrum();
			}
			return spec_mtx;
		}
	}
}

// END
