#ifndef _SDIFConvert_
#define _SDIFConvert_

#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFStorage.hxx"
#include "Spectrum.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include <string.h>

/* this is for dumping PeakArray Data into a text file...*/
/* and to check the sinusoidal tracks */
//#define DUMPDATA

#ifdef DUMPDATA
  #include <stdio.h>
  #include <iostream>
  #include <fstream>
  #include <string>
#endif

using namespace CLAM;

class SDIFConvert:public SDIFStorage
{
public:

	SDIFConvert(Segment* pSpecSeg,TIndex maxNumPeaks);
	
	virtual ~SDIFConvert();
	
	void	Add(SDIFFrame* pFrame);
	
	bool GetEnableResidual()        {return mEnableResidual;}
	bool GetEnablePeakArray()       {return mEnablePeakArray;}
	bool GetEnableFundFreq()        {return mEnableFundFreq;}
//	long GetAllocatedMemoryApprox() {return mAllocatedMemory;}
	
	void SetEnableResidual(bool b)  {mEnableResidual = b;}
	void SetEnablePeakArray(bool b) {mEnablePeakArray = b;}
	void SetEnableFundFreq(bool b)  {mEnableFundFreq = b;}
	
private:
	

// member variables

	Segment*			mpSpecSeg;
	Frame					mFrame;
	SpectralPeakArray		mPeakArray;
	IndexArray						mCurrIndexArray;
	IndexArray						mPrevIndexArray;
	

	double				mSpectralRange;
	TIndex				mMaxNumPeaks;
//	long          mAllocatedMemory;   // this holds only a raw approximation of new allocated bytes
	
	bool
	  mEnableResidual,
	  mEnablePeakArray,
	  mEnableFundFreq;
	  
// for testing only
#ifdef DUMPDATA
  std::fstream  mFout;
  std::fstream  mFout2; 
#endif
};
#endif
