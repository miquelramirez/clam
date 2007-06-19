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
#include "SpectrumProduct.hxx"
#include "SpectrumConfig.hxx"
#include "Factory.hxx"

namespace CLAM 
{

	namespace detail
	{
		static Factory<Processing>::Registrator<SpectrumProduct> regSpectrumProduct("SpectrumProduct");
	}

	SpectrumProduct::SpectrumProduct(const Config &c)
		: mSize(0),
		  mProtoState(SOther),
		  mInput1("Input 1",this),
		  mInput2("Input 2",this),
		  mOutput("Output",this)
	{
		Configure(c);
	}

	SpectrumProduct::~SpectrumProduct() 
	{
	}

	bool SpectrumProduct::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_DEBUG_ASSERT(IsRunning(),
		                  "SpectrumProduct::Do(): Not in execution mode");

		switch (mProtoState) {
		// Fast prototype configurations
		case SMagPhase:
			MultiplyMagPhase(in1,in2,out);
			break;
		case SComplex:
			MultiplyComplex(in1,in2,out);
			break;
		case SPolar:
			MultiplyPolar(in1,in2,out);
			break;
		case SBPF:
			MultiplyBPF(in1,in2,out);
			break;
		case SBPFMagPhase:
			MultiplyBPFMagPhase(in1,in2,out);
			break;
		case SBPFComplex:
			MultiplyBPFComplex(in1,in2,out);
			break;
		case SBPFPolar:
			MultiplyBPFPolar(in1,in2,out);
			break;
		case SMagPhaseBPF:
			MultiplyMagPhaseBPF(in1,in2,out);
			break;
		case SComplexBPF:
			MultiplyComplexBPF(in1,in2,out);
			break;
		case SPolarBPF:
			MultiplyPolarBPF(in1,in2,out);
			break;
		// Slow prototype configurations
		case SOther:
			Multiply(in1,in2,out);
			break;
		default:
			CLAM_ASSERT(false,"Do(...) : internal inconsistency (invalid mProtoState)");
		}

		return true;
	}

	bool SpectrumProduct::Do()
	{
		bool res = Do(mInput1.GetData(),mInput2.GetData(),mOutput.GetData());
		mInput1.Consume();
		mInput2.Consume();
		mOutput.Produce();
		return res;
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the product computation. 
	bool SpectrumProduct::SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out)
	{
		// Check common attributes
		SpectrumConfig s1;
		in1.GetConfig(s1);
		SpectrumConfig s2;
		in2.GetConfig(s2);
		SpectrumConfig so;
		out.GetConfig(so);
		SpecTypeFlags t1;
		in1.GetType(t1);
		SpecTypeFlags t2;
		in2.GetType(t2);
		SpecTypeFlags to;
		out.GetType(to);

		// Sanity check:
		CLAM_ASSERT(t1.bMagPhase || t1.bComplex || t1.bPolar || t1.bMagPhaseBPF,
			"SpectrumProducts: First input Spectrum without content");
		CLAM_ASSERT(t2.bMagPhase || t2.bComplex || t2.bPolar || t2.bMagPhaseBPF,
			"SpectrumProducts: Second input Spectrum without content");
		CLAM_ASSERT(to.bMagPhase || to.bComplex || to.bPolar || to.bMagPhaseBPF,
			"SpectrumProducts: Output Spectrum object without content");

		// Product size. "pure" BPFs are not considered here.
		mSize = 0;
		if (t1.bMagPhase || t1.bComplex || t1.bPolar) {
			mSize = in1.GetSize();
			CLAM_ASSERT(mSize,"SpectrumProduct::SetPrototypes: Zero size spectrum");
		}
		if (t2.bMagPhase || t2.bComplex || t2.bPolar) 
		{
			CLAM_ASSERT(in2.GetSize(),"SpectrumProduct::SetPrototypes: Zero size spectrum");
			if (!mSize) mSize = in2.GetSize();
			else CLAM_ASSERT(mSize == in2.GetSize(),
				"SpectrumProduct::SetPrototypes: Sizes Mismatch");
		}
		if (to.bMagPhase || to.bComplex || to.bPolar)
		{
			CLAM_ASSERT(out.GetSize(),"SpectrumProduct::SetPrototypes: Zero size spectrum");
			if (!mSize) mSize = out.GetSize();
			else CLAM_ASSERT(mSize == out.GetSize(),
				"SpectrumProduct::SetPrototypes: Output size Mismatch");
		}

		// Spectral Range.  
		// We could also ignore BPF-only objects here, but in
		// practice, if a BPF is designed for a certain spectral
		// range, error will probably be too big out of the range, so
		// we always force range matching
		CLAM_ASSERT(in1.GetSpectralRange() == in2.GetSpectralRange(),
			"SpectrumProduct::SetPrototypes: Spectral range mismatch on inputs");
		CLAM_ASSERT(in1.GetSpectralRange() == out.GetSpectralRange(),
			"SpectrumProduct::SetPrototypes: Spectral range mismatch on output");

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
		CLAM_ASSERT(out.GetScale() != EScale::eLog,
			"SpectrumProduct: Log Scale Output not implemented");

		// Prototypes.

		// BPF PRODUCTS.
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
				"SpectrumProduct::SetPrototypes: Data flags internal inconsistency");
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
			}
			if (t1.bPolar || to.bPolar) {
				mProtoState=SPolarBPF;
				return true;
			}
			// Should never get here:
			CLAM_ASSERT(false, "SpectrumProduct::SetPrototypes:"
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


	bool SpectrumProduct::SetPrototypes()
	{
		CLAM_ASSERT(false,"SpectrumProduct::SetPrototypes(): Not implemented");
		return true;
	}

	bool SpectrumProduct::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumProduct::Multiply(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(in1,in2,out);
		Do(in1,in2,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumProduct::MultiplyMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			MultiplyMagPhaseLinLog(in1,in2,out);
			break;
		case Sloglin:
			MultiplyMagPhaseLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumProduct::MultiplyMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because some of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
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
			mo[i]=m1[i]*m2[i];
			fo[i]=f1[i]+f2[i];
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

	void SpectrumProduct::MultiplyComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyComplexLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyComplexLog(in1,in2,out);
			break;
		case Slinlog:
			MultiplyComplexLinLog(in1,in2,out);
			break;
		case Sloglin:
			MultiplyComplexLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumProduct::MultiplyComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
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
			co[i]=c1[i]*c2[i];

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


	void SpectrumProduct::MultiplyPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyPolarLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyPolarLog(in1,in2,out);
			break;
		case Slinlog:
			MultiplyPolarLinLog(in1,in2,out);
			break;
		case Sloglin:
			MultiplyPolarLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumProduct::MultiplyPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
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
			po[i]=p1[i]*p2[i];

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


	void SpectrumProduct::MultiplyBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyBPFMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"MultiplyBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			MultiplyBPFMagPhaseLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumProduct::MultiplyMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFMagPhaseLin(in2,in1,out);
			break;
		case Sloglog:
			MultiplyBPFMagPhaseLog(in2,in1,out);
			break;
		case Slinlog:
			MultiplyBPFMagPhaseLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"MultiplyBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumProduct::MultiplyBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			mo[i]=m1.GetValue(pos)*m2[i];
			fo[i]=f1.GetValue(pos)+f2[i];
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

	void SpectrumProduct::MultiplyBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			mo[i]=TData(pow(10.0,m1.GetValue(pos)/10.0))*m2[i];
			fo[i]=f1.GetValue(pos)+f2[i];
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

	void SpectrumProduct::MultiplyBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFComplexLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyBPFComplexLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"MultiplyBPFMagPhaseLinLog: Not implemented");
			break;
		case Sloglin:
			MultiplyBPFComplexLogLin(in1,in2,out);
			break;
		}
	}
	void SpectrumProduct::MultiplyComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFComplexLin(in2,in1,out);
			break;
		case Sloglog:
			MultiplyBPFComplexLog(in2,in1,out);
			break;
		case Slinlog:
			MultiplyBPFComplexLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"MultiplyBPFMagPhaseLinLog: Not implemented");
			break;
		}
	}

	void SpectrumProduct::MultiplyBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			TData BRe = TData(fabs(m1.GetValue(pos)) * cos(f1.GetValue(pos)));
			TData BIm = TData(fabs(m1.GetValue(pos)) * sin(f1.GetValue(pos)));
			co[i]= Complex(BRe,BIm) * c2[i];
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
	void SpectrumProduct::MultiplyBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			TData BRe = TData(pow(10.0,fabs(m1.GetValue(pos))/10.0) * cos(f1.GetValue(pos)));
			TData BIm = TData(pow(10.0,fabs(m1.GetValue(pos))/10.0) * sin(f1.GetValue(pos)));
			co[i]= Complex(BRe,BIm) * c2[i];
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


	void SpectrumProduct::MultiplyBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFPolarLin(in1,in2,out);
			break;
		case Sloglog:
			MultiplyBPFPolarLog(in1,in2,out);
			break;
		case Slinlog:
			CLAM_ASSERT(false,"MultiplyBPFPolarLinLog: Not implemented");
			break;
		case Sloglin:
			MultiplyBPFPolarLogLin(in1,in2,out);
			break;
		}
	}
	void SpectrumProduct::MultiplyPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			MultiplyBPFPolarLin(in2,in1,out);
			break;
		case Sloglog:
			MultiplyBPFPolarLog(in2,in1,out);
			break;
		case Slinlog:
			MultiplyBPFPolarLogLin(in2,in1,out);
			break;
		case Sloglin:
			CLAM_ASSERT(false,"MultiplyBPFPolarLinLog: Not implemented");
			break;
		}
	}

	void SpectrumProduct::MultiplyBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			po[i]=Polar(m1.GetValue(pos),f1.GetValue(pos))*p2[i];
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

	void SpectrumProduct::MultiplyBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
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
			TData BMag = TData(pow(10.0,m1.GetValue(pos)/10.0));
			TData BPha = f1.GetValue(pos);
			po[i]=Polar(BMag,BPha)*p2[i];
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

	void SpectrumProduct::MultiplyBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		// First we check if the abcisas agree
		CLAM_ASSERT(false,"MultiplyBPF: method not implemented");
		for (int i=0;i<mSize;i++) {
			PointTmpl<TData,TData> &pm1=in1.GetMagBPF().GetPointArray()[i];
			PointTmpl<TData,TData> &pm2=in2.GetMagBPF().GetPointArray()[i];
			PointTmpl<TData,TData> &pmo=out.GetMagBPF().GetPointArray()[i];
			PointTmpl<TData,TData> &pf1=in1.GetPhaseBPF().GetPointArray()[i];
			PointTmpl<TData,TData> &pf2=in2.GetPhaseBPF().GetPointArray()[i];
			PointTmpl<TData,TData> &pfo=out.GetPhaseBPF().GetPointArray()[i];
			CLAM_ASSERT(pm1.GetX() == pm2.GetX(),
				"MultiplyBPF: input BPF abcisas do not match "
				 "(and BPF merging not yet iplemented)");
			CLAM_ASSERT(pm1.GetX() == pmo.GetX(),
				"MultiplyBPF: output BPF abcisas do not match "
				 "(and BPF merging not yet iplemented)");
			pmo.SetY(pm1.GetY()*pm2.GetY());
			pfo.SetY(pf1.GetY()+pf2.GetY());
		}

	}

	void SpectrumProduct::MultiplyMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyMagPhaseLog: Not implemented");
	}
	void SpectrumProduct::MultiplyMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyMagPhaseLinLog: Not implemented");
	}
	void SpectrumProduct::MultiplyComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyComplexLog: Not implemented");
	}
	void SpectrumProduct::MultiplyComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyComplexLinLog: Not implemented");
	}
	void SpectrumProduct::MultiplyPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyPolarLog: Not implemented");
	}
	void SpectrumProduct::MultiplyPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyPolarLinLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFComplexLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFComplexLinLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFPolarLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFPolarLinLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFMagPhaseLog: Not implemented");
	}
	void SpectrumProduct::MultiplyBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_ASSERT(false,"MultiplyBPFMagPhaseLinLog: Not implemented");
	}
}

