#ifndef __SPECTRUMPLOTCONTROLLER__
#define __SPECTRUMPLOTCONTROLLER__
#include <qwt/qwt_scldiv.h>
#include "Spectrum.hxx"
#include "DataRenderer.hxx"
#include "SelPosPlotController.hxx"
namespace CLAM
{
	namespace VM
	{
		class SpectrumPlotController : public SelPosPlotController
		{
			Q_OBJECT
			public:
				SpectrumPlotController();
				virtual ~SpectrumPlotController();
				void SetData(const Spectrum& spec);
				void SetDataColor(Color c);
				void SurfaceDimensions(int w,int h);

				virtual void Draw();
				void SetMousePos(TData x,TData y);

				virtual void SetSelPos(const TData& value);
			signals:
				void mag(TData);
				void freq(TData);
				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);

			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);
				void FullView();
				TData GetSpectralRange() const;
				bool MustProcessData() const;
			private:
				Spectrum  _spec;
				DataRenderer _renderer;
				DataArray _magBuffer;
				DataArray _phaseBuffer;
				DataArray _processedData;

				bool _mustProcessData;
				TData _spectralRange;
				void AdaptSpectralData();
				void CacheData();
				void ProcessData();
				
				int GetXMaxMajStep(int ref) const;
				int GetYMaxMajStep(int ref) const;
		};
	}
}

#endif




