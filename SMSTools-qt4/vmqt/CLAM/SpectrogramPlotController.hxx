#ifndef __SPECTROGRAMPLOTCONTROLLER__
#define __SPECTROGRAMPLOTCONTROLLER__

#include <CLAM/Spectrum.hxx>
#include <CLAM/QtPalette.hxx>
#include <CLAM/SpectrogramRenderer.hxx>
#include <CLAM/PlotController.hxx>

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

			void SetData(const Array<Spectrum>& specMtx, double dur);
			void DisplayDimensions(int w, int h);
			void Draw();

			void SetMousePos(double x, double y);

			std::vector<Color> GetColorScale(int size);
			std::vector<Color> GetGrayScale(int size);

			void SetRenderingMode(CLAM::VM::SonogramCM colorMap);

			int FFTSize() const;
			int TotalSlices() const;
			int SpectralRange() const;

			void LeaveMouse();

			void SetSelPos(double value, bool render);

		signals:
			void sendLabels(QString, QString, QString);

		public slots:
			void setHBounds(double, double);
			void setVBounds(double, double);
			void setSelectedXPos(double);

		protected:
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);

			void FullView();

		private:
			Array<Spectrum>                   mCacheData;
			std::vector< std::vector<TData> > mComputedData;
			SpectrogramRenderer               mRenderer;
			double                            mDuration;
			double                            mSpectralRange;
			double                            mSampleRate;
			double                            mLocalMaxX;
			double                            mLocalMaxY;
			QtPalette                         mPalette;
			bool                              mMustProcessData;
			bool                              mHasData;

			enum { MaxSpectrums=257, MaxSpectrumSize=257};

			void ProcessData();
			void Colorize();

			float ClampToRange(float value) const;
	    
			void CacheData();
			void AdaptSpectralData();

			void ComputeIndexes();

			QString TimeToStr(TData seconds);

			void ComputeData();
			TData MatrixBlockMean(TIndex firstRow, TIndex lastRow, TIndex firstCol, TIndex lastCol);
			
		};
    }
}

#endif

