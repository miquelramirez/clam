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

#ifndef _SPECTRUM_ADDER2_
#define _SPECTRUM_ADDER2_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class SpecAdder2Config: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecAdder2Config, 0,ProcessingConfig);
	};

/** This calss performs the sum of two Spectrum processing data
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
 * Possible optimisations (which require more states):
 * <ul>
 * <li> Implement direct sum routines with inputs/outpust in
 *      different formats, and add the corresponding prototype states.
 * <li> Expand the state space to avoid checking if the attribute to be
 *      used in the computation is instantiated in each of the objects.
 *      Right now the same state is used when the three objects have a
 *      common attribute, and when a common attribute is to be used, but
 *      some of the objects lack it (and need format conversion).
 * </ul>
 * <p>
 * The BPFxBPF sum is being thought. If both BPFs have the same
 * range and point possition, the way to go is obvious, but in other
 * situations it is not so simple. Whe should probably merge both
 * BPFs, into a new BPF.  */
	class SpectrumAdder2: public Processing {
		SpecAdder2Config mConfig;

		/** Size of the input/output vectors */
		int mSize;

		InPort<Spectrum> mIn1;
		InPort<Spectrum> mIn2;
		OutPort<Spectrum> mOut;

		/** Possible configuration/prototype states */
		typedef enum {
			// Type states in with the same attribute is used for all
			// of the inputs and the outputs (it may or may not be
			// present; in the second case it will be added at Do(...)
			// time.
			SMagPhase, SComplex, SPolar,

			// BPF output sum
			SBPF,

			// Type states with only a BPF attribute in one of the
			// inputs, other type in the other input and the
			// output. The non-BPF attribute may or may not be
			// instantiated. In the second case it will be added at
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

		const char *GetClassName() const {return "SpectrumAdder2";}


		/** Config change method
		 * @pre argument should be an SpecAdder2Config
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectrumAdder2();

		SpectrumAdder2(const SpecAdder2Config &c);

		~SpectrumAdder2() {};

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
		inline void Add(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Adder methods for optimized configurations of the inputs/output

		// Direct sums
		inline void AddMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// BPF Adder
		inline void AddBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// Adding BPFs to non-BPFs.
		inline void AddBPFLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void AddBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);

	};

}

#endif // _SPECTRUM_ADDER_

