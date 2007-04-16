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

#ifndef _SPECTRUM_PRODUCT_
#define _SPECTRUM_PRODUCT_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class SpecProductConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecProductConfig, 0,ProcessingConfig);
	};

/** This class performs the product of two Spectrum processing data
 * objects. Its primary use will be to allow frecuency domain
 * filtering, by taking the frecuency response as one of its inputs.
 * (It implements the funcionality provided in the old
 * CCLAMApplyFDFilter class).
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
 * <li> Implement direct product routines with inputs/outpust in
 *      different formats, and add the corresponding prototype states.
 * <li> Expand the state space to avoid checking if the attribute to be
 *      used in the computation is instantiated in each of the objects.
 *      Right now the same state is used when the three objects have a
 *      common attribute, and when a common attribute is to be used, but
 *      some of the objects lack it (and need format conversion).
 * </ul>
 * <p>
 * The BPFxBPF product is being thought. If both BPFs have the same
 * range and point possition, the way to go is obvious, but in other
 * situations it is not so simple. Whe should probably merge both
 * BPFs, into a new BPF.  */
	class SpectrumProduct: public Processing {
		SpecProductConfig mConfig;

		/** Size of the input/output vectors */
		int mSize;

		/** Possible configuration/prototype states */
		typedef enum {
			// Type states in with the same attribute is used for all
			// of the inputs and the outputs (it may or may not be
			// present; in the second case it will be added at Do(...)
			// time.
			SMagPhase, SComplex, SPolar,

			// BPF output product
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

		const char *GetClassName() const {return "SpectrumProduct";}


		/** Configuration change method
		 * @pre argument should be an SpectrumProductConfig
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

	public:

		InPort<Spectrum> Input1;
		InPort<Spectrum> Input2;
		OutPort<Spectrum> Output;

		SpectrumProduct();

		SpectrumProduct(const SpecProductConfig &c);

		~SpectrumProduct() {};

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
		inline void Multiply(Spectrum& in1, Spectrum& in2, Spectrum& out);

		// Product methods for optimized configurations of the inputs/output

		// Direct products
		inline void MultiplyMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// BPF Product
		inline void MultiplyBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		// BPF filters application
		inline void MultiplyBPFLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFMagPhase(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyMagPhaseBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFMagPhaseLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFMagPhaseLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFMagPhaseLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFMagPhaseLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFComplex(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyComplexBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFComplexLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFComplexLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFComplexLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFComplexLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFPolar(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyPolarBPF(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFPolarLin(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFPolarLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFPolarLinLog(Spectrum& in1, Spectrum& in2, Spectrum& out);
		inline void MultiplyBPFPolarLogLin(Spectrum& in1, Spectrum& in2, Spectrum& out);

	};

}

#endif // _SPECTRUM_PRODUCT_

