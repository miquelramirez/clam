#ifndef _CSaltoSynthFrame_
#define _CSaltoSynthFrame_

#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include <cstring>

namespace CLAM
{

class CSaltoSynthFrame
{
public:
	// constructor/destructor
	CSaltoSynthFrame(TSize nMaxPeaks,TIndex spectralFrameSize,double sampleRate);
	~CSaltoSynthFrame();

	// accessors
	void                SetTimeTag(double timeTag)          {mTimeTag = timeTag;}
	double              GetTimeTag()                        {return mTimeTag;}

	void                SetSynthTime(double synthesizedTime){mSynthesizedTime = synthesizedTime;}
	double              GetSynthTime()                      {return mSynthesizedTime;}

	SpectralPeakArray*  GetPeakArrayPtr()                   {return &mPeakArray;}
	SpectralPeakArray&  GetPeakArray()                       {return mPeakArray;}
	void                SetPeakArraySize(TSize size);
	void                SetPeakArrayIndex(TIndex pos,TIndex index)  {mPeakArray.SetIndex(pos,index);}  
	inline TSize        GetNumCurrPeaks()                   {return mPeakArray.GetnPeaks();}

	void                SetFundFreq(double fundFreq)        {mFundFreq = fundFreq;}
	double              GetFundFreq()                       {return mFundFreq;}

	double*             GetPreviousPhasePtr()               {return mpPreviousPhase;}
	double*             GetPreviousFreqPtr()                {return mpPreviousFreq;}

	void                SetResidualPtr(Spectrum* ptr)       {mpResidual = ptr;}
	Spectrum*           GetResidualPtr()                    {return mpResidual;}

	Spectrum*           GetSpectrumPtr()                    {return  &mSpectrum;}

	// public methods
	void                SetPeakArray          (const SpectralPeakArray &peakArray);
	void                ClearSpectrum          ();
	void                SetPeak                (TIndex pos, const SpectralPeak &peak);
	void                BufferLastPhasesAndFreqs();

	private:
	// private methods
	void                    InitIndices();

	// private members
	SpectralPeakArray  mPeakArray;
	TSize              mMaxPeaks;

	double             *mpPreviousPhase;
	double             *mpPreviousFreq;

	Spectrum           mSpectrum;
	Spectrum           mEmptySpectrum;
	Spectrum*          mpResidual;

	double             mFundFreq;
	double             mTimeTag;
	double             mSynthesizedTime;
};

} // end of namespace CLAM
#endif
