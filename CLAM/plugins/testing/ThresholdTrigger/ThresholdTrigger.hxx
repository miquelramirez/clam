/*
 * Copyright (c) 2008 Hernán Ordiales
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

#ifndef _ThresholdTrigger_
#define _ThresholdTrigger_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>

namespace CLAM {

	/**	\brief Trigger processing
	 *
	 *	OutControl value will be '0' until the audio input gets louder than a threshold, when the output will turn into '1'.
	 *	The threshold is set by one InControl.
	 *
	 */
	class ThresholdTrigger: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "ThresholdTrigger"; }

		/** Ports **/
		AudioInPort mAudioInput;

		/** Controls **/
		FloatInControl mThreshold;
		FloatOutControl mTrigger;

	public:
		ThresholdTrigger(const Config & config=Config())
			:
			mAudioInput("Audio Input", this),
			mThreshold("Threshold", this),
			mTrigger("Trigger", this)
		{
			Configure( config );

			mThreshold.SetBounds(0,1);
			mThreshold.SetDefaultValue(0.5);
			mThreshold.DoControl(0.5);
		}

		~ThresholdTrigger() {}

		bool Do()
		{
			bool result = Do( mAudioInput.GetAudio() );
			mAudioInput.Consume(); 
			return result;
		}

		bool Do(const Audio& in)
		{
			int size = in.GetSize();
			const DataArray& inb = in.GetBuffer();
			TData threshold = mThreshold.GetLastValue();
			bool trigger = 0;
			for (int i=0;i<size;i++) 
			{
				std::cout << "inb[i]: " << inb[i] << " - trigger: " << trigger << std::endl;
				if (std::fabs(inb[i])>threshold)
					trigger = 1;
			}
			mTrigger.SendControl(trigger);
			return true;
		}

	private:

	/** Child processings **/

	};

};//namespace CLAM

#endif // _ThresholdTrigger_
