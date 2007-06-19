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
#include "SpectrumSubstracter2.hxx"
#include "BPF.hxx"
#include "Point.hxx"
#include "SpecTypeFlags.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM 
{

namespace Hidden 
{
	static FactoryRegistrator<ProcessingFactory, SpectrumSubstracter2> regSpectrumSubstracter2("SpectrumSubstracter2");
}

	SpectrumSubstracter2::SpectrumSubstracter2()
		: mSize(0),
		  mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mProtoState(SOther)
	{
		Configure(SpecSubstracter2Config());
	}

	SpectrumSubstracter2::SpectrumSubstracter2(const SpecSubstracter2Config &c)
		: mSize(0),
		  mIn1("Input 1",this),
		  mIn2("Input 2",this),
		  mOut("Output",this),
		  mProtoState(SOther)
	{
		Configure(c);
	}

	std::string SpectrumSubstracter2::NewUniqueName()
	{
		static int ObjectCount=0;

		std::stringstream name;
		name << "SpectrumSubstracter2_" << ObjectCount++;

		return name.str();
	}

	bool SpectrumSubstracter2::ConcreteConfigure(const ProcessingConfig&c)
	{
		CopyAsConcreteConfig(mConfig, c);
		
		return true;
	}

	// Unsupervised Do() function.
	bool SpectrumSubstracter2::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_DEBUG_ASSERT(IsRunning(),
			"SpectrumSubstracter2::Do(): Not in execution mode");

		switch (mProtoState) {
		// Fast prototype configurations
		case SMagPhase:
			SubstractMagPhase(in1,in2,out);
			break;
		case SComplex:
			SubstractComplex(in1,in2,out);
			break;
		case SPolar:
			SubstractPolar(in1,in2,out);
			break;
		case SBPF:
			SubstractBPF(in1,in2,out);
			break;
		case SBPFMagPhase:
			SubstractBPFMagPhase(in1,in2,out);
			break;
		case SBPFComplex:
			SubstractBPFComplex(in1,in2,out);
			break;
		case SBPFPolar:
			SubstractBPFPolar(in1,in2,out);
			break;
		case SMagPhaseBPF:
			SubstractMagPhaseBPF(in1,in2,out);
			break;
		case SComplexBPF:
			SubstractComplexBPF(in1,in2,out);
			break;
		case SPolarBPF:
			SubstractPolarBPF(in1,in2,out);
			break;
		// Slow type configurations
		case SOther:
			Substract(in1,in2,out);
			break;
		default:
			CLAM_ASSERT(false,"Do(...) : internal inconsistency (invalid mProtoState)");
		}

		return true;
	}

	bool SpectrumSubstracter2::Do(void)
	{
		mOut.GetData().SetSize( mIn1.GetData().GetSize() );
		mOut.GetData().SetSpectralRange( mIn1.GetData().GetSpectralRange() );
		bool result = Do( mIn1.GetData(), mIn2.GetData(), mOut.GetData() );
		
		mIn1.Consume();
		mIn2.Consume();
		mOut.Produce();
		
		return result;
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the substrac computation. 
	bool SpectrumSubstracter2::SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out)
	{
		// Check common attributes
		SpecTypeFlags t1;
		in1.GetType(t1);
		SpecTypeFlags t2;
		in2.GetType(t2);
		SpecTypeFlags to;
		out.GetType(to);

		// Sanity check:
		CLAM_ASSERT(t1.bMagPhase || t1.bComplex || t1.bPolar || t1.bMagPhaseBPF,
			"SpectrumSubstracter2: First input Spectrum without content");
		CLAM_ASSERT(t2.bMagPhase || t2.bComplex || t2.bPolar || t2.bMagPhaseBPF,
			"SpectrumSubstracter2: Second input Spectrum without content");
		CLAM_ASSERT(to.bMagPhase || to.bComplex || to.bPolar || to.bMagPhaseBPF,
			"SpectrumSubstracter2: Output Spectrum object without content");

		// Product size. "pure" BPFs are not considered here.
		mSize = 0;
		if (t1.bMagPhase || t1.bComplex || t1.bPolar) {
			mSize = in1.GetSize();
			CLAM_ASSERT(mSize,"SpectrumSubstracter2::SetPrototypes: Zero size spectrum");
		}
		if (t2.bMagPhase || t2.bComplex || t2.bPolar) 
		{
			CLAM_ASSERT(in2.GetSize(),"SpectrumSubstracter2::SetPrototypes: Zero size spectrum");
			if (!mSize) mSize = in2.GetSize();
			else CLAM_ASSERT(mSize == in2.GetSize(),
				"SpectrumSubstracter2::SetPrototypes: Sizes Mismatch");
		}
		if (to.bMagPhase || to.bComplex || to.bPolar)
		{
			CLAM_ASSERT(out.GetSize(),"SpectrumSubstracter2::SetPrototypes: Zero size spectrum");
			if (!mSize) mSize = out.GetSize();
			else CLAM_ASSERT(mSize == out.GetSize(),
				"SpectrumSubstracter2::SetPrototypes: Output size Mismatch");
		}

		// Spectral Range.  
		// We could also ignore BPF-only objects here, but in
		// practice, if a BPF is designed for a certain spectral
		// range, error will probably be too big out of the range, out
		// we always force range matching
		CLAM_ASSERT(in1.GetSpectralRange() == in2.GetSpectralRange(),
			"SpectrumSubstracter2::SetPrototypes: Spectral range mismatch on inputs");
		CLAM_ASSERT(in1.GetSpectralRange() == out.GetSpectralRange(),
			"SpectrumSubstracter2::SetPrototypes: Spectral range mismatch on output");

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
		CLAM_ASSERT (out.GetScale() != EScale::eLog,
			"SpectrumSubstracter2: Log Scale Output not implemented");

		// Prototypes.

		// BPF SUMS.
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
			CLAM_ASSERT(false,"SpectrumSubstracter2::SetPrototypes:"
				" Data flags internal inconsistency");
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
			CLAM_ASSERT(false, "SpectrumSubstracter2::SetPrototypes:"
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


	bool SpectrumSubstracter2::SetPrototypes()
	{
		CLAM_ASSERT(false,"SpectrumSubstracter2::SetPrototypes(): Not implemented");

		return true;
	}

	bool SpectrumSubstracter2::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumSubstracter2::Substract(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(in1,in2,out);
		Do(in1,in2,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumSubstracter2::SubstractMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			SubstractMagPhaseLinLog(in1,in2,out);
			break;
		case Sloglin:
			SubstractMagPhaseLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because outme of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
		
			TData r1,i1,r2,i2,r3,i3;
		
			r1 = Abs(m1[i]) * CLAM_cos(f1[i]); 
			i1 = Abs(m1[i]) * CLAM_sin(f1[i]); 
			r2 = Abs(m2[i]) * CLAM_cos(f2[i]);
			i2 = Abs(m2[i]) * CLAM_sin(f2[i]);
		
			r3 = r1-r2;
			i3 = i1-i2;
/*#ifdef CLAM_OPTIMIZE
			const float insignificant = 0.000001;
			TData absIm = Abs(i3);
			TData absRe = Abs(r3);
			if(absIm<insignificant && absRe>insignificant) mo[i] = absRe;
			else if(absRe<insignificant && absIm>insignificant) mo[i] = absIm;
			else mo[i] = CLAM_sqrt (r3*r3 + i3*i3);
#else
*/			mo[i] = CLAM_sqrt (r3*r3 + i3*i3);
//#endif		
			fo[i] = CLAM_atan2 (i3,r3);
			
			//Polar po=Polar(m1[i],f1[i])-Polar(m2[i],f2[i]);
			//mo[i]=po.Mag();
			//fo[i]=po.Ang();
		}

//#ifndef CLAM_OPTIMIZE
//if optimizations are on we asume the spectrums do not need to be converted back
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
//#endif

	}

	void SpectrumSubstracter2::SubstractComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractComplexLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractComplexLog(in1,in2,out);
			break;
		case Slinlog:
			SubstractComplexLinLog(in1,in2,out);
			break;
		case Sloglin:
			SubstractComplexLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because outme of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++)
			co[i]=c1[i]-c2[i];

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


	void SpectrumSubstracter2::SubstractPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractPolarLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractPolarLog(in1,in2,out);
			break;
		case Slinlog:
			SubstractPolarLinLog(in1,in2,out);
			break;
		case Sloglin:
			SubstractPolarLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because outme of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
		// it it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++)
			po[i]=p1[i]-p2[i];

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


	void SpectrumSubstracter2::SubstractBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractBPFMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"SubstractBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			SubstractBPFMagPhaseLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFMagPhaseLin(in2,in1,out);
			break;
		case Sloglog:
			SubstractBPFMagPhaseLog(in2,in1,out);
			break;
		case Slinlog:
			SubstractBPFMagPhaseLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"SubstractBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumSubstracter2::SubstractBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			Polar po = Polar(m1.GetValue(pos),f1.GetValue(pos)) - 
			            Polar(m2[i],f2[i]);
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

	void SpectrumSubstracter2::SubstractBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			Polar po = Polar(log2lin(m1.GetValue(pos)),f1.GetValue(pos)) - 
			            Polar(m2[i],f2[i]);
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

	void SpectrumSubstracter2::SubstractBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFComplexLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractBPFComplexLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"SubstractBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			SubstractBPFComplexLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFComplexLin(in2,in1,out);
			break;
		case Sloglog:
			SubstractBPFComplexLog(in2,in1,out);
			break;
		case Slinlog:
			SubstractBPFComplexLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"SubstractBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumSubstracter2::SubstractBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			TData BRe = fabs(m1.GetValue(pos)) * CLAM_cos(f1.GetValue(pos));
			TData BIm = fabs(m1.GetValue(pos)) * CLAM_sin(f1.GetValue(pos));
			co[i]= Complex(BRe,BIm) - c2[i];
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
	void SpectrumSubstracter2::SubstractBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			TData BRe = log2lin(fabs(m1.GetValue(pos)) * CLAM_cos(f1.GetValue(pos)));
			TData BIm = log2lin(fabs(m1.GetValue(pos)) * CLAM_sin(f1.GetValue(pos)));
			co[i]= Complex(BRe,BIm) - c2[i];
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


	void SpectrumSubstracter2::SubstractBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFPolarLin(in1,in2,out);
			break;
		case Sloglog:
			SubstractBPFPolarLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"SubstractBPFPolarLinLog: Not implemented");
			break;
		case Sloglin:
			SubstractBPFPolarLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumSubstracter2::SubstractPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			SubstractBPFPolarLin(in2,in1,out);
			break;
		case Sloglog:
			SubstractBPFPolarLog(in2,in1,out);
			break;
		case Slinlog:
			SubstractBPFPolarLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"SubstractBPFPolarLinLog: Not implemented");
			break;
		}
	}

	void SpectrumSubstracter2::SubstractBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			po[i]=Polar(m1.GetValue(pos),f1.GetValue(pos))-p2[i];
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

	void SpectrumSubstracter2::SubstractBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
		// it is missed. This could be optimised out by Substracting more
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
		for (int i=0;i<mSize;i++) {
			TData BMag = log2lin(m1.GetValue(pos));
			TData BPha = f1.GetValue(pos);
			po[i]=Polar(BMag,BPha)-p2[i];
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

	void SpectrumSubstracter2::SubstractBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		// First we check if the abcisas agree

		for (int i=0;i<mSize;i++) {
			Point &pm1=in1.GetMagBPF().GetPointArray()[i];
			Point &pm2=in2.GetMagBPF().GetPointArray()[i];
			Point &pmo=out.GetMagBPF().GetPointArray()[i];
			Point &pf1=in1.GetPhaseBPF().GetPointArray()[i];
			Point &pf2=in2.GetPhaseBPF().GetPointArray()[i];
			Point &pfo=out.GetPhaseBPF().GetPointArray()[i];
			CLAM_ASSERT(pm1.GetX() == pm2.GetX(),
				"SubstractBPF: BPF abcisas do not match "
					 "(and BPF merging not yet iplemented)");
			CLAM_ASSERT(pm1.GetX() == pmo.GetX(),
				"SubstractBPF: BPF abcisas do not match "
					 "(and BPF merging not yet iplemented)");
			pmo.SetY(pm1.GetY()/pm2.GetY());
			pfo.SetY(pf1.GetY()-pf2.GetY());
		}

	}

	// UNINMPLEMENTED METHODS. outme day...
	void SpectrumSubstracter2::SubstractMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractMagPhaseLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractMagPhaseLinLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractComplexLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractComplexLinLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractPolarLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractPolarLinLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFComplexLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFComplexLinLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFPolarLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFPolarLinLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFMagPhaseLog: Not implemented");
	}
	void SpectrumSubstracter2::SubstractBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"SubstractBPFMagPhaseLinLog: Not implemented");
	}
}

