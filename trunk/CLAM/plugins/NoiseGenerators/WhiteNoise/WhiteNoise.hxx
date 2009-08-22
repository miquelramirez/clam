/*
 * Copyright (C) 2009 Club Audio Fiuba
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

#ifndef _WhiteNoise_
#define _WhiteNoise_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>

#include <time.h>

namespace CLAM {

	/**	\brief Short description
	*
	*	Description
	*/
	class WhiteNoise: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "WhiteNoise"; }

		/** Ports **/
		AudioOutPort mOutput;

		/** Controls **/
		FloatInControl mMean;
		FloatInControl mStd; // standard deviation

		//TODO: add a gaussian white noise implementation
		//TODO: add a configuration parameter to choose between uniform and gaussian noise

	public:
		WhiteNoise(const Config & config=Config())
			:
			mOutput("mOutput", this),
			mMean("Noise Mean", this),
			mStd("Noise Standard Deviation", this)
		{
			Configure( config );

			mMean.SetBounds(-1,1);
			mMean.SetDefaultValue(0.0);
			mMean.DoControl(0.0);

			mStd.SetBounds(0,1);
			mStd.SetDefaultValue(1.0);
			mStd.DoControl(1.0);

			/* initialize random seed: */
			srand( time(NULL) );
		}

		~WhiteNoise() {}

		bool Do()
		{
			bool result = Do( mOutput.GetAudio() );

			mOutput.Produce();

			return result;
		}
	
		bool Do(Audio& out)
		{
			int size = out.GetSize();

			DataArray& buf = out.GetBuffer();

			TData mean = mMean.GetLastValue();
			TData std = mStd.GetLastValue();
			for (int i=0;i<size;i++) 
			{
				//Uniform noise
				buf[i] = 2.*rand()/RAND_MAX - 1.;

				//Scaling to meet required noise parameters
				buf[i] *= std;
				buf[i] += mean;
			}
			return true;
		}

	};

};//namespace CLAM

#endif // _WhiteNoise_
