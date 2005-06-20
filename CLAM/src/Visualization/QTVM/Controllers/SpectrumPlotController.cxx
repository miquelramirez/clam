/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <algorithm>
#include "SpecTypeFlags.hxx"
#include "SpectrumPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		SpectrumPlotController::SpectrumPlotController()
			: mMustProcessData(false)
			, mSpectralRange(TData(0.0))
		{
			SetVMin(TData(5.0));
			SetvRange(TData(150.0));
		}
		
		SpectrumPlotController::~SpectrumPlotController()
		{
		}

		void SpectrumPlotController::SetData(const Spectrum& spec)
		{
			mSpec = spec;
			CacheData();
			mSpectralRange = mSpec.GetSpectralRange();
			SetnSamples(mMagBuffer.Size());
			FullView();
			mMustProcessData = true;
			SetSelPos(TData(0.0));
			emit requestRefresh();
			emit freq(TData(0.0));
			emit mag(mMagBuffer[0]);
		}

		void SpectrumPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
		}

		void SpectrumPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelPosPlotController::SetHBounds(left,right);
			mMustProcessData = true;
			
			double lBound = double(GetLeftBound()*mSpectralRange/TData(GetnSamples()));
			double hBound = double(GetRightBound()*mSpectralRange/TData(GetnSamples()));
		 
			emit xRulerRange(lBound,hBound);
		}

		void SpectrumPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelPosPlotController::SetVBounds(bottom,top);
			
			double bBound = double(GetBottomBound()-GetvRange());
			double tBound = double(GetTopBound()-GetvRange());
		       
			emit yRulerRange(bBound,tBound);
		}

		void SpectrumPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()*mSpectralRange/TData(GetnSamples()));
			double hBound = double(GetRightBound()*mSpectralRange/TData(GetnSamples()));
			
			emit xRulerRange(lBound,hBound);
				
			double bBound = double(GetBottomBound()-GetvRange());
			double tBound = double(GetTopBound()-GetvRange());
			
			emit yRulerRange(bBound,tBound);
		}

		void SpectrumPlotController::Draw()
		{
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			SelPosPlotController::Draw();
		}

		void SpectrumPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			mSpec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				// check for conversions
				if(spFlags.bComplex)
				{
					mSpec.Complex2MagPhase();
				}
				else if(spFlags.bPolar)
				{
					mSpec.Polar2MagPhase();
				}
				else if(spFlags.bMagPhaseBPF)
				{
					mSpec.BPF2MagPhase();
				}
			}
			// convert to dB
			mSpec.ToDB();
		}

		void SpectrumPlotController::CacheData()
		{
			AdaptSpectralData();

			mMagBuffer=mSpec.GetMagBuffer();
			mPhaseBuffer=mSpec.GetPhaseBuffer();
		}

		void SpectrumPlotController::FullView()
		{
			mView.left = TData(0.0);
			mView.right = TData(mMagBuffer.Size());
			mView.top = TData(150.0);
			mView.bottom = TData(0.0);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit sendView(mView);
		}

		void SpectrumPlotController::ProcessData()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(mProcessedData.Size() <= len)
				mProcessedData.Resize(len);
			mProcessedData.SetSize(len);

			std::copy(mMagBuffer.GetPtr()+offset,mMagBuffer.GetPtr()+offset+len,mProcessedData.GetPtr());

			for(int i = 0;i < mProcessedData.Size();i++) mProcessedData[i] += TData(150.0);

			TData range = GetRightBound()-GetLeftBound();
			TData threshold = GetHMin()*TData(2.0);
			int mode = (range < threshold) ? DetailMode : NormalMode;
			mRenderer.SetDataPtr(mProcessedData.GetPtr(),mProcessedData.Size(),mode);	
			mMustProcessData = false;
		}

		void SpectrumPlotController::SetSelPos(const TData& value)
		{
			SelPosPlotController::SetSelPos(value);

			int index = int(value);
			emit mag(mMagBuffer[index]);
			emit freq(value*mSpectralRange/TData(GetnSamples()));
		}

		TData SpectrumPlotController::GetSpectralRange() const
		{
			return mSpectralRange;
		}

		bool SpectrumPlotController::MustProcessData() const
		{
			return mMustProcessData;
		}
        
		void SpectrumPlotController::SetMousePos(TData x,TData y)
		{
			SegmentationMarksPlotController::SetMousePos(x,y);
			TData xcoord=x;
			xcoord *= mSpectralRange;
			xcoord /= TData(GetnSamples());
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound()-TData(150.0);
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(mViewport.h);
			ycoord += bBound;
			PlotController::SetMousePos(xcoord,ycoord);
			if(!HasSentTag())
			{
				TData freq=GetMouseXPos();
				TData dB=GetMouseYPos();
				QString s = "freq="+QString::number(freq,'f',0)+"Hz mag="+QString::number(dB,'f',0)+"dB";
			
				emit toolTip(s);
			}
		}
       	
    }
}

// END

