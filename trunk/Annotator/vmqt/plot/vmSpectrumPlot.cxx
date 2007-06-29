/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "CLAM_Math.hxx"
#include "SpecTypeFlags.hxx"
#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmPeaksRenderer.hxx"
#include "vmDataArrayRenderer.hxx"
#include "vmSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrumPlot::SpectrumPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			InitSpectrumPlot();
		}

		SpectrumPlot::~SpectrumPlot()
		{
		}

		void SpectrumPlot::SetData(const Spectrum& spec, bool update)
		{
			DataArray spec_data = GetSpectrumData(spec);
			if(!update)
			{
				SetXRange(0.0,spec.GetSpectralRange());
				SetYRange(-150.0,0.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(spec_data.Size());
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetHugeModeEnabled(false);
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetData(spec_data);
		}

		void SpectrumPlot::SetData(const Spectrum& spec, const SpectralPeakArray& peaks, bool update)
		{
			DataArray spec_data = GetSpectrumData(spec);
			if(!update)
			{
				SetXRange(0.0,spec.GetSpectralRange());
				SetYRange(-150.0,0.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(spec_data.Size());
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetHugeModeEnabled(false);
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetData(spec_data);
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetPeaks(GetPeaksData(peaks));
		}

		void SpectrumPlot::showPeaksOnly()
		{
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetEnabled(false);
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetEnabled(true);
		}

		void SpectrumPlot::showSpectrumOnly()
		{
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetEnabled(true);
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetEnabled(false);
		}

		void SpectrumPlot::showSpectrumAndPeaks()
		{
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetEnabled(true);
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetEnabled(true);
		}

		void SpectrumPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetDataColor(QColor(0,0,255));
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetPeaksColor(QColor(255,0,0),QColor(8,180,70));
		}

		void SpectrumPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("spectrum"))->SetDataColor(QColor(0,255,0));
			static_cast<PeaksRenderer*>(mPlot->GetRenderer("peaks"))->SetPeaksColor(QColor(255,0,0),QColor(0,255,255));

		}

		void SpectrumPlot::InitSpectrumPlot()
		{
			mPlot->AddRenderer("spectrum", new DataArrayRenderer());
			mPlot->AddRenderer("peaks", new PeaksRenderer());
			mPlot->SendToBack("peaks");
			mPlot->SendToBack("spectrum");
			mPlot->BringToFront("locator");
//			mXRuler->SetStep(5.0);
//			mYRuler->SetStep(1.0);
			mXRuler->ShowIntegers();
			mYRuler->ShowIntegers();
			backgroundWhite();
		}
		   
		void SpectrumPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::SetXRange(xmin,xmax,scale);
		}

		void SpectrumPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::SetYRange(ymin,ymax, scale);
		}

		void SpectrumPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			SegmentationPlot::SetZoomSteps(hsteps,vsteps);
		}

		DataArray SpectrumPlot::GetSpectrumData(const Spectrum& in)
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

		Array<Peak> SpectrumPlot::GetPeaksData(const SpectralPeakArray& in)
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

		std::pair<int,int> SpectrumPlot::GetZoomSteps(TSize size)
		{
			double n = 50.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;
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

