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
#include "SpectrumAdder2.hxx"
#include "ErrProcessingObj.hxx"
#include "BPF.hxx"
#include "Point.hxx"

namespace CLAM {

	void SpecAdder2Config::DefaultInit()
	{
		if (!HasName()) {
			AddName();
			UpdateData();
		}
	}


	SpectrumAdder2::SpectrumAdder2()
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mProtoState(SOther)
	{
		Configure(SpecAdder2Config());
	}

	SpectrumAdder2::SpectrumAdder2(const SpecAdder2Config &c)
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mProtoState(SOther)
	{
		Configure(c);
	}

	std::string SpectrumAdder2::NewUniqueName()
	{
		static int ObjectCount=0;

		std::stringstream name;
		name << "SpectrumAdder2_" << ObjectCount++;

		return name.str();
	}

	bool SpectrumAdder2::ConcreteConfigure(const ProcessingConfig&c) throw(std::bad_cast)
	{
		// Nothing specific to configure here...
		mConfig = dynamic_cast<const SpecAdder2Config&>(c);
		
		return true;
	}

	// Unsupervised Do() function.
	bool SpectrumAdder2::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "SpectrumAdder2::Do(): Not in execution mode");

		if (GetExecState() == Disabled)
			return true;

		switch (mProtoState) {
		// Fast prototype configurations
		case SMagPhase:
			AddMagPhase(in1,in2,out);
			break;
		case SComplex:
			AddComplex(in1,in2,out);
			break;
		case SPolar:
			AddPolar(in1,in2,out);
			break;
		case SBPF:
			AddBPF(in1,in2,out);
			break;
		case SBPFMagPhase:
			AddBPFMagPhase(in1,in2,out);
			break;
		case SBPFComplex:
			AddBPFComplex(in1,in2,out);
			break;
		case SBPFPolar:
			AddBPFPolar(in1,in2,out);
			break;
		case SMagPhaseBPF:
			AddMagPhaseBPF(in1,in2,out);
			break;
		case SComplexBPF:
			AddComplexBPF(in1,in2,out);
			break;
		case SPolarBPF:
			AddPolarBPF(in1,in2,out);
			break;
		// Slow type configurations
		case SOther:
			Add(in1,in2,out);
			break;
		default:
			throw(ErrProcessingObj("Do(...) : internal inconsistency (invalid mProtoState)",this));
		}

		return true;
	}

	bool SpectrumAdder2::Do(void)
	{
		throw(ErrProcessingObj("SpectrumAdder2::Do(): Not implemented"),this);
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the sum computation. 
	bool SpectrumAdder2::SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out)
	{
		// Check common attributes
		SpecTypeFlags t1;
		in1.GetType(t1);
		SpecTypeFlags t2;
		in2.GetType(t2);
		SpecTypeFlags to;
		out.GetType(to);

		// Sanity check:
		if (!(t1.bMagPhase || t1.bComplex || t1.bPolar || t1.bMagPhaseBPF) ||
			!(t2.bMagPhase || t2.bComplex || t2.bPolar || t2.bMagPhaseBPF) ||
			!(to.bMagPhase || to.bComplex || to.bPolar || to.bMagPhaseBPF) )
			throw(ErrProcessingObj("SpectrumAdder2s:"
								   " Spectrum object with no attributes"));

		// Adder size. "pure" BPFs are not considered here.
		mSize = 0;
		if (t1.bMagPhase || t1.bComplex || t1.bPolar) {
			mSize = in1.GetSize();
			if (!mSize) 
				throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
									   " Zero size spectrum",this));
		}
		if (t2.bMagPhase || t2.bComplex || t2.bPolar)
			if (mSize) {
				if (mSize != in2.GetSize())
					throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
										   "Size mismatch in spectrum sum"
										   ,this));
			}
			else {
				mSize = in2.GetSize();
				if (!mSize) 
					throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
										   " Zero size spectrum",this));
			}
		if (to.bMagPhase || to.bComplex || to.bPolar)
			if (mSize) {
				if (mSize != out.GetSize())
					throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
										   "Size mismatch in spectrum sum"
										   ,this));
			}
			else {
				mSize = out.GetSize();
				if (!mSize)
					throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
										   " Zero size spectrum",this));
			}

		// Spectral Range.  
		// We could also ignore BPF-only objects here, but in
		// practice, if a BPF is designed for a certain spectral
		// range, error will probably be too big out of the range, out
		// we always force range matching
		if (in1.GetSpectralRange() != in2.GetSpectralRange() ||
			in1.GetSpectralRange() != out.GetSpectralRange() )
			throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
								   "Spectral range mismatch in spectrum sum"
								   ,this));

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
		if (out.GetScale() == EScale::eLog)
			throw(ErrProcessingObj("SpectrumAdder2:"
								   " Log Scale Output not implemented",this));

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
			throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
								   " Data flags internal inconsistency",this));
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
			throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes:"
								   " invalid data flags",this));
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


	bool SpectrumAdder2::SetPrototypes()
	{
		throw(ErrProcessingObj("SpectrumAdder2::SetPrototypes(): Not implemented"),this);
	}

	bool SpectrumAdder2::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumAdder2::Add(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(in1,in2,out);
		Do(in1,in2,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumAdder2::AddMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			AddMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			AddMagPhaseLinLog(in1,in2,out);
			break;
		case Sloglin:
			AddMagPhaseLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumAdder2::AddMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because outme of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
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
			Polar po=Polar(m1[i],f1[i])+Polar(m2[i],f2[i]);
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

	void SpectrumAdder2::AddComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddComplexLin(in1,in2,out);
			break;
		case Sloglog:
			AddComplexLog(in1,in2,out);
			break;
		case Slinlog:
			AddComplexLinLog(in1,in2,out);
			break;
		case Sloglin:
			AddComplexLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumAdder2::AddComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because outme of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
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
			co[i]=c1[i]+c2[i];

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


	void SpectrumAdder2::AddPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddPolarLin(in1,in2,out);
			break;
		case Sloglog:
			AddPolarLog(in1,in2,out);
			break;
		case Slinlog:
			AddPolarLinLog(in1,in2,out);
			break;
		case Sloglin:
			AddPolarLinLog(in2,in1,out);
			break;
		}
	}

	void SpectrumAdder2::AddPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because outme of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, out we must check and instantiate the attribute
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
			po[i]=p1[i]+p2[i];

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


	void SpectrumAdder2::AddBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFMagPhaseLin(in1,in2,out);
			break;
		case Sloglog:
			AddBPFMagPhaseLog(in1,in2,out);
			break;
		case Slinlog:
			throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
			break;
		case Sloglin:
			AddBPFMagPhaseLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumAdder2::AddMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFMagPhaseLin(in2,in1,out);
			break;
		case Sloglog:
			AddBPFMagPhaseLog(in2,in1,out);
			break;
		case Slinlog:
			AddBPFMagPhaseLogLin(in2,in1,out);
			break;
		case Sloglin:
			throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
			break;
		}
	}

	void SpectrumAdder2::AddBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			Polar po = Polar(m1.GetValue(pos),f1.GetValue(pos)) + 
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

	void SpectrumAdder2::AddBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			Polar po = Polar(pow(TData(10),m1.GetValue(pos)/TData(10.0)),f1.GetValue(pos)) + 
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

	void SpectrumAdder2::AddBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFComplexLin(in1,in2,out);
			break;
		case Sloglog:
			AddBPFComplexLog(in1,in2,out);
			break;
		case Slinlog:
			throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
			break;
		case Sloglin:
			AddBPFComplexLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumAdder2::AddComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFComplexLin(in2,in1,out);
			break;
		case Sloglog:
			AddBPFComplexLog(in2,in1,out);
			break;
		case Slinlog:
			AddBPFComplexLogLin(in2,in1,out);
			break;
		case Sloglin:
			throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
			break;
		}
	}

	void SpectrumAdder2::AddBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			TData BRe = fabs(m1.GetValue(pos)) * cos(f1.GetValue(pos));
			TData BIm = fabs(m1.GetValue(pos)) * sin(f1.GetValue(pos));
			co[i]= Complex(BRe,BIm) + c2[i];
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
	void SpectrumAdder2::AddBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			TData BRe = pow(TData(10),fabs(m1.GetValue(pos))/TData(10.0)) * cos(f1.GetValue(pos));
			TData BIm = pow(TData(10),fabs(m1.GetValue(pos))/TData(10.0)) * sin(f1.GetValue(pos));
			co[i]= Complex(BRe,BIm) + c2[i];
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


	void SpectrumAdder2::AddBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFPolarLin(in1,in2,out);
			break;
		case Sloglog:
			AddBPFPolarLog(in1,in2,out);
			break;
		case Slinlog:
			throw(ErrProcessingObj("AddBPFPolarLinLog: Not implemented"));
			break;
		case Sloglin:
			AddBPFPolarLogLin(in1,in2,out);
			break;
		}
	}

	void SpectrumAdder2::AddPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		switch(mScaleState) {
		case Slinlin:
			AddBPFPolarLin(in2,in1,out);
			break;
		case Sloglog:
			AddBPFPolarLog(in2,in1,out);
			break;
		case Slinlog:
			AddBPFPolarLogLin(in2,in1,out);
			break;
		case Sloglin:
			throw(ErrProcessingObj("AddBPFPolarLinLog: Not implemented"));
			break;
		}
	}

	void SpectrumAdder2::AddBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			po[i]=Polar(m1.GetValue(pos),f1.GetValue(pos))+p2[i];
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

	void SpectrumAdder2::AddBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and outme of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, out we must check and instantiate the attribute it
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
			TData BMag = pow(TData(10),m1.GetValue(pos)/TData(10.0));
			TData BPha = f1.GetValue(pos);
			po[i]=Polar(BMag,BPha)+p2[i];
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

	void SpectrumAdder2::AddBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		// First we check if the abcisas agree

		for (int i=0;i<mSize;i++) {
			Point &pm1=in1.GetMagBPF().GetPointArray()[i];
			Point &pm2=in2.GetMagBPF().GetPointArray()[i];
			Point &pmo=out.GetMagBPF().GetPointArray()[i];
			Point &pf1=in1.GetPhaseBPF().GetPointArray()[i];
			Point &pf2=in2.GetPhaseBPF().GetPointArray()[i];
			Point &pfo=out.GetPhaseBPF().GetPointArray()[i];
			if (pm1.GetX() != pm2.GetX() ||
				pm1.GetX() != pmo.GetX() )
				throw(ErrProcessingObj("AddBPF: BPF abcisas do not match "
								 "(and BPF merging not yet iplemented)",this));
			pmo.SetY(pm1.GetY()*pm2.GetY());
			pfo.SetY(pf1.GetY()+pf2.GetY());
		}

	}

	// UNINMPLEMENTED METHODS. outme day...
	void SpectrumAdder2::AddMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddMagPhaseLog: Not implemented"));
	}
	void SpectrumAdder2::AddMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddMagPhaseLinLog: Not implemented"));
	}
	void SpectrumAdder2::AddComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddComplexLog: Not implemented"));
	}
	void SpectrumAdder2::AddComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddComplexLinLog: Not implemented"));
	}
	void SpectrumAdder2::AddPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddPolarLog: Not implemented"));
	}
	void SpectrumAdder2::AddPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddPolarLinLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFComplexLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFComplexLinLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFPolarLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFPolarLinLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFMagPhaseLog: Not implemented"));
	}
	void SpectrumAdder2::AddBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
	}
}
