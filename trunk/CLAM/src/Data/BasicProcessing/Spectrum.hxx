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

#ifndef _Spectrum_
#define _Spectrum_

#include <typeinfo>
#include "Array.hxx"
#include "Complex.hxx"
#include "Polar.hxx"
#include "BPF.hxx"
#include "ProcessingData.hxx"

namespace CLAM {

class SpectrumConfig;

struct SpecTypeFlags;

/** 
 *	Spectrum is a ProcessingData class that allows the following representations: 
 *	Mag/Phase, Complex, Polar and Break Point Function (BPF). Different interfaces are
 *	offered but for efficiency you should directly access buffers. Synchronization routines
 *	between different possible representations are also offered.
 *	@see ProcessingData
 */
class Spectrum : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Spectrum, 10, ProcessingData);
	/**
	* The kind of scale
	*/
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, SpectralRange);
	
private:
	DYN_ATTRIBUTE (2, private, int, prSize);
	DYN_ATTRIBUTE (3, private, int, prBPFSize);
public:
	DYN_ATTRIBUTE (4, public, DataArray, MagBuffer);
	DYN_ATTRIBUTE (5, public, DataArray, PhaseBuffer);
	DYN_ATTRIBUTE (6, public, Array<Complex>, ComplexArray);
	DYN_ATTRIBUTE (7, public, Array<Polar>, PolarArray);
	DYN_ATTRIBUTE (8, public, BPF, MagBPF);
	DYN_ATTRIBUTE (9, public, BPF, PhaseBPF);

public:
	Spectrum(const SpectrumConfig &newConfig);
 
	void Configure(const SpectrumConfig &newConfig);

	/** This method synchronizes the Type attribute in the prConfig attribute
	 * before returning it. 
	 * The case of BPFSize is a little triky: the configuration object will have it
	 * instantiated only if it's different from the spectrum size. 
	 * When a Spectrum is configurated with a config without BPFSize, it's assumed
	 * that the "normal" size will be used for the BPF
	 */
	void GetConfig(SpectrumConfig& c) const;
protected:
	void DefaultInit();

public:
	/** Returns spectral magnitude in a given bin position.
	 *	Inefficient Get, for efficiency work directly on the buffer 
	 */
	TData GetMag(TIndex pos) const;
	/** Returns spectral magnitude from a given frequency
	 *	Inefficient Get, for efficiency work directly on the buffer 
	 */
	TData GetMag(TData freq) const;
	/** Returns spectral phase in a given bin position
	 *	Inefficient Get, for efficiency work directly on the buffer 
	 */
	TData GetPhase(TIndex pos) const;
	/** Returns spectral phase from a given frequency
	 *	Inefficient Get, for efficiency work directly on the buffer 
	 */
	TData GetPhase(TData freq) const;
	/** Sets spectral magnitude in a given bin position
	 *	Inefficient Set, for efficiency work directly on the buffer 
	 */
	void SetMag(TIndex pos,TData newMag);
	/** Sets spectral magnitude from a given frequency
	 *	Inefficient Set, for efficiency work directly on the buffer 
	 */
	void SetMag(TData freq,TData newMag);
	/** Sets spectral phase in a given bin position
	 *	Inefficient Set, for efficiency work directly on the buffer 
	 */
	void SetPhase(TIndex pos,TData newPhase);
	/** Sets spectral phase from a given frequency
	 *	Inefficient Set, for efficiency work directly on the buffer 
	 */
	void SetPhase(TData freq,TData newPhase);
	/** Synchronizes all data to the one of the specified type */
	void SynchronizeTo(const SpecTypeFlags& tmpFlags);
	/** Synchronizes all data to the one of the input spectrum */
	void SynchronizeTo(const Spectrum& in);
    /** Actual Synchronization routine */
	void SynchronizeTo(const SpecTypeFlags& tmpFlags,const Spectrum& in);



// Config shortcut interface.

	/** Getter for accessing size of the spectrum */
	int GetSize() const;
	/** Sets spectrum size */
	void SetSize(int s);

	/** Getter for accessing BPF size, useful only if that representation is being used.
	 *	Note that BPF size has nothing to do with spectral size, it is just
	 *	the number of points used to represent the spectrum in a break-point
	 *	function manner. 
	 *	@see BPF
	 */
	int GetBPFSize() const;
	/** Setter for BPF size, useful only if that representation is being used.
	 *	Note that BPF size has nothing to do with spectral size, it is just
	 *	the number of points used to represent the spectrum in a break-point
	 *	function manner. 
	 *	@see BPF
	 */
	void SetBPFSize(int s);
	
	/** This is not just an accessor. The Type attribute is updated in
	 * the mConfig object before returning it. 
	 */
	void GetType(SpecTypeFlags& f) const;

	/** This method updates the instantiation of dynamic attributes
	 * acording to the given flags, and sets up the attribute buffers
	 * to the correct sice. 
	 */
	void SetType(const SpecTypeFlags& newFlags);
	
	/** Same as set Type but additionaly the data on new buffers is
	 * synchronized to existing data
	 */
	void SetTypeSynchronize(const SpecTypeFlags& newFlags);

	/** Converts scale from linear to dB. If scale is already set to dB, it
	 *  does nothing
	 */
	void ToDB();
	
	/** Converts scales from dB to linear. If scale is already set to linear, it
	 *  does nothing
	 */
	void ToLinear();

private:
	
	/** Private part of the SetType method. This method is called from 
	 * either the public SetType or the Configure method. It is actually
	 * in charge of adding and removing buffers */
	void PRSetType(const SpecTypeFlags& tmpFlags,int size,int bpfsize);

public:
	/** Returns index from a given frequency */
	inline TIndex IndexFromFreq(TData freq) const;


public:
// Internal converting routines
	/** Convert from Complex to Polar Buffer */
	inline void Complex2Polar() ;
	/** Convert from Polar to Complex Buffer */
	inline void Polar2Complex() ;
	/** Convert from Complex to Mag/Phase buffers */
	inline void Complex2MagPhase() ;
	/** Convert from Polar to Mag/Phase buffers */
	inline void Polar2MagPhase() ;
	/** Convert from Mag/Phase to Polar */
	inline void MagPhase2Polar() ;
	/** Convert from Mag/Phase to Complex */
	inline void MagPhase2Complex() ;
	/** Convert to BPF */
	inline void MagPhase2BPF() ;
	/** Convert from BPF to MagPhase*/
	inline void BPF2MagPhase();

// Converting routines from a second input spectrum
/** Convert from Complex to Polar Buffer */
	inline void Complex2Polar(const Spectrum& in) ;
	/** Convert from Polar to Complex Buffer */
	inline void Polar2Complex(const Spectrum& in) ;
	/** Convert from Complex to Mag/Phase buffers */
	inline void Complex2MagPhase(const Spectrum& in) ;
	/** Convert from Polar to Mag/Phase buffers */
	inline void Polar2MagPhase(const Spectrum& in) ;
	/** Convert from Mag/Phase to Polar */
	inline void MagPhase2Polar(const Spectrum& in) ;
	/** Convert from Mag/Phase to Complex */
	inline void MagPhase2Complex(const Spectrum& in) ;
	/** Convert to BPF */
	inline void MagPhase2BPF(const Spectrum& in) ;
	/** Convert from BPF to MagPhase*/
	inline void BPF2MagPhase(const Spectrum& in);
private:
//Low level converting routines
	inline void Polar2MagPhase(const Array<Polar>& polarArray,DataArray& magBuffer, DataArray& phaseBuffer);
	inline void Complex2MagPhase(const Array<Complex>& complexArray, DataArray& magBuffer, DataArray& phaseBuffer);
	inline void MagPhase2Polar(const DataArray& magBuffer,const DataArray& phaseBuffer,Array<Polar>& polarArray);
	inline void MagPhase2Complex(const DataArray& magBuffer,const DataArray& phaseBuffer,Array<Complex>& complexArray);
	inline void MagPhase2BPF(const DataArray& magBuffer, const DataArray& phaseBuffer, BPF& magBPF, BPF& phaseBPF);
	inline void BPF2MagPhase( const BPF& magBPF, const BPF& phaseBPF, DataArray& magBuffer, DataArray& phaseBuffer);

};

}; // namespace CLAM

#endif

