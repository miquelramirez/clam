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

#include "Complex.hxx"
#include "SpecTypeFlags.hxx"
#include "FrameInterpolator.hxx"
#include "ErrProcessingObj.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	void FrameInterpConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}


	FrameInterpolator::FrameInterpolator()
		: mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this,&FrameInterpolator::DoMagFactorControl),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this,&FrameInterpolator::DoFreqFactorControl),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this,&FrameInterpolator::DoPitchFactorControl),
		  mResidualInterpolationFactorCtl("ResidualInterpolationFactor",this,&FrameInterpolator::DoResidualFactorControl),
		  mFrameInterpolationFactorCtl("FrameInterpolationFactor",this,&FrameInterpolator::DoFrameFactorControl),
		  mIsHarmonicCtl("IsHarmonic",this,&FrameInterpolator::DoHarmonicControl),
		  mPitch1Ctl("Pitch1",this,&FrameInterpolator::DoPitch1Control),
		  mPitch2Ctl("Pitch2",this,&FrameInterpolator::DoPitch2Control)
	{
		Configure(FrameInterpConfig());
	}

	FrameInterpolator::FrameInterpolator(const FrameInterpConfig &c)
		: mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this,&FrameInterpolator::DoMagFactorControl),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this,&FrameInterpolator::DoFreqFactorControl),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this,&FrameInterpolator::DoPitchFactorControl),
		  mResidualInterpolationFactorCtl("ResidualInterpolationFactor",this,&FrameInterpolator::DoResidualFactorControl),
		  mFrameInterpolationFactorCtl("FrameInterpolationFactor",this,&FrameInterpolator::DoFrameFactorControl),
		  mIsHarmonicCtl("IsHarmonic",this,&FrameInterpolator::DoHarmonicControl),
		  mPitch1Ctl("Pitch1",this,&FrameInterpolator::DoPitch1Control),
		  mPitch2Ctl("Pitch2",this,&FrameInterpolator::DoPitch2Control)
	{
		Configure(c);
	}


	bool FrameInterpolator::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);

		//Configure children processing
		PeaksInterpConfig pkInterpConfig;
		pkInterpConfig.SetMagInterpolationFactor(mConfig.GetMagInterpolationFactor());
		pkInterpConfig.SetFreqInterpolationFactor(mConfig.GetFreqInterpolationFactor());
		pkInterpConfig.SetPitchInterpolationFactor(mConfig.GetPitchInterpolationFactor());
		pkInterpConfig.SetHarmonic(mConfig.GetHarmonic());

		mPO_PeaksInterpolator.Configure(pkInterpConfig);

		SpecInterpConfig spInterpConfig;
		spInterpConfig.SetInterpolationFactor(mConfig.GetResidualInterpolationFactor());

		mPO_SpectrumInterpolator.Configure(spInterpConfig);

		//Initialize interpolation factor control from value in the configuration
		mMagInterpolationFactorCtl.DoControl(mConfig.GetMagInterpolationFactor());
		mFreqInterpolationFactorCtl.DoControl(mConfig.GetFreqInterpolationFactor());
		mPitchInterpolationFactorCtl.DoControl(mConfig.GetPitchInterpolationFactor());
		mResidualInterpolationFactorCtl.DoControl(mConfig.GetResidualInterpolationFactor());
		mIsHarmonicCtl.DoControl(mConfig.GetHarmonic());

		return true;
	}

	bool FrameInterpolator::ConcreteStart()
	{
		mPO_SpectrumInterpolator.Start();
		mPO_PeaksInterpolator.Start();
		return true;
	}

	// Unsupervised Do() function.
	bool FrameInterpolator::Do(const Frame& in1, const Frame& in2, Frame& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "FrameInterpolator::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		if(in1.GetFundamentalFreq()!=0 && in2.GetFundamentalFreq()!=0 )
			mIsHarmonicCtl.DoControl(1);
		else mIsHarmonicCtl.DoControl(0);

		mPitch1Ctl.DoControl(in1.GetFundamentalFreq());
		mPitch2Ctl.DoControl(in2.GetFundamentalFreq());

		TData newPitch=mPitch1Ctl.GetLastValue()*(1-mPitchInterpolationFactorCtl.GetLastValue())+mPitch2Ctl.GetLastValue()*mPitchInterpolationFactorCtl.GetLastValue();
		if(!mIsHarmonicCtl.GetLastValue()) newPitch=0;
		//Sets new fund freq
		
		if(out.GetFundamental().GetnCandidates()==0)
			out.GetFundamental().AddElem(newPitch,0);
		else
			out.GetFundamental().SetFreq(0,newPitch);
		out.GetFundamental().SetnCandidates(1);

		mPO_PeaksInterpolator.Do(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),out.GetSpectralPeakArray());
		mPO_SpectrumInterpolator.Do(in1.GetResidualSpec(),in2.GetResidualSpec(),out.GetResidualSpec());

		return true;
	}

	bool FrameInterpolator::Do(void)
	{
		throw(ErrProcessingObj("FrameInterpolator::Do(): Not implemented"),this);
	}

	
	int FrameInterpolator::DoFrameFactorControl(TData value)
	{
		mMagInterpolationFactorCtl.DoControl(value);
		mFreqInterpolationFactorCtl.DoControl(value);
		mPitchInterpolationFactorCtl.DoControl(value);
		mResidualInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoMagFactorControl(TData value)
	{
		mPO_PeaksInterpolator.mMagInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoFreqFactorControl(TData value)
	{
		mPO_PeaksInterpolator.mFreqInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoPitchFactorControl(TData value)
	{
		mPO_PeaksInterpolator.mPitchInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoResidualFactorControl(TData value)
	{
		mPO_SpectrumInterpolator.mInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoPitch1Control(TData value)
	{
		mPO_PeaksInterpolator.mPitch1Ctl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoPitch2Control(TData value)
	{
		mPO_PeaksInterpolator.mPitch2Ctl.DoControl(value);
		return 0;
	}

	int FrameInterpolator::DoHarmonicControl(TData value)
	{
		mPO_PeaksInterpolator.mIsHarmonicCtl.DoControl(value);
		return 0;
	}



}