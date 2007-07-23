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

#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "SpecTypeFlags.hxx"
#include "ArrayToBPFCnv.hxx"
#include "ComplexToPolarCnv.hxx"
#include "ProcessingDataPlugin.hxx"

using namespace CLAM;

namespace CLAM
{
	namespace Hidden
	{
		static ProcessingDataPlugin::Registrator<Spectrum> dataRegistrator("yellowgreen");
	}
}
Flags<4>::tFlagValue SpecTypeFlags::sFlagValues[] = {
 	{SpecTypeFlags::eComplex, "Complex"},
 	{SpecTypeFlags::ePolar, "Polar"},
 	{SpecTypeFlags::eMagPhase, "MagPhase"},
 	{SpecTypeFlags::eMagPhaseBPF, "MagPhaseBPF"},
 	{0,NULL}
}; 


///////////////////////////////////////////////////////
//
// SpectrumConfig
//
///////////////////////////////////////////////////////

/** By default the BPFSize dyn. attr. is not added. That means that BPF 
 * uses the same size as the other types .
 */
void SpectrumConfig::DefaultInit()
{
	AddScale();
	AddSpectralRange();
	AddType();
	AddSize();

	SetPreAllocateAllAttributes();
	UpdateData();
	DefaultValues();  
}


void SpectrumConfig::DefaultValues()
{
	SpecTypeFlags flags; // initilized with the default value
	SetType(flags);
	SetScale(EScale(EScale::eLinear));
	SetSize(0);

	SetSpectralRange(22050);
}



//////////////////////////////////////////////////////////////////////
//
// Spectrum
//
//////////////////////////////////////////////////////////////////////


Spectrum::Spectrum(const SpectrumConfig &newConfig) : ProcessingData(eNumAttr)
{
	MandatoryInit(); // Macro-expanded function. Necessary for some dynamic type initialization.
	Configure(newConfig);
}

void Spectrum::GetConfig(SpectrumConfig& c) const
{
	SpecTypeFlags f;

	f.bMagPhaseBPF = HasMagBPF() && HasPhaseBPF();
	f.bMagPhase = HasMagBuffer() && HasPhaseBuffer();	
	f.bPolar = HasPolarArray();
	f.bComplex = HasComplexArray();

	c.SetType(f);
	c.SetSize( GetSize() );
	c.SetSpectralRange(GetSpectralRange());
	c.SetScale(GetScale());
	
	if (GetBPFSize()<0 || GetBPFSize()==GetSize()) {
		c.RemoveBPFSize();
		c.UpdateData();
	}
	else
	{	
		c.AddBPFSize();
		c.UpdateData();
		c.SetBPFSize( GetBPFSize() );
	}
}



void Spectrum::DefaultInit()
{
	Configure(SpectrumConfig());
}
 
void Spectrum::Configure(const SpectrumConfig &newConfig)
{
	AddScale();
	AddSpectralRange();
	AddprSize();

	SetPreAllocateAllAttributes();
	UpdateData();
	
	SetScale(newConfig.GetScale());
	SetSpectralRange(newConfig.GetSpectralRange());
	int bpfsize= newConfig.HasBPFSize() ? newConfig.GetBPFSize() : newConfig.GetSize();
	PRSetType(newConfig.GetType(),newConfig.GetSize(),bpfsize);
}

void Spectrum::PRSetType(const SpecTypeFlags& tmpFlags,int size, int bpfsize)
{
	if(tmpFlags.bMagPhase)
	{
		AddMagBuffer();
		AddPhaseBuffer();
	}
	else
	{
		RemoveMagBuffer();
		RemovePhaseBuffer();
	}
	if(tmpFlags.bComplex)
	{
		AddComplexArray();
	}
	else
	{
		RemoveComplexArray();
	}
	if(tmpFlags.bPolar)
	{
		AddPolarArray();
	}
	else
	{
		RemovePolarArray();
	}
	if(tmpFlags.bMagPhaseBPF)
	{
		AddMagBPF();
		AddPhaseBPF();
	}
	else
	{
		RemoveMagBPF();
		RemovePhaseBPF();
	}
	UpdateData();
	SetSize(size);
	SetBPFSize(bpfsize);

}

void Spectrum::SetType(const SpecTypeFlags& tmpFlags)
{
	int size = GetSize();
	int bpfsize= HasprBPFSize() ? GetprBPFSize() : size;
	PRSetType(tmpFlags,size,bpfsize);
}
 

/* @TODO: These methods use a limit for -INFINITY dB and 0 linear, these limits should be
set as constants elsewhere */
void Spectrum::ToDB()
{
	if(GetScale()!=EScale::eLinear)
		return;

	int i;
	SpecTypeFlags flags;
	TSize specSize = GetSize();
	if (HasMagBuffer())
	{
			DataArray &mag = GetMagBuffer();
			for (i=0; i<specSize; i++)
			{
#ifdef CLAM_OPTIMIZE
				//we discard magnitudes lower than 100 dB
				//todo: this optimization should also be applied to other places
				if(mag[i]<0.00001f) mag[i]=-200;
#else				
				if(mag[i]==0) mag[i]=-200;
#endif
				else mag[i]= CLAM_20log10(mag[i]); 
			}
	}
	if (HasPolarArray()) // WARNING: computational expensive operation
	{
			Array<Polar> &polar = GetPolarArray();
			for (i=0; i<specSize; i++)
			{
				TData magLin = polar[i].Mag();
				TData magLog;
#ifdef CLAM_OPTIMIZE
				//we discard magnitudes lower than 100 dB
				//todo: this optimization should also be applied to the reverse conversion
				if(magLin<0.00001f) magLog=-200;
#else				
				if(magLin==0) magLog=-200;
#endif				
				else magLog = CLAM_20log10(magLin);
				polar[i].SetMag(magLog);
			}
			flags.bPolar = true;
			flags.bMagPhase = false;
	}
	if (HasComplexArray())  // WARNING: computational expensive operation
	{
			Array<Complex> &complex = GetComplexArray();
			for (i=0; i<specSize; i++)
			{
				TData re = complex[i].Real();
				TData im = complex[i].Imag();
				TData magLin = CLAM_sqrt(re*re + im*im);
				TData magLog;
#ifdef CLAM_OPTIMIZE
				if(magLin<0.00001f) magLog=-200;
#else				
				if(magLin==0) magLog=-200;
#endif								
				else magLog = CLAM_20log10(magLin);
				complex[i].SetReal(magLog * re / magLin);
				complex[i].SetImag(magLog * im / magLin);
			}
			flags.bComplex = true;
			flags.bMagPhase = false;
	}
	if (HasMagBPF())  // WARNING: computational expensive operation
	{
			BPF &magBPF= GetMagBPF();
			const int bpfSize=GetBPFSize();
			for (i=0; i<bpfSize; i++)
			{
				TData magLin=magBPF.GetValueFromIndex(i);
				TData magLog;
#ifdef CLAM_OPTIMIZE
				if(magLin<0.00001f) magLog=-200;
#else				
				if(magLin==0) magLog=-200;
#endif								
				else magLog = CLAM_20log10(magLin);
				magBPF.SetValue(i,magLog);
			}
			flags.bMagPhaseBPF = true;
			flags.bMagPhase = false;
	}
	SetScale(EScale(EScale::eLog));
}


void Spectrum::ToLinear()
{
	if(GetScale()==EScale::eLog)
	{
		int i;
		SpecTypeFlags flags;
		TSize specSize = GetSize();
		if (HasMagBuffer())
		{
			DataArray &mag = GetMagBuffer();
			for (i=0; i<specSize; i++)
			{
				if(mag[i]<=-200) mag[i]=0;
				else mag[i]= log2lin(mag[i]); 
			}
		}
		if (HasPolarArray())  // WARNING: computational expensive operation
		{
			Array<Polar> &polar = GetPolarArray();
			for (i=0; i<specSize; i++)
			{
				TData magLog = polar[i].Mag();
				TData magLin;
				if(magLog<=-200) magLin=0;
				else magLin = log2lin(magLog);
				polar[i].SetMag(magLin);
			}
			flags.bPolar = true;
			flags.bMagPhase = false;
		}
		if (HasComplexArray())  // WARNING: computational expensive operation
		{
			Array<Complex> &complex = GetComplexArray();
			for (i=0; i<specSize; i++)
			{
				TData re = complex[i].Real();
				TData im = complex[i].Imag();
				TData magLog = CLAM_sqrt(re*re + im*im);
				TData magLin;
				if(magLog<=-200) magLin=0;
				else magLin = log2lin(magLog);
				complex[i].SetReal(magLin * re / magLin);
				complex[i].SetImag(magLin * im / magLin);
			}
			flags.bComplex = true;
			flags.bMagPhase = false;
		}
		if (HasMagBPF())  // WARNING: computational expensive operation
		{
			BPF &magBPF = GetMagBPF();
			int bpfSize=GetBPFSize();
			for (i=0; i<bpfSize; i++)
			{
				TData magLog = magBPF.GetValueFromIndex(i);
				TData magLin;
				if(magLog<=-200) magLin=0;
				else magLin = log2lin(magLog);
				magBPF.SetValue(i,magLin);
			}
			flags.bMagPhaseBPF = true;
			flags.bMagPhase = false;
		}
		SetScale(EScale(EScale::eLinear));

	}
}

TData Spectrum::GetMag(TIndex pos) const
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	if(tmpFlags.bMagPhase)
		return GetMagBuffer()[pos];
	if (tmpFlags.bMagPhaseBPF)
		return GetMagBPF().GetValueFromIndex(pos);
	if (tmpFlags.bPolar)
		return GetPolarArray()[pos].Mag();
	if (tmpFlags.bComplex)
		return GetComplexArray()[pos].Mag();
	CLAM_ASSERT(false, "Spectrum::GetMag: Spectrum no initialized");
	return 0;
}

TData Spectrum::GetMag(TData freq) const
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	TIndex pos = IndexFromFreq(freq);
	if (tmpFlags.bMagPhaseBPF)
		return GetMagBPF().GetValue(freq);
	if(tmpFlags.bMagPhase)
		return GetMagBuffer()[pos];
	if (tmpFlags.bPolar)
		return GetPolarArray()[pos].Mag();
	if (tmpFlags.bComplex)
		return GetComplexArray()[pos].Mag();
	CLAM_ASSERT(false, "Spectrum::GetMag: Spectrum no initialized");
	return 0;
}

TData Spectrum::GetPhase(TIndex pos) const
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	if(tmpFlags.bMagPhase)
		return GetPhaseBuffer()[pos];
	if (tmpFlags.bMagPhaseBPF)
		return GetPhaseBPF().GetValueFromIndex(pos);
	if (tmpFlags.bPolar)
		return GetPolarArray()[pos].Ang();
	if (tmpFlags.bComplex)
		return GetComplexArray()[pos].Ang();
	CLAM_ASSERT(false, "Spectrum::GetPhase: Spectrum no initialized");
	return 0;
}

TData Spectrum::GetPhase(TData freq) const
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	TIndex pos = IndexFromFreq(freq);
	if (tmpFlags.bMagPhaseBPF)
		return GetPhaseBPF().GetValue(freq);
	if(tmpFlags.bMagPhase)
		return GetPhaseBuffer()[pos];
	if (tmpFlags.bPolar)
		return GetPolarArray()[pos].Ang();
	if (tmpFlags.bComplex)
		return GetComplexArray()[pos].Ang();
	CLAM_ASSERT(false, "Spectrum::GetPhase: Spectrum no initialized");
	return 0;
}

void Spectrum::SetMag(TIndex pos, TData newMag)
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	if(tmpFlags.bMagPhase)
		GetMagBuffer()[pos]=newMag;
	if (tmpFlags.bMagPhaseBPF)
		GetMagBPF().SetValue(pos,newMag);
	if (tmpFlags.bPolar)
		GetPolarArray()[pos].SetMag(newMag);
	if (tmpFlags.bComplex){
		TData tmpAng=GetComplexArray()[pos].Ang();
		GetComplexArray()[pos].SetReal(newMag*CLAM_cos(tmpAng));
		GetComplexArray()[pos].SetImag(newMag*CLAM_sin(tmpAng));}
}

void Spectrum::SetMag(TData freq, TData newMag)
{
	SetMag(IndexFromFreq(freq),newMag);
}
	
void Spectrum::SetPhase(TIndex pos,TData newPhase)//inefficient Set, for efficiency work directly on the buffer
{
	SpecTypeFlags tmpFlags; 
	GetType(tmpFlags);
	if(tmpFlags.bMagPhase)
		GetPhaseBuffer()[pos]=newPhase;
	else if (tmpFlags.bMagPhaseBPF)
		GetPhaseBPF().SetValue(pos,newPhase);
	else if (tmpFlags.bPolar)
		GetPolarArray()[pos].SetAng(newPhase);
	else if (tmpFlags.bComplex){
		TData tmpMag=GetComplexArray()[pos].Mag();
		GetComplexArray()[pos].SetReal(tmpMag*CLAM_cos(newPhase));
		GetComplexArray()[pos].SetImag(tmpMag*CLAM_sin(newPhase));
	}
}

void Spectrum::SetPhase(TData freq, TData newPhase)
{
	SetPhase(IndexFromFreq(freq),newPhase);
}

void Spectrum::SynchronizeTo(const SpecTypeFlags& tmpFlags)
{
	SynchronizeTo(tmpFlags,(*this));
}	

void Spectrum::SynchronizeTo(const SpecTypeFlags& tmpFlags,const Spectrum& in)
{
	//if tmpFlags are set so different synchronizations are possible, the easiest one is chosen
	
	SpecTypeFlags currentFlags;
	GetType(currentFlags);
	int size = GetSize();

	if(tmpFlags.bMagPhase)
	{
		if(currentFlags.bPolar) MagPhase2Polar(in);
		if(currentFlags.bComplex) MagPhase2Complex(in);
		if(currentFlags.bMagPhaseBPF) MagPhase2BPF(in);
	}
	else if(tmpFlags.bComplex)
	{
		if(currentFlags.bPolar)  Complex2Polar(in);
		if(currentFlags.bMagPhase) Complex2MagPhase(in);
		if(currentFlags.bMagPhaseBPF)
		{
			if(currentFlags.bMagPhase) MagPhase2BPF();
			else
			{
				AddMagBuffer();
				AddPhaseBuffer();
				UpdateData();
				GetMagBuffer().Resize(size);
				GetPhaseBuffer().Resize(size);
				GetMagBuffer().SetSize(size);
				GetPhaseBuffer().SetSize(size);
				Complex2MagPhase(in);
				MagPhase2BPF();
		
				RemoveMagBuffer();
				RemovePhaseBuffer();
				UpdateData();
			}
		}
	}
	else if(tmpFlags.bPolar)
	{
		if(currentFlags.bComplex)  Polar2Complex(in);
		if(currentFlags.bMagPhase) Polar2MagPhase(in);
		if(currentFlags.bMagPhaseBPF)
		{
			if(currentFlags.bMagPhase) MagPhase2BPF();
			else
			{
				AddMagBuffer();
				AddPhaseBuffer();
				UpdateData();
				GetMagBuffer().Resize(size);
				GetPhaseBuffer().Resize(size);
				GetMagBuffer().SetSize(size);
				GetPhaseBuffer().SetSize(size);
				Polar2MagPhase(in);
				MagPhase2BPF();

				RemoveMagBuffer();
				RemovePhaseBuffer();
				UpdateData();
			}
		}
	}
	else if(tmpFlags.bMagPhaseBPF)
	{
		bool deleteMagPhase=false;
		if(!currentFlags.bMagPhase) 
		{
			deleteMagPhase=true;
			AddMagBuffer();
			AddPhaseBuffer();
			UpdateData();
			GetMagBuffer().Resize(size);
			GetPhaseBuffer().Resize(size);
			GetMagBuffer().SetSize(size);
			GetPhaseBuffer().SetSize(size);
		}
		BPF2MagPhase(in);

		if(currentFlags.bPolar) MagPhase2Polar();
		if(currentFlags.bComplex) MagPhase2Complex();
		if(deleteMagPhase)
		{
			RemoveMagBuffer();
			RemovePhaseBuffer();
			UpdateData();
		}
	}


}


void Spectrum::SynchronizeTo(const Spectrum& in)
{
	//if tmpFlags are set so different synchronizations are possible, the easiest one is chosen

	SpecTypeFlags tmpFlags;
	in.GetType(tmpFlags);

	SynchronizeTo(tmpFlags,in);
	


}

////////Converting routines////////////////


void Spectrum::Complex2Polar(const Spectrum& in)
{
	ComplexToPolarCnv_ convert;
	convert.ToPolar(in.GetComplexArray(), GetPolarArray());	
}

void Spectrum::Complex2Polar() 
{
	ComplexToPolarCnv_ convert;
	convert.ToPolar(GetComplexArray(), GetPolarArray());
}

void Spectrum::Polar2Complex(const Spectrum& in) 
{
	ComplexToPolarCnv_ convert;
	convert.ToComplex(in.GetPolarArray(),GetComplexArray());
}

void Spectrum::Polar2Complex() 
{
	ComplexToPolarCnv_ convert;
	convert.ToComplex(GetPolarArray(),GetComplexArray());
}

void Spectrum::Polar2MagPhase(const Array<Polar>& polarArray,DataArray& magBuffer, DataArray& phaseBuffer)
{
	int size = GetSize();
	for (int i=0; i<size; i++) 
	{
		magBuffer[i] = polarArray[i].Mag();
		phaseBuffer[i] = polarArray[i].Ang();
	}
}

void Spectrum::Polar2MagPhase(const Spectrum& in) 
{
	Polar2MagPhase(in.GetPolarArray(),GetMagBuffer(),GetPhaseBuffer());
}

void Spectrum::Polar2MagPhase() 
{
	Polar2MagPhase(GetPolarArray(),GetMagBuffer(),GetPhaseBuffer());
}

void Spectrum::Complex2MagPhase(const Array<Complex>& complexArray, DataArray& magBuffer, DataArray& phaseBuffer)
{
	int size = GetSize();
	for (int i=0; i<size; i++) 
	{
		magBuffer[i] = complexArray[i].Mag();
		phaseBuffer[i] = complexArray[i].Ang();
	}
}

void Spectrum::Complex2MagPhase(const Spectrum& in) 
{
	Complex2MagPhase(in.GetComplexArray(),GetMagBuffer(),GetPhaseBuffer());
}

void Spectrum::Complex2MagPhase() 
{
	Complex2MagPhase(GetComplexArray(),GetMagBuffer(),GetPhaseBuffer());
}

void Spectrum::MagPhase2Polar(const DataArray& magBuffer,const DataArray& phaseBuffer,Array<Polar>& polarArray)
{
	int size = GetSize();
	for (int i=0; i<size; i++)
	{
		polarArray[i].SetMag(magBuffer[i]);
		polarArray[i].SetAng(phaseBuffer[i]);
	}
}

void Spectrum::MagPhase2Polar(const Spectrum& in) 
{
	MagPhase2Polar(in.GetMagBuffer(),in.GetPhaseBuffer(),GetPolarArray());

}

void Spectrum::MagPhase2Polar() 
{
	MagPhase2Polar(GetMagBuffer(),GetPhaseBuffer(),GetPolarArray());

}

void Spectrum::MagPhase2Complex(const DataArray& magBuffer,const DataArray& phaseBuffer,Array<Complex>& complexArray) 
{
	int size = GetSize();
	/* Xamat: I think this is not necessary, check in tests
	  complexArray.Resize(size);
	  complexArray.SetSize(size);*/
	for (int i=0; i<size; i++) 
	{
		complexArray[i].SetReal(magBuffer[i]*CLAM_cos(phaseBuffer[i]));
		complexArray[i].SetImag(magBuffer[i]*CLAM_sin(phaseBuffer[i]));
	}
}

void Spectrum::MagPhase2Complex(const Spectrum& in) 
{
	MagPhase2Complex(in.GetMagBuffer(),	in.GetPhaseBuffer(),GetComplexArray());
}

void Spectrum::MagPhase2Complex() 
{
	MagPhase2Complex(GetMagBuffer(), GetPhaseBuffer(),GetComplexArray());
}

void Spectrum::MagPhase2BPF(const DataArray& magBuffer, const DataArray& phaseBuffer, BPF& magBPF, BPF& phaseBPF) 
{
	int size = GetSize();
	CLAM_ASSERT(size == GetBPFSize(),
		"Spectrum::MagPhase2BPF(): BPFSize and MagPhase size differ.");

	DataArray freqBuffer(size);
	TData delta = GetSpectralRange()/(size-1);

	ConvertToBPF( magBPF, TData(0), delta, magBuffer );
	ConvertToBPF( phaseBPF, TData(0), delta, phaseBuffer );
}


void Spectrum::MagPhase2BPF(const Spectrum& in) 
{
	MagPhase2BPF(in.GetMagBuffer(),in.GetPhaseBuffer(),GetMagBPF(),GetPhaseBPF());
}

void Spectrum::MagPhase2BPF() 
{
	MagPhase2BPF(GetMagBuffer(),GetPhaseBuffer(),GetMagBPF(),GetPhaseBPF());
}

void Spectrum::BPF2MagPhase( const BPF& magBPF, const BPF& phaseBPF, DataArray& magBuffer, DataArray& phaseBuffer)
{
	int i;
	TData freq;
	int size = GetSize();
	TData delta = GetSpectralRange()/(size-1);
	for(i=0; i<size; i++){
		freq = i*delta;
		magBuffer[i]  = magBPF.GetValue(freq); // interpolation
		phaseBuffer[i]= phaseBPF.GetValue(freq);
	}
}

void Spectrum::BPF2MagPhase(const Spectrum& in)
{
	BPF2MagPhase(in.GetMagBPF(),in.GetPhaseBPF(),GetMagBuffer(),GetPhaseBuffer());
}

void Spectrum::BPF2MagPhase()
{
	BPF2MagPhase(GetMagBPF(),GetPhaseBPF(),GetMagBuffer(),GetPhaseBuffer());	
}



int Spectrum::GetSize() const
{
	const int size= GetprSize();

	CLAM_BEGIN_CHECK
	if(HasMagBuffer() && GetMagBuffer().Size())
		CLAM_ASSERT(GetMagBuffer().Size() == size,
		           "Spectrum::GetSize(): Mag size and Size mismatch.");
	if(HasPhaseBuffer() && GetPhaseBuffer().Size())
		CLAM_ASSERT(GetPhaseBuffer().Size() == size,
		           "Spectrum::GetSize(): Phase size and Size mismatch.");
	if(HasComplexArray() && GetComplexArray().Size())
		CLAM_ASSERT(GetComplexArray().Size() == size,
		           "Spectrum::GetSize(): Complex size and Size mismatch.");
	if(HasPolarArray() && GetPolarArray().Size())
		CLAM_ASSERT(GetPolarArray().Size() == size,
		           "Spectrum::GetSize(): Polar size and Size mismatch.");
	if (!HasprBPFSize()) {
		if(HasMagBPF() && GetMagBPF().Size())
			CLAM_ASSERT(GetMagBPF().Size() == size,
			           "Spectrum::GetSize(): MagBPF size and Size mismatch.");
		if(HasPhaseBPF() && GetPhaseBPF().Size())
			CLAM_ASSERT(GetPhaseBPF().Size() == size,
			           "Spectrum::GetSize(): PhaseBPF size and Size mismatch.");
	}
	CLAM_END_CHECK

	return size;
}


void Spectrum::SetSize(int newSize)
{
	SetprSize(newSize);

	if(HasMagBuffer()){
		GetMagBuffer().Resize(newSize);
		GetMagBuffer().SetSize(newSize);}
	if(HasPhaseBuffer()){
		GetPhaseBuffer().Resize(newSize);
		GetPhaseBuffer().SetSize(newSize);}
	if (HasPolarArray()){
		GetPolarArray().Resize(newSize);
		GetPolarArray().SetSize(newSize);}
	if (HasComplexArray()){
		GetComplexArray().Resize(newSize);
		GetComplexArray().SetSize(newSize);}
	if (!HasprBPFSize()) {
		if (HasMagBPF()){
			GetMagBPF().Resize(newSize);
			GetMagBPF().SetSize(newSize);}
		if (HasPhaseBPF()) {
			GetPhaseBPF().Resize(newSize);
			GetPhaseBPF().SetSize(newSize);}
	}
}

int Spectrum::GetBPFSize() const
{
	const int bpfsize= HasprBPFSize() ? GetprBPFSize() : GetSize();

	CLAM_BEGIN_CHECK
	CLAM_ASSERT(! (HasMagBPF() && GetMagBPF().Size() && GetMagBPF().Size() != bpfsize) ,
		"Spectrum::GetBPFSize(): MagBPF size and BPFSize mismatch.");
	CLAM_ASSERT(!(HasPhaseBPF() && GetPhaseBPF().Size() && GetPhaseBPF().Size() != bpfsize),
		"Spectrum::GetBPFSize(): PhaseBPF size and BPFSize mismatch.");
	CLAM_END_CHECK

	return bpfsize;
}

void Spectrum::SetBPFSize(int s)
{
	if (s != GetprSize()) {
		AddprBPFSize();
		UpdateData();
		SetprBPFSize(s);
	}
	else {
		RemoveprBPFSize();
		UpdateData();
	}
		
	if (HasMagBPF() && HasPhaseBPF())
	{
		GetMagBPF().Resize(s);
		GetMagBPF().SetSize(s);
		GetPhaseBPF().Resize(s);
		GetPhaseBPF().SetSize(s);
	}
}
	
void Spectrum::SetTypeSynchronize(const SpecTypeFlags& newFlags)
{
	SpecTypeFlags previousFlags;
	GetType(previousFlags );
	SpecTypeFlags tmpFlags;
	
	tmpFlags.bComplex = previousFlags.bComplex | newFlags.bComplex;
	tmpFlags.bPolar = previousFlags.bPolar | newFlags.bPolar;
	tmpFlags.bMagPhase = previousFlags.bMagPhase | newFlags.bMagPhase;
	tmpFlags.bMagPhaseBPF = previousFlags.bMagPhaseBPF | newFlags.bMagPhaseBPF;
		
	SetType(tmpFlags);
	SynchronizeTo(previousFlags);
	SetType(newFlags);
}


void Spectrum::GetType(SpecTypeFlags& f) const
{
	f.bMagPhaseBPF = HasMagBPF() && HasPhaseBPF();
	f.bMagPhase = HasMagBuffer() && HasPhaseBuffer();
	f.bPolar = HasPolarArray();
	f.bComplex = HasComplexArray();
}

TIndex Spectrum::IndexFromFreq(TData freq) const
{
	return Round(freq*((GetSize()-1)/GetSpectralRange()));
}

