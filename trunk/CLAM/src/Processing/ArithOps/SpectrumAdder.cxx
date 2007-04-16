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
#include "SpectrumAdder.hxx"
#include "SpectrumConfig.hxx"

#include <sstream>

namespace CLAM {

	void SpecAdderConfig::DefaultInit()
	{
		AddNInputs();
		UpdateData();
		SetNInputs(0);
	}


	SpectrumAdder::SpectrumAdder()
		: mSize(0),
		  mInputs(0),
		  complex_bufs(0),polar_bufs(0),mag_bufs(0),phase_bufs(0),
		  remove(0),
		  mOut("Output",this),
		  mProtoState(SOther)
	{
		Configure(SpecAdderConfig());
	}

	SpectrumAdder::SpectrumAdder(const SpecAdderConfig &c)
		: mSize(0),
		  mInputs(0),
		  complex_bufs(0),polar_bufs(0),mag_bufs(0),phase_bufs(0),
		  remove(0),
		  mOut("Output",this),
		  mProtoState(SOther)
	{
		Configure(c);
	}

	std::string SpectrumAdder::NewUniqueName()
	{
		static int ObjectCount=0;

		std::stringstream name;

		name << "SpectrumAdder_" << ObjectCount++;

		return name.str();
	}

	bool SpectrumAdder::ConcreteConfigure(const ProcessingConfig&c)
	{
		int oldNInputs;

		// Nothing specific to configure here...
		if (mConfig.HasNInputs())
			oldNInputs = mConfig.GetNInputs();
		else
			oldNInputs = 0;

		CopyAsConcreteConfig(mConfig, c);
		if ( !mConfig.HasNInputs() ||
			 mConfig.GetNInputs() <= 0 ) {
			mNInputs = 0;
			return false;
		}
			
		mNInputs = mConfig.GetNInputs();

		if (mNInputs == oldNInputs)
			return true;


		if (mInputs)
			delete(mInputs);

		if (complex_bufs) {
			delete(complex_bufs);
			complex_bufs=0;
		}
		if (polar_bufs) {
			delete(polar_bufs);
			polar_bufs=0;
		}
		if (mag_bufs) {
			delete(mag_bufs);
			mag_bufs=0;
		}
		if (phase_bufs) {
			delete(phase_bufs);
			phase_bufs=0;
		}
		if (remove) {
			delete(remove);
			remove=0;
		}

		mInputs = new InPort<Spectrum>*[mNInputs];
		complex_bufs = new Complex*[mNInputs];
		polar_bufs = new Polar*[mNInputs];
		mag_bufs = new TData*[mNInputs];
		phase_bufs = new TData*[mNInputs];
		remove = new bool[mNInputs];

		for (int i=0; i<mNInputs; i++) {
			std::stringstream name;
			name << "Input " << i;
			mInputs[i]=new InPort<Spectrum>(name.str(),this);
		}

		return true;
	}

	SpectrumAdder::~SpectrumAdder()
	{
		if (mInputs)
			delete(mInputs);

		if (complex_bufs) {
			delete(complex_bufs);
			complex_bufs=0;
		}
		if (polar_bufs) {
			delete(polar_bufs);
			polar_bufs=0;
		}
		if (mag_bufs) {
			delete(mag_bufs);
			mag_bufs=0;
		}
		if (phase_bufs) {
			delete(phase_bufs);
			phase_bufs=0;
		}
		if (remove) {
			delete(remove);
			remove=0;
		}
	}


	// Unsupervised Do() function.
	bool SpectrumAdder::Do(Spectrum **inputs, Spectrum& out)
	{
		CLAM_DEBUG_ASSERT( GetExecState() == Running,
			"SpectrumAdder::Do(): Not in execution mode");

		switch (mProtoState) {
		// Fast prototype configurations
		case SMagPhase:
			AddMagPhase(inputs,out);
			break;
		case SComplex:
			AddComplex(inputs,out);
			break;
		case SPolar:
			AddPolar(inputs,out);
			break;
		case ShasBPF:
			CLAM_ASSERT(false,"SpectrumAdder::Do(): BPF addition not implemented");
			break;
		// Slow type configurations
		case SOther:
			Add(inputs,out);
			break;
		default:
			CLAM_ASSERT(false,"Do(...) : internal inconsistency (invalid mProtoState)");
		}

		return true;
	}

	bool SpectrumAdder::Do(void)
	{
		CLAM_ASSERT(false,"SpectrumAdder::Do(): Not implemented");
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the sum computation. 
	bool SpectrumAdder::SetPrototypes(Spectrum **inputs,const Spectrum& out)
	{
		CLAM_ASSERT(GetExecState() != Unconfigured,
		            "SpectrumAdder::SetPrototypes(): Not configured");
		CLAM_ASSERT(mNInputs > 0,
		            "SpectrumAdder::SetPrototypes(): Inconsistent mNInputs");

		// Check common attributes
		SpectrumConfig *si = new SpectrumConfig[mNInputs];
		SpectrumConfig so;
		SpecTypeFlags *ti = new SpecTypeFlags[mNInputs];
		SpecTypeFlags to;

		for (int i=0; i<mNInputs; i++) {
			inputs[i]->GetConfig(si[i]);
			inputs[i]->GetType(ti[i]);
		}
		out.GetConfig(so);
		out.GetType(to);


		// Do we have the necesary attributes?
		CLAM_BEGIN_CHECK
			for (int i=0;i<mNInputs; i++)
				CLAM_ASSERT(ti[i].bMagPhase || ti[i].bComplex || ti[i].bPolar,
					"SpectrumAdders: Output spectrum object with no non-BPF attributes");
			CLAM_ASSERT(to.bMagPhase || to.bComplex || to.bPolar,
				"SpectrumAdders: Output spectrum object with no non-BPF attributes");
			// We check that the size, the spectral range and the scale of the arrays all match.
			CLAM_ASSERT(so.GetSize(), "SpectrumAdder::SetPrototypes: Zero size spectrum output");
		CLAM_END_CHECK

		mSize=so.GetSize();
		TData range = so.GetSpectralRange();
		EScale scale = so.GetScale();

		CLAM_BEGIN_CHECK
		for (int i=0; i<mNInputs; i++) {
			CLAM_ASSERT(mSize == si[i].GetSize(),
				"SpectrumAdder::SetPrototypes: Size mismatch in spectrum sum");
			CLAM_ASSERT(range == si[i].GetSpectralRange(),
				"SpectrumAdder::SetPrototypes: Spectral range mismatch in spectrum sum");
			CLAM_ASSERT(scale == si[i].GetScale(),
				"SpectrumAdder::SetPrototypes: Scale mismatch in spectrum sum");
		}
		CLAM_END_CHECK

		if (scale == EScale::eLinear)
			mScaleState=Slin;
		else
			mScaleState=Slog;

		// Prototypes.

		// We first count how many inputs have each type of attribute
		// instantiated.
		int Ncomplex=0,Npolar=0,Nmagphase=0;
		for (int i=0; i<mNInputs;i++) {
			if (ti[i].bMagPhase)
				Nmagphase++;
			if (ti[i].bComplex)
				Ncomplex++;
			if (ti[i].bPolar)
				Npolar++;
		}
		if (to.bMagPhase)
			Nmagphase++;
		if (to.bComplex)
			Ncomplex++;
		if (to.bPolar)
			Npolar++;

		// Now we look for the best choice.
		for (int i=mNInputs+1; i>0; i--) {
			if (Nmagphase == i) {
				mProtoState=SMagPhase;
				return true;
			}
			if (Ncomplex == i) {
				mProtoState=SComplex;
				return true;
			}
			if (Npolar == i) {
				mProtoState=SPolar;
				return true;
			}
		}
		CLAM_ASSERT(false,"SpectrumAdder::SetPrototypes: Prototype inconsistency");
	}


	bool SpectrumAdder::SetPrototypes()
	{
		CLAM_ASSERT(false,"SpectrumAdder::SetPrototypes(): Not implemented");
	}

	bool SpectrumAdder::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumAdder::Add(Spectrum **inputs, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(inputs,out);
		Do(inputs,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumAdder::AddMagPhase(Spectrum **inputs, Spectrum& out)
	{
		switch(mScaleState) {
		case Slin:
			AddMagPhaseLin(inputs,out);
			break;
		case Slog:
			AddMagPhaseLog(inputs,out);
			break;
		}
	}

	void SpectrumAdder::AddMagPhaseLin(Spectrum **inputs, Spectrum& out)
	{
		bool removeo=false;
		SpecTypeFlags f;
		int i;

		// This function was choosed because some of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// if it is missing.
		for (i=0;i<mNInputs; i++) {
			inputs[i]->GetType(f);
			if (!f.bMagPhase) {
				remove[i]=true;
				f.bMagPhase=true;
				inputs[i]->SetTypeSynchronize(f);
			}
			else
				remove[i]=false;
			mag_bufs[i] = inputs[i]->GetMagBuffer().GetPtr();
			phase_bufs[i] = inputs[i]->GetPhaseBuffer().GetPtr();
			
		}
		out.GetType(f);
		if (!f.bMagPhase) {
			removeo=true;
			f.bMagPhase=true;
			out.SetType(f);
		}
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();

		for (int s=0;s<mSize; s++) {
			TData re=0.0,im=0.0;
			for (i=0;i<mNInputs; i++) {
				re+=mag_bufs[i][s]*TData(cos(phase_bufs[i][s]));
				im+=mag_bufs[i][s]*TData(sin(phase_bufs[i][s]));
			}
			mo[s]=TData(sqrt(re*re+im*im));
			fo[s]=TData(atan2(im,re));
		}

		f.bComplex=f.bPolar=f.bMagPhaseBPF=false;
		f.bMagPhase=true;
		out.SynchronizeTo(f);

		for (i=0; i<mNInputs; i++)
			if (remove[i]) {
				inputs[i]->RemoveMagBuffer();
				inputs[i]->RemovePhaseBuffer();
				inputs[i]->UpdateData();
			}
		if (removeo) {
			out.RemoveMagBuffer();
			out.RemovePhaseBuffer();
			out.UpdateData();
		}

	}

	void SpectrumAdder::AddComplex(Spectrum **inputs, Spectrum& out)
	{
		switch(mScaleState) {
		case Slin:
			AddComplexLin(inputs,out);
			break;
		case Slog:
			AddComplexLog(inputs,out);
			break;
		}
	}

	void SpectrumAdder::AddComplexLin(Spectrum **inputs, Spectrum& out)
	{
		bool removeo=false;
		SpecTypeFlags f;
		int i;

		// This function was choosed because some of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		for (i=0;i<mNInputs; i++) {
			inputs[i]->GetType(f);
			if (!f.bComplex) {
				remove[i]=true;
				f.bComplex=true;
				inputs[i]->SetTypeSynchronize(f);
			}
			else
				remove[i]=false;
			complex_bufs[i] = inputs[i]->GetComplexArray().GetPtr();
			
		}
		out.GetType(f);
		if (!f.bComplex) {
			removeo=true;
			f.bComplex=true;
			out.SetType(f);
		}
		Complex *co = out.GetComplexArray().GetPtr();

		for (int s=0;s<mSize;s++) {
			co[s]=0;
			for (i=0;i<mNInputs; i++)
				co[s]+=complex_bufs[i][s];
		}

		f.bMagPhase=f.bPolar=f.bMagPhaseBPF=false;
		f.bComplex=true;
		out.SynchronizeTo(f);

		for (i=0; i<mNInputs; i++)
			if (remove[i]) {
				inputs[i]->RemoveComplexArray();
				inputs[i]->UpdateData();
			}
		if (removeo) {
			out.RemoveComplexArray();
			out.UpdateData();
		}
	}


	void SpectrumAdder::AddPolar(Spectrum **inputs, Spectrum& out)
	{
		switch(mScaleState) {
		case Slin:
			AddPolarLin(inputs,out);
			break;
		case Slog:
			AddPolarLog(inputs,out);
			break;
		}
	}

	void SpectrumAdder::AddPolarLin(Spectrum **inputs, Spectrum& out)
	{
		bool removeo=false;
		SpecTypeFlags f;
		int i;
		
		// This function was choosed because some of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		for (i=0;i<mNInputs; i++) {
			inputs[i]->GetType(f);
			if (!f.bPolar) {
				remove[i]=true;
				f.bPolar=true;
				inputs[i]->SetTypeSynchronize(f);
			}
			else
				remove[i]=false;
			polar_bufs[i] = inputs[i]->GetPolarArray().GetPtr();
			
		}
		out.GetType(f);
		if (!f.bPolar) {
			removeo=true;
			f.bPolar=true;
			out.SetType(f);
		}

		Polar *po = out.GetPolarArray().GetPtr();
		for (int s=0;s<mSize;s++) {
			po[s]=Polar(0.0,0.0);
			for (i=0;i<mNInputs;i++)
				po[s]+=polar_bufs[i][s];
		}

		f.bComplex=f.bMagPhase=f.bMagPhaseBPF=false;
		f.bPolar=true;
		out.SynchronizeTo(f);

		for (i=0; i<mNInputs; i++)
			if (remove[i]) {
				inputs[i]->RemovePolarArray();
				inputs[i]->UpdateData();
			}
		if (removeo) {
			out.RemovePolarArray();
			out.UpdateData();
		}
	}


	// UNINMPLEMENTED METHODS. Some day...
	void SpectrumAdder::AddMagPhaseLog(Spectrum **inputs, Spectrum& out)
	{
		CLAM_ASSERT(false,"AddMagPhaseLog: Not implemented");
	}
	void SpectrumAdder::AddComplexLog(Spectrum **inputs, Spectrum& out)
	{
		CLAM_ASSERT(false,"AddComplexLog: Not implemented");
	}
	void SpectrumAdder::AddPolarLog(Spectrum **inputs, Spectrum& out)
	{
		CLAM_ASSERT(false,"AddPolarLog: Not implemented");
	}
}

