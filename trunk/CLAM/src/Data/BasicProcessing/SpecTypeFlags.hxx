#ifndef __SpecTypeFlags__
#define __SpecTypeFlags__

#include "Flags.hxx"

namespace CLAM
{

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
