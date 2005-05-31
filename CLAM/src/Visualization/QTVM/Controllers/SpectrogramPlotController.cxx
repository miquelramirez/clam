#include <numeric>
#include "SpecTypeFlags.hxx"
#include "CLAM_Math.hxx"
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
	    SetVMin(TData(16.0));
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
	    int specIndex = int(x);
	    TData hz = ycoord*mSpectralRange/GetvRange();
	    int bucket = int(ycoord);
	    TData dB = mComputedData[specIndex][bucket];
	    emit sendLabels(QString::number(hz,'f',0),QString::number(dB,'f',0),QString::number(specIndex+1));
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
	    int dataSize = mComputedData.size();
	    int specSize = mComputedData[0].size();
	 
	    std::vector< std::vector<Color> > black_white_matrix(dataSize);
	    std::vector< std::vector<Color> > color_matrix(dataSize);
	    std::vector<Color> black_white_vector(specSize);
	    std::vector<Color> color_vector(specSize);

	    for(int i=0; i < dataSize; i++)
	    {
		for(int j = 0; j < specSize; j++)
		{
		    Color c;
		    float value = ClampToRange(mComputedData[i][j]);
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
	    ComputeData();
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
	    SetnSamples(mComputedData.size()-1);
	    SetvRange(TData(mComputedData[0].size()-1));
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
	    TIndex bottomIndex=TIndex(GetBottomBound());
	    TIndex topIndex=TIndex(GetTopBound())+1;
	    TIndex leftIndex = TIndex(GetLeftBound());
	    TIndex rightIndex = TIndex(GetRightBound())+1;

	    mRenderer.SetIndexes(leftIndex, rightIndex, bottomIndex, topIndex);	    
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
	    return mComputedData.size()-1;
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

	void SpectrogramPlotController::ComputeData()
	{
	    double dx = 1.0;
	    double dy = 1.0;

	    int nSpectrums = mCacheData.Size();
	    int specSize = mCacheData[0].GetMagBuffer().Size();

	    mComputedData.clear();

	    if(nSpectrums > MaxSpectrums)
	    {
		dx = double(nSpectrums)/double(MaxSpectrums);
		nSpectrums = MaxSpectrums;
	    }

	    if(specSize > MaxSpectrumSize)
	    {
		dy = double(specSize)/double(MaxSpectrumSize);
		specSize = MaxSpectrumSize;
	    }

	    mComputedData.resize(nSpectrums);
	    for(unsigned i=0; i < mComputedData.size(); i++) mComputedData[i].resize(specSize);
	    
	    double row_diff = dx - double(int(dx));
	    double col_diff = dy - double(int(dy));

	    double row_accum = 0.0;
	    double col_accum = 0.0;

	    int total_spectrums = mCacheData.Size();
	    int original_spectrum_size = mCacheData[0].GetMagBuffer().Size();

	    TIndex row_step = int(dx);
	    TIndex col_step = int(dy);

	    TIndex i_step = row_step;
	    TIndex j_step = col_step;
	    int n, m;
	    TIndex i,j;
	    for(i=0, n=0; i < total_spectrums; i+=i_step, n++)
	    {
		for(j=0, m=0; j < original_spectrum_size; j+=j_step, m++)
		{
		    mComputedData[n][m] = MatrixBlockMean(i,i+row_step,j,j+col_step);
		    if(m == specSize-1)
		    {
			j_step = original_spectrum_size-j;
		    }
		    else
		    {
			col_accum += col_diff;
			if(col_accum >= 1.0)
			{
			    j_step = col_step+1;
			    col_accum -= 1.0;
			}
			else
			{
			    j_step = col_step;
			}
		    }
		}
		if(n == nSpectrums-1)
		{
		    i_step = total_spectrums-i;
		}
		else
		{
		    row_accum += row_diff;
		    if(row_accum >= 1.0)
		    {
			i_step = row_step+1;
			row_accum -= 1.0;
		    }
		    else
		    {
			i_step = row_step;
		    }
		}
	    }
	    
	}

	TData SpectrogramPlotController::MatrixBlockMean(const TIndex& firstRow, const TIndex& lastRow, 
							 const TIndex& firstCol, const TIndex& lastCol)
	{
	    TData sum = TData(0.0);
	    for(TIndex i=firstRow; i < lastRow; i++)
	    {
		sum = std::accumulate(mCacheData[i].GetMagBuffer().GetPtr()+firstCol,
				      mCacheData[i].GetMagBuffer().GetPtr()+lastCol,
				      sum);
	    }
	    return sum/TData((lastRow-firstRow)*(lastCol-firstCol));
	}
    }
}

// END

