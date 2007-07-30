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

#ifndef _SPECTRUM_InterpolateER2_
#define _SPECTRUM_InterpolateER2_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Spectrum.hxx"
#include "InControl.hxx"

namespace CLAM {


	class SpecInterpConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecInterpConfig, 1,ProcessingConfig);
		DYN_ATTRIBUTE(0, public, TData, InterpolationFactor);
	protected:
		void DefaultInit();

	};

/** This calss performs the interpolation of two Spectrum processing data
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
 *      different formats, and Interpolate the corresponding prototype states.
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
	class SpectrumInterpolator: public Processing {
		
		typedef InControlTmpl<SpectrumInterpolator> SpectrumInterpolatorCtl;	
		
		SpecInterpConfig mConfig;

		/** Size of the input/output vectors */
		int mSize;

		InPort<Spectrum> mIn1;
		InPort<Spectrum> mIn2;
		OutPort<Spectrum> mOut;

		/** Possible configuration/prototype states */
		typedef enum {
			// Type states in with the same attribute is used for all
			// of the inputs and the outputs (it may or may not be
			// present; in the second case it will be Interpolateed at Do(...)
			// time.
			SMagPhase, SComplex, SPolar,

			// BPF output sum
			SBPF,

			// Type states with only a BPF attribute in one of the
			// inputs, other type in the other input and the
			// output. The non-BPF attribute may or may not be
			// instantiated. In the second case it will be Interpolateed at
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


		const char *GetClassName() const {return "SpectrumInterpolator";}


		/** Config change method
		 * @pre argument should be an SpecInterpConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:
		SpectrumInterpolator();

		SpectrumInterpolator(const SpecInterpConfig &c);

		~SpectrumInterpolator() {};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

// 		FIXME bool Do(const Spectrum& in1, const Spectrum& in2, Spectrum& out);
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

		/** Input control for interpolation factor */
		SpectrumInterpolatorCtl  mInterpolationFactorCtl;

	private:

		/** Unoptimised internal multiplication method, when
		 * prototypes are not known (state SOther)
		 */
		inline void Interpolate(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Interpolateer methods for optimized configurations of the inputs/output

		// Direct sums
		inline void InterpolateMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolatePolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolatePolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolatePolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolatePolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// BPF Interpolateer
		inline void InterpolateBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// Interpolateing BPFs to non-BPFs.
		inline void InterpolateBPFLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolatePolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void InterpolateBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);

	};

}

#endif // _SPECTRUM_InterpolateER_

