/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#include <iostream>
#include <fstream>
#include "SMSMorph.hxx"

using namespace CLAM;


SMSMorph::SMSMorph():
	mAmountCtrl("Amount",this),  
	mHybBPF("MorphFactor",this),
	mSynchronizeTime("Time", this),
	mHybSinAmp("SinAmp", this),
	mHybSinSpectralShape("SinShape", this),
	mHybSinShapeW1("SinShapeW1", this),
	mHybSinShapeW2("SinShapeW2", this),
	mHybPitch("Pitch", this),
	mHybSinFreq("SinFreq", this),
	mHybResAmp("ResAmp", this),
	mHybResSpectralShape("ResShape", this),
	mHybResShapeW("ResShapeW", this),
	mHybResPhase("ResPhase", this),
	mInput1("Input1",this,1),mInput2("Input2",this,1),mOutput("Output",this,1)
{
		mHaveInternalSegment=false;

		mFrameFactor=-1;
		mMagFactor=-1;
		mFreqFactor=-1;
		mPitchFactor=-1;
}

SMSMorph::SMSMorph(const SMSMorphConfig &c):
	mAmountCtrl("Amount",this),
	mHybBPF("MorphFactor",this),
	mSynchronizeTime("Time", this),
	mHybSinAmp("SinAmp", this),
	mHybSinSpectralShape("SinShape", this),
	mHybSinShapeW1("SinShapeW1", this),
	mHybSinShapeW2("SinShapeW2", this),
	mHybPitch("Pitch", this),
	mHybSinFreq("SinFreq", this),
	mHybResAmp("ResAmp", this),
	mHybResSpectralShape("ResShape", this),
	mHybResShapeW("ResShapeW", this),
	mHybResPhase("ResPhase", this),
	mInput1("Input1",this,1),mInput2("Input2",this,1),mOutput("Output",this,1)
{
	mHaveInternalSegment=false;
	mFrameFactor=-1;
	mMagFactor=-1;
	mFreqFactor=-1;
	mPitchFactor=-1;
	Configure(c);
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig=dynamic_cast<const SMSMorphConfig&>(c);
	mHaveInternalSegment=false;

	if(mConfig.HasFileName())
	{
		LoadSDIF(mConfig.GetFileName(),mSegment);
		mInput2.Attach(mSegment);
		mHaveInternalSegment=true;
	}
	
	if(!mConfig.HasHybBPF())
	{
		mConfig.AddHybBPF();
		mConfig.UpdateData();
		
		BPF tmpBPF(2);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,1);
		tmpBPF.SetXValue(1,1);
		mConfig.SetHybBPF(tmpBPF);
	}
	if(!mConfig.HasInterpolateFrame())
	{
		mConfig.AddInterpolateFrame();
		mConfig.UpdateData();
		mConfig.SetInterpolateFrame(false);
	}
	if(!mConfig.HasHybSinFreq() && mConfig.HasHybBPF())
	{
		mConfig.AddHybSinFreq();
		mConfig.UpdateData();
		mConfig.SetHybSinFreq(mConfig.GetHybBPF());
	}
	if(!mConfig.HasHybSinAmp() && mConfig.HasHybBPF())
	{
		mConfig.AddHybSinAmp();
		mConfig.UpdateData();
		mConfig.SetHybSinAmp(mConfig.GetHybBPF());
	}
	if(mConfig.HasHybSinSpectralShape())
		mHarmSpectralShapeMorph=true;
	if(!mConfig.HasHybPitch())
	{
		mConfig.AddHybPitch();
		mConfig.UpdateData();
		mConfig.SetHybPitch(mConfig.GetHybSinFreq());
	}
	if(!mConfig.HasHybResAmp())
	{
		mConfig.AddHybResAmp();
		mConfig.UpdateData();
		mConfig.SetHybResAmp(mConfig.GetHybBPF());
	}
	if(!mConfig.HasSynchronizeTime())
	{
		mConfig.AddSynchronizeTime();
		mConfig.UpdateData();
		BPF tmpBPF(2);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(1,1);
		tmpBPF.SetValue(1,1);
		mConfig.SetSynchronizeTime(tmpBPF);
	}
	
	return UpdateControlValueFromBPF(0);
}


void SMSMorph::UpdateMorphFactors()
{
	if(mFrameFactor==-1)//No Frame Interpolation
	{
		mFrameFactor=mHybBPF.GetLastValue();
		mMagFactor=mHybSinAmp.GetLastValue();
		mFreqFactor=mHybSinFreq.GetLastValue();
		mPitchFactor=mHybPitch.GetLastValue();
	}
	else
	{
		mMagFactor=mFreqFactor=mPitchFactor=mFrameFactor;
	}
	mResMagFactor=mHybResAmp.GetLastValue();
}

bool SMSMorph::SinusoidalMorph(const Frame& in1,const Frame& in2, Frame& out)
{
		///////////////////////
	/*Sinusoidal Morphing*/
	///////////////////////
	TData pitch1=in1.GetFundamentalFreq();
	TData pitch2=in2.GetFundamentalFreq();

	TData newPitch=pitch1*(1-mPitchFactor)+pitch2*mPitchFactor;

	InterpolateSpectralPeaks(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),out.GetSpectralPeakArray(),pitch1,pitch2);
	//Sets new fund freq
	if(mHarmonicMorph)
		out.SetFundamentalFreq(0,newPitch);
	else
		out.SetFundamentalFreq(0,0);

	return true;
}

bool SMSMorph::ResidualMorph(const Frame& in1,const Frame& in2, Frame& out)
{
	///////////////////////
	/**Residual Morphing**/
	///////////////////////
	
	/** Xavier: we cannot be sure that incoming spectrum is in MagPhase format.
	We will do it the "slow way" and try to optimize later (TODO)
	*/
	Spectrum &inRes1=in1.GetResidualSpec();
	Spectrum &inRes2=in2.GetResidualSpec();
	Spectrum &outRes=out.GetResidualSpec();
	
	TSize specSize=out.GetResidualSpec().GetSize();

	int i;
	for(i=0;i<specSize;i++)
		outRes.SetMag(i,inRes1.GetMag(i)*(1-mResMagFactor)+inRes2.GetMag(i)*mResMagFactor);
	if(mFrameFactor>0.5)
		for(i=0;i<specSize;i++)
			outRes.SetPhase(i,inRes2.GetPhase(i));
	return true;
}


bool SMSMorph::MorphFrames(const Frame& in1,const Frame& in2, Frame& out)
{
	if(in1.GetFundamentalFreq()!=0 && in2.GetFundamentalFreq()!=0 )
		mHarmonicMorph=true;
	else mHarmonicMorph=false;

	UpdateMorphFactors();
	SinusoidalMorph(in1,in2,out);
	ResidualMorph(in1,in2,out);
	
	return true;
}


bool SMSMorph::Do()
{
	return Do(mInput1.GetData(),mInput2.GetData(),mOutput.GetData());

}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	CLAM_ASSERT(mHaveInternalSegment, "SMSMorph::Do: you cannot call this overload if internal segment has not been previously lodade");
	return Do(in1,mInput2.GetData(),out);
}

bool SMSMorph::Do(const Segment& in1,const Segment& in2, Segment& out)
{
	int i=in1.mCurrentFrameIndex;
	
	TSize nFrames1=in1.GetnFrames();
	TSize nFrames2=in2.GetnFrames();
	TSize nFrames;
	nFrames=nFrames1;

	TData synchroTimeFactor; 

	UpdateControlValueFromBPF((TData)i/nFrames);
	synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	mHarmonicMorph=false;
	
	//With Frame Interpolation
	if(mConfig.GetInterpolateFrame())
	{
		//Initializes interpolated frame 
		Frame tempFrame2=in1.GetFrame(i);
		//Interpolation data
		int frameNo1=floor(synchroTimeFactor);
		int frameNo2=ceil(synchroTimeFactor);
		
		if(in2.GetFrame(frameNo1).GetFundamentalFreq()!=0 && in2.GetFrame(frameNo2).GetFundamentalFreq()!=0 ) 
			mHarmonicMorph=true;

		//Interpolating
		mFrameFactor=synchroTimeFactor-frameNo1;
		MorphFrames(in2.GetFrame(frameNo1) , in2.GetFrame(frameNo2) , tempFrame2);			
	
		//Morphing
		const Frame& frame1=in1.GetFrame(i);
		mFrameFactor=-1; //we don't want to interpolate
		MorphFrames(frame1,tempFrame2,out.GetFrame(i));
	}
	//Without Frame Interpolation
	else
	{
		const Frame& frame1=in1.GetFrame(i);
		const Frame& frame2=in2.GetFrame(int(synchroTimeFactor));
		mFrameFactor=-1; //we don't want to interpolate
		MorphFrames(frame1,frame2,out.GetFrame(i));
	}
					
	return true;
}


bool SMSMorph::FindHarmonic(const IndexArray& indexArray,int index,int& lastPosition)
{
	int i;
	bool found=false;
	int nPeaks=indexArray.Size();
	for(i=lastPosition;i<nPeaks;i++)
	{
		if(indexArray[i]==index)
		{
			lastPosition=i;
			found=true;
			break;
		}
	}
	return found;

}


bool SMSMorph::InterpolateSpectralPeaks(const SpectralPeakArray& in1,const SpectralPeakArray& in2, SpectralPeakArray& out,TData pitch1/*=0*/, TData pitch2/*=0*/)
{
	//we need to copy input peak arrays to convert them to linear
	SpectralPeakArray tmpIn1=in1;
	SpectralPeakArray tmpIn2=in2;
	tmpIn1.ToLinear();
	tmpIn2.ToLinear();

	//We initialize out with in1
	out=tmpIn1;
	
	int nPeaks1=in1.GetnPeaks();
	int nPeaks2=in2.GetnPeaks();

	if(nPeaks1==0)
	{
		return true;
	}
	if(nPeaks2==0)
	{
		out=in1;
		return true;
	}

	DataArray& in1Mag=tmpIn1.GetMagBuffer();
	DataArray& in2Mag=tmpIn2.GetMagBuffer();
	DataArray& outMag=out.GetMagBuffer();

	DataArray& in1Freq=tmpIn1.GetFreqBuffer();
	DataArray& in2Freq=tmpIn2.GetFreqBuffer();
	DataArray& outFreq=out.GetFreqBuffer();

	IndexArray& in1Index=tmpIn1.GetIndexArray();
	IndexArray& in2Index=tmpIn2.GetIndexArray();
	
	//TODO: this computation is duplicated
	TData newPitch=pitch1*(1-mPitchFactor)+pitch2*mPitchFactor;
	
	TData factor2=nPeaks2/nPeaks1;
	int pos=0,i=0;
	do
	{
		if(!mHarmonicMorph)
		{
			outMag[i]=in1Mag[i]*(1-mMagFactor)+in2Mag[i*factor2]*mMagFactor;
			outFreq[i]=in1Freq[i]*(1-mFreqFactor)+in2Freq[i*factor2]*mFreqFactor;
		}
		else if(FindHarmonic(in2Index,in1Index[i],pos))
		{
			//Morphing Using Harmonic No*/
			outMag[i]=in1Mag[i]*(1-mMagFactor)+in2Mag[pos]*mMagFactor;
			outFreq[i]=((in1Freq[i]/pitch1)*(1-mFreqFactor)+(in2Freq[pos]/pitch2)*mFreqFactor)*newPitch;
		}
		else
		{
			outMag[i]=0.000001;
		}
		i++;
	}while(i<nPeaks1);
	
	//Finally we convert output to dB
	out.TodB();

	return true;
}


bool SMSMorph::LoadSDIF( std::string fileName, Segment& segment )
{
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks( 100 );
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );
	mSDIFReader.Configure( cfg );
		
	segment.AddAll(  );
	segment.UpdateData(  );
	mSDIFReader.Output.Attach( segment );

	try{
		mSDIFReader.Start(  );
		while( mSDIFReader.Do() ) {  }
		mSDIFReader.Stop(  );
	} catch (Err e)
	{
		std::cout << e.what() << std::endl;
	}

	return true;
}


bool SMSMorph::UpdateControlValueFromBPF(TData pos)
{
	bool ret=true;


	if(mConfig.HasHybBPF())
	{
		mAmountCtrl.DoControl(mConfig.GetHybBPF().GetValue(pos));
		mHybBPF.DoControl(mConfig.GetHybBPF().GetValue(pos));
	}
	else
		ret=false;
	if(mConfig.HasSynchronizeTime())
		mSynchronizeTime.DoControl(mConfig.GetSynchronizeTime().GetValue(pos));
	if(mConfig.HasHybSinAmp())
		mHybSinAmp.DoControl(mConfig.GetHybSinAmp().GetValue(pos));
	if(mConfig.HasHybSinSpectralShape())
		mHybSinSpectralShape.DoControl(mConfig.GetHybSinSpectralShape().GetValue(pos));
	if(mConfig.HasHybSinShapeW1())
		mHybSinShapeW1.DoControl(mConfig.GetHybSinShapeW1().GetValue(pos));
	if(mConfig.HasHybSinShapeW2())
		mHybSinShapeW2.DoControl(mConfig.GetHybSinShapeW2().GetValue(pos));	
	if(mConfig.HasHybPitch())
		mHybPitch.DoControl(mConfig.GetHybPitch().GetValue(pos));
	if(mConfig.HasHybSinFreq())
		mHybSinFreq.DoControl(mConfig.GetHybSinFreq().GetValue(pos));
	if(mConfig.HasHybResAmp())
		mHybResAmp.DoControl(mConfig.GetHybResAmp().GetValue(pos));
	if(mConfig.HasHybResSpectralShape())
		mHybResSpectralShape.DoControl(mConfig.GetHybResSpectralShape().GetValue(pos));
	if(mConfig.HasHybResShapeW1())
		mHybResShapeW.DoControl(mConfig.GetHybResShapeW1().GetValue(pos));
	if(mConfig.HasHybResPhase())
		mHybResPhase.DoControl(mConfig.GetHybResPhase().GetValue(pos));
	return ret;
}