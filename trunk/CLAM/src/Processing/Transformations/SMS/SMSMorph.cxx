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
#include "SpectrumConfig.hxx"

using namespace CLAM;


SMSMorph::SMSMorph():
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
	mHybResShapeW1("ResShapeW1", this),
	mHybResShapeW2("ResShapeW2", this),
	mInput2("Input2",this,1)
	
{
		mHaveInternalSegment=false;
		mUseTemporalBPF = true;
		mUseSinSpectralShape=false;
		mUseGlobalFactor=false;
		mUseSynchronizeTime=false;
		mUseSinAmp=false;
		mUsePitch=false;
		mUseSinFreq=false;
		mUseResAmp=false;
		mUseResSpectralShape=false;
		mUseSinSpectralShape=false;
}

SMSMorph::SMSMorph(const SMSMorphConfig &c):
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
	mHybResShapeW1("ResShapeW1", this),
	mHybResShapeW2("ResShapeW2", this),
	mInput2("Input2",this,1)	

{
	mHaveInternalSegment=false;
	mUseTemporalBPF = true;

	mUseSinSpectralShape=false;
	mUseGlobalFactor=false;
	mUseSynchronizeTime=false;
	mUseSinAmp=false;
	mUsePitch=false;
	mUseSinFreq=false;
	mUseResAmp=false;
	mUseResSpectralShape=false;
	mUseSinSpectralShape=false;
	Configure(c);
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& c) 
{
	CopyAsConcreteConfig(mConfig,c);
	mHaveInternalSegment=false;
	
	if(mConfig.HasFileName())
	{
		if(LoadSDIF(mConfig.GetFileName(),mSegment))
		{
			mInput2.Attach(mSegment);
			mHaveInternalSegment=true;
		}
	}

	SpecTypeFlags type;
	SpectrumConfig cfg;
	type.bMagPhase=false;
	type.bMagPhaseBPF=true;
	cfg.SetType(type);
	cfg.SetSpectralRange(mSegment.GetFrame(0).GetResidualSpec().GetSpectralRange());

	mSpectralShape.Configure(cfg);
	mResSpectralShape.Configure(cfg);
	
	InitializeFactorsToUse();

	FrameInterpConfig frIntCfg;

	if(mUseSinSpectralShape)
	{
		frIntCfg.SetUseSpectralShape(true);
		mPO_FrameInterpolator.mSpectralShape.Attach(mSpectralShape);
	}

	CLAM_ASSERT( mPO_FrameInterpolator.Configure(frIntCfg),
				 "Failed to configure Frame interpolator in SMSMorph::ConcreteConfigure" );
	


	return UpdateControlValueFromBPF(0);
}

bool SMSMorph::ConcreteStart()
{
	mPO_FrameInterpolator.Start();
	return true;
}

void SMSMorph::UpdateFrameInterpolatorFactors(bool useFrameFactor=false)
{
	if(useFrameFactor)
	{
		mPO_FrameInterpolator.mFrameInterpolationFactorCtl.DoControl(mHybBPF.GetLastValue());
	}
	else//No Global Factor
	{
		mPO_FrameInterpolator.mMagInterpolationFactorCtl.DoControl(mHybSinAmp.GetLastValue());
		mPO_FrameInterpolator.mFreqInterpolationFactorCtl.DoControl(mHybSinFreq.GetLastValue());
		mPO_FrameInterpolator.mPitchInterpolationFactorCtl.DoControl(mHybPitch.GetLastValue());
		mPO_FrameInterpolator.mResidualInterpolationFactorCtl.DoControl(mHybResAmp.GetLastValue());
	}
}

bool SMSMorph::Do(const Frame& in1, Frame& out)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	if(mSynchronizeTime.GetLastValue()<0.0001||mSynchronizeTime.GetLastValue()>0.9999)
	{
		//it means we are at the boudaries of segment to morph
		out=in1;

		return true;
	}

	Frame tempFrame2;

	//With Frame Interpolation
	if(mConfig.GetInterpolateFrame())
	{
		FindInterpolatedFrameFromSegment2Morph(tempFrame2);
		//Morphing
		UpdateFrameInterpolatorFactors();
		mPO_FrameInterpolator.Do(in1,tempFrame2,out);

	}
	//Without Frame Interpolation
	else
	{
		UpdateFrameInterpolatorFactors();
		mPO_FrameInterpolator.Do(in1,mInput2.GetData().GetFrame(int(synchroTimeFactor)),out);

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
	
	//Interpolating
	TData frameFactor=synchroTimeFactor-frameNo1;
	mHybBPF.DoControl(frameFactor);
	UpdateFrameInterpolatorFactors(true);
	return mPO_FrameInterpolator.Do(mInput2.GetData().GetFrame(frameNo1) , mInput2.GetData().GetFrame(frameNo2) , interpolatedFrame);			
}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	if(!mHaveInternalSegment) return false;
	return SMSTransformationTmpl<Frame>::Do(in1,out);
}

bool SMSMorph::Do(const Segment& in1,Segment& in2, Segment& out)
{
	mInput2.Attach(in2);
	return Do(in1,out);
}

bool SMSMorph::UpdateControlValueFromBPF(TData pos)
{
	bool ret=true;

	TData globalFactor;
	
	//Warning, maybe controls that are not used should be initialize to something sensible (does not affect)
	
	if(mUseGlobalFactor)
	{
		globalFactor=mConfig.GetHybBPF().GetValue(pos);
		mHybBPF.DoControl(globalFactor);
	}
	else
		ret=false;
	if(mUseSynchronizeTime)
	{
		pos=mConfig.GetSynchronizeTime().GetValue(pos);
		mSynchronizeTime.DoControl(pos);
	}

	if(mUseSinAmp)
		mHybSinAmp.DoControl(mConfig.GetHybSinAmp().GetValue(pos));
	else
		mHybSinAmp.DoControl(globalFactor);

	if(mUsePitch)
		mHybPitch.DoControl(mConfig.GetHybPitch().GetValue(pos));
	else
		mHybPitch.DoControl(globalFactor);

	if(mUseSinFreq)
		mHybSinFreq.DoControl(mConfig.GetHybSinFreq().GetValue(pos));
	else
		mHybSinFreq.DoControl(globalFactor);

	if(mUseResAmp)
		mHybResAmp.DoControl(mConfig.GetHybResAmp().GetValue(pos));
	else
		mHybResAmp.DoControl(globalFactor);

	//Updating spectral shapes
	if(mUseResSpectralShape)
	{
		mHybResSpectralShape.DoControl(mConfig.GetHybResSpectralShape().GetValue(pos));
		UpdateSpectralShape(mConfig.GetHybResShapeW1(),mConfig.GetHybResShapeW2(),mHybResSpectralShape.GetLastValue(),mResSpectralShape);
	}
	if(mUseSinSpectralShape)
	{
		mHybSinSpectralShape.DoControl(mConfig.GetHybSinSpectralShape().GetValue(pos));
		UpdateSpectralShape(mConfig.GetHybSinShapeW1(),mConfig.GetHybSinShapeW2(),mHybSinSpectralShape.GetLastValue(),mSpectralShape);
	}

	return ret;
}

void SMSMorph::InitializeFactorsToUse()
{
	if(mConfig.HasHybBPF())
		mUseGlobalFactor=true;
	if(mConfig.HasSynchronizeTime() && mConfig.GetSynchronizeTime().Size() )
		mUseSynchronizeTime=true;
	if(mConfig.HasHybSinAmp() && mConfig.GetHybSinAmp().Size())
		mUseSinAmp=true;
	if(mConfig.HasHybPitch() && mConfig.GetHybPitch().Size() )
		mUsePitch=true;
	if(mConfig.HasHybSinFreq() && mConfig.GetHybSinFreq().Size())
		mUseSinFreq=true;
	if(mConfig.HasHybResAmp() && mConfig.GetHybResAmp().Size() )
		mUseResAmp=true;
	if(mConfig.HasHybResSpectralShape() && mConfig.GetHybResSpectralShape().Size()&&
		mConfig.HasHybResShapeW1() && mConfig.GetHybResShapeW1().Size()&&
		mConfig.HasHybResShapeW2() && mConfig.GetHybResShapeW2().Size())
	{
			mUseResSpectralShape=true;
			mUseResAmp=false;
	}
	//Updating spectral shapes
	if(mConfig.HasHybSinSpectralShape() && mConfig.GetHybSinSpectralShape().Size()&&
		mConfig.HasHybSinShapeW1() && mConfig.GetHybSinShapeW1().Size()&&
		mConfig.HasHybSinShapeW2() && mConfig.GetHybSinShapeW2().Size())
	{
		mUseSinSpectralShape=true;
		mUseSinAmp=false;
	}

}

void SMSMorph::UpdateSpectralShape(const BPF& weightBPF1, const BPF& weightBPF2,TData interpFactor, Spectrum& spectralShape)
{
	BPF spectralShapeBPF;
	TData spectralRange=spectralShape.GetSpectralRange();
	//we will always add as many points as possible, so we take weightBPF with maximum points
	int nPoints;
	bool usingFirst=false;
	if(weightBPF1.Size()>weightBPF2.Size())
	{
		nPoints=weightBPF1.Size();
		usingFirst=true;
	}
	else nPoints=weightBPF2.Size();
	int i;
	TData xValue,yValue;
	if(usingFirst)
	{
		for(i=0;i<nPoints;i++)
		{
			xValue=weightBPF1.GetXValue(i);
			yValue=weightBPF1.GetValueFromIndex(i)*interpFactor+weightBPF2.GetValue(xValue)*(1-interpFactor);
			xValue*=spectralRange;
			spectralShapeBPF.Insert(xValue,yValue);
		}
	}
	else
	{
		for(i=0;i<nPoints;i++)
		{
			xValue=weightBPF2.GetXValue(i);
			yValue=weightBPF2.GetValueFromIndex(i)*interpFactor+weightBPF1.GetValue(xValue)*(1-interpFactor);
			xValue*=spectralRange;
			spectralShapeBPF.Insert(xValue,yValue);
		}
	}
	spectralShape.SetMagBPF(spectralShapeBPF);
	spectralShape.SetSize(spectralShapeBPF.Size());

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

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSMorph> regtSMSMorph( "SMSMorph" );
