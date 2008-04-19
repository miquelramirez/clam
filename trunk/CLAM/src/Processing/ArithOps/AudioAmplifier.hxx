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

#ifndef _AudioAmplifier_
#define _AudioAmplifier_

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Audio.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"
#include "InControl.hxx"

namespace CLAM{

	/**
	 *	AudioAmplifier configuration object.
	 */
	class AudioAmplifierConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( AudioAmplifierConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, double, MaxGain );

	protected:
		void DefaultInit();
	};


	/**	\brief Time-domain audio gain
	 *
	 *	Very simple time-domain linear audio amplifier. For lower the volume choose values between 0 and 1.
	 *  Values over 1 will amplify the audio signal. Gain=1 means no change.
	 */
	class AudioAmplifier: public Processing
	{	
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return "AudioAmplifier"; }
		
		/** Ports **/
		AudioInPort mInputAudio;
		AudioOutPort mOutputAudio;

		/** Controls **/
		InControl mInputControl; ///< gain control


	public:

		AudioAmplifier()
			:
			mInputAudio("Input Audio",this ),
			mOutputAudio("Audio Output",this),

			mInputControl("Gain", this)
		{
			Configure( mConfig );
		}

 		~AudioAmplifier() {}

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

			TData gain = mInputControl.GetLastValue();

			DataArray& inb = in.GetBuffer();
			DataArray& outb = out.GetBuffer();

			for (int i=0;i<size;i++) 
			{
				outb[i] = inb[i]*gain;
			}

			return true;
		}

		typedef AudioAmplifierConfig Config;


	protected:

		const ProcessingConfig& GetConfig() const {	return mConfig; }

		bool ConcreteConfigure(const ProcessingConfig& config) {

			CopyAsConcreteConfig( mConfig, config );

			double max_gain = mConfig.GetMaxGain();
			mInputControl.SetBounds(0.,max_gain);
			mInputControl.SetDefaultValue(1.);
			mInputControl.DoControl(1.);
			return true;
		}


	private:

		/** Configuration **/
		Config mConfig;
	};	
	
};//namespace CLAM

#endif // _AudioAmplifier_

