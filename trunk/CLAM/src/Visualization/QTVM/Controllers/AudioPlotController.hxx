#ifndef __AUDIOPLOTCONTROLLER__
#define __AUDIOPLOTCONTROLLER__

#include <qwt/qwt_scldiv.h>
#include "Audio.hxx"
#include "DataRenderer.hxx"
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class AudioPlotController : public SelTimeRegionPlotController
		{
			Q_OBJECT

			public:
				AudioPlotController();
				virtual ~AudioPlotController();

				void SetData(const Audio& audio);
				void SetDataColor(Color c);
				void SurfaceDimensions(int w,int h);
				void Draw();

				void SetSelPos(const TData& value);
				void SetMousePos(TData x,TData y);

				TData GetAmp(TData t) const;

			signals:
				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);

			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

				void FullView();
				bool MustProcessData() const;
				
			private:
				DataRenderer _dRenderer;
				Audio _audio;
				DataArray _maxs;
				DataArray _mins;
				DataArray _processedData;
				bool _mustProcessData;
				bool _hugeArrayCondition;
				
				int GetXMaxMajStep(int ref) const;
				double GetYStep(int ref) const;
				void DrawAxis();

				void DetermineVisibleSamples();
				void ProcessData();
				void BuildMaxMinArrays(TSize offset,TSize len);

				void InitialRegionTime();

		};
	}
}

#endif

