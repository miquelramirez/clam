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

#include "SDIFFileReader.hxx"
#include "SpectrumConfig.hxx"
#include "ErrOpenFile.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

namespace CLAM
{

SDIFFileReader::SDIFFileReader():
	mPrevIndexArray(0), isFileOpen(false)
{
	mpFile=NULL;
	mLastCenterTime=-1;
	Configure(SDIFInConfig());
}

SDIFFileReader::SDIFFileReader(const SDIFInConfig& c):
	mPrevIndexArray(0), isFileOpen(false)
{
	mpFile=NULL;
	mLastCenterTime=-1;

	Configure(c);
}

SDIFFileReader::~SDIFFileReader()
{
	if (mpFile != NULL)
		mpFile->Close();
	delete mpFile;
}

bool SDIFFileReader::Configure(const SDIFInConfig& c)
{
	mConfig = c;
	
	return true;
}

bool SDIFFileReader::OpenFile()
{
	if(mpFile) delete mpFile;
	mpFile = new SDIF::File(mConfig.GetFileName().c_str(),SDIF::File::eInput);

	try
	{
		mpFile->Open();
		isFileOpen = true;
		return true;
	}
	catch ( ErrOpenFile& e )
	{
		std::cerr << "Inner exception thrown: File <" << mConfig.GetFileName().c_str() << "> could not be opened" << std::endl;

		return false;
	}

	return true;
}

const ProcessingConfig& SDIFFileReader::GetConfig() const
{
	return mConfig;
}

bool SDIFFileReader::ReadFrame(	CLAM::Fundamental& argFundamental,
								CLAM::SpectralPeakArray& argSpectralPeaks,
								CLAM::Spectrum& argResidual)
{
	TTime throwAwayFloat;
	return SDIFFileReader::ReadFrame(	argFundamental,
									argSpectralPeaks,
									argResidual,
									throwAwayFloat);
}

bool SDIFFileReader::ReadFrame(	CLAM::Fundamental& argFundamental,
								CLAM::SpectralPeakArray& argSpectralPeaks,
								CLAM::Spectrum& argResidual,
								TTime& argFrameCenterTime)
{
	if (!isFileOpen) OpenFile();
	if(!mpFile) return false;
	if(mpFile->Done())
	{
		mpFile->Close();
		return false;
	}

	//Residual Spectrum in frame should be configured to have the ComplexArray
	SpectrumConfig Scfg;
	SpecTypeFlags sflags;
	sflags.bComplex = 1;
	sflags.bMagPhase = 0;
	argResidual.SetType(sflags);

	double frameTimeTag;
	int counter = 0;
	// for most files, we will need to call mpFile->Read() once for each of its parts. if
	// the SDIF file has a sinusoidal, residual, and fundamental part, we loop here three
	// times, but not all files have all three parts.
	// for most frames we will leave this loop when the condition frameTimeTag != mLastCenterTime
	// obtains. (Look 20 lines down.)
	// !mpFile->Done() obtains only for the last frame in the file.
	while ( !mpFile->Done() )
	{
		SDIF::Frame tmpSDIFFrame;
		int currentFilePosition = mpFile->Pos();
		mpFile->Read(tmpSDIFFrame);

		frameTimeTag = tmpSDIFFrame.Time();

		// if this is the first iteration then update the mLastCenterTime variable with the
		// the center time of the current frame
		if (counter == 0)
		{
			mLastCenterTime = frameTimeTag;
			argFrameCenterTime = frameTimeTag;
		}
		// check to make sure that the frame's center
		// time hasn't changed. if it has, then we're reading from a new frame, and we need
		// to return the values from the last frame before we start processing this new frame
		else if (frameTimeTag != mLastCenterTime)	// new SpectralFrame, need to add it to segment
		{
			// we've reached the next frame of data.
			// push the file position back to where it was before we began reading the next frame
			// AND BREAK HERE IN ORDER TO RETURN THE LAST FRAME.
			mpFile->Pos(currentFilePosition);
			break;
		}

		CopyFramesDataObjects( tmpSDIFFrame, argFundamental, argSpectralPeaks, argResidual );

		counter++;
	}
	
	return true;

}

void SDIFFileReader::CopyFramesDataObjects(SDIF::Frame& tmpSDIFFrame, CLAM::Fundamental& argFundamental,
											CLAM::SpectralPeakArray& argSpectralPeaks, CLAM::Spectrum& argResidual)
{
	SDIF::Frame::MatrixIterator frameIt = tmpSDIFFrame.Begin();

	//SDIF::Matrix* pMatrix = tmpSDIFFrame.mpFirst;

	SDIF::ConcreteMatrix<TFloat32>* pMatrix=
		dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);

	/* its a fundamental frequency ..*/
	if (tmpSDIFFrame.Type()=="1FQ0" && mConfig.GetEnableFundFreq())
	{
		argFundamental.AddElem(pMatrix->GetValue(0,0));
	}
	/* it is residual data ..*/
	else if(tmpSDIFFrame.Type()=="1STF" && mConfig.GetEnableResidual())	// we use always the first 2 matrices
	{
		CLAM_ASSERT(pMatrix->Type() == "ISTF","SDIFIn::Add ISTF Header in Matrix expected");

		// these lines are important. all objects need to be added to the residual
		// now or later it will not be able to convert from complex format to a
		// phase / magnitude representation
		argResidual.AddAll();
		argResidual.UpdateData();

		// MRJ: We set the sampling rate for the segment
		//segment.SetSamplingRate( pMatrix->GetValue( 0, 0 ) );
		mSamplingRate = pMatrix->GetValue( 0, 0 );

		argResidual.SetSpectralRange(pMatrix->GetValue(0,0)*0.5);

		// move pointer to next matrix in frame
		frameIt++;
		pMatrix= dynamic_cast< SDIF::ConcreteMatrix<TFloat32>* >(*frameIt);
		//pMatrix=pMatrix->mpNext;

		CLAM_ASSERT(pMatrix->Type() =="1STF","SDIFIn::Add 1STF Headerin Matrix expected");
		argResidual.SetSize(pMatrix->Rows());
		Array<Complex>& complexBuffer=argResidual.GetComplexArray();
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

		argSpectralPeaks.AddAll();
		argSpectralPeaks.UpdateData();
		SpectralPeakArray& tmpPeakArray = argSpectralPeaks;
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
}

int SDIFFileReader::GetReadPosition()
{
	return mpFile->Pos();
}

void SDIFFileReader::SetReadPosition(int readPosition)
{
	mpFile->Pos(readPosition);
}

int SDIFFileReader::GetSamplingRate()
{
	return mSamplingRate;
}

int SDIFFileReader::GetFrameCenterTime()
{
	return mLastCenterTime;
}

} // namespace CLAM

