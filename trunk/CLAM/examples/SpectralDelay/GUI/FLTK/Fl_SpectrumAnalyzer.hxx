#ifndef __SPECTRALDELAY_FL_SPECTRUMANALYZER_HXX
#define __SPECTRALDELAY_FL_SPECTRUMANALYZER_HXX

// --------------------------------------------------------------------------------------

#include <FL/Fl_Window.H>
#include "SpectrumPresentation.hxx"
#include "SpectrumRenderingManager.hxx"

// --------------------------------------------------------------------------------------

// Forwards declarations:
namespace CLAMVM
{
	class Fl_Gl_Single_Display;
	class Fl_GridLayout;
}

// --------------------------------------------------------------------------------------

namespace SpectralDelay
{

	/**
	 * Widget which displays a spectrum (specific version for the SpectralDelay application).
	 */
	class Fl_SpectrumAnalyzer : public CLAMVM::SpectrumPresentation, public Fl_Window
	{
	private:
		CLAMVM::Fl_GridLayout *mLayout;
		CLAMVM::Fl_Gl_Single_Display *mDisplay;
		CLAMVM::SpectrumRenderingManager mDrawMgr;

	protected:
		virtual void OnNewSpectrum( const CLAM::DataArray&, CLAM::TData );

	public:
		Fl_SpectrumAnalyzer( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SpectrumAnalyzer(); // XXX: perhaps should be virtual (since there's a protected section)?

		void resize( int X, int Y, int W, int H );
		void Show();
		void Hide();
	};


} // namespace SpectralDelay

// --------------------------------------------------------------------------------------

#endif // __SPECTRALDELAY_FL_SPECTRUMANALYZER_HXX
