/*
 * 
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

#ifndef _FilterByCoefExample_
#define _FilterByCoefExample_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>

#include "../Filter/Filter.hxx"

namespace CLAM {

	/**	\brief Band pass filter defined by direct form coefficients
	*
	*	Example
	*/
	class FilterByCoefExample: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "FilterByCoefExample"; }

		/** Ports **/
		AudioInPort mInput;
		AudioOutPort mOutput;

		/** Controls **/

	public:
		FilterByCoefExample(const Config & config=Config())
			:
			mInput("Input", this),
			mOutput("Output", this)
		{
			Configure( config );

			unsigned N = 11; //filter order + 1
			FilterConfig cfg;
			cfg.SetAmountOfInputCoefficients(N);
			mFilter.Configure( cfg );

			//FIR band pass filter coefficients. Order 10. 11 coefficients.
			TData b_k[] = {
					0.0555,    0.0933,   -0.1187,   -0.2476,    0.0628,    0.3265,    0.0628,   -0.2476,   -0.1187,\
					0.0933,    0.0555\
			};
			char bkstr[5];
			for(unsigned i=0;i<N;i++)
			{ 
				sprintf(bkstr, "b_%i", i);
				SendFloatToInControl(mFilter, bkstr, b_k[i]);
			}
		}

		~FilterByCoefExample() {}

		bool Do()
		{
			mFilter.Do( mInput.GetAudio(), mOutput.GetAudio() );
			mInput.Consume(); 
			mOutput.Produce();
			return true;
		}
	
	private:
		/** Child processings **/
		Filter mFilter;
	};

};//namespace CLAM

#endif // _FilterByCoefExample_
