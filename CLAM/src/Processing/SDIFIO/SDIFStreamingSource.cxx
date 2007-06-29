/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SDIFStreamingSource.hxx"
#include "SpectrumConfig.hxx"
#include "ErrOpenFile.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

using std::iterator;

namespace CLAM
{

SDIFStreamingSource::SDIFStreamingSource():
	mFundamentalOutput("Fundamental",this),
	mResidualSpectrumOutput("Residual Spectrum",this),
	mSpectralPeakArrayOutput("Sinusoidal Peaks",this),
	mPrevIndexArray(0)
{ 
	mpFile=NULL;
	mLastCenterTime=-1;
	Configure(SDIFInConfig());
}

SDIFStreamingSource::SDIFStreamingSource(const SDIFInConfig& c):
	mFundamentalOutput("Fundamental",this),
	mResidualSpectrumOutput("Residual Spectrum",this),
	mSpectralPeakArrayOutput("Sinusoidal Peaks",this),
	mPrevIndexArray(0)
{ 
	mpFile=NULL;
	mLastCenterTime=-1;

	Configure(c);
}
	
SDIFStreamingSource::~SDIFStreamingSource()
{
	delete mpFile;
}

bool SDIFStreamingSource::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	return true;
}

bool SDIFStreamingSource::ConcreteStart()
{

	if(mpFile) delete mpFile;
	mpFile = new SDIF::File(mConfig.GetFileName().c_str(),SDIF::File::eInput);

	try
	{
		mpFile->Open();
		return true;
	}
	catch ( ErrOpenFile& e )
	{
		AddConfigErrorMessage("Inner exception thrown: File could not be opened");
		AddConfigErrorMessage( e.what() );

		return false;
	}

	return true;
}

bool SDIFStreamingSource::ConcreteStop()
{
	mpFile->Close();

	return true;
}

const ProcessingConfig& SDIFStreamingSource::GetConfig() const
{
	return mConfig;
}

bool SDIFStreamingSource::LoadFrameOfSDIFData( CLAM::Fundamental& fundamental, CLAM::Spectrum& residualSpectrum, CLAM::SpectralPeakArray& spectralPeaks )
{
	if(!mpFile) return false;
	if(mpFile->Done()) return false;
	    
    double frameTimeTag;
    bool isInitialRun = true;	
    bool isFrameFinished = false;	
    while ( !isFrameFinished && !mpFile->Done() )
    {        
        SDIF::Frame tmpSDIFFrame;
        int currentFilePosition = mpFile->Pos();
        mpFile->Read(tmpSDIFFrame);
    
        frameTimeTag = tmpSDIFFrame.Time();
            
        //Residual Spectrum in frame should be configured to have the ComplexArray
        SpectrumConfig Scfg;
        SpecTypeFlags sflags;
        sflags.bComplex = 1;
        sflags.bMagPhase = 0;
        residualSpectrum.SetType(sflags);
                
        //std::cout << "Last center time: " << mLastCenterTime << ", Frame Time: " << frameTimeTag << "." << std::endl; 
        if (isInitialRun)
        {
            mLastCenterTime=frameTimeTag;
            isInitialRun = false;            
        }
        else if (frameTimeTag != mLastCenterTime)	// new SpectralFrame, need to add it to segment
        {
            // push the file position back to where it was before we began reading the next frame
            mpFile->Pos(currentFilePosition);
            isFrameFinished = true;
            break;
        }
            
        SDIF::Frame::MatrixIterator frameIt = tmpSDIFFrame.Begin();
    
        //SDIF::Matrix* pMatrix = tmpSDIFFrame.mpFirst;
        
        SDIF::ConcreteMatrix<TFloat32>* pMatrix=
            dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);
    
        /* its a fundamental frequency ..*/
        if (tmpSDIFFrame.Type()=="1FQ0" && mConfig.GetEnableFundFreq())
        {
            fundamental.AddElem(pMatrix->GetValue(0,0));
        }	
        
        /* it is residual data ..*/
        else if(tmpSDIFFrame.Type()=="1STF" && mConfig.GetEnableResidual())	// we use always the first 2 matrices
        {
            CLAM_ASSERT(pMatrix->Type() == "ISTF","SDIFStreamingSource::Add ISTF Header in Matrix expected");
    
            // The sampling rate used to be set in the segment
            //int samplingRate = pMatrix->GetValue( 0, 0 );
            
            residualSpectrum.SetSpectralRange(pMatrix->GetValue(0,0)*0.5);
            
            // move pointer to next matrix in frame
            frameIt++;
            pMatrix=
                dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);
            //pMatrix=pMatrix->mpNext;	
            
            //std::cout << "Setting residual size: " << pMatrix->Rows() << std::endl;
            
            CLAM_ASSERT(pMatrix->Type() =="1STF","SDIFStreamingSource::Add 1STF Headerin Matrix expected");
            residualSpectrum.SetSize(pMatrix->Rows());
            Array<Complex>& complexBuffer=residualSpectrum.GetComplexArray();
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
                    
            spectralPeaks.AddAll();
            spectralPeaks.UpdateData();
            SpectralPeakArray& tmpPeakArray=spectralPeaks;
            tmpPeakArray.SetScale(EScale::eLinear);
    
            tmpPeakArray.SetnMaxPeaks(nElems); //number of peaks in the sdif file
            tmpPeakArray.SetnPeaks(nElems); //number of peaks in the sdif file
            tmpPeakArray.ResetIndices();		// resets all indeces, make valid..
                            
            // read file data into SpectralPeakArray 
            DataArray& pkfreqBuffer=tmpPeakArray.GetFreqBuffer();
            DataArray& pkmagBuffer=tmpPeakArray.GetMagBuffer();
            DataArray& pkPhaseBuffer=tmpPeakArray.GetPhaseBuffer();
            DataArray& pkBinPosBuffer=tmpPeakArray.GetBinPosBuffer();
            DataArray& pkBinWidthBuffer=tmpPeakArray.GetBinWidthBuffer();
            IndexArray& pkIndexArray=tmpPeakArray.GetIndexArray();

            // BEGIN CHANGES
            /*
            spectralPeaks.SetScale(EScale::eLinear);
    
            spectralPeaks.SetnMaxPeaks(nElems); //number of peaks in the sdif file
            spectralPeaks.SetnPeaks(nElems); //number of peaks in the sdif file
            spectralPeaks.ResetIndices();		// resets all indeces, make valid..
                            
            // read file data into SpectralPeakArray 
            DataArray& pkfreqBuffer=spectralPeaks.GetFreqBuffer();
            DataArray& pkmagBuffer=spectralPeaks.GetMagBuffer();
            DataArray& pkPhaseBuffer=spectralPeaks.GetPhaseBuffer();
            DataArray& pkBinPosBuffer=spectralPeaks.GetBinPosBuffer();
            DataArray& pkBinWidthBuffer=spectralPeaks.GetBinWidthBuffer();
            IndexArray& pkIndexArray=spectralPeaks.GetIndexArray();
            */
            // END CHANGES
            
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
                bool    bIndexFound=false;
                
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
    }
        
	return true;	
}

bool SDIFStreamingSource::Do( CLAM::Fundamental& fundamental, CLAM::Spectrum& residualSpectrum, CLAM::SpectralPeakArray& spectralPeaks )
{
	//TODO uncomment: bool thereIsMoreData = false;
	
//TODO	while( ( thereIsMoreData = LoadSDIFDataIntoSegment( segment ) ) );
	bool response = LoadFrameOfSDIFData( fundamental, residualSpectrum, spectralPeaks );

    //std::cout << "Size of Residual Spectrum: " << residualSpectrum.GetSize() << std::endl;    
    
	return response;
}

bool SDIFStreamingSource::Do(void)
{    
	bool result = LoadFrameOfSDIFData( mFundamentalOutput.GetData(), mResidualSpectrumOutput.GetData(), mSpectralPeakArrayOutput.GetData() );
	//mOutput.Produce();
    //std::cout << "Size of mResidualSpectrumOutput.GetData(): " << mResidualSpectrumOutput.GetData().GetSize() << std::endl;    
	
    mFundamentalOutput.Produce();
	mResidualSpectrumOutput.Produce();
	mSpectralPeakArrayOutput.Produce();

	return result;
}

} // namespace CLAM

