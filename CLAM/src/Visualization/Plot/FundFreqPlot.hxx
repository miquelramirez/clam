#ifndef __FUNDFREQPLOT__
#define __FUNDFREQPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Segment;
}

namespace CLAMVM
{
	class FundamentalAdapter;
	class Fl_SMS_FundFreq_Browser;

	class FundFreqPlot : public PlotBase
	{
	public:

		FundFreqPlot( );
		FundFreqPlot( std::string name );
		virtual ~FundFreqPlot();

		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* fmtStr );

		void SetData( const CLAM::Segment& obj );
		
	protected:
		FundamentalAdapter*       mpAdapter;
		Fl_SMS_FundFreq_Browser*  mpWidget;
	};
}

#endif // FundFreqPlot.hxx
