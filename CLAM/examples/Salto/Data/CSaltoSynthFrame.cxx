#include "CSaltoSynthFrame.hxx"
#include "Err.hxx"
#include "SpectrumConfig.hxx"

namespace CLAM
{

CSaltoSynthFrame::CSaltoSynthFrame( TSize nMaxPeaks,
																		TSize spectralFrameSize,
																	  double sampleRate )
	://mPeakArray			(eLinear,nMaxPeaks), //BUGSPECTRUM?
// 	mSpectrum				(eLinear,eComplex,spectralFrameSize,sampleRate/2),
// 	mEmptySpectrum  (eLinear,eComplex,spectralFrameSize,sampleRate/2),
	mpPreviousFreq	(NULL),
	mpPreviousPhase	(NULL),
	mpResidual(NULL),
	mMaxPeaks				(nMaxPeaks),
	mTimeTag				(-1),
	mSynthesizedTime(0)
{
	//init PeakArray
/*	SpectralPeakArrayConfig paConfig;
	paConfig.SetScale(eLinear);
	paConfig.SetnPeaks(0);
	paConfig.SetnMaxPeaks(nMaxPeaks);
	SpectralPeakArray mPeakArray(paConfig);*/
	//SpectralPeakArray mPeakArray; //BUGSPECTRUM?
	
	/*XA: Maybe some are not necessary*/
	mPeakArray.AddAll();
	mPeakArray.UpdateData();
	mPeakArray.SetnMaxPeaks(nMaxPeaks);

	SpectrumConfig cfg;
	cfg.SetScale(EScale::eLinear);
  SpecTypeFlags sflags;
    sflags.bComplex = 1;
		sflags.bPolar = 0;
		sflags.bMagPhase = 0;
		sflags.bMagPhaseBPF = 0;
	cfg.SetType(sflags);
	cfg.SetSize(spectralFrameSize);
	cfg.SetSpectralRange(sampleRate/2);

 	mSpectrum.Configure(cfg);
 	mEmptySpectrum.Configure(cfg);
//	mpResidual->Configure(cfg); //BUG RESIDUAL!!

	// allocate phase&freq buffer
	mpPreviousPhase	= new double[mMaxPeaks];
	mpPreviousFreq	= new double[mMaxPeaks];
	
	if(mpPreviousPhase==NULL||mpPreviousFreq==NULL)
		throw Err("OOM in CSaltoSynthFrame::CSaltoSynthFrame ");
	
	// init index array and make it upToDate..
	InitIndices();
	
	// init an empty spectrum which is only used to clear the current 
	// synth spectrum via memcpy
	Complex zero(0.0,0.0);
	TIndex nComplex=mEmptySpectrum.GetSize();
	for(int i=0;i<nComplex;i++)
	{
//		mEmptySpectrum.Set(i,zero);
		mEmptySpectrum.GetComplexArray()[i] = zero;
	}
}
//----------------------------------------------------------------------------//
CSaltoSynthFrame::~CSaltoSynthFrame()
{
	if (mpPreviousPhase!=NULL){
		delete [] mpPreviousPhase;
		mpPreviousPhase=NULL;
	}
	if (mpPreviousFreq!=NULL){
		delete [] mpPreviousFreq;
		mpPreviousFreq=NULL;
	}
}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::SetPeakArray(const SpectralPeakArray &peakArray)
{
	mPeakArray = peakArray;		// makes a copy ! 
}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::SetPeak(TIndex pos,const SpectralPeak &peak)
{
	mPeakArray.SetSpectralPeak(pos,peak);		//	makes copy !
}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::SetPeakArraySize(TSize size)
{
  //SetSize
	mPeakArray.SetnPeaks(size);
}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::ClearSpectrum()
{
	// the new faster way to clear the spectrum ( uses 90% less CPU power
	// than the old way with get,set...)
	// as soon as the Spectrum has a nice and fast "ClearSpectrum"-Method
	// this unnice acces to the Buffer should be avoided.... 
/*	memcpy( mSpectrum.GetRefToComplexBuffer().GetPtr(),
      	  mEmptySpectrum.GetRefToComplexBuffer().GetPtr(),
	        mSpectrum.GetRefToComplexBuffer().SizeInBytes());
*/ //BUGSPECTRUM?

//	mSpectrum.GetComplexArray() = mEmptySpectrum.GetComplexArray();

//	memcpy( mSpectrum.GetRefToComplexBuffer().GetPtr(),mEmptySpectrum.GetRefToComplexBuffer().GetPtr(),mSpectrum.GetRefToComplexBuffer().SizeInBytes()); //BAD!!
//		memcpy((void*)&(mSpectrum.GetComplexArray()),(void*)&(mEmptySpectrum.GetComplexArray()),mSpectrum.GetComplexArray().SizeInBytes());
//	memset((void*)&(mSpectrum.GetComplexArray()),0,mSpectrum.GetComplexArray().SizeInBytes());	

//Buena:	memcpy(mSpectrum.GetComplexArray().GetPtr(),mEmptySpectrum.GetComplexArray().GetPtr(),mSpectrum.GetComplexArray().SizeInBytes()); //problems if I don't clear the spectrum

	Complex& zero = mEmptySpectrum.GetComplexArray()[0];
	TSize specSize = mEmptySpectrum.GetSize();
	Complex* pSpecCmplx = mSpectrum.GetComplexArray().GetPtr();


	do
	{
		(*pSpecCmplx++)=zero;

	} while ( --specSize );

}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::InitIndices()
{
	mPeakArray.AddIndexArray();
	mPeakArray.AddIsIndexUpToDate();
	mPeakArray.UpdateData();
	mPeakArray.SetIndicesTo(-1);
}
//----------------------------------------------------------------------------//
void CSaltoSynthFrame::BufferLastPhasesAndFreqs()
{
  for(int i=0;i<mPeakArray.GetnPeaks();i++)
  {
   	mpPreviousPhase[i] = mPeakArray.GetPhase(i);
   	mpPreviousFreq[i]  = mPeakArray.GetFreq(i);
  }
}
//----------------------------------------------------------------------------//

} // end of namesapce CLAM
