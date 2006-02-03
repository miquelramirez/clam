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

		void SpectrumPlot::set_data(const Spectrum& spec, bool update)
		{
			DataArray spec_data = get_spectrum_data(spec);
			if(!update)
			{
				set_xrange(0.0,spec.GetSpectralRange());
				set_yrange(-150.0,0.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(spec_data.Size());
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_huge_mode_enabled(false);
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data(spec_data);
		}

		void SpectrumPlot::set_data(const Spectrum& spec, const SpectralPeakArray& peaks, bool update)
		{
			DataArray spec_data = get_spectrum_data(spec);
			if(!update)
			{
				set_xrange(0.0,spec.GetSpectralRange());
				set_yrange(-150.0,0.0);
				std::pair<int, int> zoom_steps = get_zoom_steps(spec_data.Size());
				set_zoom_steps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_huge_mode_enabled(false);
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data(spec_data);
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks(get_peaks_data(peaks));
		}

		void SpectrumPlot::showPeaksOnly()
		{
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(false);
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(true);
		}

		void SpectrumPlot::showSpectrumOnly()
		{
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(true);
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(false);
		}

		void SpectrumPlot::showSpectrumAndPeaks()
		{
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_enabled(true);
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_enabled(true);
		}

		void SpectrumPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data_color(Color(0,0,255));
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks_color(Color(255,0,0),Color(8,180,70));
		}

		void SpectrumPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();

			static_cast<DataArrayRenderer*>(wp_plot->get_renderer("spectrum"))->set_data_color(Color(0,255,0));
			static_cast<PeaksRenderer*>(wp_plot->get_renderer("peaks"))->set_peaks_color(Color(255,0,0),Color(0,255,255));

		}

		void SpectrumPlot::init_spectrum_plot()
		{
			wp_plot->add_renderer("spectrum", new DataArrayRenderer());
			wp_plot->add_renderer("peaks", new PeaksRenderer());
			wp_plot->send_to_back("peaks");
			wp_plot->send_to_back("spectrum");
			wp_plot->bring_to_front("locator");
			backgroundWhite();
		}
		   
		void SpectrumPlot::set_xrange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::set_xrange(xmin,xmax,scale);
		}

		void SpectrumPlot::set_yrange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::set_yrange(ymin,ymax, scale);
		}

		void SpectrumPlot::set_zoom_steps(int hsteps, int vsteps)
		{
			SegmentationPlot::set_zoom_steps(hsteps,vsteps);
		}

		DataArray SpectrumPlot::get_spectrum_data(const Spectrum& in)
		{
			Spectrum spec = in;
			SpecTypeFlags old_flags;
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

		Array<Peak> SpectrumPlot::get_peaks_data(const SpectralPeakArray& in)
		{
			TSize nPeaks = in.GetMagBuffer().Size();
			bool linear = (in.GetScale() == CLAM::EScale::eLinear);
			Array<Peak> peaks_data;
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

		std::pair<int,int> SpectrumPlot::get_zoom_steps(TSize size)
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
