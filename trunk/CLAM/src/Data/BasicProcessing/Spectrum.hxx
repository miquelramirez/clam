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

#ifndef _Spectrum_
#define _Spectrum_

#include "DynamicType.hxx"
#include "Array.hxx"
#include "Complex.hxx"
#include "Polar.hxx"
#include "BPF.hxx"
#include "ArrayToBPFCnv.hxx"
#include "ComplexToPolarCnv.hxx"
#include "DataTypes.hxx"
#include "Flags.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "Enum.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"
#include "GlobalEnums.hxx"

namespace CLAM{

/**
* This class tells which formats are instantiated in a Spectrum.
* It could be:
* <ul>
* <li> Complex: Array of Complex objects
* <li> Polar: Array of Polar objects
* <li> MagPhase: Array of magnitude/phase pairs
* <li> MagPhaseBPF: BPF of magnitude/phase pairs
* </ul>
* It can be used as any other class derived from Flags: you can
* use the std::bit interface and Flags added functionalities.
* @see Flags
*/
struct SpecTypeFlags : public Flags<4>
{
public:
	static tFlagValue sFlagValues[];
	static tValue sDefault;
	virtual Component * Species() const {
		return new SpecTypeFlags();
	}
	typedef enum {
		eComplex=0,
		ePolar=1,
		eMagPhase=2,
		eMagPhaseBPF=3
	} tFlags;
// Constructors
	SpecTypeFlags () :
		Flags<4>(sFlagValues),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))
	{
		bComplex = 0;
		bPolar = 0; 
		bMagPhase = 1;
		bMagPhaseBPF = 0;
	}


	SpecTypeFlags (const SpecTypeFlags &t) : 
		Flags<4>(sFlagValues,t),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))

	{};

/*
	template <typename T> SpecTypeFlags (const T &t) : 
		Flags<4>(sFlagValues,t),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))

	{};
*/

	template <typename T1, typename T2> SpecTypeFlags (const T1 &t1,const T2 &t2) : 
		Flags<4>(sFlagValues,t1,t2),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))
	{};


	reference bComplex;
	reference bPolar; 
	reference bMagPhase;
	reference bMagPhaseBPF;

};

////////////////////////////////////////////////////////////////////////////////////////////////
// Class SpectrumConfig :
//

class Spectrum;


/**
* The Spectrum configuration object.
*/
class SpectrumConfig : public ProcessingDataConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectrumConfig, 5, ProcessingDataConfig);
	/**
	* The kind of scale
	*/
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, SpectralRange);
	DYN_ATTRIBUTE (2, public, int, Size);
	DYN_ATTRIBUTE (3, public, SpecTypeFlags, Type);
	DYN_ATTRIBUTE (4, public, int, BPFSize);
public:
	/*
	* Sets to the default configuration.
	*/
	void DefaultValues();
	~SpectrumConfig(){};
protected:
	/*
	* Dynamic Type constructor: instanciates all the attributes.
	*/
	void DefaultInit();
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Class Spectrum :
//
/** 
 * It allows the following representations: Mag/Phase, Complex, Polar and Break Point Function (BPF) 
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
	Spectrum(const SpectrumConfig &newConfig) : ProcessingData(eNumAttr)
	{
		MandatoryInit(); // Macro-expanded function. Necessary for some dynamic type initialization.
		Configure(newConfig);
	}
 
	void Configure(const SpectrumConfig &newConfig);

	/** This is not just an accessor. The  */
	void GetConfig(SpectrumConfig& c) const;
protected:
	void DefaultInit();

public:
	/** inefficient Get, for efficiency work directly on the buffer */
	TData GetMag(TIndex pos);
	/** inefficient Get, for efficiency work directly on the buffer */
	TData GetMag(TData freq);
	/** inefficient Get, for efficiency work directly on the buffer */
	TData GetPhase(TIndex pos);
	/** inefficient Get, for efficiency work directly on the buffer */
	TData GetPhase(TData freq);
	/** inefficient Set, for efficiency work directly on the buffer */
	void SetMag(TIndex pos,TData newMag);
	/** inefficient Set, for efficiency work directly on the buffer */
	void SetPhase(TIndex pos,TData newPhase);
	/** Synchronizes all data to the one of the specified type */
	void SynchronizeTo(const SpecTypeFlags& tmpFlags);


	// Config shortcut interface.

	int GetSize() const;
	void SetSize(int s);

	int GetBPFSize() const;
	void SetBPFSize(int s);
	
	/** This is not just an accessor. The Type attribute is updated in
	 * the mConfig object before returning it. 
	 */
	void GetType(SpecTypeFlags& f) const;

	/** This methods updates the instantiation of dynamic attributes
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
	// Converting routines
public:
	void Complex2Polar() ;
	void Polar2Complex() ;
	void Complex2MagPhase() ;
	void Polar2MagPhase() ;
	void MagPhase2Polar() ;
	void MagPhase2Complex() ;
	void MagPhase2BPF() ;
	void BPF2MagPhase();

};

}; // namespace CLAM

#endif
