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

#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmGrid.hxx"
#include "vmDataArrayRenderer.hxx"
#include "vmAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		AudioPlot::AudioPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			InitAudioPlot();
		}

		AudioPlot::~AudioPlot()
		{
		}

		void AudioPlot::SetData(const Audio& audio, bool update)
		{
			if(!update)
			{
				SetXRange(0.0,audio.GetDuration()/1000.0);
				SetYRange(-1.0,1.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(audio.GetBuffer().Size());
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			mGridRenderer->SetGridSteps(audio.GetDuration()/1000.0,1.0);
			mAudioRenderer->SetData(audio.GetBuffer());
		}

		void AudioPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			mGridRenderer->SetGridColor(QColor(0,0,255));
			mAudioRenderer->SetDataColor(QColor(0,0,255));
		}

		void AudioPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			mGridRenderer->SetGridColor(QColor(0,255,0));
			mAudioRenderer->SetDataColor(QColor(0,255,0));
		}

		void AudioPlot::InitAudioPlot()
		{
			mGridRenderer = new Grid();
			mAudioRenderer = new DataArrayRenderer();
			mPlot->AddRenderer("grid", mGridRenderer);
			mPlot->AddRenderer("audio", mAudioRenderer);
			mPlot->SendToBack("audio");
			mPlot->SendToBack("grid");
			mPlot->BringToFront("locator");
//			mXRuler->SetStep(0.01);
//			mYRuler->SetStep(0.05);
			mGridRenderer->ShowGrid(true);
			backgroundWhite();
		}

		std::pair<int,int> AudioPlot::GetZoomSteps(TSize size)
		{
			double n = 100.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;
			}
			
			n = 0.2;
			int yratio = 0;
			while(n < 2.0)
			{
				n *= 2.0;
				yratio++;
			}
			return std::make_pair(--xratio,--yratio);
		}

		void AudioPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::SetXRange(xmin,xmax,scale);
		}

		void AudioPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::SetYRange(ymin,ymax,scale);
		}

		void AudioPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			SegmentationPlot::SetZoomSteps(hsteps,vsteps);
		}
	}
}

// END

