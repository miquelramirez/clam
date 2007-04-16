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

#ifndef _SPECTRUM_Substracter2_
#define _SPECTRUM_Substracter2_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class SpecSubstracter2Config: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecSubstracter2Config, 0,ProcessingConfig);
	};

/** This calss performs the substraction of two Spectrum processing data
 * objects.
 * <p>
 * It Allows any possible attribute configuration in its inputs and in
 * its output, but it performs faster when prototype configuration of
 * the data is specified using SetPrototypes(...), in certain
 * situations:
 * <ul>
 * <li> When all the inputs and the outputs have a common attirbute
 *      (not the BPF), and the same scale.
 * <li> When one of the inputs has just a BPF attribute, and both the
 *      other input and the output have a common (non-BPF) attribute
 *      with the same scale in both objects.
 * <li> In other cases, at least a vector conversion will be executed
 *      in one of the involved processing data objects. In some bad
 *      situations two conversions might be needed.
 * </ul><p>
 * @todo
 * @see SpectrumSubstracter2
 */
	class SpectrumSubstracter2: public Processing {
		SpecSubstracter2Config mConfig;

		/** Size of the input/output vectors */
		int mSize;

		InPort<Spectrum> mIn1;
		InPort<Spectrum> mIn2;
		OutPort<Spectrum> mOut;

		/** Possible configuration/prototype states */
		typedef enum {
			// Type states in with the same attribute is used for all
			// of the inputs and the outputs (it may or may not be
			// present; in the second case it will be Substracted at Do(...)
			// time.
			SMagPhase, SComplex, SPolar,

			// BPF output sum
			SBPF,

			// Type states with only a BPF attribute in one of the
			// inputs, other type in the other input and the
			// output. The non-BPF attribute may or may not be
			// instantiated. In the second case it will be Substracted at
			// Do(...) time.

			SBPFMagPhase, SBPFComplex, SBPFPolar, SMagPhaseBPF,
			SComplexBPF, SPolarBPF,

			// State in which nothing is known about prototypes.
			SOther
		} PrototypeState;

		/** Possible scale combinations */
		typedef enum { Slinlin,	Sloglog, Slinlog, Sloglin} ScaleState;

		/** Config/Prototype state */
		PrototypeState mProtoState;
		/** Scale combination state */
		ScaleState mScaleState;


		/** OBSOLETE */
		std::string NewUniqueName();

		const char *GetClassName() const {return "SpectrumSubstracter2";}


		/** Config change method
		 * @pre argument should be an SpecSubstracter2Config
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectrumSubstracter2();

		SpectrumSubstracter2(const SpecSubstracter2Config &c);

		~SpectrumSubstracter2() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(const Spectrum& in1,const Spectrum& in2,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

	private:

		/** Unoptimised internal multiplication method, when
		 * prototypes are not known (state SOther)
		 */
		inline void Substract(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Substracter methods for optimized configurations of the inputs/output

		// Direct sums
		inline void SubstractMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// BPF Substracter
		inline void SubstractBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// Substracting BPFs to non-BPFs.
		inline void SubstractBPFLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void SubstractBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);

	};

}

#endif // _SPECTRUM_Substracter_

