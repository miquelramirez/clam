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

#ifndef _FIR_H_
#define _FIR_H_

#include "Processing.hxx"
#include "Audio.hxx"

/** @file
* This source is more heavily documented than other processing because is
* intended to be a reference implementation for time domain processings.
*/

namespace CLAM
{
	/**
	 * FIRConfig is a configuration for class FIR processings.
	 */

	class FIRConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FIRConfig, 1, ProcessingConfig);
		/** The convolution kernel for fixed coeficients processing */
		DYN_ATTRIBUTE (0, public, ConvolutionKernel, ConvolutionKernel);
	};

	/**
	 * Finite Impulse Response (FIR) filter.
	 * This processing allows to implement a family of time domain
	 * filters based on a Finite Impulse Response function:
	 * \f[{ y(t) = \sum_{k=0}^{n}{ a_n·x(t-k)}}\f]
	 * The convolution kernel is the vector that contains the <it>a<sub>n</sub></it>
	 * coeficients that defines the filter finite response.
	 * <p> You can use this filter in two ways:
	 * <ul>
	 * <li>Providing a fixed convolution kernel as a configuration
	 * parameter, or</li>
	 * <li>Providing variable convolution kernel as input data on
	 * on running status.
	 * Convolution kernels generators.
	 * </ul></p>
	 */
	class FIR : public Processing
	{
		FIRConfig mConfig;
		const TData * mCoeficients;
		TSize mNCoeficients;
	public:

		InPortTmpl<Audio>    InputAudio;
		InPortTmpl<ConvolutionKernel> Coeficients;
		OutPortTmpl<Audio>   OutputAudio;

	public:
		const char *GetClassName() const {return "FIR";}

		FIR() :
			InputAudio("AudioIn", this),
			OutputAudio("AudioOut",this),
			Coeficients("Coeficients",this)
		{
			Configure( FIRConfig() );
		}

		FIR(const FIRConfig &c) :
			InputAudio("AudioIn", this),
			OutputAudio("AudioOut",this),
			Coeficients("Coeficients",this)
		{
			Configure( c );
		}

		virtual ~FIR()
		{
		}

	protected:
		/** Config change method
		 */
		virtual bool ConcreteConfigure(const ProcessingConfig& c)
		{
			// Copy the configuration locally
			CopyAsConcreteConfig(mConfig, c);
			// Then use the configuration to adapt the processing inners
			if (mConfig.HasCoeficients) {
				const Array<TData> & convolutionKernel = mConfig.GetConvolutionKernel();
				mCoeficients  = convolutionKernel.GetCoeficients.GetPtr();
				mNCoeficients = convolutionKernel.GetSize();
			}
			else {
				mCoeficients = NULL;
				mNCoeficients = 0;
			}
			return true;

		}

	public:
		virtual const ProcessingConfig &GetConfig() const { return mConfig;}

		/**
		 * Processing step in supervised mode (using ports)
		 * @return Wheter an output has been generated or not.
		 * @
		 */
		virtual bool Do(void)
		{
			bool res;
			// Obtain references to ports data
			const Audio & in  = InputAudio.GetData();
			      Audio & out = OutputAudio.GetData()
			// Select the Do overload to use depending on wheter the
			// configuration had set fixed coeficients
			if (mCoeficients) {
				res = Do(in, out);
			}
			else {
				// On this case you can der
				res = Do(in, Coeficients.GetData(), out);
				Coeficients.LeaveData();
			}
			InputAudio.LeaveData();
			OutputAudio.LeaveData();
			return res;
		}


		/**
		 * Processing step using the convolution kernel fixed with the configuration
		 *  @param in The input audio buffer
		 *  @param out The output audio buffer
		 *  @pre Configured with a valid fixed convolution kernel
		 *  @pre sizeof(coeficients)>0 && sizeof(in)>0 && sizeof(out)>0
		 *  @pre sizeof(in)>=sizeof(coeficients)
		 *  @pre sizeof(in)+1==sizeof(coeficients)+sizeof(out)
		 */
		bool Do(const Audio& in, Audio& out)
		{
			// Keep precalculed some values that are constant inside the method
			// and can represent extra method calls
			const unsigned int nIn   = in.GetSize();
			const unsigned int nOut  = out.GetSize();

			// Now we will check some of the method preconditions
			CLAM_ASSERT(mCoeficients,
				"FIR: Using fix coeficients which are not configured.");
			CLAM_ASSERT(mNCoeficients<=nIn,
				"FIR: Input window is smaller than the convolution kernel size");
			CLAM_DEBUG_WARNING(nIn+1-mNCoeficients==nOut,
				"FIR: Input and output windows are not balanced considering the "
				"convolution kernel size. "
				"Some input or output samples will not be processed.");
			// Warnings must still work so...
			const unsigned int nToCompute = min(nOut, nIn+1-mNCoeficients);

			// Fetching the internal buffers to make cheap the step loop.
			// Input data is kept always const.
			const TData * samplesIn  = in .GetBuffer().GetPtr();
			      TData * samplesOut = out.GetBuffer().GetPtr();

			// Call the common implementation of the step loop
			Calculate(samplesIn, mCoeficients, samplesOut, mNCoeficients, nToCompute);

			return true;
		}

		/**
		 * Processing step using a convolution kernel as second input to allow variable
		 * convolution kernels.
		 *  @param in The input audio buffer
		 *  @param coeficients The convolution kernel for the filter
		 *  @param out The output audio buffer
		 *  @pre sizeof(coeficients)>0 && sizeof(in)>0 && sizeof(out)>0
		 *  @pre sizeof(in)>=sizeof(coeficients)
		 *  @pre sizeof(in)==sizeof(coeficients)+sizeof(out)-1
		 */
		bool Do(const Audio& in, const ConvolutionKernel& coeficients, Audio& out)
		{
			const unsigned int nIn   = in.GetSize();
			const unsigned int nOut  = out.GetSize();
			const unsigned int nCoef = coeficients.GetSize(); // Extra action for this version

			// Now we will check some of the method preconditions
			CLAM_ASSERT(nCoef<=nIn,
				"FIR: Input window is smaller than the convolution kernel size");
			CLAM_DEBUG_WARNING(nIn+1-nCoef==nOut,
				"FIR: Input and output windows are not balanced considering the "
				"convolution kernel size. "
				"Some input or output samples will not be processed.");
			// Warnings must still work so...
			const unsigned int nToCompute = min(nOut, nIn+1-nCoef);

			// Fetching the internal buffers to make cheap the step loop.
			// Input data is kept always const.
			const TData * samplesIn = in.GetBuffer().GetPtr();
			const TData * coefs = coeficients.GetCoeficients().GetPtr(); // Extra action for this version
			      TData * samplesOut = out.GetBuffer().GetPtr();

			// Call the common implementation of the step loop
			Calculate(samplesIn, coefs, samplesOut, nMCoeficients, nToCompute);

			return true;
		}

	private:
		/**
		 * Private implementation shared between the different Do entry points.
		 */
		void Calculate(const TData *samplesIn, const TData *coeficients, TData* samplesOut,
				const unsigned int kernelSize, const unsigned int nToCompute)
		{
#if 0
			// Educational Version: The following bucle is the one you can find in books
			// Keeping this clearer implementation you can compare the validity of the results,
			// you can have a reference for the performance and clarifies to mantainers
			// what is the intent for the optimal version.
			for (unsigned int outIndex=0; outIndex<nToCompute; outIndex++)
			{
				samplesOut[outIndex]=0;
				for (unsigned int coefIndex=0; coefIndex<kernelSize; coefIndex++)
				{
					samplesOut[outIndex]+=
						coeficients[coefIndex]*samplesIn[outIndex+coefIndex];
				}
			}
#else
			// Optimal Version: We have optimized this by extracting calculations from
			// inner loop levels and by sharing the calculations.
			// Reverse counts often saves a comparation per loop.
			for (unsigned int iOut=nToCompute; iOut--; samplesOut++)
			{
				const TData * currentIn = samplesIn++;
				const TData * currentCoef = mCoeficients;
				*samplesOut=0;
				for (unsigned int iCoef = kernelSize; iCoef--;)
				{
					*samplesOut += *currentCoef++ * *currentIn++;
				}
			}
#endif
		}
	};

}

#endif // _FIR_H_
