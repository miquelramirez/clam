/*
 * Copyright (C) 2009 Hernan Ordiales
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

#ifndef _Filter_
#define _Filter_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControlArray.hxx>

#include <deque>

namespace CLAM {

	/**
	 *	Filter configuration object.
	 */
	class FilterConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( FilterConfig, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, AmountOfInputCoefficients );
		DYN_ATTRIBUTE( 1, public, unsigned, AmountOfOutputCoefficients );

	protected:
		void DefaultInit();
	};


	/**	\brief Filter 
	*
	*	Direct form implementation of the linear, time-invariant difference equation:
	*  \f$ \sum_{k=0}^{N} a_{k+1} y[n-k] = \sum_{k=0}^{M} b_{k+1} x[n-k] \f$
	*
	*/
	class Filter: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "Filter"; }

		/** Ports **/
		AudioInPort mInput;
		AudioOutPort mOutput;

		/** Controls **/
		InControlArray mA; ///< Output coefficients
		InControlArray mB; ///< Input coefficients

		/**  **/
		std::deque<TData> x; ///< Past x values buffer
		std::deque<TData> y; ///< Past y values buffer

	public:
		Filter()
			:
			mInput("Input", this),
			mOutput("Output", this),
			mA(1, "a", this),
			mB(1, "b", this)
		{
			Configure( mConfig );
		}

		~Filter() {}

		typedef FilterConfig Config;

		bool Do()
		{
			bool result = Do( mInput.GetAudio(), mOutput.GetAudio() );
			mInput.Consume(); 
			mOutput.Produce();
			return result;
		}
	
		bool Do(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			const DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			unsigned x_k = mConfig.GetAmountOfInputCoefficients();
			unsigned y_k = mConfig.GetAmountOfOutputCoefficients();
			TData a0 = mA[0].GetLastValue();
			for (int i=0;i<size;i++) 
			{
				x.push_front(inb[i]);
				x.pop_back();

				outb[i] = 0.;

				//FIR
 				for (unsigned k=0;k<x_k;k++)
 					outb[i] += mB[k].GetLastValue()*x[k];
			
				//IIR
  				for (unsigned k=1;k<y_k;k++)
  					outb[i] -= mA[k].GetLastValue()*y[k-1];
 				outb[i] /= a0;


				y.push_front(outb[i]);
				y.pop_back();
			}
			return true;
		}
	
 	protected:
		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );

			if ( mConfig.GetAmountOfInputCoefficients()<1 || mConfig.GetAmountOfOutputCoefficients()<1 )
			{
				AddConfigErrorMessage("Input and Output amount of coefficients must be greater than 1");
				return false;
			}

			mA.Resize(mConfig.GetAmountOfOutputCoefficients() , "a", this);
			for (int i=0;i<mA.Size(); i++)
			{
				mA[i].SetBounds(-100.,100.);
				mA[i].SetDefaultValue(1.);
				mA[i].DoControl(1.);
			}
			mB.Resize(mConfig.GetAmountOfInputCoefficients(), "b", this);
			for (int i=0;i<mB.Size(); i++)
			{
				mB[i].SetBounds(-100.,100.);
				mB[i].SetDefaultValue(1.);
				mB[i].DoControl(1.);
			}

			//null initial conditions
			x.resize( mConfig.GetAmountOfInputCoefficients() );
			for(unsigned i=0;i<x.size();i++)
				x[i] = 0.;
			y.resize( mConfig.GetAmountOfOutputCoefficients() );
			for(unsigned i=0;i<y.size();i++)
				y[i] = 0.;

			return true;
		}

	private:
		/** Configuration **/
		Config mConfig;
	};

};//namespace CLAM

#endif // _Filter_"
