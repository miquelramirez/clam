#include "SDIFIn.hxx"
#include "SpectrumConfig.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

using namespace CLAM;
using std::iterator;

void SDIFInConfig::DefaultInit()
{
	AddAll();
	UpdateData();

/*	This may have to change to false but right now, Salto is the most important app that
	uses it and needs it set to true.*/
	SetRelativePeakIndices(true);
	SetFileName( "nofile" );
	SetEnableResidual(true);
	SetEnablePeakArray(true);
	SetEnableFundFreq(true);
	SetSpectralRange(22050);
	SetMaxNumPeaks(100);

}

SDIFIn::SDIFIn():
	Output("Output",this,1),
	mPrevIndexArray(0)
{ 
	mpFile=NULL;
	mLastCenterTime=-1;
	Configure(SDIFInConfig());
}

SDIFIn::SDIFIn(const SDIFInConfig& c):
	Output("Output",this,1),
	mPrevIndexArray(0)
{ 
	mpFile=NULL;
	mLastCenterTime=-1;

	Configure(c);
}
	
SDIFIn::~SDIFIn()
{
	delete mpFile;
}

bool SDIFIn::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	
	if ( mConfig.GetFileName() == "nofile") // MRJ: default configuration provided, we just left the object "Unconfigured"
	  return false;

	if(mpFile) delete mpFile;
	mpFile = new SDIF::File(mConfig.GetFileName().c_str(),SDIF::File::eInput);

	try
	  {
	    mpFile->Open();
	  }
	catch( Err& e )
	{
	  e.Print();
	  return false;
	}

	mpFile->Close();//must leave closed file ready to start()

	return true;
}

bool SDIFIn::ConcreteStart()
{
	mpFile->Open();
	return true;
}

bool SDIFIn::ConcreteStop()
{
	mpFile->Close();

	return true;
}

const ProcessingConfig& SDIFIn::GetConfig() const
{
	return mConfig;
}

bool SDIFIn::LoadSDIFDataIntoSegment( CLAM::Segment& segment )
{
	if(!mpFile) return false;
	if(mpFile->Done()) return false;
	
	SDIF::Frame tmpSDIFFrame;
	mpFile->Read(tmpSDIFFrame);
	

	double frameTimeTag	= tmpSDIFFrame.Time();
	if (frameTimeTag != mLastCenterTime)	// new SpectralFrame, need to add it to segment
	{
		Frame initFrame;
		initFrame.AddSpectralPeakArray();
		initFrame.AddResidualSpec();
		initFrame.AddFundamental();
   		initFrame.UpdateData();

		//Residual Spectrum in frame should be configured to have the ComplexArray
  		SpectrumConfig Scfg;
  		SpecTypeFlags sflags;
		sflags.bComplex = 1;
  		sflags.bMagPhase = 0;
  		initFrame.GetResidualSpec().SetType(sflags);
				
		mLastCenterTime=frameTimeTag;
		initFrame.SetCenterTime(frameTimeTag);
		segment.AddFrame(initFrame);
	}

	Frame& tmpFrame=segment.GetFrame(segment.GetnFrames()-1);
	
	SDIF::Frame::MatrixIterator frameIt = tmpSDIFFrame.Begin();

	//SDIF::Matrix* pMatrix = tmpSDIFFrame.mpFirst;
	
	SDIF::ConcreteMatrix<TFloat32>* pMatrix=
		dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);

	/* its a fundamental frequency ..*/
	if (tmpSDIFFrame.Type()=="1FQ0" && mConfig.GetEnableFundFreq())
	{
 		tmpFrame.GetFundamental().AddElem(pMatrix->GetValue(0,0));
	}	
	
	/* it is residual data ..*/
	else if(tmpSDIFFrame.Type()=="1STF" && mConfig.GetEnableResidual())	// we use always the first 2 matrices
	{
		CLAM_ASSERT(pMatrix->Type() == "ISTF","SDIFIn::Add ISTF Header in Matrix expected");

		// MRJ: We set the sampling rate for the segment
		segment.SetSamplingRate( pMatrix->GetValue( 0, 0 ) );
		
		tmpFrame.GetResidualSpec().SetSpectralRange(pMatrix->GetValue(0,0)*0.5);
		
		// move pointer to next matrix in frame
		frameIt++;
		pMatrix=
			dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);
		//pMatrix=pMatrix->mpNext;	
		
		CLAM_ASSERT(pMatrix->Type() =="1STF","SDIFIn::Add 1STF Headerin Matrix expected");
		tmpFrame.GetResidualSpec().SetSize(pMatrix->Rows());
		Array<Complex>& complexBuffer=tmpFrame.GetResidualSpec().GetComplexArray();
		for (int r=0;r<pMatrix->Rows();r++)	//read in complex data
		{
			Complex tmpComplex(pMatrix->GetValue(r,0),pMatrix->GetValue(r,1));
			complexBuffer[r] = tmpComplex;
		}
		
	}	
	
	/* its sinusoidal track data */ 
	else if(tmpSDIFFrame.Type()=="1TRC" && mConfig.GetEnablePeakArray())
	{				
		TIndex nElems = pMatrix->Rows();
	
		
		tmpFrame.GetSpectralPeakArray().AddAll();
		tmpFrame.GetSpectralPeakArray().UpdateData();
		SpectralPeakArray& tmpPeakArray=tmpFrame.GetSpectralPeakArray();
		tmpPeakArray.SetScale(EScale::eLinear);

		tmpPeakArray.SetnMaxPeaks(nElems); //number of peaks in the sdif file
		tmpPeakArray.SetnPeaks(nElems); //number of peaks in the sdif file
		tmpPeakArray.ResetIndices();		// resets all indeces, make valid..
						
		/* read file data into SpectralPeakArray */
		DataArray& pkfreqBuffer=tmpPeakArray.GetFreqBuffer();
		DataArray& pkmagBuffer=tmpPeakArray.GetMagBuffer();
		DataArray& pkPhaseBuffer=tmpPeakArray.GetPhaseBuffer();
		DataArray& pkBinPosBuffer=tmpPeakArray.GetBinPosBuffer();
		DataArray& pkBinWidthBuffer=tmpPeakArray.GetBinWidthBuffer();
		IndexArray& pkIndexArray=tmpPeakArray.GetIndexArray();
		if(!mConfig.GetRelativePeakIndices())
		{
			for (int r=0;r<nElems;r++)	
			{

				// get frequency , mag and phase
				pkfreqBuffer[r]=pMatrix->GetValue(r,1);
				pkmagBuffer[r]=pMatrix->GetValue(r,2);
				pkPhaseBuffer[r]=pMatrix->GetValue(r,3);
				pkBinPosBuffer[r]=-1;
				pkBinWidthBuffer[r]=-1;
				pkIndexArray[r]=(int)pMatrix->GetValue(r,0) - 1;	// -1 because SDIF doesnt allow Track 0
			}
		}
		else
		{
			IndexArray tmpIndexArray;
			for (int r=0;r<nElems;r++)	
			{

				// get frequency , mag and phase
				pkfreqBuffer[r]=pMatrix->GetValue(r,1);
				pkmagBuffer[r]=pMatrix->GetValue(r,2);
				pkPhaseBuffer[r]=pMatrix->GetValue(r,3);
				pkBinPosBuffer[r]=-1;
				pkBinWidthBuffer[r]=-1;
				if(mConfig.GetRelativePeakIndices())
				{
					pkIndexArray[r]=-1;			
					// track index and buffer it
					int	tempIndex = (int)pMatrix->GetValue(r,0) - 1;	// -1 because SDIF doesnt allow Track 0
					tmpIndexArray.AddElem(tempIndex);
				}
			}
			/* compare new indizes with the previous
			 * the indizes of the current peakarray should hold 
			 * then the related 
			 * peak positions to the previous peakarray */
			
			TIndex	nPeaks = tmpIndexArray.Size();
			TIndex	nPrevPeaks = mPrevIndexArray.Size();
			TIndex	currIndex,prevIndex;
			bool		bIndexFound=false;
			
			for (int i=0;i<nPeaks;i++)
			{
				bIndexFound=false;
				currIndex = tmpIndexArray[i];
				
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
			mPrevIndexArray = tmpIndexArray;
		}		
	}

	return true;	
	
}

bool SDIFIn::Do( CLAM::Segment& segment )
{
	bool thereIsMoreData = false;
	
	while( ( thereIsMoreData = LoadSDIFDataIntoSegment( segment ) ) );

	return true;
}

bool SDIFIn::Do(void)
{
	return LoadSDIFDataIntoSegment( Output.GetData() );


}

