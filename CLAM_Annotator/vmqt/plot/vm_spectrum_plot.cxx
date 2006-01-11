#include "CLAM_Math.hxx"
#include "SpecTypeFlags.hxx"
#include "vm_plot2d.hxx"
#include "vm_peaks_renderer.hxx"
#include "vm_data_array_renderer.hxx"
#include "vm_spectrum_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrumPlot::SpectrumPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			init_spectrum_plot();
		}

		SpectrumPlot::~SpectrumPlot()
		{
		}

		void SpectrumPlot::set_data(const CLAM::Spectrum& spec, bool update)
		{
			wp_spectrum_data = get_spectrum_data(spec);
			if(!update)
			{
				set_xrange(0.0,spec.GetSpectralRange());
				set_yrange(-150.0,0.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(wp_spectrum_data.Size());
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_huge_mode_enabled(false);
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data(wp_spectrum_data);
		}

		void SpectrumPlot::set_data(const CLAM::Spectrum& spec, const CLAM::SpectralPeakArray& peaks, bool update)
		{
			wp_spectrum_data = get_spectrum_data(spec);
			if(!update)
			{
				set_xrange(0.0,spec.GetSpectralRange());
				set_yrange(-150.0,0.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(wp_spectrum_data.Size());
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_huge_mode_enabled(false);
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data(wp_spectrum_data);
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks(get_peaks_data(peaks));
		}

		void SpectrumPlot::showPeaksOnly()
		{
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(false);
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(true);
		}

		void SpectrumPlot::showSpectrumOnly()
		{
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(true);
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(false);
		}

		void SpectrumPlot::showSpectrumAndPeaks()
		{
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(true);
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(true);
		}

		void SpectrumPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data_color(CLAM::VM::Color(0,0,255));
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks_color(CLAM::VM::Color(255,0,0),CLAM::VM::Color(0,255,255));
		}

		void SpectrumPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();

			static_cast<CLAM::VM::DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data_color(CLAM::VM::Color(0,255,0));
			static_cast<CLAM::VM::PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks_color(CLAM::VM::Color(255,0,0),CLAM::VM::Color(255,255,0));

		}

		void SpectrumPlot::init_spectrum_plot()
		{
			wp_plot->add_renderer("spectrum", new CLAM::VM::DataArrayRenderer());
			wp_plot->add_renderer("peaks", new CLAM::VM::PeaksRenderer());
			wp_plot->send_to_back("peaks");
			wp_plot->send_to_back("spectrum");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}
		   
		void SpectrumPlot::set_xrange(double xmin, double xmax)
		{
			SegmentationPlot::set_xrange(xmin,xmax);
		}

		void SpectrumPlot::set_yrange(double ymin, double ymax)
		{
			SegmentationPlot::set_yrange(ymin,ymax);
		}

		void SpectrumPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		CLAM::DataArray SpectrumPlot::get_spectrum_data(const CLAM::Spectrum& in)
		{
			CLAM::Spectrum spec = in;
			CLAM::SpecTypeFlags old_flags;
			spec.GetType(old_flags);

			if(!old_flags.bMagPhase)
			{
				SpecTypeFlags new_flags;
				spec.SetTypeSynchronize(new_flags);
			}
			// convert to dB
			spec.ToDB();
			return spec.GetMagBuffer();
		}

		CLAM::Array<CLAM::VM::Peak> SpectrumPlot::get_peaks_data(const CLAM::SpectralPeakArray& in)
		{
			TSize nPeaks = in.GetMagBuffer().Size();
			bool linear = (in.GetScale() == CLAM::EScale::eLinear);
			CLAM::Array<CLAM::VM::Peak> peaks_data;
			peaks_data.Resize(nPeaks);
			peaks_data.SetSize(nPeaks);

			for(TIndex i = 0;i < nPeaks; i++)
			{
				double magnitude = (double)in.GetMagBuffer()[i];
				if(linear) magnitude = 20.0*log10(magnitude);
				peaks_data[i].mag = magnitude;
				peaks_data[i].freq = in.GetFreqBuffer()[i];
			}
			return peaks_data;
		}

		std::pair<int,int> SpectrumPlot::get_zoom_steps(CLAM::TSize size)
		{
			double n = 50.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;;
			}
			
			n = 5.0;
			int yratio = 0;
			while(n < 150.0)
			{
				n *= 2.0;
				yratio++;
			}
			return std::make_pair(--xratio,--yratio);
		}
	}
}

// END
