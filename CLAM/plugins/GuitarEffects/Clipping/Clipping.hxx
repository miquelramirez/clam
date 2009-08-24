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

#ifndef _Clipping_
#define _Clipping_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/CLAM_Math.hxx>

namespace CLAM {

	/**	\brief Clipping
	*
	*	Clips the input signal to -1..1 range or whatever input control says
	*/
	class Clipping: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "Clipping"; }

		/** Ports **/
		AudioInPort mInput;
		AudioOutPort mOutput;

		/** Controls **/
		FloatInControl mClippingValue;

	public:
		Clipping(const Config & config=Config())
			:
			mInput("Input", this),
			mOutput("Output", this),
			mClippingValue("Clipping Value", this)
		{
			Configure( config );

			mClippingValue.SetBounds(0,1);
			mClippingValue.SetDefaultValue(1.0);
			mClippingValue.DoControl(1.0);
		}

		~Clipping() {}

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

			TData value = mClippingValue.GetLastValue();
			for (int i=0;i<size;i++) 
			{
				if( fabs(inb[i])>value )
					outb[i] = inb[i]<0.? -value:value;
				else
					outb[i] = inb[i];
			}
			return true;
		}
	
	};

};//namespace CLAM

#endif // _Clipping_
