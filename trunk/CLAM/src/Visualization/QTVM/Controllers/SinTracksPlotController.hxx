#ifndef __SINTRACKSPLOTCONTROLLER__
#define __SINTRACKSPLOTCONTROLLER__

#include <qwt/qwt_scldiv.h>
#include "Segment.hxx"
#include "SinTrackHClipper.hxx"
#include "SinTrackVClipper.hxx"
#include "SinTracksRenderer.hxx"
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SinTracksPlotController : public SelTimeRegionPlotController 
		{
			Q_OBJECT

			public:
				SinTracksPlotController();
				virtual ~SinTracksPlotController();

				void SetData(const Segment& segment);
				void SetData(const Array< SpectralPeakArray >& peakMtx, const TData& sr, const TData& dur);
				void SurfaceDimensions(int w,int h);
				void Draw();

				void SetMousePos(TData x,TData y);

			signals:
				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);
			
			protected:
				virtual void SetHBounds(const TData& left,const TData& right);
				virtual void SetVBounds(const TData& bottom,const TData& top);

			private:
				SineTrackList	_cachedTracks;
				SinTrackHorClipper	_horClipper;
				SinTrackVerClipper	_verClipper;
				SinTracksRenderer	_renderer;
				SineTrackSpanEnds	_starts;
				SineTrackSpanEnds	_ends;
				bool	_mustProcessData;
				int	_nFrames;

				void FullView();
				int GetXMaxMajStep(int ref) const;
				int GetYMaxMajStep(int ref) const;

				void ProcessData();
				void Colorize();

				float ClampToRange(TData value) const;
				void InitialRegionTime();
			
		};
	}
}

#endif

