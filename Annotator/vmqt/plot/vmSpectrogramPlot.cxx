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

#include <QtGui/QGridLayout>
#include "vmRuler.hxx"
#include "vmPlot2D.hxx"
#include "vmSonogramBuilder.hxx"
#include "vmSpecgramPanel.hxx"
#include "vmSpectrogramRenderer.hxx"
#include "vmSpectrogramPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		SpectrogramPlot::SpectrogramPlot(QWidget* parent)
			: SegmentationPlot(parent)
			, mSonogramColormap(COLOR_SONOGRAM)
			, mColorScaleWidth(50)
		{
			InitSpectrogramPlot();
		}

		SpectrogramPlot::~SpectrogramPlot()
		{
		}

		void SpectrogramPlot::SetData(const Segment& segment, bool update)
		{
			if(!update)
			{
				SetXRange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				SetYRange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			mSpectrogramRenderer->SetData(GetSpecMatrix(segment));
		}

		void SpectrogramPlot::colorSpecgram()
		{
			SonogramBuilder b;
			mPanel->SetColorScale(b.GetColorScale(mColorScaleWidth));
			mSonogramColormap = COLOR_SONOGRAM;
			mSpectrogramRenderer->colorSonogram();
		}

		void SpectrogramPlot::blackWhiteSpecgram()
		{
			SonogramBuilder b;
			mPanel->SetColorScale(b.GetGrayScale(mColorScaleWidth));
			mSonogramColormap = BW_SONOGRAM;
			mSpectrogramRenderer->blackWhiteSonogram();
		}

		void SpectrogramPlot::updateColorScale(int width)
		{
			mColorScaleWidth = width;
			SonogramBuilder b;
			switch(mSonogramColormap)
			{
				case COLOR_SONOGRAM:
					mPanel->SetColorScale(b.GetColorScale(mColorScaleWidth));
					break;
				case BW_SONOGRAM:
					mPanel->SetColorScale(b.GetGrayScale(mColorScaleWidth));
					break;
				default:
					break;
			}
		}

		void SpectrogramPlot::InitSpectrogramPlot()
		{
			mSpectrogramRenderer = new SpectrogramRenderer;
			mPlot->AddRenderer("spectrogram", mSpectrogramRenderer);
			mPlot->SendToBack("spectrogram");
			mPlot->BringToFront("locator");

//			mXRuler->SetStep(0.01);
//			mYRuler->SetStep(5.0);
			mYRuler->ShowIntegers();

			mPanel = new SpecgramPanel(this);
			mLayout->addWidget(mPanel,3,0,1,3);

			colorSpecgram();
			backgroundWhite();

			connect(mPanel,SIGNAL(colorScaleWidthChanged(int)),this,SLOT(updateColorScale(int)));
			connect(mSpectrogramRenderer,
					SIGNAL(fixedLabels(QString,QString)),
					mPanel,SLOT(setFixedLabels(QString,QString)));
			connect(mSpectrogramRenderer,
					SIGNAL(labels(QString,QString,QString,QString)),
					mPanel,SLOT(updateLabels(QString,QString,QString,QString)));
		}
		   
		void SpectrogramPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::SetXRange(xmin,xmax,scale);
		}

		void SpectrogramPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::SetYRange(ymin,ymax,scale);
		}

		void SpectrogramPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			SegmentationPlot::SetZoomSteps(hsteps,vsteps);
		}

		std::pair<int,int> SpectrogramPlot::GetZoomSteps(TSize size, TData yrange)
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

		Array<Spectrum> SpectrogramPlot::GetSpecMatrix(const Segment& in)
		{
			Array<Spectrum> spec_mtx;
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

