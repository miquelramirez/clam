#include <vector>
#include "SpecTypeFlags.hxx"
#include "CLAM_Math.hxx"
#include "VMColor.hxx"
#include "SpectrogramPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	SpectrogramPlotController::SpectrogramPlotController()
	    : mDuration(0.0),
	      mSpectralRange(11025.0),
	      mPalette(0.0f),
	      mMustProcessData(false)
	{
	    SetVMin(TData(50.0));
	}

	SpectrogramPlotController::~SpectrogramPlotController()
	{
	}

	void SpectrogramPlotController::SetData(const Array<Spectrum>& specMtx, const TData& dur)
	{
	    mCacheData = specMtx;
	    mDuration = dur;
	    mSpectralRange = mCacheData[0].GetSpectralRange();
	    CacheData();
	    FullView();
	    mMustProcessData = true;
	    emit requestRefresh();
	}

	void SpectrogramPlotController::SurfaceDimensions(int w,int h)
	{
	    PlotController::SurfaceDimensions(w,h);
	    mMustProcessData = true;
		
	    double lBound = double(GetLeftBound()*mDuration/TData(GetnSamples()));
	    double hBound = double(GetRightBound()*mDuration/TData(GetnSamples()));
			
	    emit xRulerRange(lBound,hBound);

	    double bBound = double(GetBottomBound()*mSpectralRange/GetvRange());
	    double tBound = double(GetTopBound()*mSpectralRange/GetvRange());
			
	    emit yRulerRange(bBound,tBound);
	}

	void SpectrogramPlotController::Draw()
	{
	    if(mMustProcessData) ProcessData();
	    mRenderer.Render();
	    SelPosPlotController::Draw();
	}

	void SpectrogramPlotController::SetMousePos(TData x,TData y)
	{	   
	    SegmentationMarksPlotController::SetMousePos(x,y);
	    TData xcoord=x;
	    xcoord *= mDuration;
	    xcoord /= TData(GetnSamples());
	    TData tbound = GetTopBound()-GetBottomBound();
	    TData bBound = GetBottomBound();
	    TData ycoord=y;
	    ycoord *= tbound;
	    ycoord /= TData(_viewport.h);
	    ycoord += bBound;
	    PlotController::SetMousePos(xcoord,ycoord);    
	    TIndex specIndex = TIndex(x);
	    TData hz = ycoord*mSpectralRange/GetvRange();
	    TIndex bucket = TIndex(ycoord);
	    TData dB = mCacheData[specIndex].GetMagBuffer()[bucket];
	    emit sendLabels(QString::number(hz,'f',0),QString::number(dB,'f',0),QString::number(int(specIndex)));
	    if(!HasSentTag())
	    {
		QString s = QString::number(dB,'f',0)+" dB at "+QString::number(hz,'f',0)+" Hz  t: "+TimeToStr(xcoord);
		emit toolTip(s);
	    }
	}

	bool SpectrogramPlotController::IsPlayable()
	{
	    return false;
	}

	void SpectrogramPlotController::SetHBounds(const TData& left,const TData& right)
	{
	    SelPosPlotController::SetHBounds(left,right);
	    mMustProcessData = true;
			
	    double lBound = double(GetLeftBound()*mDuration/TData(GetnSamples()));
	    double hBound = double(GetRightBound()*mDuration/TData(GetnSamples()));
			
	    emit xRulerRange(lBound,hBound);
	}

	void SpectrogramPlotController::SetVBounds(const TData& bottom,const TData& top)
	{
	    SelPosPlotController::SetVBounds(bottom,top);
	    mMustProcessData = true;
			
	    double bBound = double(GetBottomBound()*mSpectralRange/GetvRange());
	    double tBound = double(GetTopBound()*mSpectralRange/GetvRange());
		       
	    emit yRulerRange(bBound,tBound);
	}

	void SpectrogramPlotController::ProcessData()
	{
	    ComputeIndexes();
	    mMustProcessData = false;
	}

	void SpectrogramPlotController::Colorize()
	{
	    TSize dataSize = mCacheData.Size();
	    TSize specSize = mCacheData[0].GetMagBuffer().Size();
	 
	    std::vector< std::vector<Color> > black_white_matrix(dataSize);
	    std::vector< std::vector<Color> > color_matrix(dataSize);
	    std::vector<Color> black_white_vector(specSize);
	    std::vector<Color> color_vector(specSize);

	    for(TIndex i=0; i < dataSize; i++)
	    {
		for(TIndex j = 0; j < specSize; j++)
		{
		    Color c;
		    float value = ClampToRange(mCacheData[i].GetMagBuffer()[j]);
		    // cplor
		    TIndex colorIndex = mPalette.Get(value);
		    mPalette.GetRGBFromIndex( colorIndex, c.r, c.g, c.b);
		    color_vector[j]=c;
		    // B&W
		    value = fabs(value-1.0f);
		    c.r = c.g = c.b = int(255.0*value);
		    black_white_vector[j]=c;
		}
		color_matrix[i]=color_vector;
		black_white_matrix[i]=black_white_vector;
	    }
	    mRenderer.SetColorData(color_matrix);
	    mRenderer.SetBWData(black_white_matrix);
	}

	float SpectrogramPlotController::ClampToRange(TData value) const
	{
	    if(value >= 0) return 1.0; // 0 = max
	    if(value <= -150.0) return 0.0; // -150 = min
	    value += 150.0;
	    value /= 150.0;
	    return value;
	}
	    
	void SpectrogramPlotController::CacheData()
	{
	    AdaptSpectralData();
	    Colorize();
	}

	void SpectrogramPlotController::AdaptSpectralData()
	{
	    SpecTypeFlags spFlags;
	    mCacheData[0].GetType(spFlags);

	    if(!spFlags.bMagPhase)
	    {
                 // check for conversions
		if(spFlags.bComplex)
		{
		    for(TIndex i=0; i < mCacheData.Size(); i++) mCacheData[i].Complex2MagPhase();
		}
		else if(spFlags.bPolar)
		{
		    for(TIndex i=0; i < mCacheData.Size(); i++) mCacheData[i].Polar2MagPhase();
		}
		else if(spFlags.bMagPhaseBPF)
		{
		    for(TIndex i=0; i < mCacheData.Size(); i++) mCacheData[i].BPF2MagPhase();
		}
	    }
	    // convert to dB
	    for(TIndex i=0; i < mCacheData.Size(); i++) mCacheData[i].ToDB();
	}

	void SpectrogramPlotController::FullView()
	{
	    SetnSamples(mCacheData.Size()-1);
	    SetvRange(mCacheData[0].GetMagBuffer().Size()-1);
	    SetSelPos(TData(0.0));
	    _view.left = TData(0.0);
	    _view.right = TData(GetnSamples());
	    _view.top = GetvRange();
	    _view.bottom = TData(0.0);
	    SetHBounds(_view.left,_view.right);
	    SetVBounds(_view.bottom,_view.top);
	    emit sendView(_view);
	    emit initialYRulerRange(double(_view.bottom),double(_view.top));
	}

	void SpectrogramPlotController::ComputeIndexes()
	{
	    TSize nSamples = mCacheData.Size();
	    TSize specSize = mCacheData[0].GetMagBuffer().Size();
	    TIndex leftIndex, rightIndex, bottomIndex, topIndex;

	    bottomIndex=TIndex(GetBottomBound());
	    topIndex=TIndex(GetTopBound());
	    leftIndex = TIndex(GetLeftBound());
	    rightIndex = TIndex(GetRightBound());
	    if(topIndex < specSize-2)
		topIndex+=3;
	    else if(topIndex < specSize-1)
		topIndex+=2;
	    else if(topIndex < specSize)
		topIndex++;
	    if(rightIndex < nSamples-2)
		rightIndex+=3;
	    else if(rightIndex < nSamples-1)
		rightIndex+=2;
	    else if(rightIndex < nSamples)
		rightIndex++;
	    mRenderer.SetIndexes(leftIndex, rightIndex, bottomIndex, topIndex);
	    float lineWidth = float(TData(_viewport.h)/(GetTopBound()-GetBottomBound()));
	    if(lineWidth > 1) 
		mRenderer.SetLineWidth(float(int(lineWidth))+1.0f);
	    else
		mRenderer.SetLineWidth(1.0f);
	}

	std::vector<Color> SpectrogramPlotController::GetColorScale(const int& size)
	{
	    float step = -150.f/float(size);
	    std::vector<Color> scale(size);
	    for(unsigned i=0; i < scale.size(); i++)
	    {
		 Color c;
		 TIndex colorIndex = mPalette.Get( ClampToRange(float(i)*step));
		 mPalette.GetRGBFromIndex( colorIndex, c.r, c.g, c.b);
		 scale[i]=c;
	    }
	    return scale;
	}

	std::vector<Color> SpectrogramPlotController::GetGrayScale(const int& size)
	{
	    float step = -150.0f/float(size);
	    std::vector<Color> scale(size);
	    for(unsigned i=0; i < scale.size(); i++)
	    {
		 Color c;
		 float value = ClampToRange(float(i)*step);
		 value = fabs(value-1.0f);
		 c.r = c.g = c.b = int(255.0*value);
		 scale[i]=c;
	    }
	    return scale;
	}

	QString SpectrogramPlotController::TimeToStr(const TData& seconds)
	{
	    QString s("");
	    int tmp = int(seconds*1000);
	    int sec = tmp/1000;
	    int min = sec/60;
	    sec %= 60;
	    int msec = tmp%1000;
	    s = s.sprintf("%02d:%02d,%03d",min,sec,msec);
	    return s;
	}

	void SpectrogramPlotController::SetRenderingMode(CLAM::VM::SonogramCM colorMap)
	{
	    mRenderer.SetRenderingMode(colorMap);
	    emit requestRefresh();
	}

	int SpectrogramPlotController::FFTSize() const
	{
	    return int((mCacheData[0].GetMagBuffer().Size()-TData(1.0))*TData(2.0));
	}

	int SpectrogramPlotController::TotalSlices() const
	{
	    return mCacheData.Size();
	}

	int SpectrogramPlotController::SpectralRange() const
	{
	    return int(mSpectralRange);
	}

	void SpectrogramPlotController::LeaveMouse()
	{
	    SegmentationMarksPlotController::LeaveMouse();
	    emit sendLabels("--","--","--");
	}
    }
}

// END

