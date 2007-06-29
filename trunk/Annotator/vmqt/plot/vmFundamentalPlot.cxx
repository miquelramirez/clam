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
#include "vmDataArrayRenderer.hxx"
#include "vmFundamentalPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		FundamentalPlot::FundamentalPlot(QWidget* parent)
			: SegmentationPlot(parent)
		{
			InitFundamentalPlot();
		}

		FundamentalPlot::~FundamentalPlot()
		{
		}

		void FundamentalPlot::SetData(const Segment& segment, bool update)
		{
			if(!update)
			{
				SetXRange(0.0,double(segment.GetEndTime())-double(segment.GetBeginTime()));
				SetYRange(0.0,double(segment.GetSamplingRate())/2.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(segment.GetnFrames(),segment.GetSamplingRate()/2.0);
				SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("fundamental"))->SetHugeModeEnabled(false);
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("fundamental"))->SetData(GetFundData(segment));
		}

		void FundamentalPlot::backgroundWhite()
		{
			SegmentationPlot::backgroundWhite();
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("fundamental"))->SetDataColor(QColor(0,0,255));
		}

		void FundamentalPlot::backgroundBlack()
		{
			SegmentationPlot::backgroundBlack();
			static_cast<DataArrayRenderer*>(mPlot->GetRenderer("fundamental"))->SetDataColor(QColor(0,255,0));
		}

		void FundamentalPlot::InitFundamentalPlot()
		{
			mPlot->AddRenderer("fundamental", new DataArrayRenderer());
			mPlot->SendToBack("fundamental");
			mPlot->BringToFront("locator");
//			mXRuler->SetStep(0.01);
//			mYRuler->SetStep(5.0);
			mYRuler->ShowIntegers();
			backgroundWhite();
		}

		std::pair<int,int> FundamentalPlot::GetZoomSteps(TSize size, TData yrange)
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

		void FundamentalPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			SegmentationPlot::SetXRange(xmin,xmax,scale);
		}

		void FundamentalPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			SegmentationPlot::SetYRange(ymin,ymax,scale);
		}

		void FundamentalPlot::SetZoomSteps(int hsteps, int vsteps)
		{
			SegmentationPlot::SetZoomSteps(hsteps,vsteps);
		}

		DataArray FundamentalPlot::GetFundData(const Segment& segment)
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

