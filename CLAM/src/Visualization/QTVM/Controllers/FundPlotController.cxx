#include "FundPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		FundPlotController::FundPlotController()
		{
			_maxFreq = TData(0.0);
			_mustProcessData = false;
			SetVMin(10.0);
		}

		FundPlotController::~FundPlotController()
		{
		}

		void FundPlotController::SetData(const Segment& segment)
		{
			_segment = segment;
			_sampleRate = _segment.GetSamplingRate();
			_duration = _segment.GetEndTime()-_segment.GetBeginTime();
			CacheData();
			FullView();
			SetnSamples(TSize(_duration*_sampleRate));
			SetvRange(TData(_maxFreq));
			InitialRegionTime();
			_mustProcessData = true;
		}

		void FundPlotController::SetDataColor(Color c)
		{
			_renderer.SetColor(c);
		}

		void FundPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SelTimeRegionPlotController::SetHBounds(left,right);
			_mustProcessData = true;
			
			double lBound = double(GetLeftBound()/_sampleRate);
		    double hBound = double(GetRightBound()/_sampleRate);
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(_viewport.w),3,false);
			emit xRulerScaleDiv(div);	
		}

		void FundPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SelTimeRegionPlotController::SetVBounds(bottom,top);
			
			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			QwtScaleDiv div;
			div.rebuild(bBound,tBound,GetYMaxMajStep(_viewport.h),3,false);
			emit yRulerScaleDiv(div);
		}

		void FundPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
		
			double lBound = double(GetLeftBound()/_sampleRate);
		    double hBound = double(GetRightBound()/_sampleRate);
			QwtScaleDiv div;
			div.rebuild(lBound,hBound,GetXMaxMajStep(w),3,false);
			emit xRulerScaleDiv(div);

			double bBound = double(GetBottomBound());
			double tBound = double(GetTopBound());
			div.rebuild(bBound,tBound,GetYMaxMajStep(h),3,false);
			emit yRulerScaleDiv(div);
		}

		void FundPlotController::Draw()
		{
			if(_mustProcessData) ProcessData();
			_renderer.Render();
			SelTimeRegionPlotController::Draw();
		}

		void FundPlotController::ProcessData()
		{
			int nFrames = _segment.GetnFrames();
			TSize offset = TSize(GetLeftBound()*nFrames/GetnSamples());
			TSize len = TSize(GetRightBound()*nFrames/GetnSamples())-offset+1;
			
			if(_processedData.Size() <=len)
				_processedData.Resize(len+1);
			_processedData.SetSize(len+1);

			std::copy(_cacheData.GetPtr()+offset,_cacheData.GetPtr()+offset+len+1,_processedData.GetPtr());
			_renderer.SetDataPtr(_processedData.GetPtr(),_processedData.Size(),NormalMode);	
			_mustProcessData = false;
		}

		void FundPlotController::SetRenderingStep()
		{
			int nFrames = _segment.GetnFrames();
			_renderer.SetStep(_duration*_sampleRate/TData(nFrames));
			SetHMin(_renderer.GetStep()*TData(5.0));
		}

		void FundPlotController::FullView()
		{
			_maxFreq += TData(50.0);
			_view.left = TData(0.0);
			_view.right = TData(_duration*_sampleRate);
			_view.top = _maxFreq;
			_view.bottom = TData(0.0);
			SetHBounds(_view.left,_view.right);
			SetVBounds(_view.bottom,_view.top);
			SetRenderingStep();
			emit sendView(_view);
		}

		void FundPlotController::CacheData()
		{
			_maxFreq = TData(0.0);
			int nFrames = _segment.GetnFrames();
			_cacheData.Resize(nFrames);
			_cacheData.SetSize(nFrames);
			TData curFreq;
			for(int i = 0; i < nFrames; i++)
			{
				curFreq = _segment.GetFrame(i).GetFundamental().GetFreq(0);
				if(curFreq > _maxFreq) _maxFreq = curFreq;
				_cacheData[i] = curFreq;	
			}
		}

		int FundPlotController::GetXMaxMajStep(int ref) const
		{
			if(ref > 1024) ref = 1024;
			int step = ref*40/1024;
			if(step < 4 || _view.right <= _sampleRate/TData(100.0)*TData(2.0)) step = 4;
			return step;
		}

		int FundPlotController::GetYMaxMajStep(int ref) const
		{
			if(ref > 750) ref = 750;
			int step = ref*35/750;
			if(step < 4) step = 4;
			return step;
		}

		TData FundPlotController::GetFreq(TData t) const
		{
			TData value = t*_sampleRate;
			int nFrames = _segment.GetnFrames();
			int index = int(value*nFrames/GetnSamples());
			return _cacheData[index];
		}

		void FundPlotController::InitialRegionTime() 
		{
			MediaTime time;
			time.SetBegin(TData(0.0));
			emit selectedRegion(time);
		}

		void FundPlotController::SetMousePos(TData x,TData y)
		{
			TData tbound = GetTopBound()-GetBottomBound();
			TData bBound = GetBottomBound();
			TData ycoord=y;
			ycoord *= tbound;
			ycoord /= TData(_viewport.h);
			ycoord += bBound;
			PlotController::SetMousePos(x,ycoord);
			TData t=GetMouseXPos()/_sampleRate;
			TData freq=GetMouseYPos();
			QString s;
			s = "t="+(s.setNum(t,'f',3))+"s freq="+(s.setNum(freq,'f',3))+"Hz";
			emit toolTip(s);
		}
	}
}

// END
