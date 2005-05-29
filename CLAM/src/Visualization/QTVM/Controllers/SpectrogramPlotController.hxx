#ifndef __SPECTROGRAMPLOTCONTROLLER__
#define __SPECTROGRAMPLOTCONTROLLER__

#include "Spectrum.hxx"
#include "QtPalette.hxx"
#include "SpectrogramRenderer.hxx"
#include "SelPosPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class SpectrogramPlotController : public SelPosPlotController 
	{
	    Q_OBJECT

	public:
	    SpectrogramPlotController();
	    ~SpectrogramPlotController();

	    void SetData(const Array<Spectrum>& specMtx, const TData& dur);
	    void SurfaceDimensions(int w,int h);
	    void Draw();

	    void SetMousePos(TData x,TData y);

	    bool IsPlayable();

	    std::vector<Color> GetColorScale(const int& size);
	    std::vector<Color> GetGrayScale(const int& size);

	    void SetRenderingMode(CLAM::VM::SonogramCM colorMap);

	    int FFTSize() const;
	    int TotalSlices() const;
	    int SpectralRange() const;

	    void LeaveMouse();

	signals:
	    void sendLabels(QString, QString, QString);

	protected:
	    void SetHBounds(const TData& left,const TData& right);
	    void SetVBounds(const TData& bottom,const TData& top);

	private:
	    Array<Spectrum> mCacheData;
	    std::vector< std::vector<TData> > mComputedData;
	    SpectrogramRenderer mRenderer;
	    TData mDuration;
	    TData mSpectralRange;
	    QtPalette mPalette;
	    bool mMustProcessData;

	    enum { MaxSpectrums=513, MaxSpectrumSize=513};

	    void ProcessData();
	    void Colorize();

	    float ClampToRange(TData value) const;
	    
	    void CacheData();
	    void AdaptSpectralData();

	    void FullView();

	    void ComputeIndexes();

	    QString TimeToStr(const TData& seconds);

	    void ComputeData();
	    TData MatrixBlockMean(const TIndex& firstRow, const TIndex& lastRow, 
				  const TIndex& firstCol, const TIndex& lastCol);
			
	};
    }
}

#endif

