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
#include "FrameAdder.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	void FrameInterpConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void FrameInterpConfig::DefaultValues()
	{
		SetMagInterpolationFactor( 0.0 );
		SetFreqInterpolationFactor( 0.0 );
		SetPitchInterpolationFactor( 0.0 );
		SetResidualInterpolationFactor( 0.0 );
		SetHarmonic( true );
		SetUseSpectralShape(false);
	}


	FrameAdder::FrameAdder()
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mpSpectralShape(0),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this,&FrameAdder::DoMagFactorControl),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this,&FrameAdder::DoFreqFactorControl),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this,&FrameAdder::DoPitchFactorControl),
		  mResidualInterpolationFactorCtl("ResidualInterpolationFactor",this,&FrameAdder::DoResidualFactorControl),
		  mFrameInterpolationFactorCtl("FrameInterpolationFactor",this,&FrameAdder::DoFrameFactorControl),
		  mIsHarmonicCtl("IsHarmonic",this,&FrameAdder::DoHarmonicControl),
		  mPitch1Ctl("Pitch1",this,&FrameAdder::DoPitch1Control),
		  mPitch2Ctl("Pitch2",this,&FrameAdder::DoPitch2Control)
	{
		AttachChildren();
		Configure(FrameInterpConfig());
	}

	FrameAdder::FrameAdder(const FrameInterpConfig &c)
		: mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mpSpectralShape(0),
		  mMagInterpolationFactorCtl("MagInterpolationFactor",this,&FrameAdder::DoMagFactorControl),
		  mFreqInterpolationFactorCtl("FreqInterpolationFactor",this,&FrameAdder::DoFreqFactorControl),
		  mPitchInterpolationFactorCtl("PitchInterpolationFactor",this,&FrameAdder::DoPitchFactorControl),
		  mResidualInterpolationFactorCtl("ResidualInterpolationFactor",this,&FrameAdder::DoResidualFactorControl),
		  mFrameInterpolationFactorCtl("FrameInterpolationFactor",this,&FrameAdder::DoFrameFactorControl),
		  mIsHarmonicCtl("IsHarmonic",this,&FrameAdder::DoHarmonicControl),
		  mPitch1Ctl("Pitch1",this,&FrameAdder::DoPitch1Control),
		  mPitch2Ctl("Pitch2",this,&FrameAdder::DoPitch2Control)
	{
		AttachChildren();
		Configure(c);
	}


	bool FrameAdder::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);

		//Configure children processing
		PeaksInterpConfig pkInterpConfig;
		pkInterpConfig.SetMagInterpolationFactor(mConfig.GetMagInterpolationFactor());
		pkInterpConfig.SetFreqInterpolationFactor(mConfig.GetFreqInterpolationFactor());
		pkInterpConfig.SetPitchInterpolationFactor(mConfig.GetPitchInterpolationFactor());
		pkInterpConfig.SetHarmonic(mConfig.GetHarmonic());
		pkInterpConfig.SetUseSpectralShape(mConfig.GetUseSpectralShape());
		mPO_PeaksAdder.Configure(pkInterpConfig);

		//todo: using Adder with ports is still not available!!
		if(mConfig.GetUseSpectralShape())
		{
			mPO_PeaksAdder.AttachSpectralShape(*mpSpectralShape);
		}

		SpecInterpConfig spInterpConfig;
		spInterpConfig.SetInterpolationFactor(mConfig.GetResidualInterpolationFactor());

		mPO_SpectrumAdder.Configure(spInterpConfig);

		//Initialize interpolation factor control from value in the configuration
		mMagInterpolationFactorCtl.DoControl(mConfig.GetMagInterpolationFactor());
		mFreqInterpolationFactorCtl.DoControl(mConfig.GetFreqInterpolationFactor());
		mPitchInterpolationFactorCtl.DoControl(mConfig.GetPitchInterpolationFactor());
		mResidualInterpolationFactorCtl.DoControl(mConfig.GetResidualInterpolationFactor());
		mIsHarmonicCtl.DoControl(mConfig.GetHarmonic());

		return true;
	}

	void FrameAdder::AttachChildren()
	{
		mPO_SpectrumAdder.SetParent(this);
		mPO_PeaksAdder.SetParent(this);
	}


	// Unsupervised Do() function.
	bool FrameAdder::Do(const Frame& in1, const Frame& in2, Frame& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "FrameAdder::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		if(in1.GetFundamentalFreq()!=0 && in2.GetFundamentalFreq()!=0 && mConfig.GetHarmonic())
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

		if(mConfig.GetUseSpectralShape())
			mPO_PeaksAdder.Do(
					in1.GetSpectralPeakArray(),
					in2.GetSpectralPeakArray(),
					*mpSpectralShape,
					out.GetSpectralPeakArray() );
		else
			mPO_PeaksAdder.Do(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),out.GetSpectralPeakArray());
		mPO_SpectrumAdder.Do(in1.GetResidualSpec(),in2.GetResidualSpec(),out.GetResidualSpec());

		return true;
	}

	bool FrameAdder::Do(void)
	{
		CLAM_ASSERT(false,"FrameAdder::Do(): Not implemented");

		return true;
	}

	
	int FrameAdder::DoFrameFactorControl(TData value)
	{
		mMagInterpolationFactorCtl.DoControl(value);
		mFreqInterpolationFactorCtl.DoControl(value);
		mPitchInterpolationFactorCtl.DoControl(value);
		mResidualInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoMagFactorControl(TData value)
	{
		mPO_PeaksAdder.mMagInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoFreqFactorControl(TData value)
	{
		mPO_PeaksAdder.mFreqInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoPitchFactorControl(TData value)
	{
		mPO_PeaksAdder.mPitchInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoResidualFactorControl(TData value)
	{
		mPO_SpectrumAdder.mInterpolationFactorCtl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoPitch1Control(TData value)
	{
		mPO_PeaksAdder.mPitch1Ctl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoPitch2Control(TData value)
	{
		mPO_PeaksAdder.mPitch2Ctl.DoControl(value);
		return 0;
	}

	int FrameAdder::DoHarmonicControl(TData value)
	{
		mPO_PeaksAdder.mIsHarmonicCtl.DoControl(value);
		return 0;
	}



}
