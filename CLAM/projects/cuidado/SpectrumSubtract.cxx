#include "Complex.hxx"
#include "SpectrumSubtract.hxx"
#include "ErrProcessingObj.hxx"

namespace MTG {

	void SpectrumSubtractConfig::Init()
	{
		if (!HasName()) {
			AddName();
			UpdateData();
		}
	}


	SpectrumSubtract::SpectrumSubtract()
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mProtoState(SOther)
	{
		Configure(SpectrumSubtractConfig());
	}

	SpectrumSubtract::SpectrumSubtract(const SpectrumSubtractConfig &c)
		: mSize(0),
		  mIn1("Input 1",this,1),
		  mIn2("Input 2",this,1),
		  mOut("Output",this,1),
		  mProtoState(SOther)
	{
		Configure(c);
	}

	std::string SpectrumSubtract::NewUniqueName()
	{
		static int ObjectCount=0;

		std::stringstream name;
		name << "SpectrumSubtract_" << ObjectCount++;
    
		return name.str();
	}

	bool SpectrumSubtract::ConcreteConfigure(const ProcessingObjectConfig&c) throw(std::bad_cast)
	{
		// Nothing specific to configure here...
		mConfig = dynamic_cast<const SpectrumSubtractConfig&>(c);
		
		return true;
	}

	// Unsupervised Do() function.
	bool SpectrumSubtract::Do(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
#		ifdef PARANOID
			if ( GetExecState() == Unconfigured ||
				 GetExecState() == Ready )
				throw(ErrProcessingObj("SpectrumSubtract::Do(): Not in execution mode",this));
#		endif

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

	bool SpectrumSubtract::Do(void)
	{
		throw(ErrProcessingObj("SpectrumSubtract::Do(): Not implemented"),this);
	}

	// This function analyses the inputs and decides which prototypes to use 
	// For the sum computation. 
	bool SpectrumSubtract::SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out)
	{
		// Check common attributes
		const SpectrumConfig s1 = in1.GetConfig();
		const SpectrumConfig s2 = in2.GetConfig();
		const SpectrumConfig so = out.GetConfig();
		const SpecTypeFlags t1 = s1.GetType();
		const SpecTypeFlags t2 = s2.GetType();
		const SpecTypeFlags to = so.GetType();

		// Sanity check:
		if (!(t1.bMagPhase || t1.bComplex || t1.bPolar || t1.bMagPhaseBPF) ||
			!(t2.bMagPhase || t2.bComplex || t2.bPolar || t2.bMagPhaseBPF) ||
			!(to.bMagPhase || to.bComplex || to.bPolar || to.bMagPhaseBPF) )
			throw(ErrProcessingObj("SpectrumSubtracts:"
								   " Spectrum object with no attributes"));

		// Adder size. "pure" BPFs are not considered here.
		mSize = 0;
		if (t1.bMagPhase || t1.bComplex || t1.bPolar) {
			mSize = s1.GetSize();
			if (!mSize) 
				throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
									   " Zero size spectrum",this));
		}
		if (t2.bMagPhase || t2.bComplex || t2.bPolar)
			if (mSize) {
				if (mSize != s2.GetSize())
					throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
										   "Size mismatch in spectrum sum"
										   ,this));
			}
			else {
				mSize = s2.GetSize();
				if (!mSize) 
					throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
										   " Zero size spectrum",this));
			}
		if (to.bMagPhase || to.bComplex || to.bPolar)
			if (mSize) {
				if (mSize != so.GetSize())
					throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
										   "Size mismatch in spectrum sum"
										   ,this));
			}
			else {
				mSize = so.GetSize();
				if (!mSize)
					throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
										   " Zero size spectrum",this));
			}

		// Spectral Range.  
		// We could also ignore BPF-only objects here, but in
		// practice, if a BPF is designed for a certain spectral
		// range, error will probably be too big out of the range, so
		// we always force range matching
		if (s1.GetSpectralRange() != s2.GetSpectralRange() ||
			s1.GetSpectralRange() != so.GetSpectralRange() )
			throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
								   "Spectral range mismatch in spectrum sum"
								   ,this));

		// Scale.
		if (s1.GetScale() == EScale::eLinear)
			if (s2.GetScale() == EScale::eLinear)
				mScaleState=Slinlin;
			else
				mScaleState=Slinlog;
		else
			if (s2.GetScale() == EScale::eLinear)
				mScaleState=Sloglin;
			else
				mScaleState=Sloglog;
		// Log scale output might be useful, for example when working
		// with BPF objects at the three ports. But right for now...
		if (so.GetScale() == EScale::eLog)
			throw(ErrProcessingObj("SpectrumSubtract:"
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
			throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
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
			throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes:"
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


	bool SpectrumSubtract::SetPrototypes()
	{
		throw(ErrProcessingObj("SpectrumSubtract::SetPrototypes(): Not implemented"),this);
	}

	bool SpectrumSubtract::UnsetPrototypes()
	{
		mProtoState=SOther;
		return true;
	}


	void SpectrumSubtract::Add(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		PrototypeState state_copy = mProtoState;
		ScaleState state2_copy = mScaleState;

		SetPrototypes(in1,in2,out);
		Do(in1,in2,out);
		
		mProtoState = state_copy;
		mScaleState = state2_copy;
	}


	void SpectrumSubtract::AddMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because some of the data objects had
		// their MagPhase attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in1.GetType();
		if (!f.bMagPhase) {
			remove1=true;
			f.bMagPhase=true;
			in1.SetTypeSynchronize(f);
		}
		f=in2.GetType();
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
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
			Polar_ po=Polar_(m1[i],f1[i])-Polar_(m2[i],f2[i]);
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

	void SpectrumSubtract::AddComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Complex attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in1.GetType();
		if (!f.bComplex) {
			remove1=true;
			f.bComplex=true;
			in1.SetTypeSynchronize(f);
		}
		f=in2.GetType();
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		Complex_ *c1 = in1.GetComplexArray().GetPtr();
		Complex_ *c2 = in2.GetComplexArray().GetPtr();
		Complex_ *co = out.GetComplexArray().GetPtr();
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


	void SpectrumSubtract::AddPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove1=false,remove2=false,remove3=false;
		SpecTypeFlags f;
		
		// This function was choosed because some of the data objects had
		// their Polar attribute instantiated. We don't know which of
		// them, though, so we must check and instantiate the attribute
		// it it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in1.GetType();
		if (!f.bPolar) {
			remove1=true;
			f.bPolar=true;
			in1.SetTypeSynchronize(f);
		}
		f=in2.GetType();
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		Polar_ *p1 = in1.GetPolarArray().GetPtr();
		Polar_ *p2 = in2.GetPolarArray().GetPtr();
		Polar_ *po = out.GetPolarArray().GetPtr();
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


	void SpectrumSubtract::AddBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bMagPhase) {
			remove3=true;
			f.bMagPhase=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		TData *m2 = in2.GetMagBuffer().GetPtr();
		TData *f2 = in2.GetPhaseBuffer().GetPtr();
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();
		for (int i=0;i<mSize;i++) {
		  Polar_ po = Polar_(m1.GetValue(pos),f1.GetValue(pos)) - 
		    Polar_(m2[i],f2[i]);
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

	void SpectrumSubtract::AddBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their MagPhase
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bMagPhase) {
			remove2=true;
			f.bMagPhase=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bMagPhase) {
			remove3=true;
			f.bMagPhase=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		TData *m2 = in2.GetMagBuffer().GetPtr();
		TData *f2 = in2.GetPhaseBuffer().GetPtr();
		TData *mo = out.GetMagBuffer().GetPtr();
		TData *fo = out.GetPhaseBuffer().GetPtr();
		for (int i=0;i<mSize;i++) {
		  Polar_ po = Polar_(pow(10,m1.GetValue(pos)/10.0),f1.GetValue(pos)) - 
		    Polar_(m2[i],f2[i]);
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

	void SpectrumSubtract::AddBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		Complex_ *c2 = in2.GetComplexArray().GetPtr();
		Complex_ *co = out.GetComplexArray().GetPtr();
		for (int i=0;i<mSize;i++) {
			TData BRe = fabs(m1.GetValue(pos)) * cos(f1.GetValue(pos));
			TData BIm = fabs(m1.GetValue(pos)) * sin(f1.GetValue(pos));
			co[i]= Complex_(BRe,BIm) - c2[i];
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
	void SpectrumSubtract::AddBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Complex
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bComplex) {
			remove2=true;
			f.bComplex=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bComplex) {
			remove3=true;
			f.bComplex=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		Complex_ *c2 = in2.GetComplexArray().GetPtr();
		Complex_ *co = out.GetComplexArray().GetPtr();
		for (int i=0;i<mSize;i++) {
			TData BRe = pow(10,fabs(m1.GetValue(pos))/10.0) * cos(f1.GetValue(pos));
			TData BIm = pow(10,fabs(m1.GetValue(pos))/10.0) * sin(f1.GetValue(pos));
			co[i]= Complex_(BRe,BIm) - c2[i];
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


	void SpectrumSubtract::AddBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
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

	void SpectrumSubtract::AddBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		Polar_ *p2 = in2.GetPolarArray().GetPtr();
		Polar_ *po = out.GetPolarArray().GetPtr();
		for (int i=0;i<mSize;i++) {
			po[i]=Polar_(m1.GetValue(pos),f1.GetValue(pos))-p2[i];
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

	void SpectrumSubtract::AddBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		bool remove2=false,remove3=false;
		SpecTypeFlags f;

		// This function was choosed because in1 is a BPF Spectrum,
		// and some of the non-BPF data objects have their Polar
		// attribute instantiated. We don't know which of them,
		// though, so we must check and instantiate the attribute it
		// it is missed. This could be optimised out by adding more
		// States, see coments on this in the class declaration.
		f=in2.GetType();
		if (!f.bPolar) {
			remove2=true;
			f.bPolar=true;
			in2.SetTypeSynchronize(f);
		}
		f=out.GetType();
		if (!f.bPolar) {
			remove3=true;
			f.bPolar=true;
			out.SetType(f);
		}

		TData pos = 0.0;
		TData delta = out.GetSpectralRange() / 
		              ((TData)out.GetSize()-1.0);
		BPF_ &m1 = in1.GetMagBPF();
		BPF_ &f1 = in1.GetPhaseBPF();
		Polar_ *p2 = in2.GetPolarArray().GetPtr();
		Polar_ *po = out.GetPolarArray().GetPtr();
		for (int i=0;i<mSize;i++) {
			TData BMag = pow(10,m1.GetValue(pos)/10.0);
			TData BPha = f1.GetValue(pos);
			po[i]=Polar_(BMag,BPha)-p2[i];
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

	void SpectrumSubtract::AddBPF(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		// First we check if the abcisas agree

		for (int i=0;i<mSize;i++) {
			Point<TData,TData> &pm1=in1.GetMagBPF().GetPointArray()[i];
			Point<TData,TData> &pm2=in2.GetMagBPF().GetPointArray()[i];
			Point<TData,TData> &pmo=out.GetMagBPF().GetPointArray()[i];
			Point<TData,TData> &pf1=in1.GetPhaseBPF().GetPointArray()[i];
			Point<TData,TData> &pf2=in2.GetPhaseBPF().GetPointArray()[i];
			Point<TData,TData> &pfo=out.GetPhaseBPF().GetPointArray()[i];
			if (pm1.GetX() != pm2.GetX() ||
				pm1.GetX() != pmo.GetX() )
				throw(ErrProcessingObj("AddBPF: BPF abcisas do not match "
								 "(and BPF merging not yet iplemented)",this));
			pmo.SetY(pm1.GetY()*pm2.GetY());
			pfo.SetY(pf1.GetY()-pf2.GetY());
		}

	}

	// UNINMPLEMENTED METHODS. Some day...
	void SpectrumSubtract::AddMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddMagPhaseLog: Not implemented"));
	}
	void SpectrumSubtract::AddMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddMagPhaseLinLog: Not implemented"));
	}
	void SpectrumSubtract::AddComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddComplexLog: Not implemented"));
	}
	void SpectrumSubtract::AddComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddComplexLinLog: Not implemented"));
	}
	void SpectrumSubtract::AddPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddPolarLog: Not implemented"));
	}
	void SpectrumSubtract::AddPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddPolarLinLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFComplexLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFComplexLinLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFPolarLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFPolarLinLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFMagPhaseLog: Not implemented"));
	}
	void SpectrumSubtract::AddBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out)
	{
		throw(ErrProcessingObj("AddBPFMagPhaseLinLog: Not implemented"));
	}
}
