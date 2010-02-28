/*
* Copyright (C) 2009 Club de Audio FIUBA (Hernan Ordiales, Santiago Piccinini, Hernan Francisco Ledesma)
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

#ifndef _AdaptativeNotch_
#define _AdaptativeNotch_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>

namespace CLAM {

	class AdaptativeNotchConfig : public ProcessingConfig
	{
	public:
	    DYNAMIC_TYPE_USING_INTERFACE(  AdaptativeNotchConfig, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, unsigned int, FilterLength );
	    DYN_ATTRIBUTE( 1, public, TData, Step );

	protected:
	    void DefaultInit( )
	    {
		AddAll();
		UpdateData();
		SetFilterLength(5);
		SetStep(0.01);
	    }
	};

	/**	\brief Adaptative notch filter
	*
	*	Adaptative notch filter that uses the Steepest Descent algorithm
	*	TODO: add bibliography
	*/
	class AdaptativeNotch: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "AdaptativeNotch"; }

		/** Ports **/
		AudioInPort mInput;
		AudioInPort mReference;
		AudioOutPort mOutput;

	public:
		AdaptativeNotch()
			:
			mInput("Input", this),
			mReference("Reference", this),
			mOutput("Output", this)
		{
			Configure( mConfig );
		}

		~AdaptativeNotch()
		{
		}

		bool Do()
		{
			bool result = Do( mInput.GetAudio(), mReference.GetAudio(), mOutput.GetAudio() );
			mInput.Consume();
			mReference.Consume();
			mOutput.Produce();
			return result;
		}

		bool Do(const Audio& input, const Audio& reference, Audio& output)
		{
			const unsigned size = input.GetSize();
			const DataArray& in = input.GetBuffer();
			const DataArray& ref = reference.GetBuffer();
			DataArray& out = output.GetBuffer();

			const unsigned M = mConfig.GetFilterLength();
			const TData mu = mConfig.GetStep();
			const unsigned L = M-1;
			
			std::vector<TData> &wn = _filterCoef;
		
			TData r[M];
			for (unsigned k=0;k<size;k++)
			{
				TData acum = 0.;
				for (unsigned n=0, i=k;n<M;n++,i++)
				{
					if (i<L)
						r[n] = _oldRef[i];
					else
						r[n] = ref[i-L];
					acum += wn[n]*r[n]; // interference estimation
				}
				
				if (k<L)
					out[k] = _oldIn[k]-acum; // ECG signal estimation
				else
					out[k] = in[k-L]-acum; // ECG signal estimation
					
  	 			for (unsigned n=0;n<M;n++) wn[n]+=mu*r[n]*out[k]; // new filter taps
			}

 			for (unsigned k=0;k<L;k++)
			{
				_oldRef[k] = ref[size-L+k];
				_oldIn[k] = in[size-L+k];
			}

			return true;
		}

		const CLAM::ProcessingConfig & GetConfig() const
		{
			return mConfig;
		}

		typedef AdaptativeNotchConfig Config;
	
	protected:
		bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
		{
			CopyAsConcreteConfig(mConfig, config);

			const unsigned M = mConfig.GetFilterLength();
			_filterCoef.resize(M);
			_oldIn.resize(M-1);
			_oldRef.resize(M-1);

			return true;
		}
	  
	private:
		/** Configuration **/
		Config mConfig;
		
		/** Auxiliar members **/
		std::vector<TData> _oldIn, _oldRef, _filterCoef;
	};

};//namespace CLAM

#endif // _AdaptativeNotch_
