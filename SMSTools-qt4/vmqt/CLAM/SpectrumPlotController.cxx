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
#include <CLAM/SpecTypeFlags.hxx>
#include <CLAM/SpectrumPlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		SpectrumPlotController::SpectrumPlotController()
			: mSpectralRange(11025.0)
			, mMustProcessData(false)
			, mHasData(false)
			, mHasPeaks(false)
			
		{
			SetMinSpanX(50.0);
			SetMinSpanY(5.0);
			mPeaksRenderer.SetVBounds(-150.0,0.0);
		}
		
		SpectrumPlotController::~SpectrumPlotController()
		{
		}

		void SpectrumPlotController::SetData(const Spectrum& spec)
		{
			mHasData = false;
			mSpec = spec;
			CacheData();
			mSpectralRange = double(mSpec.GetSpectralRange());
			SetnSamples(double(mMagBuffer.Size()));
			FullView();
			mMustProcessData = true;
			SetSelPos(0.0,true);
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrumPlotController::SetData(const Spectrum& spec,const SpectralPeakArray& peaks)
		{
			mHasData = false;
			mHasPeaks = true;
			mSpec = spec;
			mPeaks = &peaks;
			mSpectralRange = double(mSpec.GetSpectralRange());
			SetnSamples(double(mSpec.GetMagBuffer().Size()));
			CacheData();
			FullView();
			mMustProcessData = true;
			SetSelPos(0.0,true);
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrumPlotController::UpdateData(const Spectrum& spec)
		{
			mHasData = false;
			mSpec = spec;
			CacheData();
			mMustProcessData = true;
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrumPlotController::UpdateData(const Spectrum& spec,const SpectralPeakArray& peaks)
		{
			mHasData = false;
			mSpec = spec;
			mPeaks = &peaks;
			CacheData();
			mMustProcessData = true;
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrumPlotController::SetDataColor(Color c)
		{
			mRenderer.SetColor(c);
			mRenderer.SaveScreen();
			emit requestRefresh();
		}

		void SpectrumPlotController::SetPeaksColor(Color cline,Color cpoint)
		{
			mPeaksRenderer.SetPeakColor(cline,cpoint);
			mRenderer.SaveScreen();
			emit requestRefresh();
		}

		void SpectrumPlotController::SetHBounds(double left, double right)
		{
			PlotController::SetHBounds(left,right);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double lBound = GetLeftBound()*mSpectralRange/GetnSamples();
			double hBound = GetRightBound()*mSpectralRange/GetnSamples();
		 
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit xRulerRange(lBound,hBound);
		}

		void SpectrumPlotController::SetVBounds(double bottom, double top)
		{
			PlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
		       
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit yRulerRange(bBound,tBound);
		}

		void SpectrumPlotController::DisplayDimensions(int w, int h)
		{
			mMustProcessData = true;
			PlotController::DisplayDimensions(w,h);
		
			double lBound = GetLeftBound()*mSpectralRange/GetnSamples();
			double hBound = GetRightBound()*mSpectralRange/GetnSamples();
			
			emit xRulerRange(lBound,hBound);
				
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			emit yRulerRange(bBound,tBound);
		}

		void SpectrumPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(!mHasData) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			if(mHasPeaks) mPeaksRenderer.Render();
			PlotController::Draw();
		}

		void SpectrumPlotController::AdaptSpectralData()
		{
			SpecTypeFlags spFlags;
			mSpec.GetType(spFlags);

			if(!spFlags.bMagPhase)
			{
				SpecTypeFlags newFlags;
				mSpec.SetTypeSynchronize(newFlags);
			}
			// convert to dB
			mSpec.ToDB();
		}

		void SpectrumPlotController::CacheData()
		{
			AdaptSpectralData();
			mMagBuffer=mSpec.GetMagBuffer();
			if(mHasPeaks) CachePeaksData();
		}

		void SpectrumPlotController::FullView()
		{
			mView.left = 0.0;
			mView.right = double(mMagBuffer.Size());
			mView.top = 0.0;
			mView.bottom = -150.0;
			SetYRange(mView.bottom, mView.top);
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit viewChanged(mView);
			emit initialYRulerRange(-150.0,0.0);
		}

		void SpectrumPlotController::ProcessData()
		{
			TSize offset = TSize(GetLeftBound());
			TSize len = TSize(GetRightBound() - GetLeftBound())+1;

			if(mProcessedData.Size() <= len)
				mProcessedData.Resize(len);
			mProcessedData.SetSize(len);

			std::copy(mMagBuffer.GetPtr()+offset,mMagBuffer.GetPtr()+offset+len,mProcessedData.GetPtr());

			double range = GetRightBound()-GetLeftBound();
			double threshold = GetMinSpanX()*2.0;
			int mode = (range < threshold) ? DetailMode : NormalMode;
			mRenderer.SetData(mProcessedData,mode);
			if(mHasPeaks) ProcessPeaksData();
			mMustProcessData = false;
		}

		void SpectrumPlotController::CachePeaksData()
		{
			TData magnitude;
			TData span = TData(GetnSamples());
			TSize nPeaks = mPeaks->GetMagBuffer().Size();
			bool linear = (mPeaks->GetScale() == EScale::eLinear);

			mCachedPeaks.resize(nPeaks);

			for(int i = 0;i < nPeaks; i++)
			{
				magnitude = mPeaks->GetMagBuffer()[i];
				if(linear) magnitude = 20.0*log10(magnitude);
				mCachedPeaks[i].mag = magnitude;
				mCachedPeaks[i].freq = mPeaks->GetFreqBuffer()[i]*span/TData(mSpectralRange);
			}
		}

		void SpectrumPlotController::ProcessPeaksData()
		{
			mProcessedPeaks.clear();
			double left = GetLeftBound();
			double right = GetRightBound();
			for(unsigned i = 0; i < mCachedPeaks.size(); i++)
			{
				double current = double(mCachedPeaks[i].freq);
				if(current > right) break;
				if(current >= left) mProcessedPeaks.push_back(mCachedPeaks[i]);
			}
			mPeaksRenderer.SetHBounds(left, right);
			mPeaksRenderer.SetData(mProcessedPeaks);
		}

		void SpectrumPlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					PlotController::SetSelPos(value, render);
					if(mHasData && IsRenderingEnabled()) emit requestRefresh();
					emit selectedXPos(value*mSpectralRange/GetnSamples());
				}
			}
		}

		void SpectrumPlotController::SetMousePos(double x, double y)
		{
			if(x < 0 || x > GetnSamples()) return;
			if(y < GetMinY() || y > GetMaxY()) return;
			
			PlotController::SetMousePos(x,y);
			double f = x;
			f *= mSpectralRange;
			f /= GetnSamples();
			double m = y;
			emit sendMagFreq(m,f);
			if(!HasSentTag())
			{	
				QString s = "mag="+QString::number(m,'f',0)+"dB freq="+QString::number(f,'f',0)+"Hz";
				emit globalToolTip(s);
			}
		}

		void SpectrumPlotController::setHBounds(double xmin, double xmax)
		{
			double left = xmin*GetnSamples()/mSpectralRange;
			double right = xmax*GetnSamples()/mSpectralRange;
			SetHBounds(left,right);
		}

		void SpectrumPlotController::setSelectedXPos(double xpos)
		{
			SetSelPos(xpos*GetnSamples()/mSpectralRange,true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrumPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}
       	
    }
}

// END

