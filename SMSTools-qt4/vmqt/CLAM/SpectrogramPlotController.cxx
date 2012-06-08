#include <numeric>
#include <CLAM/SpecTypeFlags.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/SpectrogramPlotController.hxx>

namespace CLAM
{
    namespace VM
    {
		SpectrogramPlotController::SpectrogramPlotController()
			: mDuration(0.0)
			, mSpectralRange(11025.0)
			, mSampleRate(22050.0)
			, mLocalMaxX(0.0)
			, mLocalMaxY(0.0)
			, mPalette(0.0f)
			, mMustProcessData(false)
			, mHasData(false)
		{
			SetMinSpanX(50.0);
			SetMinSpanY(40.0);
		}

		SpectrogramPlotController::~SpectrogramPlotController()
		{
		}

		void SpectrogramPlotController::SetData(const Array<Spectrum>& specMtx, double dur)
		{
			mCacheData.Resize(0);
			mCacheData.SetSize(0);
			mComputedData.clear();
			mHasData = false;
			mCacheData = specMtx;
			mDuration = dur;
			mSpectralRange = double(mCacheData[0].GetSpectralRange());
			CacheData();
			FullView();
			mMustProcessData = true;
			mHasData = true;
			mRenderer.SaveScreen();
			if(IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrogramPlotController::DisplayDimensions(int w, int h)
		{
			mMustProcessData = true;
			PlotController::DisplayDimensions(w,h);
			mMustProcessData = true;
		
			double lBound = GetLeftBound()/mSampleRate;
			double hBound = GetRightBound()/mSampleRate;
			
			emit xRulerRange(lBound,hBound);

			double bBound = GetBottomBound();
			double tBound = GetTopBound();
			
			emit yRulerRange(bBound,tBound);
		}

		void SpectrogramPlotController::Draw()
		{
			if(!mHasData || !IsRenderingEnabled()) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
			PlotController::Draw();
		}

		void SpectrogramPlotController::SetMousePos(double x, double y)
		{
			if(x < 0 || x > GetnSamples()) return;
			if(y < 0 || y > GetMaxSpanY()) return;
			
			PlotController::SetMousePos(x,y);
			double xcoord=x/mSampleRate;
			
			int specIndex = int(x*mLocalMaxX/GetnSamples());
			int bucket = int(y*mLocalMaxY/GetMaxSpanY());
			double hz = y;
			double dB = mComputedData[specIndex][bucket];
			emit sendLabels(QString::number(hz,'f',0),QString::number(dB,'f',0),QString::number(specIndex+1));
			if(!HasSentTag())
			{
				QString s = QString::number(dB,'f',0)+" dB at "+QString::number(hz,'f',0)+" Hz  t: "+TimeToStr(xcoord);
				emit globalToolTip(s);
			}

		}

		void SpectrogramPlotController::SetHBounds(double left, double right)
		{
			PlotController::SetHBounds(left,right);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double lBound = GetLeftBound()/mSampleRate;
			double hBound = GetRightBound()/mSampleRate;
			
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
			emit xRulerRange(lBound,hBound);
		}

		void SpectrogramPlotController::SetVBounds(double bottom, double top)
		{
			PlotController::SetVBounds(bottom,top);
			mMustProcessData = true;
			mRenderer.SaveScreen();
			double bBound = GetBottomBound();
			double tBound = GetTopBound();
		       
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
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
					float value = ClampToRange(float(mComputedData[i][j]));
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

		float SpectrogramPlotController::ClampToRange(float value) const
		{
			if(value >= 0) return 1.0f; // 0 = max
			if(value <= -150.0) return 0.0f; // -150 = min
			value += 150.0f;
			value /= 150.0f;
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
				SpecTypeFlags newFlags;
				for(TIndex i=0; i < mCacheData.Size(); i++) 
				{
					mCacheData[i].SetTypeSynchronize(newFlags);
				}
			}
			// convert to dB
			for(TIndex i=0; i < mCacheData.Size(); i++) mCacheData[i].ToDB();
		}

		void SpectrogramPlotController::FullView()
		{
			mSampleRate = mSpectralRange*2.0;
			mLocalMaxX = double(mComputedData.size()-1);
			mLocalMaxY = double(mComputedData[0].size()-1);
			SetnSamples(mDuration*mSampleRate);
			SetYRange(0.0,mSpectralRange);
			SetSelPos(0.0,true);
			mView.left = 0.0;
			mView.right = GetnSamples();
			mView.top = GetMaxSpanY();
			mView.bottom = 0.0;
			SetHBounds(mView.left,mView.right);
			SetVBounds(mView.bottom,mView.top);
			emit viewChanged(mView);
			emit initialYRulerRange(mView.bottom, mView.top);
		}

		void SpectrogramPlotController::ComputeIndexes()
		{
			double bottom = GetBottomBound()*mLocalMaxY/GetMaxSpanY();
			double top = GetTopBound()*mLocalMaxY/GetMaxSpanY();
			double left = GetLeftBound()*mLocalMaxX/GetnSamples();
			double right = GetRightBound()*mLocalMaxX/GetnSamples();
			TIndex bottomIndex=TIndex(bottom);
			TIndex topIndex=TIndex(top)+1;
			TIndex leftIndex = TIndex(left);
			TIndex rightIndex = TIndex(right)+1;

			mRenderer.SetHBounds(0.0,right-left);
			mRenderer.SetVBounds(bottom,top);
			mRenderer.SetIndexes(leftIndex, rightIndex, bottomIndex, topIndex);	    
		}

		std::vector<Color> SpectrogramPlotController::GetColorScale(int size)
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

		std::vector<Color> SpectrogramPlotController::GetGrayScale(int size)
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

		QString SpectrogramPlotController::TimeToStr(TData seconds)
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
			mRenderer.SaveScreen();
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
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
			PlotController::LeaveMouse();
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

		TData SpectrogramPlotController::MatrixBlockMean(TIndex firstRow, TIndex lastRow, TIndex firstCol, TIndex lastCol)
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

		void SpectrogramPlotController::SetSelPos(double value, bool render)
		{
			if(CanDrawSelectedPos())
			{
				if(GetDialPos() != value)
				{
					PlotController::SetSelPos(value, render);
					if(mHasData && IsRenderingEnabled()) emit requestRefresh();
					emit selectedXPos(value/mSampleRate);
				}
			}
		}

		void SpectrogramPlotController::setSelectedXPos(double xpos)
		{
			SetSelPos(xpos*mSampleRate,true);
			if(mHasData && IsRenderingEnabled()) emit requestRefresh();
		}

		void SpectrogramPlotController::setHBounds(double xmin, double xmax)
		{
			double left = xmin*mSampleRate;
			double right = xmax*mSampleRate;
			SetHBounds(left,right);
		}

		void SpectrogramPlotController::setVBounds(double ymin, double ymax)
		{
			SetVBounds(ymin,ymax);
		}
    }
}

// END

