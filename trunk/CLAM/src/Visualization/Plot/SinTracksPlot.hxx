#ifndef __SINTRACKSPLOT__
#define __SINTRACKSPLOT__

#include "PlotBase.hxx"
#include "Array.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM
{
	class Segment;
}

namespace CLAMVM
{
	class SinTracksAdapter;
	class Fl_SMS_SinTracks_Browser;

	class SinTracksPlot : public PlotBase
	{
	public:
		SinTracksPlot();
		SinTracksPlot( std::string name );
		virtual ~SinTracksPlot();

		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFmt );
		void SetData( const CLAM::Segment& obj );
		void SetData( const CLAM::Array< CLAM::SpectralPeakArray >& obj,
					  double sampleRate );

	protected:
		
		SinTracksAdapter*          mpAdapter;
		Fl_SMS_SinTracks_Browser*   mpWidget;
	};
}

#endif // SinTracksPlot.hxx
