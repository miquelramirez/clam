#ifndef __FUNDPLOTCONTROLLER__
#define __FUNDPLOTCONTROLLER__

#include <qwt/qwt_scldiv.h>
#include "Segment.hxx"
#include "DataRenderer.hxx"
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class FundPlotController : public SelTimeRegionPlotController
		{
			Q_OBJECT

			public:
				FundPlotController();
				virtual ~FundPlotController();

				void SetData(const Segment& segment);
				void SetDataColor(Color c);
				void SurfaceDimensions(int w,int h);
				void Draw();

				TData GetFreq(TData t) const;

				void SetMousePos(TData x,TData y);

			signals:
				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);

			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

			private:
				Segment _segment;
				DataRenderer _renderer;
				DataArray _cacheData;
				DataArray _processedData;
				bool _mustProcessData;
				TData _maxFreq;

				void FullView();
				void CacheData();
				void ProcessData();
				void SetRenderingStep();
				int GetXMaxMajStep(int ref) const;
				int GetYMaxMajStep(int ref) const;

				void InitialRegionTime();
		};
	}
}

#endif
