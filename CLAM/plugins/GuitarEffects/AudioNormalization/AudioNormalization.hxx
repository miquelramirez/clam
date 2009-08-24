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

#ifndef _AudioNormalization_
#define _AudioNormalization_

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/CLAM_Math.hxx>

namespace CLAM {

	/**	\brief AudioNormalization
	*
	*	Normalizes the input to -1..1 range checking for the max
	*/
	class AudioNormalization: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "AudioNormalization"; }

		/** Ports **/
		AudioInPort mInput;
		AudioOutPort mOutput;

		/** Controls **/
		FloatInControl mAudioNormalizationValue;

	public:
		AudioNormalization(const Config & config=Config())
			:
			mInput("Input", this),
			mOutput("Output", this),
			mAudioNormalizationValue("AudioNormalization Value", this),
			_max(1.)
		{
			Configure( config );

			mAudioNormalizationValue.SetBounds(0,2);
			mAudioNormalizationValue.SetDefaultValue(1.0);
			mAudioNormalizationValue.DoControl(1.0);
		}

		~AudioNormalization() {}

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

			TData value = mAudioNormalizationValue.GetLastValue();
			for (int i=0;i<size;i++) 
			{
				if (fabs(inb[i])>_max) _max = fabs(inb[i]);
				
				outb[i] = inb[i]/_max*fabs(value);
			}
			return true;
		}

	private:
		TData _max;	
	};

};//namespace CLAM

#endif // _AudioNormalization_
