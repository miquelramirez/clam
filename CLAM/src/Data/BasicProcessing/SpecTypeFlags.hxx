/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SpecTypeFlags__
#define __SpecTypeFlags__

#include "Flags.hxx"

namespace CLAM
{

/**
* This class tells which formats are instantiated in a Spectrum.
* It could be:
* - Complex: Array of Complex objects
* - Polar: Array of Polar objects
* - MagPhase: Array of magnitude/phase pairs
* - MagPhaseBPF: BPF of magnitude/phase pairs
* 
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
	template <typename T1, typename T2> SpecTypeFlags (const T1 &t1,const T2 &t2) :
		Flags<4>(sFlagValues,t1,t2),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))
	{};
*/

	reference bComplex;
	reference bPolar; 
	reference bMagPhase;
	reference bMagPhaseBPF;

};

}

#endif

