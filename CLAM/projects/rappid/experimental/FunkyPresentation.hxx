#ifndef _MTSPECTRUMPRESENTATION_H
#define _MTSPECTRUMPRESENTATION_H

#include "GlobalEnums.hxx"

#include "MTPresentation.hxx"

#include "GLPointArrayRenderer.hxx"
#include "GLLineArrayRenderer.hxx"
#include "GLPulseArrayRenderer.hxx"
#include "GLFillArrayRenderer.hxx"
#include "GLPointsRenderer.hxx"
#include "VertexFormats.hxx"
#include "FunctionDisplay.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "IndexArray.hxx"

namespace CLAMGUI {

	struct FunctionStyleControls {

		bool     on;
		bool     togle;
		c3f      color;
		GLfloat  width;
		bool     stipled;
		GLint    stiple_factor;
		GLushort stiple_pattern;

		FunctionStyleControls()
			: on(true),
			  togle(false),
			  color(c3f(0.0,1.0,0.0)),
			  width(1.0),
			  stipled(false),
			  stiple_factor(1),
			  stiple_pattern(0x8888)
		{}
	};

	struct SignalStyleControls {
		FunctionStyleControls points;
		FunctionStyleControls lines;
		FunctionStyleControls pulses;
		FunctionStyleControls filled;
	};



	struct GridControls : public FunctionStyleControls {

		TData    step;

		GridControls()
			: step(1000.0)
		{}
	};

	struct FunkyControls {
		GridControls        grid;
		SignalStyleControls spectrum;
		SignalStyleControls peaks;
		SignalStyleControls envelope;
		GLfloat             visible_x_pos;
		GLfloat             visible_x_width;
		GLfloat             visible_y_pos;
		GLfloat             visible_y_width;
	};

	class FunkyPresentation : public MTPresentation {

	public:

		typedef enum {
			eSpectrum=0,
			ePeaks,
			eEnvelope,
			eNUMElements
		} EElements;

		typedef enum {
			eLines = 0,
			ePoints,
			ePulses,
			eFilled,
			eNUMRepresentations
		} ERepresentations;

		// Configuration API

		FunkyPresentation(EPolicy policy = eBlocking);

		void BindSpectrum(const CLAM::Spectrum&);

		void BindPeaks(const CLAM::SpectralPeakArray&);

		void BindSpectralEnvelope(const CLAM::Spectrum&);

		void SetDisplay(FunctionDisplay&);

		// GUI API

		void Draw(void);

		void SetVisibleXRange(GLfloat min, GLfloat width);

		void SetVisibleYRange(GLfloat min, GLfloat width);

		void SetVisibleRanges(GLfloat x_min, GLfloat x_width,
		                      GLfloat y_min, GLfloat y_width);

		void Togle(int element, int representation);

		void SetColor(int element, int representation, c3f color);

		void SetWidth(int element, int representation, GLfloat width);

		void SetStiple(int element, int representation,
					   bool active, GLint factor, GLushort pattern);

		// MODEL API

		bool Refresh();

	protected:

		// To be called from the (unique) processing thread
		void UpdateMagData( const DataArray& array );

		void UpdatePhaseData( const DataArray& array );

		void UpdateScale( unsigned int nbins, CLAM::EScale scale );

		void UpdateSpectralRangeData( TData value );

		void UpdatePeaksData(const DataArray& freqs,
							 const DataArray& magss,
							 const int nPeaks);

		void UpdateEnvelopeData( const DataArray& array );

	private:

		inline void UpdatePendingRequests(FunctionStyleControls &sty);
		inline void UpdatePendingRequests(); // GUI thread

		inline void FunkyPresentation::InitializeXRange(int size,
														GLfloat x_min,
														GLfloat x_width);

		inline FunctionStyleControls* GetStyle(int element, int representation);
		inline Renderer2D* GetRenderer(int element, int representation);

		FunkyControls mCfg;
		
		GLfloat mXmin, mXwidth, mMinXwidth;
		GLfloat mYmin, mYwidth, mMinYwidth;

		GLfloat mVisibleXmin, mVisibleXwidth;
		GLfloat mVisibleYmin, mVisibleYwidth;

		bool mConfiguring;

		FunctionDisplay *pDisplay;

		inline void DrawSpectrum();
		GLLineArrayRenderer  mSpectrumLineRenderer;
		GLPointArrayRenderer mSpectrumPointRenderer;
		GLPulseArrayRenderer mSpectrumPulseRenderer;
		const CLAM::Spectrum          *pSpectrum;

		inline void DrawPeaks();
//		GLLineArrayRenderer  mPeaksLineRenderer;
		GLPointsRenderer     mPeaksPointRenderer;
//		GLPulseArrayRenderer mPeaksPulseRenderer;
		const CLAM::SpectralPeakArray *pPeaks;

		inline void DrawEnvelope();
//		GLFillArrayRenderer  mEnvelopeFillRenderer;
		GLFillArrayRenderer  mEnvelopeFillRenderer;
		const CLAM::Spectrum          *pEnvelope;

	};
}
#endif
