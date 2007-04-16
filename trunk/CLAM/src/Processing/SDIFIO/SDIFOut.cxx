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

#include "SDIFOut.hxx"
#include "ErrOpenFile.hxx"
#include "SpectrumConfig.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

using namespace CLAM;


void SDIFOutConfig::DefaultInit()
{
	AddAll();
	UpdateData();

	SetEnableResidual(true);
	SetEnablePeakArray(true);
	SetEnableFundFreq(true);
	SetSpectralRange(22050);
	SetMaxNumPeaks(100);
	SetFileName("nofile");

	SetSamplingRate(44100);
	SetFrameSize(1024);
	SetSpectrumSize(513);
}

SDIFOut::SDIFOut():
mPrevIndexArray(0)
{ 
	mpFile=NULL;
	Configure(SDIFOutConfig());
}

SDIFOut::SDIFOut(const SDIFOutConfig& c):
mPrevIndexArray(0)
{ 
	mpFile=NULL;

	Configure(c);
}
	
SDIFOut::~SDIFOut()
{
	mpFile->Close();
	delete mpFile;
}

bool SDIFOut::ConcreteStart()
{

	if(mpFile) delete mpFile;
	mpFile = new SDIF::File(mConfig.GetFileName().c_str(),SDIF::File::eOutput);

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

bool SDIFOut::ConcreteStop()
{
	mpFile->Close();

	return true;
}

bool SDIFOut::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	return true;
}


const ProcessingConfig& SDIFOut::GetConfig() const
{
	return mConfig;
}

bool SDIFOut::Do(const Frame& frame)
{
	if(!mpFile) return false;
	
//If enabled, first frame will contain fundamental frequency
	if(mConfig.GetEnableFundFreq())
	{
		SDIF::Frame tmpSDIFFrame("1FQ0",frame.GetCenterTime(),0);
		//Note: other Frame Header values could be set but are not available in segment data
		SDIF::ConcreteMatrix<TFloat32>* pMatrix;
				
		//First matrix to add to frame
		pMatrix=new SDIF::ConcreteMatrix<TFloat32>("1FQ0",1,1);

		//We add fundamental frequency
		pMatrix->SetValue(0,0,frame.GetFundamental().GetFreq());
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
//If enabled, second frame will contain residual spectrum
	if(mConfig.GetEnableResidual())
	{
		SDIF::Frame tmpSDIFFrame("1STF",frame.GetCenterTime(),1);
		
		SDIF::ConcreteMatrix<TFloat32>* pMatrix;
		//First matrix to add to frame
		pMatrix=new SDIF::ConcreteMatrix<TFloat32>("ISTF",1,3);

		pMatrix->SetValue(0,0,mConfig.GetSamplingRate());
		pMatrix->SetValue(0,1,mConfig.GetFrameSize());
		pMatrix->SetValue(0,2,mConfig.GetSpectrumSize());
		tmpSDIFFrame.Add(pMatrix);
		//Next matrix


		pMatrix=new SDIF::ConcreteMatrix<TFloat32>("1STF",frame.GetResidualSpec().GetSize(),2);

		//We have to convert residual spectrum to complex
  		SpectrumConfig Scfg;
  		SpecTypeFlags sflags;
		frame.GetResidualSpec().GetType(sflags);
		if(!sflags.bComplex )
		{
			sflags.bComplex = 1;
  			frame.GetResidualSpec().SetTypeSynchronize(sflags);
		}
		//SDIF only accepts linear data
		frame.GetResidualSpec().ToLinear();

		Array<Complex>& complexBuffer=frame.GetResidualSpec().GetComplexArray();
		for (int r=0;r<pMatrix->Rows();r++)	//Write in complex data
		{
			pMatrix->SetValue(r,0,complexBuffer[r].Real());
			pMatrix->SetValue(r,1,complexBuffer[r].Imag());
		}
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
//If enabled, third frame will contain sinusoidal spectral peaks
	if(mConfig.GetEnablePeakArray())
	{
		SpectralPeakArray& tmpPeakArray=frame.GetSpectralPeakArray();
		//SDIF only accepts linear data
		tmpPeakArray.ToLinear();

		SDIF::Frame tmpSDIFFrame("1TRC",frame.GetCenterTime(),2);

		int nElems=tmpPeakArray.GetnPeaks();

		SDIF::ConcreteMatrix<TFloat32>* pMatrix;

		//First matrix to add to frame
		pMatrix=new SDIF::ConcreteMatrix<TFloat32>("1TRC",nElems,4);
		DataArray& pkfreqBuffer=tmpPeakArray.GetFreqBuffer();
		DataArray& pkmagBuffer=tmpPeakArray.GetMagBuffer();
		DataArray& pkPhaseBuffer=tmpPeakArray.GetPhaseBuffer();
		//Unused variable: DataArray& pkBinPosBuffer=tmpPeakArray.GetBinPosBuffer();
		//Unused variable: DataArray& pkBinWidthBuffer=tmpPeakArray.GetBinWidthBuffer();
		IndexArray& pkIndexArray=tmpPeakArray.GetIndexArray();

		for (int r=0;r<nElems;r++)
		{
			//write track index
			pMatrix->SetValue(r,0,pkIndexArray[r]+1);	// +1 because SDIF doesnt allow Track 0
			// write frequency , mag and phase
			pMatrix->SetValue(r,1,pkfreqBuffer[r]);
			pMatrix->SetValue(r,2,pkmagBuffer[r]);
			pMatrix->SetValue(r,3,pkPhaseBuffer[r]);
			/* cannot store binpos and binwidth in SDIF?
			pkBinPosBuffer[r];
			pkBinWidthBuffer[r];*/
		}
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
	return true;
}

