#include "FunkyPresentation.hxx"


using namespace CLAMGUI;

FunkyPresentation::FunkyPresentation(EPolicy policy)
	: MTPresentation(policy),
	  mConfiguring(true),
	  pSpectrum(0),
	  pPeaks(0),
	  pEnvelope(0)
{
	Lock();

	mSpectrumLineRenderer.SetColor(c3f(0.0,1.0,0.0));
	mSpectrumLineRenderer.SetWidth(2.0);
	mSpectrumPointRenderer.SetColor(c3f(1.0,0.0,0.0));
	mSpectrumLineRenderer.SetWidth(4.0);
	mSpectrumPulseRenderer.SetColor(c3f(0.0,0.6,0.0));
	mSpectrumLineRenderer.SetWidth(1.0);

	mPeaksPointRenderer.SetColor(c3f(0.2,0.2,1.0));
	mPeaksPointRenderer.SetWidth(10.0);

	mEnvelopeFillRenderer.SetColor(c3f(0.0,0.0,0.4));
}


void FunkyPresentation::InitializeXRange(int size,
										 GLfloat x_min,
										 GLfloat x_width)
{
	mXmin   = x_min;
	mXwidth = x_width;
	mVisibleXmin   = mXmin;
	mVisibleXwidth = mXwidth;

	mSpectrumLineRenderer.SetXLimits(mXmin,mXwidth);
	mSpectrumLineRenderer.SetDataSize(size);
	mSpectrumPointRenderer.SetXLimits(mXmin,mXwidth);
	mSpectrumPointRenderer.SetDataSize(size);
	mSpectrumPulseRenderer.SetXLimits(mXmin,mXwidth);
	mSpectrumPulseRenderer.SetDataSize(size);
	
	mPeaksPointRenderer.SetXLimits(mXmin,mXwidth);
	mPeaksPointRenderer.SetDataSize(size);

	mEnvelopeFillRenderer.SetXLimits(mXmin,mXwidth);
	mEnvelopeFillRenderer.SetDataSize(size);
	
}

void FunkyPresentation::BindSpectrum(const CLAM::Spectrum& s)
{
	CLAM_ASSERT(mConfiguring,
				"FunkyPresentation: not in configuration state");
	pSpectrum = &s;
	InitializeXRange(s.GetSize(),0.0,s.GetSpectralRange());

	mMinXwidth = s.GetSpectralRange() / s.GetSize();

	mYmin = 0.0;
	mYwidth = 10.0;
	mMinYwidth = 0.01;
}

void FunkyPresentation::BindPeaks(const CLAM::SpectralPeakArray& p)
{
	CLAM_ASSERT(mConfiguring,
				"FunkyPresentation: not in configuration state");
	pPeaks = &p;
}

void FunkyPresentation::BindSpectralEnvelope(const CLAM::Spectrum& s)
{
	CLAM_ASSERT(mConfiguring,
				"FunkyPresentation: not in configuration state");
	pEnvelope = &s;
}

void FunkyPresentation::SetDisplay(FunctionDisplay& d)
{
	CLAM_ASSERT(mConfiguring,
				"FunkyPresentation: not in configuration state");
	CLAM_ASSERT(pSpectrum,
				"FunkyPresentation: no spectrum binded");
	pDisplay = &d;
	d.AddMe(*this);
	d.SetFunctionArea(mXmin,mXwidth,mMinXwidth,
					  mYmin,mYwidth,mMinYwidth);
}


void FunkyPresentation::DrawSpectrum()
{
	if (mCfg.spectrum.lines.on)
		mSpectrumLineRenderer.Draw(mVisibleXmin, mVisibleXwidth);

	if (mCfg.spectrum.points.on)
		mSpectrumPointRenderer.Draw(mVisibleXmin, mVisibleXwidth);

	if (mCfg.spectrum.pulses.on)
		mSpectrumPulseRenderer.Draw(mVisibleXmin, mVisibleXwidth);
	
//	if (mCfg.spectrum.filled.on)
//		mSpectrumFilledRenderer.Draw(mVisibleXmin, mVisibleXwidth);
}

void FunkyPresentation::DrawPeaks()
{
//	if (mCfg.peaks.lines.on)
//		mPeaksLineRenderer.Draw(mVisibleXmin, mVisibleXwidth);

	if (mCfg.peaks.points.on)
		mPeaksPointRenderer.Draw(mVisibleXmin, mVisibleXwidth);

//	if (mCfg.peaks.pulses.on)
//		mPeaksPulseRenderer.Draw(mVisibleXmin, mVisibleXwidth);
	
//	if (mCfg.peaks.filled.on)
//		mPeaksFilledRenderer.Draw(mVisibleXmin, mVisibleXwidth);
}

void FunkyPresentation::DrawEnvelope()
{
//	if (mCfg.envelope.lines.on)
//		mEnvelopeLineRenderer.Draw(mVisibleXmin, mVisibleXwidth);

//	if (mCfg.envelope.points.on)
//		mEnvelopePointRenderer.Draw(mVisibleXmin, mVisibleXwidth);

//	if (mCfg.envelope.pulses.on)
//		mEnvelopePulseRenderer.Draw(mVisibleXmin, mVisibleXwidth);
	
	if (mCfg.envelope.filled.on)
		mEnvelopeFillRenderer.Draw(mVisibleXmin, mVisibleXwidth);
}

void FunkyPresentation::UpdatePendingRequests(FunctionStyleControls &sty)
{
	if (sty.togle) {
		sty.togle = false;
		sty.on = !sty.on;
	}
}

void FunkyPresentation::UpdatePendingRequests()
{
	UpdatePendingRequests(mCfg.spectrum.lines);
	UpdatePendingRequests(mCfg.spectrum.points);
	UpdatePendingRequests(mCfg.spectrum.pulses);
	UpdatePendingRequests(mCfg.spectrum.filled);

	UpdatePendingRequests(mCfg.peaks.lines);
	UpdatePendingRequests(mCfg.peaks.points);
	UpdatePendingRequests(mCfg.peaks.pulses);
	UpdatePendingRequests(mCfg.peaks.filled);

	UpdatePendingRequests(mCfg.envelope.lines);
	UpdatePendingRequests(mCfg.envelope.points);
	UpdatePendingRequests(mCfg.envelope.pulses);
	UpdatePendingRequests(mCfg.envelope.filled);
}

void FunkyPresentation::Draw()
{
	Lock();

	CLAM_ASSERT(!mConfiguring,
				"Funky::Draw(): Called while configuring");

	DrawEnvelope();
	DrawSpectrum();
	DrawPeaks();
	UpdatePendingRequests();
//	std::cout << "drawn" << std::endl;
	Unlock();
}


void FunkyPresentation::SetVisibleXRange(GLfloat min, GLfloat width)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	mVisibleXmin   = min;
	mVisibleXwidth = width;
	Unlock();
}

void FunkyPresentation::SetVisibleYRange(GLfloat min, GLfloat width)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	mVisibleYmin   = min;
	mVisibleYwidth = width;
	Unlock();
}

void FunkyPresentation::SetVisibleRanges(GLfloat x_min, GLfloat x_width,
										 GLfloat y_min, GLfloat y_width)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	mVisibleXmin   = x_min;
	mVisibleXwidth = x_width;
	mVisibleYmin   = y_min;
	mVisibleYwidth = y_width;
	Unlock();
}

void FunkyPresentation::Togle(int element, int representation)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Togle(): Called while configuring");
	Lock();
	FunctionStyleControls* style;	
	if (( style = GetStyle(element,representation) ))
		style->togle = !style->togle;
	Unlock();
}

void FunkyPresentation::SetColor(int element, int representation, c3f color)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	FunctionStyleControls* style;	
	if (( style = GetStyle(element,representation ) ))
		style->color = color;
	Renderer2D* renderer;
	if (( renderer = GetRenderer(element,representation ) ))
		renderer->SetColor(color);
	Unlock();
}

void FunkyPresentation::SetWidth(int element, int representation, GLfloat width)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	FunctionStyleControls* style;	
	if (( style = GetStyle(element,representation ) ))
		style->width = width;
	Renderer2D* renderer;
	if (( renderer = GetRenderer(element,representation ) ))
		renderer->SetWidth(width);
	Unlock();
}

void FunkyPresentation::SetStiple(int element, int representation,
								  bool active, GLint factor, GLushort pattern)
{
	CLAM_ASSERT(!mConfiguring,
				"Funky::Set...(): Called while configuring");
	Lock();
	FunctionStyleControls* style;	
	if (( style = GetStyle(element,representation ) )) {
		style->stipled = active;
		if (active) {
			style->stiple_factor  = factor;
			style->stiple_pattern = pattern;
		}
	}
	Unlock();
}


bool FunkyPresentation::Refresh()
{
	if (!mConfiguring) {
		
		CLAM_ASSERT(pDisplay && pSpectrum && pPeaks && pEnvelope,
					"Funky::Refresh(): Missing data bindings.");

		if (Frozen())
			return false;

		if (!WaitStepAndTryLock())
			return false;
	}

	UpdateMagData(pSpectrum->GetMagBuffer());

	UpdatePeaksData(pPeaks->GetFreqBuffer(),
					pPeaks->GetMagBuffer(),
					pPeaks->GetnPeaks());

	UpdateEnvelopeData(pEnvelope->GetMagBuffer());

	mConfiguring = false;

	pDisplay->SetModified();

	Unlock();

	return true;
}

void FunkyPresentation::UpdateMagData( const DataArray& array )
{
	if (mCfg.spectrum.lines.on)
		mSpectrumLineRenderer.CacheData(array);

	if (mCfg.spectrum.points.on)
		mSpectrumPointRenderer.CacheData(array);

	if (mCfg.spectrum.pulses.on)
		mSpectrumPulseRenderer.CacheData(array);
	
}


void FunkyPresentation::UpdateEnvelopeData( const DataArray& array )
{
	if (mCfg.envelope.filled.on)
		mEnvelopeFillRenderer.CacheData(array);
}

void FunkyPresentation::UpdatePhaseData( const DataArray& array )
{
	// TODO
}

void FunkyPresentation::UpdateScale( unsigned int nbins, CLAM::EScale scale )
{
	// TODO
}

void FunkyPresentation::UpdateSpectralRangeData( TData value )
{
	// TODO
}

void FunkyPresentation::UpdatePeaksData( const DataArray& freqs,
										 const DataArray& mags,
										 const int nPeaks)
{
//	if (mCfg.peaks.lines.on)
//		mPeaksLineRenderer.CacheData(mags,inds,nPeaks);

	if (mCfg.peaks.points.on)
		mPeaksPointRenderer.CacheData(freqs,mags,nPeaks);

//	if (mCfg.peaks.pulses.on)
//		mPeaksPulseRenderer.CacheData(mags,inds,nPeaks);
	
//	if (mCfg.peaks.filled.on)
//		mPeaksFilledRenderer.CacheData(mags,inds,nPeaks);
}


FunctionStyleControls* FunkyPresentation::GetStyle(int element, int representation)
{
	SignalStyleControls   *signal;

	switch (element) {
	case eSpectrum:
		signal = &mCfg.spectrum;
		break;
	case ePeaks:
		signal = &mCfg.peaks;
		break;
	case eEnvelope:
		signal = &mCfg.envelope;
		break;
	default:
		return 0;
	}

	switch (representation) {
	case eLines:
		return &signal->lines;
		break;
	case ePoints:
		return &signal->points;
		break;
	case ePulses:
		return &signal->pulses;
		break;
	case eFilled:
		return &signal->filled;
		break;
	}

	return 0;

}


Renderer2D* FunkyPresentation::GetRenderer(int element, int representation)
{
	switch (element) {
	case eSpectrum:
		switch (representation) {
		case eLines:
			return &mSpectrumLineRenderer;
		case ePoints:
			return &mSpectrumPointRenderer;
		case ePulses:
			return &mSpectrumPulseRenderer;
		}
		return 0;
	case ePeaks:
		switch (representation) {
//		case eLines:
//			return &mPeaksLineRenderer;
		case ePoints:
			return &mPeaksPointRenderer;
//		case ePulses:
//			return &mPeaksPulseRenderer;
		}
		return 0;
	case eEnvelope:
		switch (representation) {
		case eFilled:
			return &mEnvelopeFillRenderer;
		}
		return 0; // TODO
	default:
		return 0;
	}
	return 0;
}
