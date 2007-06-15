/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "SpectrumInterpolator.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	void SpecInterpConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}


	SpectrumInterpolator::SpectrumInterpolator()
		: mSize(0),
		  mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mProtoState(SOther),
		  mInterpolationFactorCtl("InterpolationFactor",this)
	{
		Configure(SpecInterpConfig());
	}

	SpectrumInterpolator::SpectrumInterpolator(const SpecInterpConfig &c)
		: mSize(0),
		  mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mProtoState(SOther),
		  mInterpolationFactorCtl("InterpolationFactor",this)
	{
		Configure(c);
	}

	bool SpectrumInterpolator::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);
		//Initialize interpolation factor control from value in the configuration
		mInterpolationFactorCtl.DoControl(mConfig.GetInterpolationFactor());

		return true;
	}

	// Unsupervised Do() function.
	bool SpectrumInterpolator::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_DEBUG_ASSERT(IsRunning(),
		                  "SpectrumInterpolator::Do(): Not in execution mode");

		switch (mProtoState) {
		// Fast prototype configurations
		case SMagPhase:
			InterpolateMagPhase(in1,in2,out);
			break;
		case SComplex:
			InterpolateComplex(in1,in2,out);
			break;
		case SPolar:
			InterpolatePolar(in1,in2,out);
			break;
		case SBPF:
			InterpolateBPF(in1,in2,out);
			break;
		case SBPFMagPhase:
			InterpolateBPFMagPhase(in1,in2,out);
			break;
		case SBPFComplex:
			InterpolateBPFComplex(in1,in2,out);
			break;
		case SBPFPolar:
			InterpolateBPFPolar(in1,in2,out);
			break;
		case SMagPhaseBPF:
			InterpolateMagPhaseBPF(in1,in2,out);
			break;
		case SComplexBPF:
			InterpolateComplexBPF(in1,in2,out);
			break;
		case SPolarBPF:
			InterpolatePolarBPF(in1,in2,out);
			break;
		// Slow type configurations
		case SOther:
			Interpolate(in1,in2,out);
			break;
		default:
			CLAM_ASSERT(false,"Do(...) : internal inconsistency (invalid mProtoState)");
		}

		return true;
	}

	bool SpectrumInterpolator::Do(void)
	{
		CLAM_ASSERT(false, "SpectrumInterpolator::Do(): Not implemented");

		return true;
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the interpolation computation. 
	bool SpectrumInterpolator::SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out)
	{
		// Check common attributes
		SpecTypeFlags t1;
		in1.GetType(t1);
		SpecTypeFlags t2;
		in2.GetType(t2);
		SpecTypeFlags to;
		out.GetType(to);

		// Sanity check:
		CLAM_ASSERT((t1.bMagPhase || t1.bComplex || t1.bPolar || t1.bMagPhaseBPF),
				"SpectrumInterpolator: First spectrum has no content");
		CLAM_ASSERT((t2.bMagPhase || t2.bComplex || t2.bPolar || t2.bMagPhaseBPF),
				"SpectrumInterpolator: Second spectrum has no content");
		CLAM_ASSERT((to.bMagPhase || to.bComplex || to.bPolar || to.bMagPhaseBPF),
				"SpectrumInterpolator: Output spectrum has no content");

		// Interpolateer size. "pure" BPFs are not considered here.
		mSize = 0;
		if (t1.bMagPhase || t1.bComplex || t1.bPolar) {
			mSize = in1.GetSize();
			CLAM_ASSERT(mSize,"SpectrumInterpolator::SetPrototypes: Zero size spectrum");
		}
		if (t2.bMagPhase || t2.bComplex || t2.bPolar)
			if (mSize) {
				CLAM_ASSERT(mSize == in2.GetSize(),"SpectrumInterpolator::SetPrototypes:Size mismatch in spectrum interpolation");
			}
			else {
				mSize = in2.GetSize();
				CLAM_ASSERT(mSize,"SpectrumInterpolator::SetPrototypes: Zero size spectrum");
			}
		if (to.bMagPhase || to.bComplex || to.bPolar)
			if (mSize) {
				CLAM_ASSERT(mSize == out.GetSize(),"SpectrumInterpolator::SetPrototypes:Size mismatch in spectrum interpolation");
			}
			else {
				mSize = out.GetSize();
				CLAM_ASSERT(mSize,"SpectrumInterpolator::SetPrototypes: Zero size spectrum");
			}

		// Spectral Range.  
		// We could also ignore BPF-only objects here, but in
		// practice, if a BPF is designed for a certain spectral
		// range, error will probably be too big out of the range, out
		// we always force range matching
		CLAM_ASSERT(in1.GetSpectralRange() == in2.GetSpectralRange() &&
			in1.GetSpectralRange() == out.GetSpectralRange() ,"SpectrumInterpolator::SetPrototypes: Spectral range mismatch in spectrum interpolation");

		// Scale.
		if (in1.GetScale() == EScale::eLinear)
			if (in2.GetScale() == EScale::eLinear)
				mScaleState=Slinlin;
			else
				mScaleState=Slinlog;
		else
			if (in2.GetScale() == EScale::eLinear)
				mScaleState=Sloglin;
			else
				mScaleState=Sloglog;
		// Log scale output might be useful, for example when working
		// with BPF objects at the three ports. But right for now...
		CLAM_ASSERT(out.GetScale() != EScale::eLog,"SpectrumInterpolator: Log Scale Output not implemented");

		// Prototypes.

		// BPF Interpolation.
		bool i1BPF=false, i2BPF=false, oBPF=false;
		if (t1.bMagPhaseBPF && !t1.bComplex && !t1.bPolar && !t1.bMagPhase)
			i1BPF=true;
		if (t2.bMagPhaseBPF && !t2.bComplex && !t2.bPolar && !t2.bMagPhase)
			i2BPF=true;
		if (to.bMagPhaseBPF && !to.bComplex && !to.bPolar && !to.bMagPhase)
			oBPF=true;

		if (oBPF) {
			// BPF output requires interpolating the inputs.
			mProtoState=SBPF;
			return true;
		}
		if (i1BPF) {
			// States with direct BPF implementation.
			if (t2.bMagPhase && to.bMagPhase) {
				mProtoState=SBPFMagPhase;
				return true;
			}
			if (t2.bComplex && to.bComplex) {
				mProtoState=SBPFComplex;
				return true;
			}
			if (t2.bPolar && to.bPolar) {
				mProtoState=SBPFPolar;
				return true;
			}
			// States requiring 1 conversion:
			if (t2.bMagPhase || to.bMagPhase) {
				mProtoState=SBPFMagPhase;
				return true;
			}
			if (t2.bComplex || to.bComplex) {
				mProtoState=SBPFComplex;
				return true;
			}
			if (t2.bPolar  || to.bPolar) {
				mProtoState=SBPFPolar;
				return true;
			}
			// Should never get here:
			CLAM_ASSERT(false, 
				"SpectrumInterpolator::SetPrototypes: Data flags internal inconsistency");
		}
		if (i2BPF) {
			// States with direct BPF implementation.
			if (t1.bMagPhase && to.bMagPhase) {
				mProtoState=SMagPhaseBPF;
				return true;
			}
			if (t1.bComplex && to.bComplex) {
				mProtoState=SComplexBPF;
				return true;
			}
			if (t1.bPolar && to.bPolar) {
				mProtoState=SPolarBPF;
				return true;
			}
			// States requiring 1 conversion:
			if (t1.bMagPhase || to.bMagPhase) {
				mProtoState=SMagPhaseBPF;
				return true;
			}
			if (t1.bComplex || to.bComplex) {
				mProtoState=SComplexBPF;
				return true;
			}
			if (t1.bPolar || to.bPolar) {
				mProtoState=SPolarBPF;
				return true;
			}
			// Should never get here:
			CLAM_ASSERT(false, 
				"SpectrumInterpolator::SetPrototypes:"
				" invalid data flags");
		}
		// Direct non-BPF states.
		if (t1.bMagPhase && t2.bMagPhase &&	to.bMagPhase) {
			mProtoState=SMagPhase;
			return true;
		}
		if (t1.bComplex && t2.bComplex && to.bComplex) {
			mProtoState=SComplex;
			return true;
		}
		if (t1.bPolar && t2.bPolar && to.bPolar) {
			mProtoState=SPolar;
			return true;
		}
		// States Requiring 1 Conversion
		if ( (t1.bMagPhase && t2.bMagPhase) ||
			 (t1.bMagPhase && to.bMagPhase) ||
			 (t2.bMagPhase && to.bMagPhase)) {
			mProtoState=SMagPhase;
			return true;
		}
		if ( (t1.bComplex && t2.bComplex) ||
			 (t1.bComplex && to.bComplex) ||
			 (t2.bComplex && to.bComplex)) {
			mProtoState=SComplex;
			return true;
		}
		if ( (t1.bPolar && t2.bPolar) ||
			 (t1.bPolar && to.bPolar) ||
			 (t2.bPolar && to.bPolar)) {
			mProtoState=SPolar;
			return true;
		}
		// Bad luck. We require 2 conversions...
		mProtoState=SMagPhase;
		return true;
	}


	bool SpectrumInterpolator::SetPrototypes()
	{
		CLAM_ASSERT(false, "SpectrumInterpolator::SetPrototypes(): Not implemented");

		return true;
	}

	bool SpectrumInterpolator::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumInterpolator::Interpolate(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(in1,in2,out);
		Do(in1,in2,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumInterpolator::InterpolateMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolateMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			InterpolateMagPhaseLinLog(in1,in2,out);
			break;
		case Sloglin:
			InterpolateMagPhaseLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolateMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because some of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in1.GetType(f);
		if (!f.bMagPhase) {
			remove1=true;
			f.bMagPhase=true;
			in1.SetTypeSynchronize(f);
		}
		in2.GetType(f);
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bMagPhase) {
			remove3=true;
			f.bMagPhase=true;
			out.SetType(f);
		}

		TData *m1 = in1.GetMagBuffer().GetPtr();
		TData *f1 = in1.GetPhaseBuffer().GetPtr();
		TData *m2 = in2.GetMagBuffer().GetPtr();
		TData *f2 = in2.GetPhaseBuffer().GetPtr();
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();
/*****************************/
//OPERATION: MAGPHASE AND MAGPHASE		
		
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);

		for (int i=0;i<mSize;i++) {
			Polar po=polInvIntFactor*Polar(m1[i],f1[i])+polIntFactor*Polar(m2[i],f2[i]);
			mo[i]=po.Mag();
			fo[i]=po.Ang();
		}

		f.bComplex=f.bPolar=f.bMagPhaseBPF=false;
		f.bMagPhase=true;
		out.SynchronizeTo(f);

		if (remove1) {
			in1.RemoveMagBuffer();
			in1.RemovePhaseBuffer();
			in1.UpdateData();
		}
		if (remove2) {
			in2.RemoveMagBuffer();
			in2.RemovePhaseBuffer();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveMagBuffer();
			out.RemovePhaseBuffer();
			out.UpdateData();
		}

	}

	void SpectrumInterpolator::InterpolateComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateComplexLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolateComplexLog(in1,in2,out);
			break;
		case Slinlog:
			InterpolateComplexLinLog(in1,in2,out);
			break;
		case Sloglin:
			InterpolateComplexLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolateComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in1.GetType(f);
		if (!f.bComplex) {
			remove1=true;
			f.bComplex=true;
			in1.SetTypeSynchronize(f);
		}
		in2.GetType(f);
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		Complex *c1 = in1.GetComplexArray().GetPtr();
		Complex *c2 = in2.GetComplexArray().GetPtr();
		Complex *co = out.GetComplexArray().GetPtr();
/*****************************/
//OPERATION: COMPLEX AND COMPLEX
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		for (int i=0;i<mSize;i++)
			co[i]=c1[i]*invIntFactor+c2[i]*intFactor;

		f.bMagPhase=f.bPolar=f.bMagPhaseBPF=false;
		f.bComplex=true;
		out.SynchronizeTo(f);

		if (remove1) {
			in1.RemoveComplexArray();
			in1.UpdateData();
		}
		if (remove2) {
			in2.RemoveComplexArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}


	void SpectrumInterpolator::InterpolatePolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolatePolarLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolatePolarLog(in1,in2,out);
			break;
		case Slinlog:
			InterpolatePolarLinLog(in1,in2,out);
			break;
		case Sloglin:
			InterpolatePolarLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolatePolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in1.GetType(f);
		if (!f.bPolar) {
			remove1=true;
			f.bPolar=true;
			in1.SetTypeSynchronize(f);
		}
		in2.GetType(f);
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		Polar *p1 = in1.GetPolarArray().GetPtr();
		Polar *p2 = in2.GetPolarArray().GetPtr();
		Polar *po = out.GetPolarArray().GetPtr();
/*****************************/
//OPERATION: POLAR AND POLAR
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);

		for (int i=0;i<mSize;i++)
			po[i]=polInvIntFactor*p1[i]+polIntFactor*p2[i];

		f.bComplex=f.bMagPhase=f.bMagPhaseBPF=false;
		f.bPolar=true;
		out.SynchronizeTo(f);

		if (remove1) {
			in1.RemovePolarArray();
			in1.UpdateData();
		}
		if (remove2) {
			in2.RemovePolarArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemovePolarArray();
			out.UpdateData();
		}
	}


	void SpectrumInterpolator::InterpolateBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolateBPFMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"InterpolateBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			InterpolateBPFMagPhaseLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolateMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFMagPhaseLin(in2,in1,out);
			break;
		case Sloglog:
			InterpolateBPFMagPhaseLog(in2,in1,out);
			break;
		case Slinlog:
			InterpolateBPFMagPhaseLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"InterpolateBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumInterpolator::InterpolateBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bMagPhase) {
			remove3=true;
			f.bMagPhase=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		TData *m2 = in2.GetMagBuffer().GetPtr();
		TData *f2 = in2.GetPhaseBuffer().GetPtr();
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();
/*****************************/
//OPERATION: BPF AND MAGPHASE
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);

		for (int i=0;i<mSize;i++) {
			Polar po = polInvIntFactor*Polar(m1.GetValue(pos),f1.GetValue(pos)) + 
			            polIntFactor*Polar(m2[i],f2[i]);
			mo[i]=po.Mag();
			fo[i]=po.Ang();
			pos+=delta;
		}

		f.bComplex=f.bPolar=f.bMagPhaseBPF=false;
		f.bMagPhase=true;
		out.SynchronizeTo(f);
		
		if (remove2) {
			in2.RemoveMagBuffer();
			in2.RemovePhaseBuffer();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveMagBuffer();
			out.RemovePhaseBuffer();
			out.UpdateData();
		}
	}

	void SpectrumInterpolator::InterpolateBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bMagPhase) {
			remove3=true;
			f.bMagPhase=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		TData *m2 = in2.GetMagBuffer().GetPtr();
		TData *f2 = in2.GetPhaseBuffer().GetPtr();
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();
/*****************************/
//OPERATION: BPF(LOG) AND MAGPHASE
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;
		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);
		
		for (int i=0;i<mSize;i++) {
			Polar po = polInvIntFactor*Polar(CLAM_pow(TData(10),m1.GetValue(pos)/TData(10.0)),f1.GetValue(pos)) + 
			            polIntFactor*Polar(m2[i],f2[i]);
			mo[i]=po.Mag();
			fo[i]=po.Ang();
			pos+=delta;
		}

		f.bComplex=f.bPolar=f.bMagPhaseBPF=false;
		f.bMagPhase=true;
		out.SynchronizeTo(f);
		
		if (remove2) {
			in2.RemoveMagBuffer();
			in2.RemovePhaseBuffer();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveMagBuffer();
			out.RemovePhaseBuffer();
			out.UpdateData();
		}
	}

	void SpectrumInterpolator::InterpolateBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFComplexLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolateBPFComplexLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"InterpolateBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			InterpolateBPFComplexLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolateComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFComplexLin(in2,in1,out);
			break;
		case Sloglog:
			InterpolateBPFComplexLog(in2,in1,out);
			break;
		case Slinlog:
			InterpolateBPFComplexLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"InterpolateBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumInterpolator::InterpolateBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		Complex *c2 = in2.GetComplexArray().GetPtr();
		Complex *co = out.GetComplexArray().GetPtr();
/*****************************/
//OPERATION: BPF AND COMPLEX
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		for (int i=0;i<mSize;i++) {
			TData BRe = fabs(m1.GetValue(pos)) * CLAM_cos(f1.GetValue(pos));
			TData BIm = fabs(m1.GetValue(pos)) * CLAM_sin(f1.GetValue(pos));
			co[i]= Complex(BRe,BIm)*invIntFactor + c2[i]*intFactor;
			pos+=delta;
		}

		f.bMagPhase=f.bPolar=f.bMagPhaseBPF=false;
		f.bComplex=true;
		out.SynchronizeTo(f);
		
		if (remove2) {
			in2.RemoveComplexArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}

	// This is probably one of the most used methods, because it can be used
	// to apply a BPF filter in log scale to a linear complex spectrum, as the
	// one naturaly generated from a FFT
	void SpectrumInterpolator::InterpolateBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		Complex *c2 = in2.GetComplexArray().GetPtr();
		Complex *co = out.GetComplexArray().GetPtr();
/*****************************/
//OPERATION: BPF(LOG) AND COMPLEX
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		for (int i=0;i<mSize;i++) {
			TData BRe = CLAM_pow(TData(10),fabs(m1.GetValue(pos))/TData(10.0)) * CLAM_cos(f1.GetValue(pos));
			TData BIm = CLAM_pow(TData(10),fabs(m1.GetValue(pos))/TData(10.0)) * CLAM_sin(f1.GetValue(pos));
			co[i]= Complex(BRe,BIm)*invIntFactor + c2[i]*intFactor;
			pos+=delta;
		}

		f.bMagPhase=f.bPolar=f.bMagPhaseBPF=false;
		f.bComplex=true;
		out.SynchronizeTo(f);
		
		
		if (remove2) {
			in2.RemoveComplexArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}


	void SpectrumInterpolator::InterpolateBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFPolarLin(in1,in2,out);
			break;
		case Sloglog:
			InterpolateBPFPolarLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"InterpolateBPFPolarLinLog: Not implemented");
			break;
		case Sloglin:
			InterpolateBPFPolarLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumInterpolator::InterpolatePolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			InterpolateBPFPolarLin(in2,in1,out);
			break;
		case Sloglog:
			InterpolateBPFPolarLog(in2,in1,out);
			break;
		case Slinlog:
			InterpolateBPFPolarLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"InterpolateBPFPolarLinLog: Not implemented");
			break;
		}
	}

	void SpectrumInterpolator::InterpolateBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		Polar *p2 = in2.GetPolarArray().GetPtr();
		Polar *po = out.GetPolarArray().GetPtr();
/*****************************/
//OPERATION: BPF AND POLAR
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;
		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);
		
		for (int i=0;i<mSize;i++) {
			po[i]=polInvIntFactor*Polar(m1.GetValue(pos),f1.GetValue(pos))+polIntFactor*p2[i];
			pos+=delta;
		}

		f.bMagPhase=f.bComplex=f.bMagPhaseBPF=false;
		f.bPolar=true;
		out.SynchronizeTo(f);
		
		if (remove2) {
			in2.RemovePolarArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemovePolarArray();
			out.UpdateData();
		}
	}

	void SpectrumInterpolator::InterpolateBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Interpolateing more
		// States, see coments on this in the class declaration.
		in2.GetType(f);
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		out.GetType(f);
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-TData(1.0));
		BPF &m1 = in1.GetMagBPF();
		BPF &f1 = in1.GetPhaseBPF();
		Polar *p2 = in2.GetPolarArray().GetPtr();
		Polar *po = out.GetPolarArray().GetPtr();
/*****************************/
//OPERATION: BPF(LOG) AND POLAR
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;
		Polar polInvIntFactor=Polar(invIntFactor);
		Polar polIntFactor=Polar(intFactor);

		for (int i=0;i<mSize;i++) {
			TData BMag = CLAM_pow(TData(10),m1.GetValue(pos)/TData(10.0));
			TData BPha = f1.GetValue(pos);
			po[i]=polInvIntFactor*Polar(BMag,BPha)+polIntFactor*p2[i];
			pos+=delta;
		}

		f.bMagPhase=f.bComplex=f.bMagPhaseBPF=false;
		f.bPolar=true;
		out.SynchronizeTo(f);
		
		if (remove2) {
			in2.RemovePolarArray();
			in2.UpdateData();
		}
		if (remove3) {
			out.RemovePolarArray();
			out.UpdateData();
		}
	}

	void SpectrumInterpolator::InterpolateBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		
		TData intFactor=mInterpolationFactorCtl.GetLastValue();
		if(intFactor>1) intFactor=1;
		if(intFactor<0) intFactor=0;
		TData invIntFactor=1-intFactor;

		// First we check if the abcisas agree

		for (int i=0;i<mSize;i++) {
			Point &pm1=in1.GetMagBPF().GetPointArray()[i];
			Point &pm2=in2.GetMagBPF().GetPointArray()[i];
			Point &pmo=out.GetMagBPF().GetPointArray()[i];
			Point &pf1=in1.GetPhaseBPF().GetPointArray()[i];
			Point &pf2=in2.GetPhaseBPF().GetPointArray()[i];
			Point &pfo=out.GetPhaseBPF().GetPointArray()[i];
			CLAM_ASSERT(pm1.GetX() == pm2.GetX(), "InterpolateBPF: input BPF abcisas do not match "
				"(and BPF merging not yet iplemented)");
			CLAM_ASSERT(pm1.GetX() == pmo.GetX(), "InterpolateBPF: ouput BPF abcisas do not match with imput "
				"(and BPF merging not yet iplemented)");
/*****************************/
//OPERATION: BPF AND BPF
			pmo.SetY(invIntFactor*pm1.GetY()+intFactor*pm2.GetY());
			pfo.SetY(invIntFactor*pf1.GetY()+intFactor*pf2.GetY());
		}

	}

	// UNINMPLEMENTED METHODS. some day...
	void SpectrumInterpolator::InterpolateMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateMagPhaseLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateMagPhaseLinLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateComplexLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateComplexLinLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolatePolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolatePolarLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolatePolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolatePolarLinLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFComplexLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFComplexLinLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFPolarLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFPolarLinLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFMagPhaseLog: Not implemented");
	}
	void SpectrumInterpolator::InterpolateBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"InterpolateBPFMagPhaseLinLog: Not implemented");
	}
}

