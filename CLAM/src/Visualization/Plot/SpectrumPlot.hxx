#ifndef __SPECTRUMPLOT__
#define __SPECTRUMPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Spectrum;
}

namespace CLAMVM
{

	class LogMagSpectrumAdapter;
	class Fl_SMS_Spectrum;

	class SpectrumPlot : public PlotBase
	{
	public:
		SpectrumPlot();
		SpectrumPlot( std::string name );
		virtual ~SpectrumPlot();

		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFmt );
		void SetData( const CLAM::Spectrum& obj );
		
	protected:
		LogMagSpectrumAdapter*   mpAdapter;
		Fl_SMS_Spectrum*         mpWidget;
	};
}


#endif // SpectrumPlot.hxx
