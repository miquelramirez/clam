#include "SDIFConvert.hxx"
#include "Fundamental.hxx"

using namespace CLAM;

SDIFConvert::SDIFConvert(Segment* pSpecSeg,TIndex maxNumPeaks):
mCurrIndexArray(maxNumPeaks),
mPrevIndexArray(maxNumPeaks),
mEnableResidual(true),
mEnablePeakArray(true),
mEnableFundFreq(true)
//,mAllocatedMemory(0)
{ 
	mpSpecSeg 					= pSpecSeg;
	mSpectralRange 			= 22050;
	//XA:mpPreviousFrame 		= NULL;
	//XA:mpPreviousPeakArray = NULL;
	mMaxNumPeaks				=	maxNumPeaks;

	mCurrIndexArray.Resize(0);
	mCurrIndexArray.SetSize(0); //BUGSPECTRUM? should call Resize?
	mPrevIndexArray.Resize(0);
	mPrevIndexArray.SetSize(0);
	
   	mFrame.AddSpectrum();
   	mFrame.AddSpectralPeakArray();
   	mFrame.AddFundamental();
	mFrame.AddResidualSpec();		
   	mFrame.UpdateData();

	mPeakArray.AddAll();
	mPeakArray.UpdateData();

	//should be configured to have the ComplexArray
  	SpectrumConfig Scfg; //BUGSPECTRUM?
  	Scfg.SetScale(EScale::eLinear);
	SpecTypeFlags sflags;
    sflags.bComplex = 1;
  	sflags.bPolar = 0;
  	sflags.bMagPhase = 0;
  	sflags.bMagPhaseBPF = 0;
  	Scfg.SetType(sflags);
//  	Scfg.SetSize(mSynthSineSpectrum.Size());
//  	Scfg.SetSpectralRange(22050);
	Spectrum tmpSpec(Scfg);
	mFrame.SetResidualSpec(tmpSpec);


  #ifdef DUMPDATA
	  mFout.open("DUMP.txt",std::ios::out);
    
    if (!mFout.is_open())
      throw Err("SDIFConvert::SDIFConvert DumpFile could not be opened");
      
    mFout2.open("DUMP2.txt",std::ios::out);
    
    if (!mFout2.is_open())
      throw Err("SDIFConvert::SDIFConvert DumpFile could not be opened");
  #endif
}
	
SDIFConvert::~SDIFConvert()
{
	
	#ifdef DUMPDATA
	  mFout.close();
	  mFout2.close();
	#endif
	
	// no need to deallocate memory here , this will happen in a higher level
}

//----------------------------------------------------------------------------//
void SDIFConvert::Add(SDIFFrame* pFrame)
{
	int flag = 0;
	double samplingRate;
	
	double frameTimeTag	= pFrame->mHeader.mTime;
	TInt32 frameSize 		= pFrame->mHeader.mSize;		
	TInt32 streamID			= pFrame->mHeader.mStreamId;
	TInt32 numMatrices	= pFrame->mHeader.mnMatrices; 	// number of Matrices in this frame


	if (frameTimeTag != mFrame.GetCenterTime())	// new SpectralFrame
	{
		mFrame.SetCenterTime(frameTimeTag);
		flag = 1;
	} 
	else
	{
		flag = 0;
	}
	
	SDIFMatrix* pMatrix = pFrame->mpFirst;
	
	/* its a fundamental frequency ..*/
	if (pFrame->mHeader.mType=="1FQ0" && mEnableFundFreq)
	{
 		Fundamental CurrFundFreq;
		CurrFundFreq.AddElem(pMatrix->GetValue(0,0)); //now
 		
		mFrame.SetFundamental(CurrFundFreq);
		
	}	
	
	/* it is residual data ..*/
	else if(pFrame->mHeader.mType=="1STF" && mEnableResidual)	// we use always the first 2 matrices
	{
		if(!(pMatrix->mHeader.mType =="ISTF"))	
			throw Err("SDIFConvert::Add ISTF Header in Matrix expected");	
		
		samplingRate					= pMatrix->GetValue(0,0);
		double FrameSize			= pMatrix->GetValue(0,1);
		double TransformSize	= pMatrix->GetValue(0,2);

		pMatrix=pMatrix->mpNext;	// move pointer to next matrix in frame
		
		if(!(pMatrix->mHeader.mType=="1STF"))	
			throw Err("SDIFConvert::Add 1STF Headerin Matrix expected");
		mFrame.GetResidualSpec().SetSize(pMatrix->mHeader.mnRows);
		Array<Complex>& complexBuffer=mFrame.GetResidualSpec().GetComplexArray();
		for (int r=0;r<pMatrix->mHeader.mnRows;r++)	//read in complex data
		{
			Complex tmpComplex(pMatrix->GetValue(r,0),pMatrix->GetValue(r,1));
			complexBuffer[r] = tmpComplex;
		}
		
	}	
	
	/* its sinusoidal track data */ 
	else if(pFrame->mHeader.mType=="1TRC" && mEnablePeakArray)
	{				
		TIndex nElems = pMatrix->mHeader.mnRows;
	
		
		//TODO XA: This is a clutch, I don't know who else touches this!
		if(mPeakArray.GetnMaxPeaks()!= mPeakArray.GetIndexArray().AllocatedSize())
			mPeakArray.SetnMaxPeaks(nElems); //number of peaks in the sdif file
		mPeakArray.SetnPeaks(nElems); //number of peaks in the sdif file
				

		
		mPeakArray.ResetIndices();		// resets all indeces, make valid..
						
		/* read file data into SpectralPeakArray */
		DataArray& pkfreqBuffer=mPeakArray.GetFreqBuffer();
		DataArray& pkmagBuffer=mPeakArray.GetMagBuffer();
		DataArray& pkPhaseBuffer=mPeakArray.GetPhaseBuffer();
		DataArray& pkBinPosBuffer=mPeakArray.GetBinPosBuffer();
		DataArray& pkBinWidthBuffer=mPeakArray.GetBinWidthBuffer();
		IndexArray& pkIndexArray=mPeakArray.GetIndexArray();
		
		for (int r=0;r<nElems;r++)	
		{

			// get frequency , mag and phase
			pkfreqBuffer[r]=pMatrix->GetValue(r,1);
			pkmagBuffer[r]=pMatrix->GetValue(r,2);
			pkPhaseBuffer[r]=pMatrix->GetValue(r,3);
			pkBinPosBuffer[r]=-1;
			pkBinWidthBuffer[r]=-1;
			pkIndexArray[r]=-1;			
			// track index and buffer it
			int	tempIndex = pMatrix->GetValue(r,0) - 1;	// -1 because SDIF doesnt allow Track 0
			mCurrIndexArray.AddElem(tempIndex);

			#ifdef DUMPDATA
			  TIndex numFrame = mpSpecSeg->GetNFrames(); 
			  mFout<<"  Frame:"<<numFrame<<"  Pos:"<<r<<"  SMSID:"<<tempIndex
			    <<"  Freq:"<<tempPeak.GetFreq()<<std::endl;			
			#endif
		}

		/* compare new indizes with the previous
		 * the indizes of the current peakarray should hold 
		 * then the related 
		 * peak positions to the previous peakarray */
		
		TIndex	nPeaks = mCurrIndexArray.Size();
		TIndex	nPrevPeaks = mPrevIndexArray.Size();
		TIndex	currIndex,prevIndex;
		bool		bIndexFound=false;
		
		for (int i=0;i<nPeaks;i++)
		{
			bIndexFound=false;
			currIndex = mCurrIndexArray[i];
			
			for (int j=0;j<nPrevPeaks;j++)
			{
				prevIndex = mPrevIndexArray[j];
				if 	(prevIndex==currIndex)
				{
					pkIndexArray[i]=j;
					bIndexFound = true;
					break;
				}
			}
			if (!bIndexFound) pkIndexArray[i]=-1;  
		}
				
		/* current IndexArray becomes the Previous */
		mPrevIndexArray = mCurrIndexArray;
		mCurrIndexArray.Resize(0); //BUGSPECTRUM? should call Resize?
		mCurrIndexArray.SetSize(0); //BUGSPECTRUM? should call Resize?
		
//XA:		mPeakArray = pCurrPeakArray;
		mFrame.SetSpectralPeakArray(mPeakArray);
		
		#ifdef DUMPDATA
  		for (int i=0;i<nPeaks;i++)
  		{
  		  SpectralPeak testPeak;
  		  pCurrPeakArray->GetPeak(i,testPeak); 
  		  TIndex numFrame = mpSpecSeg->GetNFrames();
  		  mFout2<<"  Frame:"<<numFrame<<"  Pos:"<<i<<"  PrevID:"<<pCurrPeakArray->GetIndexFromPosition(i)
  		    <<"  Freq:"<<testPeak.GetFreq()<<std::endl;			
  		}
		#endif
		
	}
	
	delete pFrame;

	if (flag == 1) mpSpecSeg->AddFrame(mFrame);
}
//----------------------------------------------------------------------------//
