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
#include "Factory.hxx"

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
	mInput2("Input2",this,1)
{
		mHaveInternalSegment=false;
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
	mInput2("Input2",this,1)
{
	mHaveInternalSegment=false;
	Configure(c);
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig=dynamic_cast<const SMSMorphConfig&>(c);
	mHaveInternalSegment=false;

	if(mConfig.HasFileName())
	{
		if(LoadSDIF(mConfig.GetFileName(),mSegment))
		{
			mInput2.Attach(mSegment);
			mHaveInternalSegment=true;
		}
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
//	if(mConfig.HasHybSinSpectralShape())
//		mHarmSpectralShapeMorph=true;
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

bool SMSMorph::ConcreteStart()
{
	mPO_FrameInterpolator.Start();
	return true;
}

bool SMSMorph::InterpolateFrames(const Frame& in1,const Frame& in2, Frame& out, TData frameFactor=-1)
{
	TData magFactor,freqFactor,pitchFactor,resFactor;
	
	if(frameFactor==-1)//No Frame Interpolation
	{
		frameFactor=mHybBPF.GetLastValue();
		magFactor=mHybSinAmp.GetLastValue();
		freqFactor=mHybSinFreq.GetLastValue();
		pitchFactor=mHybPitch.GetLastValue();
	}
	else
	{
		magFactor=freqFactor=pitchFactor=frameFactor;
	}
	
	resFactor=mHybResAmp.GetLastValue();

	mPO_FrameInterpolator.mMagInterpolationFactorCtl.DoControl(magFactor);
	mPO_FrameInterpolator.mFreqInterpolationFactorCtl.DoControl(freqFactor);
	mPO_FrameInterpolator.mPitchInterpolationFactorCtl.DoControl(pitchFactor);
	mPO_FrameInterpolator.mResidualInterpolationFactorCtl.DoControl(resFactor);

	mPO_FrameInterpolator.Do(in1,in2,out);
	
	return true;
}

bool SMSMorph::Do(const Frame& in1, Frame& out)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	Frame tempFrame2;

	//With Frame Interpolation
	if(mConfig.GetInterpolateFrame())
	{
		FindInterpolatedFrameFromSegment2Morph(tempFrame2);
		//Morphing
		InterpolateFrames(in1,tempFrame2,out);
	}
	//Without Frame Interpolation
	else
	{
		InterpolateFrames(in1,mInput2.GetData().GetFrame(int(synchroTimeFactor)),out);
	}
					
	return true;


}

bool SMSMorph::FindInterpolatedFrameFromSegment2Morph(Frame& interpolatedFrame)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	//Initializes interpolated frame 
	interpolatedFrame=mInput2.GetData().GetFrame(mInput2.GetData().mCurrentFrameIndex);
	//Interpolation data
	int frameNo1=floor(synchroTimeFactor);
	int frameNo2=ceil(synchroTimeFactor);
	
	if(mInput2.GetData().GetFrame(frameNo1).GetFundamentalFreq()!=0 && mInput2.GetData().GetFrame(frameNo2).GetFundamentalFreq()!=0 ) 
		mHarmonicMorph=true;

	//Interpolating
	TData frameFactor=synchroTimeFactor-frameNo1;
	return InterpolateFrames(mInput2.GetData().GetFrame(frameNo1) , mInput2.GetData().GetFrame(frameNo2) , interpolatedFrame, frameFactor);			
}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	if(!mHaveInternalSegment) return false;
	
	TSize nFrames=in1.GetnFrames();
	int currentFrameIndex=in1.mCurrentFrameIndex;

	UpdateControlValueFromBPF((TData)currentFrameIndex/nFrames);
	mHarmonicMorph=false;
	
	return Do(in1.GetFrame(currentFrameIndex),out.GetFrame(currentFrameIndex));
}

bool SMSMorph::Do(const Segment& in1,Segment& in2, Segment& out)
{
	mInput2.Attach(in2);
	return Do(in1,out);
}

bool SMSMorph::LoadSDIF( std::string fileName, Segment& segment )
{
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks( 100 );
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );
	if(!mSDIFReader.Configure( cfg )) return false;//wrong filename or non-existing sdif
		
	segment.AddAll(  );
	segment.UpdateData(  );
	mSDIFReader.Output.Attach( segment );

	try{
		mSDIFReader.Start(  );}
	catch (Err)
	{
		return false;//wrong filename or non-existing sdif
	}
	while( mSDIFReader.Do() ) {  }
	mSDIFReader.Stop(  );
	
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
	if(mConfig.HasSynchronizeTime() && mConfig.GetSynchronizeTime().Size() )
		mSynchronizeTime.DoControl(mConfig.GetSynchronizeTime().GetValue(pos));

	if(mConfig.HasHybSinAmp() && mConfig.GetHybSinAmp().Size())
		mHybSinAmp.DoControl(mConfig.GetHybSinAmp().GetValue(pos));

	if(mConfig.HasHybSinSpectralShape() && mConfig.GetHybSinSpectralShape().Size())
		mHybSinSpectralShape.DoControl(mConfig.GetHybSinSpectralShape().GetValue(pos));

	if(mConfig.HasHybSinShapeW1() && mConfig.GetHybSinShapeW1().Size())
		mHybSinShapeW1.DoControl(mConfig.GetHybSinShapeW1().GetValue(pos));

	if(mConfig.HasHybSinShapeW2() && mConfig.GetHybSinShapeW2().Size())
		mHybSinShapeW2.DoControl(mConfig.GetHybSinShapeW2().GetValue(pos));	

	if(mConfig.HasHybPitch() && mConfig.GetHybPitch().Size() )
		mHybPitch.DoControl(mConfig.GetHybPitch().GetValue(pos));

	if(mConfig.HasHybSinFreq() && mConfig.GetHybSinFreq().Size())
		mHybSinFreq.DoControl(mConfig.GetHybSinFreq().GetValue(pos));

	if(mConfig.HasHybResAmp() && mConfig.GetHybResAmp().Size() )
		mHybResAmp.DoControl(mConfig.GetHybResAmp().GetValue(pos));

	if(mConfig.HasHybResSpectralShape() && mConfig.GetHybResSpectralShape().Size())
		mHybResSpectralShape.DoControl(mConfig.GetHybResSpectralShape().GetValue(pos));

	if(mConfig.HasHybResShapeW1() && mConfig.GetHybResShapeW1().Size())
		mHybResShapeW.DoControl(mConfig.GetHybResShapeW1().GetValue(pos));

	if(mConfig.HasHybResPhase() && mConfig.GetHybResPhase().Size())
		mHybResPhase.DoControl(mConfig.GetHybResPhase().GetValue(pos));

	return ret;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSMorph> regtSMSMorph( "SMSMorph" );
