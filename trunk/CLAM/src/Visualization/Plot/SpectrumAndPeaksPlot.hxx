#ifndef __SPECTRUMANDPEAKSPLOT__
#define __SPECTRUMANDPEAKSPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Spectrum;
	class SpectralPeakArray;
}

namespace CLAMVM
{
	class LogMagSpectrumAdapter;
	class SpectralPeakArrayAdapter;
	class Fl_SMS_SpectrumAndPeaks;

	class SpectrumAndPeaksPlot : public PlotBase
	{
	public:
		SpectrumAndPeaksPlot();
		SpectrumAndPeaksPlot( std::string name );
		virtual ~SpectrumAndPeaksPlot( );
		
		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFmt );
		
		void SetData( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& p );
		void SetData( const CLAM::Spectrum& s );
		void SetData( const CLAM::SpectralPeakArray& p, double spectralRange  );

	protected:
		LogMagSpectrumAdapter*     mpSpectrumAdapter;
		SpectralPeakArrayAdapter*  mpPeakArrayAdapter;
		Fl_SMS_SpectrumAndPeaks*   mpWidget;
	};
}

#endif // SpectrumAndPeaksPlot.hxx
