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

/** @file SoftSwitch
 *  @todo
 *  Efficiency can be improved by:
 *    - Generating a fadeout table at configuration time.
 *    - Keeping and index position.
 *    - Using a fade-attack transition position table.
 */


#include "SoftSwitch.hxx"

namespace CLAM {

	Enum::tEnumValue EFadeType::sEnumValues[] = {
		{EFadeType::eSharp,"Sharp"},
		{EFadeType::eLinear,"Linear"},
		{EFadeType::eExponential,"Exponential"},
		{0,NULL}
	};

	Enum::tValue EFadeType::sDefault = EFadeType::eSharp;

	class LinearFader {
	public:
		static TData Apply(TTime x, TTime length, TData amplitude)
		{
			if (x>length)
				return 0.0;
			return amplitude * (length - x) / length;
		}
	};

	class SharpFader {
	public:
		static TData Apply(TTime x, TTime length, TData amplitude)
		{
			return 0.0;
		}
	};

	class ExponentialFader {
	public:
		static TData Apply(TTime x, TTime length, TData amplitude)
		{
			if (x>length)
				return 0.0;
			return amplitude * exp (- 9.0 * x / length);
		}
	};


	void SoftSwitchConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);
		SetTransitionTime(1.0);
		SetFadeType(EFadeType::eLinear);
		SetEnabledAtStart(true);
	}


	SoftSwitch::SoftSwitch(const SoftSwitchConfig& c)
		: Input("Input Audio",this,1),
		  Output("Output Audio",this,1),
		  Enable("Output Enable",this)
	{
		Configure(c);
	}

	bool SoftSwitch::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mType = mConfig.GetFadeType();

		mFrameSize = mConfig.GetFrameSize();

		mTransitionLength = mConfig.GetTransitionTime();

		mDelta = 1000.0 / mConfig.GetSampleRate();

		Input.SetParams(mConfig.GetFrameSize());
		Output.SetParams(mConfig.GetFrameSize());
		return true;
	}

	bool SoftSwitch::ConcreteStart()
	{
		mTransitionPos = 0.0;
		if (mConfig.GetEnabledAtStart()) {
			mState = eOn;
			mTransitionCurrentValue = 1.0;
			mTransitionInitValue = 1.0;
			Enable.DoControl(TData(true));
		}
		else {
			mState = eOff;
			mTransitionCurrentValue = 0.0;
			mTransitionInitValue = 0.0;
			Enable.DoControl(TData(false));
		}
		return true;
	}

	template<class FADER>
	void SoftSwitch::FadeoutTmpl(const Array<TData> &inp, Array<TData> &outp)
	{
		int i;

		outp[0] = inp[0] * mTransitionCurrentValue;

		for (i=1; i<mFrameSize; i++) {
			outp[i] = inp[i] * FADER::Apply(mTransitionPos,
											mTransitionLength,
											mTransitionInitValue);
			mTransitionPos += mDelta;
		}

		mTransitionCurrentValue = FADER::Apply(mTransitionPos,
											   mTransitionLength,
											   mTransitionInitValue);
	}

	template<class FADER>
	void SoftSwitch::AttackTmpl(const Array<TData> &inp, Array<TData> &outp)
	{
		int i;
		TData init = 1.0 - mTransitionInitValue;

		outp[0] = inp[0] * mTransitionCurrentValue;

		for (i=1; i<mFrameSize; i++) {
				outp[i] = inp[i] * (1.0 - FADER::Apply(mTransitionPos,
													   mTransitionLength,
													   init));
				mTransitionPos += mDelta;
		}

		mTransitionCurrentValue = (1.0 - FADER::Apply(mTransitionPos,
													  mTransitionLength,
													  init));
	}

	void SoftSwitch::Attack(const Array<TData> &inp, Array<TData> &outp)
	{
		switch (mType) {
		case EFadeType::eSharp:
			AttackTmpl<SharpFader>(inp,outp);
			break;
		case EFadeType::eLinear:
			AttackTmpl<LinearFader>(inp,outp);
			break;
		case EFadeType::eExponential:
			AttackTmpl<ExponentialFader>(inp,outp);
			break;
		};
		if (mTransitionPos > mTransitionLength)
			mState = eOn;
	}

	void SoftSwitch::Fadeout(const Array<TData> &inp, Array<TData> &outp)
	{
		switch (mType) {
		case EFadeType::eSharp:
			FadeoutTmpl<SharpFader>(inp,outp);
			break;
		case EFadeType::eLinear:
			FadeoutTmpl<LinearFader>(inp,outp);
			break;
		case EFadeType::eExponential:
			FadeoutTmpl<ExponentialFader>(inp,outp);
			break;
		};
		if (mTransitionPos > mTransitionLength)
			mState = eOff;
	}

	void SoftSwitch::Silence(Array<TData> &out)
	{
		int i;
		for (i=0; i<mFrameSize; i++)
			out[i]=0.0;
	}

	void SoftSwitch::CheckControl()
	{
		bool enabled = Enable.GetLastValue();

		switch (mState) {
		case eOn:
			if (!enabled) {
				mTransitionInitValue = 1.0;
				mTransitionPos = 0.0;
				mState = eFadeout;
			}
			break;
		case eOff:
			if (enabled) {
				mTransitionInitValue = 0.0;
				mTransitionPos = 0.0;
				mState = eAttack;
			}
			break;
		case eAttack:
			if (!enabled) {
				mTransitionInitValue = mTransitionCurrentValue;
				mTransitionPos = 0.0;
				mState = eFadeout;
			}
			break;
		case eFadeout:
			if (enabled) {
				mTransitionInitValue = mTransitionCurrentValue;
				mTransitionPos = 0.0;
				mState = eAttack;
			}
			break;
		}
	}

	bool SoftSwitch::Do()
	{
		bool res = Do(Input.GetData(),
					  Output.GetData());
		Input.LeaveData();
		Output.LeaveData();	  
		return res;
	}

	bool SoftSwitch::Do(const Audio& inp, Audio& out)
	{
		CLAM_ASSERT(inp.GetSize() == out.GetSize() &&
					inp.GetSize() == mFrameSize,
					"SoftSwitch::Do(...): Size mismatch");
		CLAM_ASSERT(inp.GetSampleRate() == out.GetSampleRate() &&
					inp.GetSampleRate() == mConfig.GetSampleRate(),
					"SoftSwitch::Do(...): sample rate mismatch");

		Array<TData> &inpa     = inp.GetBuffer();
		Array<TData> &outa     = out.GetBuffer();

		CheckControl();

		switch (mState) {
		case eOn:
			outa = inpa;
			break;
		case eOff:
			Silence(outa);
			break;
		case eAttack:
			Attack(inpa,outa);
			break;
		case eFadeout:
			Fadeout(inpa,outa);
			break;
		}

		return true;
	}

}
