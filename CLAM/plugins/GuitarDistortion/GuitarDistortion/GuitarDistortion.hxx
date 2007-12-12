/*
 * Copyright (c) 2007 by Hernan Ordiales <audiocode@uint8.com.ar>
 *
 * Special thanks to Daniel Vidal Chornet <vidal_dan@hotmail.com>
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

#ifndef _GuitarDistortion_
#define _GuitarDistortion_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>

#include <CLAM/Audio.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>

#include <CLAM/CLAM_Math.hxx>

namespace CLAM{

	/**	\brief GuitarDistortion
	 *
	 *	overdriven guitar distortion
	 */
	class GuitarDistortion: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "GuitarDistortion"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControl mDistortion; ///< amount of distortion
		InControl mCompression; ///< amount of compression

		const TData mCompMaxValue; ///< max compression default value
		TData gain; ///< automatic gain control
		TData gstep; ///< adaptation gain step

	public:
		GuitarDistortion(const Config & config=Config())
			:
			mInputAudio("Input Audio",this ),
			mOutputAudio("Audio Output",this),

			mDistortion("Distortion amount", this),
			mCompression("Compression amount", this),

			mCompMaxValue(10.)
		{
			Configure( config );

			gstep = 0.1; //fixed default value
			gain = 1.; // starting gain

			mDistortion.SetBounds(0.,1.);
			mDistortion.SetDefaultValue(0.);
			mDistortion.DoControl(0.);

			mCompression.SetBounds(0.,mCompMaxValue);
			mCompression.SetDefaultValue(mCompMaxValue*0.9);
			mCompression.DoControl(mCompMaxValue*0.9);
		}

 		~GuitarDistortion() {}

		bool Do()
		{
			bool result = Do( mInputAudio.GetAudio(), mOutputAudio.GetAudio() );

			mInputAudio.Consume(); 
			mOutputAudio.Produce();

			return result;
		}
	
		bool Do(const Audio& in, Audio& out)
		{
			int size = in.GetSize();
			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			TData ogainref = mCompression.GetLastValue(); //output power reference
			gstep = 0.1;
			if (ogainref>mCompMaxValue-0.01)
				ogainref = mCompMaxValue-0.01;
			else if (ogainref<0.01) 
			{
				//compression off
				gstep = 0.;
				gain = 1.;
			}
			ogainref = (mCompMaxValue-ogainref)/100.; //adjust and inverts the slider value

			TData amount = mDistortion.GetLastValue();
			amount = 1. - CLAM_pow( 1.-amount, 4. ); //value adjust to get more resolution on 0.7-1.0 range
			if (amount > 0.99) amount = 0.99; // to get it stable avoids division by zero

			TData k = 2. * amount / ( 1. - amount );
			for (int i=0;i<size;i++) 
			{
				outb[i] = (1.+k)*inb[i] / ( 1. + k*Abs(inb[i]) ) * gain;
				gain += gstep * ( ogainref - outb[i]*outb[i] );
			}
			return true;
		}
	};	
	
};//namespace CLAM

#endif // _GuitarDistortion_
