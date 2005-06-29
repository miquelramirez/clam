#ifndef __SPECTROGRAMPLOTCONTROLLER__
#define __SPECTROGRAMPLOTCONTROLLER__

#include "Spectrum.hxx"
#include "QtPalette.hxx"
#include "SpectrogramRenderer.hxx"
#include "PlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class SpectrogramPlotController : public PlotController 
		{
			Q_OBJECT

		public:
			SpectrogramPlotController();
			~SpectrogramPlotController();

			void SetData(const Array<Spectrum>& specMtx, const double& dur);
			void DisplayDimensions(const int& w, const int& h);
			void Draw();

			void SetMousePos(const double& x, const double& y);

			std::vector<Color> GetColorScale(const int& size);
			std::vector<Color> GetGrayScale(const int& size);

			void SetRenderingMode(CLAM::VM::SonogramCM colorMap);

			int FFTSize() const;
			int TotalSlices() const;
			int SpectralRange() const;

			void LeaveMouse();

			void SetSelPos(const double& value, bool render);

		signals:
			void sendLabels(QString, QString, QString);

		public slots:
			void setHBounds(double, double);
			void setSelectedXPos(double);

		protected:
			void SetHBounds(const double& left, const double& right);
			void SetVBounds(const double& bottom,const double& top);

			void FullView();

		private:
			Array<Spectrum>                   mCacheData;
			std::vector< std::vector<TData> > mComputedData;
			SpectrogramRenderer               mRenderer;
			double                            mDuration;
			double                            mSpectralRange;
			QtPalette                         mPalette;
			bool                              mMustProcessData;
			bool                              mHasData;

			enum { MaxSpectrums=513, MaxSpectrumSize=513};

			void ProcessData();
			void Colorize();

			float ClampToRange(float value) const;
	    
			void CacheData();
			void AdaptSpectralData();

			void ComputeIndexes();

			QString TimeToStr(const TData& seconds);

			void ComputeData();
			TData MatrixBlockMean(const TIndex& firstRow, const TIndex& lastRow, 
								  const TIndex& firstCol, const TIndex& lastCol);
			
		};
    }
}

#endif

